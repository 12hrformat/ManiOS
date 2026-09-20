# Mani Linux package repo (charge)

This directory is the **charge repository**. It is served as static files
via GitHub Pages — no server-side code, charge does all the work locally.

```
repo/
  index.txt      # price list: every package name -> file + sha256 + tier
  <name>-TBD.tar.zst   # actual packages (built + git pushed here)
```

## How a package gets in here

1. Build the tool on the Mani build box (see `core/charge/`)
2. Pack it: `tar --zstd -cf <name>-<version>.tar.zst -C / <files...>` with a `.PKGINFO`
3. `git add` + `git push` the `.tar.zst` into this folder
4. Update `index.txt`: real version, real sha256, real deps
5. Users run `sudo charge update` then `sudo charge <name>`

## Enabling GitHub Pages

Settings -> Pages -> Deploy from a branch -> select `main` -> save.
The repo index is then reachable at:

```
https://<user>.github.io/ManiOS/repo/
```

and on Mani:

```
sudo charge repos add https://<user>.github.io/ManiOS/repo
sudo charge update
sudo charge nmap
```

## Why `.nojekyll`

GitHub Pages runs Jekyll on *all* static sites unless a `.nojekyll`
file exists in the served tree. Without it, `.tar.zst` files are not
served as plain files. This file makes Pages behave like a plain
web server.