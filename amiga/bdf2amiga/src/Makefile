#    bdf2amiga 1.3--BDF to Amiga font converter
#    Copyright © 1998 Kriton Kyrimis
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

CC = gcc
CFLAGS = -O2 -fomit-frame-pointer
LDFLAGS = -s

PROG = bdf2amiga
OBJ = bdf2amiga.o getopt.o

$(PROG): $(OBJ)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJ)

$(PROG).man: $(PROG).1
	groff -Tascii -man $(PROG).1 >$(PROG).man
	
clean:
	rm -f $(OBJ) $(PROG) $(PROG).man
