I made this port using the source code contained in an Aminet package called
[bdf2amiga](http://aminet.net/package/text/font/bdf2amiga). I have included
the source code, since I had to make a few changes.

I replaced a call to `stricmp()` with a `strcasecmp()` call, since `stricmp()`
appears to be both non-standard and unavailable in most modern C libraries. 

I also had to edit the Makefile to remove some m68k-specific C and LD flags
to make it build a modern Linux/Unix binary.

The final thing i did was running the C program itself through 'iconv', to
change it from an ISO-8859 encoded file into a UTF-8 file. The program uses
the 'copyright' symbol, which was showing up wrong on my Unix system.

If using Windows, the copyright sign will look wrong when checking the version
number. Just deal with it. It has no impact on the conversion. Fixing it is
more trouble than it's worth.

The code is licensed under GPL v2, as per the original author.

I have yet to try this on my actual Amiga 500, mainly because it takes up a
bunch of space to set up, but also because I broke a trimmer in my Amiga
monitor while working on it the other day and am awaiting the delivery of
a new one before I can use it.
