THIS IS A HACKY QUICK AND BAD CONVERSION.

It has undergone some manual bit-shifting in the BDF to produce a borderline
acceptable font for FreeBSD vt terminals.

Letter spacing and vertical positionings will likely be wrong for MANY
characters. Basically, don't use this yet. It needs a different approach on
my end.

# Installation

copy the .fnt file to `/usr/share/vt/fonts/ibm3161-8x16.fnt`.

run:

    vidcontrol -f ibm3161-8x16.fnt

Set in `rc.conf`:

    font8x16="ibm3161-8x16"

See the manpage for vidcontrol(1) for more details on rc.conf settings and how
to set the font properly, if this doesn't work well enough for you.

