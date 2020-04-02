/*
    bdf2amiga 1.3--BDF to Amiga font converter
    Copyright © 1998 Kriton Kyrimis

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if defined(AMIGA) && !defined(__amigaos__)
#define __amigaos__ 1
#endif

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "getopt.h"
#ifdef __amigaos__
#include <proto/dos.h>
#endif

#define VERSION "1.4"
#define PROGRAM_DATE "02.04.20"

/* #define streq(s1, s2) (stricmp(s1, s2) == 0) */
#define streq(s1, s2) (strcasecmp(s1, s2) == 0)
#define min(a, b) ((a) < (b) ? (a) : (b))
#ifdef __amigaos__
#define BASENAME(x)	FilePart((unsigned char *)(x))
#elif defined _WIN32
/*
 * 'rindex' isn't defined in Windows. Apparently even POSIX recommends strrchr
 * these days, but in the name of not messing older systems up I only define
 * this for win32.
 * Also, BASENAME needs to use backslashes to work as desired in win32.
 */
#define rindex(a,b) strrchr( (a), (b))
#define BASENAME(x)	(rindex((x),'\\') ? rindex((x),'\\')+1 : (x))
#else
/* Otherwise we're likely on a unixy system: */
#define BASENAME(x)	(rindex((x),'/') ? rindex((x),'/')+1 : (x))
#endif


static char version[] = "$VER: bdf2amiga " VERSION " (" PROGRAM_DATE ")";

void usage(char *);
int parse(void);
int splitString(char *, char *, char *, char *, char *, char *);
void stripQuotes(char *);
void copyString(char *, char *, int *);
void error(void);
unsigned char *getBytes(int *);
void *xmalloc(size_t);

static int lineNo = 0;
static char ofile[256];
static char *ifile = NULL;
static FILE *in, *out;
static int useIncludes = 1;
static int maxSize = 0;
static char buf[256], fName[80], weight[16], slant[16], spacing[16];
static char dirName[256];
static char cmd[80], arg1[80], arg2[80], arg3[80], arg4[80];

struct fontData {
  int dwidth;
  int width;
  int height;
  int offX;
  int offY;
  int nBytes;
  unsigned char *bitmap;
};

int
main(int argc, char *argv[])
{
  int c, status, last;
  char *prog = BASENAME(argv[0]);

  fName[0] = '\0';
  dirName[0] = '\0';
  while ((c = getopt(argc, argv, "d:f:hiv")) != -1) {
    switch(c) {
      case 'd':
        strcpy(dirName, optarg);
	last = strlen(dirName) - 1;
#ifdef __amigaos__
	if (dirName[last] != '/' && dirName[last] != ':')
#else
	if (dirName[last] != '/')
#endif
	{
	  strcat(dirName, "/");
	}
	break;
      case 'f':
        strcpy(fName, optarg);
	break;
      case 'h':
        usage(prog);
	exit(0);
      case 'i':
        useIncludes = 0;
	break;
      case 'v':
        printf(
	  "bdf2amiga " VERSION "--BDF to Amiga font converter\n"
	  "Copyright © 1998 Kriton Kyrimis\n"
	  "\n"
	  "This program is free software; you can redistribute it\n"
	  "and/or modify it under the terms of the GNU General\n"
	  "Public License as published by the Free Software Foundation;\n"
	  "either version 2 of the License, or (at your option)\n"
	  "any later version.\n"
	);
	exit(0);
      default:
        usage(prog);
	exit(20);
    }
  }
  argc -= optind;
  argv += optind;

  if (argc != 1) {
    usage(prog);
    exit(20);
  }
  ifile = argv[0];
#if defined _WIN32
  in = fopen(ifile, "rb"); // Do Amiga C libraries handle the 'b' gracefully?
#else
  in = fopen(ifile, "r");
#endif
  if (in == NULL) {
    fprintf(stderr, "Can't open %s for input\n", ifile);
    exit(20);
  }

  status = parse();

  fclose(out);
  fclose(in);
  exit(status);
}

void
usage(char *prog)
{
  printf(
    "Usage:\n"
    "%s [-d OutputDirectory] [-f FamilyName] [-h] [-v] BDFfile\n"
    "\n"
    "-d OutputDirectory\n"
    "   place output in the specified directory\n"
    "-f FamilyName\n"
    "   override font family name, or supply one if it is missing\n"
    "-h\n"
    "   display this text\n"
    "-i\n"
    "   do not generate references to Amiga include files\n"
    "-v\n"
    "   display program version\n", prog
  );
}

