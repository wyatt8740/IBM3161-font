# X11 font

There is also a .otb (OpenType Bitmap) font which you should probably install,
if you haven't yet.

## X11 PCF installation

The .otb font will also work for most applications in unix/linux systems, but
really old "vintage" X11 programs may only accept PCF style fonts.

This is one such font.

Copy it to the correct directory (example: on my machine this is
`/usr/share/fonts/X11/misc/`).

You may have to run `update-fonts-dir` in order to make this font appear in,
say, `xfontsel`. I manually added the entry to `fonts.dir` in the directory I
installed it in on my machine.

#### Make it work in X programs
First, copy the PCF file to the same location you copied the TTF and/or OTB's
to.
Then, in a file that loads your X server resources, add a line like the
following to make `xterm` use it. Other apps probably follow similar
guidelines:
````
xterm*font:   -gnu-IBM3161bitmap-medium-R-normal-sans-17-120-100-100-c-*-ISO10646-1
````