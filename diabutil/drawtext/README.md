# drawtext

![Don't have a cow man](sample.png)

Render text using `BigTGold.cel` (or font with equivalent width, kerning, and frames). Rendered text is output to `test.png`.

```
drawtext BigTGold.CEL palette.pal message
```

  * `BigTGold.CEL` is the filename locally of the required font CEL
  * `palette.pal` is 256 colors in RGB format (one byte per color channel for a total of 768 bytes total)
      * basically `struct { BYTE r; BYTE g; BYTE b; } colors[256];`
  * `message` is the text to render