int
parse()
{
  int i, j, n;
  int size = 0, maxWidth = 0, maxHeight = 0, offX = 0, offY = 0;
  int currentChar, endFont = 0;
  int loChar, hiChar, totalWidth, modulo, curPos;
  int startRow, endRow, bitsLeft, startPos, curByte;
  int useBits, needBits, appendPos;
  int monoSpace = 1, fillRight = 0;
  unsigned char **bits, byte;
  static struct fontData inputData[256];

  weight[0] = '\0';
  slant[0] = '\0';
  spacing[0] = '\0';

  for (i=0; i<256; i++) {
    inputData[i].dwidth = 0;
    inputData[i].width = 0;
    inputData[i].height = 0;
    inputData[i].offX = 0;
    inputData[i].offY = 0;
    inputData[i].nBytes = 0;
    inputData[i].bitmap = NULL;
  }

  if (fgets(buf, sizeof(buf), in) == NULL) {
    fprintf(stderr, "This is not a bdf file\n");
    return 20;
  }
  lineNo++;
  if (splitString(buf, cmd, arg1, arg2, arg3, arg4) != 1 ||
      !streq(cmd, "STARTFONT")) {
    fprintf(stderr, "This is not a bdf file\n");
    return 20;
  }
  
  while (fgets(buf, sizeof(buf), in) != NULL) {
    lineNo++;
    n = splitString(buf, cmd, arg1, arg2, arg3, arg4);
    if (streq(cmd, "SIZE")) {
      if (n !=  3) {
	error();
      }
      size = atoi(arg1);
    }
    if (streq(cmd, "FONTBOUNDINGBOX")) {
      if (n !=  4) {
	error();
      }
      maxWidth = atoi(arg1);
      maxHeight = atoi(arg2);
      offX = atoi(arg3);
      offY = atoi(arg4);
      if ((maxWidth % 8) == 0) {
        maxSize = (maxWidth / 8) * maxHeight;
      }else{
        maxSize = ((maxWidth / 8) + 1) * maxHeight;
      }
    }
    if (streq(cmd, "FAMILY_NAME")) {
      if (n != 1) {
	error();
      }
      if (fName[0] == '\0') {
	strcpy(fName, arg1);
      }
    }
    if (streq(cmd, "WEIGHT_NAME")) {
      if (n != 1) {
	error();
      }
      strcpy(weight, arg1);
    }
    if (streq(cmd, "SLANT")) {
      if (n != 1) {
	error();
      }
      strcpy(slant, arg1);
    }
    if (streq(cmd, "SPACING")) {
      if (n != 1) {
	error();
      }
      strcpy(spacing, arg1);
    }
    if (streq(cmd, "ENDPROPERTIES")) {
      if (n != 0) {
        error();
      }
      break;
    }
  }

  /* Supply reasonable defaults for missing values */
  if (weight[0] == '\0') {
    strcpy(weight, "Medium");
  }
  if (slant[0] == '\0') {
    strcpy(slant, "R");
  }
  if (spacing[0] == '\0') {
    strcpy(spacing, "M");
  }

  /* Can't do much if these values are missing */
  if (fName[0] == '\0') {
    fprintf(stderr,
      "Missing family name. Please supply one using the -f option.\n");
    exit(20);
  }
  if (size == 0) {
    fprintf(stderr, "Missing font size\n");
    exit(20);
  }
  if (maxSize == 0) {
    fprintf(stderr, "Missing font bounding box\n");
    exit(20);
  }

  if (streq(spacing, "P")) {
    monoSpace = 0;
  }

  while (fgets(buf, sizeof(buf), in) != NULL) {
    lineNo++;
    n = splitString(buf, cmd, arg1, arg2, arg3, arg4);
    if (streq(cmd, "ENDFONT")) {
      if ( n != 0) {
	error();
      }
      endFont = 1;
      break;
    }
    if (streq(cmd, "STARTCHAR")) {
      if ( n != 1) {
	error();
      }
      currentChar = -1;
      while (fgets(buf, sizeof(buf), in) != NULL) {
	lineNo++;
	n = splitString(buf, cmd, arg1, arg2, arg3, arg4);
	if (streq(cmd, "ENCODING")) {
	  if (n != 1) {
	    error();
	  }
	  currentChar = atoi(arg1);
	}
	if (streq(cmd, "DWIDTH")) {
	  if (n != 2) {
	    error();
	  }
	  inputData[currentChar].dwidth = atoi(arg1);
	}
	if (streq(cmd, "BBX")) {
	  if (n != 4) {
	    error();
	  }
	  inputData[currentChar].width = atoi(arg1);
	  inputData[currentChar].height = atoi(arg2);
	  inputData[currentChar].offX = atoi(arg3);
	  /* Shouldn't happen */
	  if (monoSpace && inputData[currentChar].offX < 0) {
	    inputData[currentChar].offX = 0;
	  }
	  inputData[currentChar].offY = atoi(arg4);
	}
	if (streq(cmd, "BITMAP")) {
	  if (n != 0) {
	    error();
	  }
	  inputData[currentChar].bitmap =
	    getBytes(&(inputData[currentChar].nBytes));
	  break;
	}
      }
    }
  }

  if (!endFont) {
    fprintf(stderr, "Premature end of file encountered\n");
    exit(20);
  }

  loChar = INT_MAX;
  hiChar = INT_MIN;
  for (i=0; i<256; i++) {
    if (inputData[i].bitmap) {
      if (i > hiChar) {
        hiChar = i;
      }
      if (i < loChar) {
        loChar = i;
      }
    }
  }

  /* Monospace fonts without a space character behave strangely.
     (This is possibly also true for proportional fonts,
     but I can't check this easily. Besides, for monospace fonts it is easy to
     add extra empty characters.
  */
  if (monoSpace) {
    if (loChar > ' ') {
      loChar = ' ';
    }else{
      if (hiChar < ' ') {  /* Of what use would *this* font be? */
        hiChar = ' ';
      }
    }
  }

  if (monoSpace) {
    totalWidth = maxWidth * (hiChar - loChar + 1);
  }else{
    for (i=loChar, totalWidth=0; i<=hiChar; i++) {
      if (inputData[i].bitmap) {
	totalWidth += inputData[i].width;
      }
    }
  }

  modulo = totalWidth / 8;
  if (totalWidth % 8) {
    modulo++;
  }
  if (modulo % 2) {
    modulo++;
  }

  bits = (unsigned char **)xmalloc(maxHeight * sizeof(unsigned char *));
  for (i=0; i<maxHeight; i++) {
    bits[i] = xmalloc(modulo * sizeof(unsigned char));
  }
  for (i=0; i<maxHeight; i++) {
    for (j=0; j<modulo; j++) {
      bits[i][j] = 0;
    }
  }

  /* Bit-pack the glyphs together, as described in the manual.
     This code was contributed by Trial N. Error.
  */
  for (i=loChar, curPos=0; i<=hiChar; i++) {
    if (!inputData[i].bitmap) {
      if (monoSpace) {
        curPos += maxWidth;
      }
      continue;
    }
    if (monoSpace) {
      curPos += inputData[i].offX;
      fillRight = maxWidth - inputData[i].offX - inputData[i].width;
      if (fillRight < 0) {  /* Shouldn't happen */
        fillRight = 0;
      }
    }
    startRow = maxHeight + offY - inputData[i].offY - inputData[i].height;
    endRow = startRow + inputData[i].height - 1;
    assert(startRow >= 0 && startRow < maxHeight);
    assert(endRow >= 0 && endRow < maxHeight);
    curByte = 0;
    for (j=startRow; j<=endRow; j++) {
      bitsLeft = inputData[i].width;
      startPos = curPos;
      while (bitsLeft > 0) {
	useBits = min(8, bitsLeft);
	needBits = 8 - (startPos % 8);
	appendPos = startPos / 8;
	if (needBits == 8) {
	  assert (appendPos >= 0 && appendPos < modulo);
	  assert(curByte >=0 && curByte < inputData[i].nBytes);
	  bits[j][appendPos] = inputData[i].bitmap[curByte];
	}else{
	  assert (appendPos >= 0 && appendPos < modulo);
	  byte = inputData[i].bitmap[curByte] >> (8 - needBits);
	  bits[j][appendPos] |= byte;
	  if (bitsLeft - needBits > 0) {
	    assert (appendPos >= 0 && appendPos < modulo-1);
	    byte = inputData[i].bitmap[curByte] << needBits;
	    bits[j][appendPos+1] |= byte;
	  }
	}
	bitsLeft -= useBits;
	curByte++;
	startPos += useBits;
      }
    }
    curPos += inputData[i].width;
    if (monoSpace) {
      curPos += fillRight;
    }
  }

  sprintf(ofile, "%s%s%d", dirName, fName, maxHeight);
  if (streq(weight, "SemiBold") || streq(weight, "Bold") ||
      streq(weight, "Heavy") || streq(weight, "ExtraBold") ||
      streq(weight, "ExtraHeavy")) {
    strcat(ofile, "b");
  }
  if (streq(slant, "I")) {
    strcat(ofile, "i");
  }
  strcat(ofile, ".a");
#if defined _WIN32
  out = fopen(ofile, "wb"); // Do Amiga C libraries handle the 'b' gracefully?
#else
  out = fopen(ofile, "w");
#endif
  if (out == NULL) {
    fprintf(stderr, "Can't open %s for output\n", ofile);
    fclose(in);
    exit(20);
  }

  if (useIncludes) {
    fprintf(out, "\tINCLUDE\t\"exec/types.i\"\n");
    fprintf(out, "\tINCLUDE\t\"exec/nodes.i\"\n");
    fprintf(out, "\tINCLUDE\t\"libraries/diskfont.i\"\n");
  }else{
    fprintf(out, "NT_FONT\t\t\tEQU\t12\n");
    fprintf(out, "DFH_ID\t\t\tEQU\t$0F80\n");
    fprintf(out, "MAXFONTNAME\t\tEQU\t32\n");
    fprintf(out, "FPF_DESIGNED\t\tEQU\t$40\n");
    fprintf(out, "FPF_DISKFONT\t\tEQU\t$02\n");
    fprintf(out, "FPF_PROPORTIONAL\tEQU\t$20\n");
  }
  fprintf(out, "\n");
  fprintf(out, "\tSECTION\ttext,code\n");
  fprintf(out, "\n");
  fprintf(out, "\tMOVEQ\t#-1,D0\n");
  fprintf(out, "\tRTS\n");
  fprintf(out, "\n");
  fprintf(out, "\tDC.L\t0\n");
  fprintf(out, "\tDC.L\t0\n");
  fprintf(out, "\tDC.B\tNT_FONT\n");
  fprintf(out, "\tDC.B\t0\n");
  fprintf(out, "\tDC.L\tfontName\n");
  fprintf(out, "\n");
  fprintf(out, "\tDC.W\tDFH_ID\n");
  fprintf(out, "\tDC.W\t1\n");
  fprintf(out, "\tDC.L\t0\n");
  fprintf(out, "\n");
  fprintf(out, "fontName:\n");
  fprintf(out, "\tDC.B\t\"%s%d", fName, maxHeight);
  if (streq(weight, "Bold")) {
    fprintf(out, "b");
  }
  if (streq(slant, "I")) {
    fprintf(out, "i");
  }
  fprintf(out, "\"\n");
  fprintf(out, "\n");
  fprintf(out, "length\tEQU\t*-fontName\n");
  fprintf(out, "\tDCB.B\tMAXFONTNAME-length,0\n");
  fprintf(out, "font:\n");
  fprintf(out, "\tDC.L\t0\n");
  fprintf(out, "\tDC.L\t0\n");
  fprintf(out, "\tDC.B\tNT_FONT\n");
  fprintf(out, "\tDC.B\t0\n");
  fprintf(out, "\tDC.L\tfontName\n");
  fprintf(out, "\tDC.L\t0\n");
  fprintf(out, "\tDC.W\t0\n");
  fprintf(out, "\tDC.W\t%d\n", maxHeight);
  fprintf(out, "\tDC.B\t0");
  if (streq(slant, "I")) {
    fprintf(out, "|FSF_ITALIC");
  }
  if (streq(weight, "Bold")) {
    fprintf(out, "|FSF_BOLD");
  }
  fprintf(out, "\n");
  fprintf(out, "\tDC.B\tFPF_DESIGNED|FPF_DISKFONT");
  if (streq(spacing, "P")){
    fprintf(out, "|FPF_PROPORTIONAL");
  }
  fprintf(out, "\n");
  fprintf(out, "\tDC.W\t%d\n", maxWidth);
  fprintf(out, "\tDC.W\t%d\n", maxHeight-1+offY);
  fprintf(out, "\tDC.W\t1\n");
  fprintf(out, "\tDC.W\t0\n");
  fprintf(out, "\tDC.B\t%d\n", loChar);
  fprintf(out, "\tDC.B\t%d\n", hiChar);
  fprintf(out, "\tDC.L\tfontData\n");
  fprintf(out, "\tDC.W\t%d\n", modulo);
  fprintf(out, "\tDC.L\tfontLoc\n");
  fprintf(out, "\tDC.L\tfontSpace\n");
  fprintf(out, "\tDC.L\tfontKern\n");
  fprintf(out, "\n");
  fprintf(out, "fontData:\n");
  for (i=0; i<maxHeight; i++) {
    for (j=0; j<modulo; j++) {
      fprintf(out, "\tDC.B\t$%02X\n", bits[i][j]);
    }
  }
  fprintf(out, "\n");
  fprintf(out, "fontLoc:\n");
  for (i=loChar, curPos=0; i<=hiChar; i++) {
    if (monoSpace) {
      fprintf(out, "\tDC.L\t$%04X%04X\n", curPos, maxWidth);
      curPos += maxWidth;
    }else{
      if (inputData[i].bitmap) {
	fprintf(out, "\tDC.L\t$%04X%04X\n", curPos, inputData[i].width);
	curPos += inputData[i].width;
      }else{
	fprintf(out, "\tDC.L\t$%04X%04X\n", curPos, 0);
      }
    }
  }
  fprintf(out, "\n");
  fprintf(out, "fontSpace:\n");
  for (i=loChar; i<=hiChar; i++) {
    if (monoSpace) {
      fprintf(out, "\tDC.W\t%d\n", maxWidth);
    }else{
      if (inputData[i].bitmap) {
	fprintf(out, "\tDC.W\t%d\n", inputData[i].dwidth);
      }else{
	fprintf(out, "\tDC.W\t%d\n", 0);
      }
    }
  }
  fprintf(out, "\n");
  fprintf(out, "fontKern:\n");
  for (i=loChar; i<=hiChar; i++) {
    if (monoSpace) {
      fprintf(out, "\tDC.W\t%d\n", 0);
    }else{
      if (inputData[i].bitmap) {
	fprintf(out, "\tDC.W\t%d\n", inputData[i].offX);
      }else{
	fprintf(out, "\tDC.W\t%d\n", 0);
      }
    }
  }
  fprintf(out, "\n");
  fprintf(out, "fontEnd:\n");
  fprintf(out, "\tEND\n");

  return 0;
}

