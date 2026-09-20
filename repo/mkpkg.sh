#!/bin/sh
# mkpkg.sh — build a charge package (.tar.zst) and refresh repo/index.txt
#
# usage: ./mkpkg.sh <name> <version> <staging-dir> [tier] [deps]
#
#   <staging-dir> : a directory mirroring the target filesystem
#                   e.g. staging/usr/bin/nmap  staging/etc/nmap.conf
#   tier          : easy|moderate|hardware|reporting|gui   (default: easy)
#   deps          : comma-separated deps, or '-' for none  (default: -)
#
# Run on the build box (Linux or WSL2). Needs: tar with zstd, sha256sum.

set -e

name="$1"
version="$2"
dir="$3"
tier="${4:-easy}"
deps="${5:--}"

if [ "$#" -lt 3 ]; then
  echo "usage: $0 <name> <version> <staging-dir> [tier] [deps]" >&2
  exit 1
fi

[ -d "$dir" ] || { echo "error: staging dir '$dir' not found" >&2; exit 1; }
command -v zstd >/dev/null 2>&1 || { echo "error: zstd not installed" >&2; exit 1; }

self_dir="$(cd "$(dirname "$0")" && pwd)"
pkgfile="$name-$version.tar.zst"
outfile="$self_dir/$pkgfile"

########################################
# 1. write .PKGINFO metadata (skipped during extraction by charge)
########################################
{
  echo "name = $name"
  echo "version = $version"
  if [ "$deps" != "-" ]; then
    printf '%s\n' "$deps" | tr ',' '\n' | sed 's/^/depend = /'
  fi
} > "$dir/.PKGINFO"

########################################
# 2. pack: .PKGINFO + files (relative paths, no ./ prefix)
########################################
cd "$dir" || exit 1
rm -f "$outfile"
filelist="/tmp/$pkgfile.list"
find . -mindepth 1 -not -name .PKGINFO -printf '%P\n' > "$filelist"
tar --zstd -cf "$outfile" .PKGINFO --files-from "$filelist"
rm -f "$filelist"

########################################
# 3. sha256 + update index.txt (replace existing line, else append)
########################################
sha="$(sha256sum "$outfile" | awk '{print $1}')"
index="$self_dir/index.txt"

if [ -f "$index" ]; then
  sed -i "s|^$name .*|$name $version $pkgfile $sha $tier $deps|" "$index"
  grep -q "^$name " "$index" || echo "$name $version $pkgfile $sha $tier $deps" >> "$index"
else
  echo "$name $version $pkgfile $sha $tier $deps" > "$index"
fi

echo "packed: $outfile"
echo "sha256: $sha"
echo "now FTP-upload $pkgfile and index.txt to InfinityFree htdocs/"