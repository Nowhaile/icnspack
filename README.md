# icnspack

`icnsutil` and Xcode's automatic iconset -> icns conversion recompress their input files, even if they were already in the correct format. This would be fine, except for that they do an extremely poor job of compressing to png and produce files twice as large as can be achieved with better compressors. This tool simply packs a .iconset into a .icns with no recompression (and not much in the way of validation, so make sure the end result works).

## Usage

Compile using CMake. Run with `icnspack output.icns input.iconset`.

Example: `icnspack assets/icons.icns assets/icon.iconset`

## Userfull links

> ### 1. [HTML icon viewer](https://relikd.github.io/icnsutil/#html-icon-viewer)
> Here are two tools to open icns files directly in your browser. Both tools can be used either with an icns file or a rgb / argb image file.
>  - The [inspector](https://relikd.github.io/icnsutil/html/inspector.html) shows the structure of an icns file (useful to understand byte-unpacking in ARGB and 24-bit RGB files).
>  - The [viewer](https://relikd.github.io/icnsutil/html/viewer.html) displays icons in ARGB or 24-bit RGB file format.

## License

Copyright (c) 2014, Thomas Goyne <tgoyne@gmail.com>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.