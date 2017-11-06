The .otb font will also work for most applications in unix/linux systems, but
really old "vintage" X11 programs may only accept PCF style fonts.

This is one such font.

Copy it to the correct directory (example: on my machine this is
`/usr/share/fonts/X11/misc/`).

You may have to run `update-fonts-dir` in order to make this font appear in,
say, `xfontsel`. I manually added the entry to `fonts.dir` in the directory I
installed it in on my machine.
