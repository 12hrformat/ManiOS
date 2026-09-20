# Mani Linux package repo (charge)

Static files, served by **InfinityFree** — free hosting that works even
while the GitHub repo is still private. No server-side code: charge does
all the work client-side, this folder is just a plain HTTP directory.

```
repo/
  index.txt              # price list (name -> file + sha256 + deps)
  *.tar.zst              # the packages
  mkpkg.sh               # build helper (run on your build box)
```

## One-time InfinityFree setup

1. Sign up at **infinityfree.com** (free account)
2. Create a hosting account, pick a subdomain — e.g. `manilinux.infinityfreeapp.com`
3. The control panel shows your FTP details:
   - host: `ftpupload.net`, port 21
   - user / pass: your InfinityFree credentials
   - web root: `htdocs/`
4. Upload `index.txt` and every `<name>-<version>.tar.zst` into `htdocs/`

On Mani Linux (or any Linux box running charge):

```
sudo charge repos add https://manilinux.infinityfreeapp.com
sudo charge update
sudo charge nmap
```

## Adding a package (build box, Linux/WSL2)

```
./mkpkg.sh nmap 7.95 ~/build/nmap easy libpcre,openssl
```

That packs `nmap-7.95.tar.zst`, computes its sha256, and updates
`index.txt`. Then FTP the `.tar.zst` and `index.txt` up and the package
is live.

## Later: GitHub Pages

Once the repo is public, Pages works for free and this folder can move
there as-is — same index, same charge commands, just a new base URL.