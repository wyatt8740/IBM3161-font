#! /usr/local/bin/ksh93

shiftright()
{
  hex="0x""$1"
  shiftamt="$2"
  # shift right, convert back to hexadecimal (base 16)
  echo "obase=16;""$(( "$hex" >> "$shiftamt" ))" | bc
}

binarize()
{
  echo "ibase=16;obase=2;""$1" | bc
}

bindigitcount()
{
# check what the binary representation's number of bits is (position of MSb)
  echo "$(echo 'ibase=16;obase=2;'"$1" |  bc | wc -c)"" 1 -p" | dc
}

isinbitmap=0
i=0
while read line; do
  if [ "$i" -le 127 ]; then
    if [ "$isinbitmap" -eq 0 ]; then
      if [ "$line" = "BITMAP" ]; then
        isinbitmap=1
      fi
      echo "$line"
    else
      if [ "$line" != "ENDCHAR" -a "$line" != "ENDFONT" ]; then
        WRK="$line"
        #      if [ "$(bindigitcount "$line")" -eq 8 ]; then
        # we can safely shift over because I've not already done anything
        # manually via my HP calculator math -_-
        # (for narrow chars like i,j,l)
        echo "$(shiftright "$WRK" "1")"
        #      else
        #        echo "$WRK"
        #      fi
      else
        isinbitmap=0
        i="$(( i + 1 ))"
        echo "$line"
      fi
    fi
  else
    echo "$line"
  fi
done < "$1"
