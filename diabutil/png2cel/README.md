# png2cel

This is a WIP and does not generate optimized images.

Convert a given PNG into a CEL frame. Use joincel afterwards to create the final .CEL

```
png2cel file.png palette.pal out.celframe [--header]
```

  * `file.png` is a PNG file that only contains colors in the given palette. The program will error out if there is a color not in the palette.
  * `palette.pal` is 256 colors in RGB format (one byte per color channel for a total of 768 bytes total)
      * basically `struct { BYTE r; BYTE g; BYTE b; } colors[256];`
  * `out.celframe` the the CEL conversion of the PNG. It lacks a frame table header so needs to be run through joincel
  * `--header` is optional. If present, then the 10 byte header at the start of `out.celframe` is added.