int
splitString(char *buf, char *cmd,
	    char *arg1, char *arg2, char *arg3, char *arg4)
{
  int ptr = 0;

  cmd[0] = 0;
  arg1[0] = 0;
  arg2[0] = 0;
  arg3[0] = 0;
  arg4[0] = 0;

  while (isspace(buf[ptr])) {
    ptr++;
  }
  if (buf[ptr] == '\0') {
    return 0;
  }

  copyString(buf, cmd, &ptr);

  while (isspace(buf[ptr])) {
    ptr++;
  }
  if (buf[ptr] == '\0') {
    return 0;
  }

  copyString(buf, arg1, &ptr);

  while (isspace(buf[ptr])) {
    ptr++;
  }
  if (buf[ptr] == '\0') {
    return 1;
  }

  copyString(buf, arg2, &ptr);

  while (isspace(buf[ptr])) {
    ptr++;
  }
  if (buf[ptr] == '\0') {
    return 2;
  }

  copyString(buf, arg3, &ptr);

  while (isspace(buf[ptr])) {
    ptr++;
  }
  if (buf[ptr] == '\0') {
    return 3;
  }

  copyString(buf, arg4, &ptr);

  return 4;
}

void
copyString(char *s1, char *s2, int *ptr)
{
  int i;

  if (s1[*ptr] == '"') {
    (*ptr)++;
    for (i=0; s1[*ptr] && s1[*ptr] != '"'; i++,(*ptr)++) {
      s2[i] = s1[*ptr];
    }
    s2[i] = '\0';
    if (s1[*ptr] == '"') {
      (*ptr)++;
    }
  }else{
    for (i=0; s1[*ptr] && !isspace(s1[*ptr]); i++,(*ptr)++) {
      s2[i] = s1[*ptr];
    }
    s2[i] = '\0';
  }
}

