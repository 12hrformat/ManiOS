MANI Linux — Development Roadmap

«Project: MANI Linux
Architecture: x86_64
Current focus: Terminal/TTY → Core system → Package manager → GUI → Public release
Primary rule: Test in a VM before testing on real hardware.»

---

Status Legend

- "[ ]" Not started
- "[~]" In progress
- "[x]" Completed
- "[!]" Blocked / needs attention

---

PHASE 0 — Project Foundation

Repository

- [x] Create GitHub repository
- [x] Make repository private during development
- [ ] Establish final repository structure
- [x] Add "README.md"
- [x] Add "CONTRIBUTING.md"
- [ ] Add "LICENSE"
- [x] Add issue templates
- [x] Add pull request template
- [x] Define contribution rules
- [ ] Define branch strategy

Development Environment

- [ ] Decide base Linux distribution
- [ ] Decide build system
- [x] Set up reproducible build environment
- [x] Set up VM testing environment
- [ ] Document development setup
- [ ] Create first automated build

---

PHASE 1 — MANI Linux Base System

Goal: Boot into a functional terminal-based Linux system.

Boot

- [ ] Produce first bootable ISO
- [ ] Boot successfully in VM
- [ ] Verify x86_64 architecture
- [ ] Kernel loads correctly
- [ ] Root filesystem mounts correctly
- [ ] Init system starts correctly
- [ ] Reach TTY/login prompt

Filesystem

- [ ] "/"
- [ ] "/bin"
- [ ] "/etc"
- [ ] "/home"
- [ ] "/usr"
- [ ] "/var"
- [ ] "/tmp"
- [ ] "/dev"
- [ ] "/proc"
- [ ] "/sys"
- [ ] "/run"

User Management

- [ ] Root account
- [ ] Normal user account
- [ ] Login system
- [ ] Password handling
- [ ] User permissions
- [ ] Groups
- [ ] "sudo"/privilege escalation mechanism

---

PHASE 2 — Core Terminal Environment

Goal: Make the basic terminal experience usable.**

Shell

- [ ] Choose shell
- [ ] Shell launches correctly
- [ ] Environment variables
- [ ] PATH configuration
- [ ] Command execution
- [ ] Pipes
- [ ] Redirection
- [ ] Exit codes
- [ ] Shell configuration

Core Commands

Filesystem

- [ ] "ls"
- [ ] "cd"
- [ ] "pwd"
- [ ] "mkdir"
- [ ] "rmdir"
- [ ] "touch"
- [ ] "cp"
- [ ] "mv"
- [ ] "rm"

File Reading / Manipulation

- [ ] "cat"
- [ ] "less"
- [ ] "head"
- [ ] "tail"
- [ ] "grep"
- [ ] "find"
- [ ] "sort"
- [ ] "wc"

System

- [ ] "ps"
- [ ] "kill"
- [ ] "top"
- [ ] "uname"
- [ ] "hostname"
- [ ] "whoami"
- [ ] "id"
- [ ] "free"
- [ ] "df"
- [ ] "du"

Networking

- [ ] "ip"
- [ ] "ping"
- [ ] "ss"
- [ ] DNS utilities
- [ ] Network interface detection

---

PHASE 3 — Networking

Goal: Mani Linux can connect to a network and access the internet.**

- [ ] Detect network interface
- [ ] Configure interface
- [ ] DHCP
- [ ] Static IP configuration
- [ ] Default route
- [ ] DNS configuration
- [ ] IPv4
- [ ] IPv6
- [ ] Network service
- [ ] Internet connectivity
- [ ] Network failure handling

Tests

ip addr
ip route
ping -c 4 1.1.1.1
ping -c 4 google.com

Expected:

Interface → detected
IP        → assigned
Route     → present
Internet  → reachable
DNS       → working

---

PHASE 4 — Package System

Goal: Mani Linux can reliably obtain and manage software.**

This is where the Mani package manager begins.

Package Manager

Working name:

mani

Possible commands:

mani update
mani search <package>
mani install <package>
mani remove <package>
mani upgrade
mani info <package>

Package Manager Components

- [ ] Package format
- [ ] Package metadata
- [ ] Package repository
- [ ] Repository index
- [ ] Package downloader
- [ ] Package installer
- [ ] Package remover
- [ ] Package updater
- [ ] Dependency resolver
- [ ] Version handling
- [ ] Package verification
- [ ] Checksums
- [ ] Signature verification
- [ ] Error handling
- [ ] Local package database

First Test Packages

- [ ] Install a simple package
- [ ] Verify installation
- [ ] Remove package
- [ ] Reinstall package
- [ ] Upgrade package
- [ ] Test dependency handling

---

PHASE 5 — MANI Security Toolkit

Goal: Turn the functional Linux base into a security-focused OS.**

Recon

- [ ] Nmap
- [ ] Amass
- [ ] Subfinder
- [ ] httpx
- [ ] Masscan

Web Security

- [ ] Burp Suite
- [ ] OWASP ZAP
- [ ] ffuf
- [ ] Gobuster
- [ ] SQLmap

Network Security

- [ ] Wireshark
- [ ] tcpdump
- [ ] Netcat

Password Auditing

- [ ] Hashcat
- [ ] John the Ripper

Active Directory

- [ ] Impacket
- [ ] BloodHound

