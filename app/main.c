// Copyright (c) 2014, Thomas Goyne <tgoyne@gmail.com>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

//--------------------------------

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "endian.h"
#include "message.h"
#include "file.h"
#include "str.h"

struct format {
    const char tag[4];
    const char *filename;
    uint8_t *data;
    uint32_t size;
};

static struct format formats[] = {
    {"icp4", "icon_16x16.png", NULL, 0},
    {"ic11", "icon_16x16@2x.png", NULL, 0},
    {"icp5", "icon_32x32.png", NULL, 0},
    {"ic12", "icon_32x32@2x.png", NULL, 0},
    {"icp6", "icon_64x64.png", NULL, 0},
    {"ic07", "icon_128x128.png", NULL, 0},
    {"ic13", "icon_128x128@2x.png", NULL, 0},
    {"ic08", "icon_256x256.png", NULL, 0},
    {"ic14", "icon_256x256@2x.png", NULL, 0}, 
    {"ic09", "icon_512x512.png", NULL, 0}, 
    {"ic10", "icon_512x512@2x.png", NULL, 0},    
};

int main(int argc, const char **argv) {
    if (argc != 3) {
        printf("-------------------------------------------\n");
        printf("Icns Packer\n");
        printf("Combines icon.iconset images into a single ICNS file\n");
        printf("Iconset can contain images of sizes <= 1024 pixels\n");
        printf("Listing:\n");
        printf("https://en.wikipedia.org/wiki/Apple_Icon_Image_format#Icon_types\n\n");
        printf("Usage: icnspack <output_icns> <input_incoset>\n\n");
        printf("Example: \n");
        printf("icnspack output.icns input.iconset\n");
        printf("-------------------------------------------\n");
        return 1;
    }

    sendMessage("info", "Creating icns: ", str_prbrk (argv[1], "/\\", false));

    for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", argv[2], formats[i].filename);

        FILE* fd = fopen(path, "rb");
        if (fd == NULL) {
            sendMessage("warn", "Missing: ", path);
            continue;
        }

        sendMessage("info", "Collecting image info: ", formats[i].filename);

        fseek(fd, 0L, SEEK_END);
        formats[i].size = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        formats[i].data = malloc(formats[i].size);
        fread(formats[i].data, 1, formats[i].size, fd);
        fclose(fd);
    }

    FILE* fd = fopen(argv[1], "w+b");
    if (fd == NULL) {
        sendMessage("warn", "Failed to open output file: ", argv[1]);
        return 1;
    }   

    // Write header
    fwrite("icns", 4, 1, fd);

    uint32_t size = 16;
    uint32_t toc_size = 8;
    for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
        if (formats[i].data) {
            size += 16 + formats[i].size;
            toc_size += 8;
        }
    }
    fwrite_uint32(size, fd);

    // Write TOC
    fwrite("TOC ", 4, 1, fd);
    fwrite_uint32(toc_size, fd);
    for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
        if (formats[i].data) {
            fwrite(formats[i].tag, 4, 1, fd);
            fwrite_uint32(formats[i].size, fd);
        }
    }

    // Write images
    for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
        if (formats[i].data) {
            sendMessage("info", "Writing image data: ", formats[i].filename);

            fwrite(formats[i].tag, 4, 1, fd);
            fwrite_uint32(formats[i].size + 8, fd);
            fwrite(formats[i].data, formats[i].size, 1, fd);
        }
    }

    sendMessage("info", "Icns created at: ", get_realpath(argv[1]));

    fclose(fd);
}