void
error()
{
  fprintf(stderr, "Error at line %d in %s: %s\n", lineNo, ifile, buf);
  exit(20);
}

unsigned char *
getBytes(int *nBytes)
{
  static unsigned char *tmpBytes = NULL;
  int n;
  unsigned char *returnBytes, *ptr;

  if (!tmpBytes) {
    tmpBytes = xmalloc(maxSize);
  }
  *nBytes = 0;
  while (fgets(buf, sizeof(buf), in) != NULL) {
    lineNo++;
    n = splitString(buf, cmd, arg1, arg2, arg3, arg4);
    if (n != 0) {
      error();
      exit(20);
    }
    if (streq(cmd, "ENDCHAR")) {
      break;
    }else{
      for (ptr=cmd; *ptr; ptr+=2) {
	unsigned int tmp;
	sscanf(ptr, "%2x", &tmp);
	tmpBytes[(*nBytes)++] = (tmp & 0xFF);
      }
    }
  }
  returnBytes = xmalloc(*nBytes);
  memcpy(returnBytes, tmpBytes, *nBytes);
  return returnBytes;
}

void *xmalloc(size_t n)
{
  void *ptr = malloc(n);

  if (!ptr) {
    fprintf(stderr, "Can't allocate %d bytes\n", n);
    exit(20);
  }
  return ptr;
}
