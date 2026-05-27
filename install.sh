#!/usr/bin/env bash
set -e

PREFIX="${PREFIX:-/usr/local}"
BINDIR="$PREFIX/bin"
DATADIR="$PREFIX/share/myfetch"
MANDIR="$PREFIX/share/man/man1"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if ! command -v gcc >/dev/null 2>&1; then
    echo "Error: gcc not found. Install gcc and try again." >&2
    exit 1
fi

echo "Building myfetch..."
gcc -O2 -Wall -Wextra "$SCRIPT_DIR/myfetch.c" -o "$SCRIPT_DIR/myfetch"

echo "Installing to $PREFIX ..."
install -Dm755 "$SCRIPT_DIR/myfetch" "$BINDIR/myfetch"
if [ -f "$SCRIPT_DIR/ascii.txt" ]; then
    install -Dm644 "$SCRIPT_DIR/ascii.txt" "$DATADIR/ascii.txt"
fi
if [ -f "$SCRIPT_DIR/myfetch.1" ]; then
    install -Dm644 "$SCRIPT_DIR/myfetch.1" "$MANDIR/myfetch.1"
fi

echo ""
echo "Installed: $BINDIR/myfetch"
echo ""
echo "To use your own ASCII art:"
echo "mkdir -p ~/.config/myfetch"
echo "p /path/to/your/ascii.txt ~/.config/myfetch/ascii.txt"
echo ""
echo "Run 'myfetch -h' for all options."
