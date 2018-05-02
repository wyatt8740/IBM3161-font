# X11 font

There is also a .otb (OpenType Bitmap) font which you should probably install,
if you haven't yet.

## OTB (OpenType Bitmap) installation
### X11 OTB and TTF Installation
On my Debian system, I made the directory `~/.fonts` and copied
IBM3161-bitmap.otb and IBM3161.ttf to it. If you don't want/need both versions,
just copy one or the other. Java programs using AWT/Swing don't seem to work
with X11 bitmap fonts on Unix systems, though (at least in OpenJDK), so I had
to keep the TrueType version around for Java programs.

The OTB is in the directory `X11/`, and the TTF in `truetype/`.

The PCF's are the only technically X11-specific fonts here. OpenType Bitmaps
just happen to not be supported anywhere that I have found but X-using unix
systems, so I've dumped that here, too.

#### If the OTB bitmap version isn't working
In Debian, Ubuntu, and possibly other distributions, FreeType has bitmap fonts
disabled by default for some silly reason.

In Debian, this is fixable by deleting 
`70-no-bitmaps.conf`: 
````
rm /etc/fonts/conf.d/70-no-bitmaps.conf
````
You may also want to consider running `dpkg-reconfigure fontconfig-config` and 
changing the setting there. Then you might also have to run `dpkg-reconfigure 
fontconfig` after that, according to random people on the internet. This is
not how I have always done it, but it would make sense and be a more 'canonical'
solution than my own.

One can also just delete `70-no-bitmaps.conf` and copy
`/usr/share/fontconfig/conf.avail/70-yes-bitmaps.conf` to `70-no-bitmaps.conf`,
and then use `chattr +i 70-no-bitmaps.conf` to make sure updates don't
overwrite my setting (by setting the 'immutable' flag).
This is likely sub-par, but that's just how I have always done it (tm).

## X11 PCF installation
The .otb font will also work for most applications in unix/linux systems, but
really old "vintage" X11 programs may only accept PCF style fonts. Examples
include the [Common Desktop Environment (CDE)](https://sourceforge.net/projects/cdesktopenv/),
which I have a bit of an attachment to, as well as [xterm](http://invisible-island.net/xterm/)
and most programs based on [Motif](http://motif.ics.com/motif).

This is such a PCF font.

Copy it to the correct directory (example: on my machine this is
`/usr/share/fonts/X11/misc/`).

You may have to run `update-fonts-dir` in order to make this font appear in,
say, `xfontsel`. I manually added the entry to `fonts.dir` in the directory I
installed it in on my machine.

### Make the PCF work in X programs
First, copy the PCF file to the same location you copied the TTF and/or OTB's
to.
Then, in a file that loads your X server resources (like `~/.Xresources` on my
system), add a line like the following to make `xterm` use it. Other apps
probably follow similar guidelines:
````
xterm*font:   -gnu-IBM3161bitmap-medium-R-normal-sans-17-120-100-100-c-*-ISO10646-1
````

If you're trying to get this working and you log in in using CDE
(Common Desktop Environment), note that `~/.Xresources` is not sourced on login
by CDE. To load the resources, add the following line to `~/.dtprofile`.
````
xrdb -merge ~/.Xresources
````