COLWIDTH=8
SHOWTEXT=true

tmpfile=$(mktemp)
cp "${1:-/dev/stdin}" "$tmpfile"
left=$(set -o pipefail; iconv -f utf8 -t utf32le "$tmpfile" | hexdump -v -e $COLWIDTH'/4 "0x%05x " "\n"' | sed -re"s/0x /   /g")


if [ $? -gt 0 ]; then
    echo "ERROR: Could not convert input" >&2
elif $SHOWTEXT; then
    right=$(tr [:space:] . < "$tmpfile" | sed -re "s/.{$COLWIDTH}/|&|\n/g" | sed -re "s/^.{1,$((COLWIDTH+1))}\$/|&|/g")
    pr -mts" " <(echo "$left") <(echo "$right")
else
    echo "$left"
fi


rm "$tmpfile"