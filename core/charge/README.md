# charge — Mani Linux package manager

The package manager for Mani Linux. Install, remove, and upgrade software
with one command.

```
sudo charge nmap        # installs nmap
sudo charge down nmap   # deletes nmap
sudo recharge nmap      # updates + upgrades nmap
sudo charge list        # shows installed packages
sudo charge help        # usage
```

Unlike apt or pacman, charge keeps the install command as the default action —
no `install` keyword needed. `charge <package>` means "install this".

## Two binaries

| binary    | job                                            |
|-----------|------------------------------------------------|
| `charge`  | install (repo or local file) / remove / list / update / repos |
| `recharge`| update package lists + upgrade (all or one)   |

`recharge nmap` updates and upgrades nmap. `recharge` with no argument
updates and upgrades everything.

## Repositories

A repo is just an HTTP folder:

```
https://<host>/repo/
    index.txt          # price list
    nmap-7.95.tar.zst  # packages
```

```
sudo charge repos add https://<host>/repo
sudo charge update        # download the index
sudo charge nmap          # resolve name -> download -> verify -> install
```

`<pkg>` can also be a local file: `sudo charge ./mytest-1.0.tar.zst`.

### Hosting options for a private repo

- **GitHub Pages is paywalled for private repos** (free only on public ones).
  Works once Mani Linux goes public.
- **InfinityFree / any static HTTP host** works regardless of repo privacy:
  upload `index.txt` + the `.tar.zst` files via FTP, then
  `sudo charge repos add https://<your>.infinityfreeapp.com`.
- GitHub Releases (free even on private repos) suits big binaries later.

## Package format (v1)

A package is a `.tar.zst` archive. Files are packed rooted at `/`. Inside the
archive there is one metadata file, `.PKGINFO`:

```
name = nmap
version = 7.95
depend = libpcre
depend = openssl
sha256sum = 3f0a92378a3d9b2d06e25a1b3c0d8e91f2a6b4c5d7e8f90123456789abcdef0
```

## Installed database

Pacman-style plain-text directories under `/var/lib/charge/`:

```
/var/lib/charge/
    lock                 # transaction lock (exists while a transaction runs)
    staging/<pkg>/       # extraction area during install
    local/<name>-<version>/
        desc             # name, version, install date
        files            # every path this package owns
        depend           # dependencies
```

Plain text on purpose — no database library, everything human-readable.

## Build

Needs: `gcc`, `libarchive`, `libzstd`, `libcurl`, `openssl` (libcrypto) dev packages.

```
make
```

Must be built on Linux (WSL2 works). `/var/lib/charge` is Linux-specific.

## Milestones

1. [x] extract a `.tar.zst` package, fetch `.PKGINFO`, record owned files
2. [ ] `install` / `down` wired through the local database
3. [ ] transaction lock + rollback on failed install
4. [ ] dependency resolution (`depend =` lines, install order)
5. [x] repository fetch (libcurl) + sha256 verification
6. [ ] pre/post install hooks (ldconfig etc.)