Wireless

- [ ] Aircrack-ng
- [ ] Wireless analysis utilities

Reverse Engineering

- [ ] Ghidra
- [ ] Radare2
- [ ] Debugging tools

Forensics

- [ ] File analysis
- [ ] Disk analysis
- [ ] Metadata analysis
- [ ] Hashing utilities

Community Tool Contributions

- [ ] Create tool contribution guidelines
- [ ] Create package template
- [ ] Accept security-tool PRs
- [ ] Review licenses
- [ ] Review dependencies
- [ ] Test submitted tools
- [ ] Document accepted tools

---

PHASE 6 — MANI CLI

Goal: Create a unified Mani command-line experience.**

Possible command:

mani

Potential functionality:

mani system
mani update
mani install
mani remove
mani search
mani tools
mani doctor
mani info
mani version

- [ ] CLI framework
- [ ] Help system
- [ ] Version command
- [ ] Error messages
- [ ] Tool management
- [ ] System diagnostics
- [ ] Package-manager integration

---

PHASE 7 — System Configuration

- [ ] Mani default configuration
- [ ] Hostname
- [ ] Timezone
- [ ] Locale
- [ ] Keyboard layout
- [ ] Network configuration
- [ ] User configuration
- [ ] Shell configuration
- [ ] Environment configuration

---

PHASE 8 — GUI

Do this only after the base terminal system is stable.

Desktop

- [ ] Choose desktop/window manager
- [ ] Display server / Wayland
- [ ] Login manager
- [ ] Desktop session
- [ ] Terminal emulator
- [ ] File manager
- [ ] Network UI
- [ ] System settings

MANI Branding

- [ ] Logo
- [ ] Wallpaper
- [ ] Icons
- [ ] Colors
- [ ] Fonts
- [ ] Boot screen
- [ ] Login screen
- [ ] Terminal theme
- [ ] Fastfetch configuration

---

PHASE 9 — Security Workflow

- [ ] Pentesting workspace
- [ ] Project directory structure
- [ ] Evidence directory
- [ ] Screenshot management
- [ ] Command logging
- [ ] Target notes
- [ ] Report integration
- [ ] Export functionality

Potential structure:

~/mani/
├── engagements/
│   └── project/
│       ├── recon/
│       ├── scans/
│       ├── evidence/
│       ├── screenshots/
│       └── reports/
└── tools/

---

PHASE 10 — Testing & Quality

VM Testing

Every major change should be tested in a VM.

- [ ] ISO boots
- [ ] x86_64 detected
- [ ] TTY works
- [ ] Login works
- [ ] Core commands work
- [ ] Filesystem works
- [ ] Networking works
- [ ] DNS works
- [ ] Internet works
- [ ] Package manager works
- [ ] Security tools work

Hardware Testing

After VM stability:

- [ ] Test physical x86_64 hardware
- [ ] Test networking
- [ ] Test storage
- [ ] Test keyboard
- [ ] Test display
- [ ] Test suspend/reboot/shutdown

---

PHASE 11 — Documentation

- [ ] Installation guide
- [ ] Building guide
- [ ] Contributor guide
- [ ] Architecture documentation
- [ ] Package-manager documentation
- [ ] Security-tool documentation
- [ ] Troubleshooting guide
- [ ] FAQ
- [ ] Developer documentation

---

PHASE 12 — Public Release

Do not make the project public until the core system is stable enough for external users.

Pre-release

- [ ] Stable boot
- [ ] Stable networking
- [ ] Stable package manager
- [ ] Security tools tested
- [ ] GUI tested
- [ ] Documentation complete
- [ ] License confirmed
- [ ] Known bugs documented
- [ ] Clean ISO build
- [ ] VM installation tested
- [ ] Hardware installation tested

Release

- [ ] Create release version
- [ ] Build final ISO
- [ ] Generate checksums
- [ ] Publish release notes
- [ ] Publish documentation
- [ ] Open repository
- [ ] Announce Mani Linux

---

Current Sprint

«Target milestone: October 4»

Must Be Completed

- [ ] Functional TTY
- [ ] Basic Linux filesystem
- [ ] Core commands
- [ ] User/login system
- [ ] Networking
- [ ] Internet access
- [ ] Package-management foundation
- [ ] VM testing
- [ ] All required source/configuration files committed

After October 4

October 4
    │
    ▼
Base system stable
    │
    ▼
Security tooling
    │
    ▼
GUI development
    │
    ▼
Mani-specific features
    │
    ▼
Public release

---

Contribution Rule

Every contributor should keep this roadmap updated when they complete a task.

When completing something:

- [ ] Networking
+ [x] Networking

When actively working on something:

- [ ] Package manager
+ [~] Package manager

When blocked:

- [ ] Hardware detection
+ [!] Hardware detection

Add a short note below the item explaining the blocker.

---

Golden Rule

Do not build the GUI before the foundation works.

The priority is:

BOOT
 ↓
TTY
 ↓
FILESYSTEM
 ↓
CORE COMMANDS
 ↓
NETWORK
 ↓
INTERNET
 ↓
PACKAGE MANAGER
 ↓
SECURITY TOOLS
 ↓
GUI
 ↓
MANI FEATURES
 ↓
PUBLIC RELEASE

Foundation first. Features second. Polish last.
