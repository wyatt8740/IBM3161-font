This is a very basic repository for my IBM 3161 ASCII terminal font.

# Contents
  - [Introduction](#introduction) 
  - [Formats](#formats)
  - [Installation](#installation)
    - [Windows Installation](#windows-installation)
    - [Unix/X11 Installation](#unixx11-installation)
      - [X11 OTB/PCF Installation](#x11-otbpcf-installation)
      - [X11 TrueType Installation](#x11-truetype-installation)
      - [Linux Console (TTY) Installation](#linux-console-installation)
      - [FreeBSD Console Installation](#freebsd-console-installation)
    - [Mac OS X ('MacOS') Installation](#mac-os-x-macos-installation)
    - [Minecraft Installation](#minecraft-installation)
    - [Rockbox Installation](#rockbox-installation)
    - [GNU Emacs Installation](#gnu-emacs-installation)
  - [How I Made It](#how-i-made-it)
  - [Still Not Done](#still-not-done)
  - [Licensing](#licensing)

### [Introduction](#contents)
![Screenshot of the ASCII set on my terminal](http://i.imgur.com/f2eG8H8.png)

![Photo of IBM 3161 terminal with amber CRT playing the text-based adventure gam
e, Zork](http://i.imgur.com/MkboCJs.jpg)

This repository contains a bitmap version of the font as well as a (slightly 
less complete) truetype version created through GNU Unifont's "tracing"
program which it uses to create its truetype versions. The truetype covers 0x00
through 0xFF (the basic ASCII set and first 'extended' bank), while I have
extended the font to cover through about 0x17F in the bitmap versions. Beyond
these thresholds, the font more or less falls back on the glyphs from Unifont,
which *almost* match the IBM font glyphs where applicable. This also means that
I can use the font for other languages like Japanese, which is handy in a
terminal if you are from an area of the world that does not use the latin
alphabet as much or when dealing with files from users of another language
in a terminal window.

Creating the truetype to the extent I already have took a lot of effort, due to
some flawed decision making early on in the process; while I plan to do better
going forward, the fact remains that fixing/translating the bitmap glyphs to
truetype is a lot of effort – too much for something I rarely use. All of the
essentials are still in the TTF, anyway, and I expect that I may slowly
continue to improve the TrueType going forward.

I also have included the Fontforge Spline Font Database (SFD) files, which
are uncompressed text (and thus pretty big; sorry). If you just want to use the
font, I therefore suggest downloading the zip on the 'releases' page instead of
cloning the repo.

I used GNU Unifont as a base because it looks somewhat similar and covers a lot
of Unicode above the basic ASCII set my IBM terminal supports. Additionally,
its sources contained some useful tools for automatically performing
transformations on fonts.

### [Formats](#contents)
Since I first created this font, I have been finding more and more formats I can
convert it to! Whenever I find another format to port to, I do so and then add
a commit here. Most recently, an Amiga version (made in Workbench 1.3 on an
Amiga 500), as well as Minecraft, Rockbox, and an X11 emacs-specific font
(to fix strange rendering issues) have been added.

This font is also available in a Minecraft 1.11-and-up compatible resource pack.
With minor tweaking it should also work for earlier and future versions.
Conveniently, Minecraft uses GNU Unifont for unicode text, which is also the
base for my font - this made it simple to add by modifying some tools included
with the Unifont source distribution to export the font.

![Photo of font on an iPod running Rockbox](https://i.imgur.com/UsE8YDw.jpg)

## [Installation](#contents)
This depends on your platform, but I will try to lay out what works where.
For some exotic formats (e.g. Minecraft, Rockbox) the directions are in the
ports' respective subdirectories.

##### Why install both the bitmap and TrueType versions?
On Unix/Linux and Windows platforms the bitmap versions benefit from not
getting any software subpixel rendering attempted on them like the TrueType
font does. This is probably true in OS X as well, but I only tested the .dfont
version of the font in OS X on account of not actually having regular access to
OS X. (the dfont is a bitmap-only font, so it probably won't be subpixel
rendered.)

### [Windows Installation](#contents)
On Windows, you will want the windows bitmap .fon file and/or the TrueType font
(.ttf). They can be copied manually to `C:\Windows\Fonts`, or installed from
the font previewer.

### [Unix/X11 Installation](#contents)
In (non-OS X/NextSTEP) Unix and Linux distros with X11, you'll want the
OpenType bitmap (.otb) and/or the TrueType font (.ttf).

There is also a PCF font (old-school X11 font) that it may be a good
idea to install for programs like `xterm` and the GUI version of `emacs`.

Installing all three versions (TTF, OTB, and PCF) lets you use the font with
nearly any program on a Unix system. The TTF and OTB can be selected with most
'font chooser' GUI programs, while the PCF is useful for traditional X11
programs like `xterm` and is usually set through a text configuration file
like `~/.Xresources` or `~/.emacs`.

I think this will be the same on the BSD's with X11 as it is for Linux distros,
since they share a codebase for their X servers and FreeType.

It definitely is the same in OpenBSD and FreeBSD, where I have tried it
personally.

#### [X11 OTB/PCF Installation](#contents)
The instructions for installing the X11 PCF font are in the
[X11/README.md](X11/README.md) file.

#### [X11 TrueType Installation](#contents)
Copy the .ttf file to either a local font path (such as `~/.fonts`, or to a
system-wide font path (such as /usr/share/fonts/). These paths are probably
distro and OS dependant.

#### [Linux Console Installation](#contents)
This only for sure applies to Debian and derivatives. It might be different on
distros that I have not tried, but I bet it works there, too.
Copy `linux-console/IBM3161.psf` to `/usr/share/consolefonts/IBM3161.psf`.
Then, edit `/etc/default/console-setup`, and add/change the `FONT=` line to
read:

    FONT="IBM3161.psf"

You're done. You might have to reboot for it to show everywhere.

The `setfont` command line program should also be able to apply it on a
specific TTY on a one-off basis.

#### [FreeBSD Console Installation](#contents)
To-do: write up the FreeBSD console installation. I currently haven't gotten
it to change in every terminal, just the primary one that you watch the boot
process in. I am hoping I don't have to recompile the kernel to get it
everywhere.

I assure you, it does work in FreeBSD, though.

### [Mac OS X ('MacOS') Installation](#contents)
On Apple systems (OS X), you'll want to use the '.dfont' file. If that fails,
try the .ttf or the .pt3 (PS type 3). IIRC, the installation process was
relatively intuitive.

I don't yet know how it would be installed on "classic" Mac OS. If anyone has
a System 7 or lower floppy which I can use to start up my Mac Plus, please let
me know about it (Or you could send me a better classic mac!)

### [Minecraft Installation](#contents)
Instructions to install the Minecraft version of the font are under the
`minecraft/` directory.

### [Rockbox Installation](#contents)
Rockbox is an alternative firmware for many mp3 players. It has been tested on
my iPod Classic. Instructions are of course in the `rockbox/` directory.

I cannot support this port anymore until I can get a new CompactFlash card, as
the one in my iPod just failed and the hard drive failed many years ago. Feel
free to shoot me a question, but I might not be able to answer it.

### [GNU Emacs Installation](#contents)
There are two main ways to go about this. The first is to just use the truetype
font. If I remember correctly, that spaces out everything correctly.

The regular OTB version gets all characters padded out to 16px, regardless of
their actual width. On account of this, it is not recommended.

However, the OTB file `8wide/IBM3161APL.otb` should work. This is (if I
remember correctly) named as such because it is derived from the APL variant of
Unifont. I discovered that the APL variant of Unifont does seem to render
correctly in emacs, and so I made a similar variant of it for my own font.

I am not sure if it's actually because it's the APL variant, or because maybe
emacs tries to prioritize `pcf` style fonts over `otb` ones. Or maybe something
else FreeType related. But whatever the case may be, if you install
`IBM3161APL.otb` somewhere (I have it in `~/.fonts/`), you should be able to
make it work in GNU emacs with `m-x customize`, or else manually add it to
`custom-set-faces` in your emacs initialization file (usually `~/.emacs` or
`~/.emacs.d/init.el`).

An example (from my `custom-set-faces` elisp) follows.

The most important line to focus on is the one containing the string
`IBM3161APL`. The rest is just provided for context. There's almost nothing I
dislike as much as insufficient context when looking at code samples,
especially in unfamiliar languages.

    (custom-set-faces
     ;; custom-set-faces was added by Custom.
     ;; If you edit it by hand, you could mess it up, so be careful.
     ;; Your init file should contain only one such instance.
     ;; If there is more than one, they won't work right.
     '(default ((t (:family "IBM3161APL" :foundry "PfEd" :slant normal :weight normal :height 128 :width normal))))
     '(term-color-blue ((t (:background "#2272D9" :foreground "#2272D9"))))
     '(term-color-cyan ((t (:background "#06989A" :foreground "#06989A"))))
     '(term-color-green ((t (:background "#4E9A06" :foreground "#4E9A06"))))
     '(term-color-magenta ((t (:background "#A168AB" :foreground "#A168AB"))))
     '(term-color-red ((t (:background "#CC0000" :foreground "#CC0000"))))
     '(term-color-white ((t (:background "#D3D7CF" :foreground "#D3D7CF"))))
     '(term-color-yellow ((t (:background "#C4A000" :foreground "#C4A000"))))
     '(tooltip ((t (:inherit tooltip-ui :background "lightyellow" :foreground "black")))))

If someone knows why this `IBM3161APL` variant actually works when the regular
OTB doesn't, please tell me about it. I really would like to know!

## [How I Made It](#contents)
I screen-scraped the font from my IBM 3161 by hand via its built-in "test"
mode, where it prints out its entire ASCII character set on a single screen. As
a result, I cannot guarantee it to be 100% accurate in terms of spacing between
characters, but I can say that I personally have yet to notice anything wrong
with it. I have used it in my terminal emulator exclusively for five months now.

You can find some of the images I used [here](http://imgur.com/a/XHyDb) (though
I have others that I did with a different amber CRT; I still need to dig those
files up).

As such, I am as confident as one can be when using guesswork and (to an extent)
tracing the letters in CAD and transforming my photos of the screen to correct
for the CRT's curvature. I am certain that the vertical pixels are accurate, as
I can make out individual scans of the electron beam in my photos. I am
relatively certain that the horizontal pixels are accurate based on common
sense, the fact that most characters would look wildly incorrect if shifted
over a pixel, and from some trigonometry I did using the CAD tracings. Character
0x7F is a checkerboard pattern, which made it easy for me to find out what the
aspect ratio of each 'pixel' is on the CRT near the center of the display.

If I had a ROM dumper, I could also dump the ROM's within the terminal, as they
are all socketed. This may happen in the future, but I have been using this
font within my terminal emulators and in other places for the last year and not
yet found any problems.

If you want to hack on this yourself, I'd suggest using the .hex file as a
jumping-off point if you want to be able to generate truetype versions. Merge
it with the hex sources of Unifont.

## [Still Not Done](#contents)
My font does not currently supply most of the non-ASCII "Extended" characters
that are in the real 3161 (characters above `0x7F`). Even those characters that
have unicode equivalents do not have them associated with the same 'numbers'
as they have in the terminal, and I am not clever/talented/obsessive enough to
map those all out since I never use them. Additionally, some characters do not
have a unicode equivalent that I am aware of at all.

I would also like to try to turn the 3161 part of it (not the whole of Unifont)
into a proper TTF font, meaning one that doesn't look pixellated but is made
using vectors for the various angles in the typeface. If you want an example of
somebody who has done this for a similar (but not identical!) font, see
[rbanffy/3270font](https://github.com/rbanffy/3270font).

If anyone wants to add them, I will point you to [the photos of the terminal's
screen in its 'testing' mode,](http://imgur.com/a/XHyDb) which contains a full
printout of all of the characters the terminal is capable of displaying. Feel
free to send me a pull request!

## [Licensing](#contents)
These font files are licensed under the GNU General Public License (where
applicable), either version 2 or (at your option) a later version, with the
exception that embedding the font in a document does not in itself constitute a
violation of the license. The full terms of the license are in GNU Unifont's
[LICENSE.txt](http://unifoundry.com/LICENSE.txt). This license is inherited
because I have based my font on GNU Unifont for nearly all non-ASCII characters.

If you wish to use only the glyphs that I created (0x00-0x7F in the truetype, or
0x00-0xFF plus unicode superscript and subscript numerical digits in the
bitmap), those glyphs are licensed (at your option) under the GNU GPL version 2
or later (with the aforementioned exception regarding embedding), the
[SIL Open Font License, version 1.1](http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL#5667e9e4), 
or the 3-Clause BSD License, which is reproduced
below. Additionally, contact me if you wish to use it in another project or
product under incompatible terms and I promise I'll be accommodating/not a jerk
about it. It might even be free.

Copyright © 2017-2023 Wyatt Ward

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----
--[(Back to top)](#contents)--