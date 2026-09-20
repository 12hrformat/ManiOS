Contributing to Mani OS

Thank you for contributing to Mani OS.

Mani OS is a security-focused Linux distribution designed for penetration testing, security research, and ethical hacking. The project is developed openly on GitHub, and contributors are encouraged to help expand both the operating system and its security tooling.

Our goal is simple:

«Build a functional security-focused Linux operating system first. Make it polished later.»

---

Current Development Stage

Mani OS is currently in the base-system stage.

Priority right now

We are not focusing on:

- GUI
- Desktop environments
- Visual effects
- Custom window managers
- Fancy themes
- Advanced graphical utilities

The immediate goal is to produce a bootable x86_64 Linux system with a working terminal and network connection.

The base system should be able to:

- Boot successfully
- Provide a functional terminal
- Execute basic Linux commands
- Access the filesystem
- Detect basic hardware
- Configure networking
- Access the internet
- Install/update packages
- Run correctly on x86_64 systems

Once this foundation is stable, development will move toward the GUI and Mani-specific features.

---

Project Roadmap

Phase 1 — Base System

Current priority.

- [ ] Bootable ISO
- [ ] x86_64 support
- [ ] Functional kernel
- [ ] Root filesystem
- [ ] Terminal
- [ ] Basic Linux utilities
- [ ] Networking
- [ ] DNS
- [ ] Internet connectivity
- [ ] Package management
- [ ] Basic hardware detection
- [ ] VM testing
- [ ] Reproducible ISO builds

Phase 2 — Security Tooling

After the base system is stable:

- [ ] Security tool repository
- [ ] Preinstalled security tools
- [ ] Tool categories
- [ ] Tool installation/update system
- [ ] Security-focused defaults
- [ ] Community tool contributions
- [ ] Tool documentation

Example categories:

Recon
Web Security
Network Security
Wireless
Password Auditing
Active Directory
Exploitation
Forensics
OSINT
Reverse Engineering
Cryptography

Phase 3 — GUI

After the terminal-based system is reliable:

- [ ] Desktop environment
- [ ] Mani branding
- [ ] Custom terminal configuration
- [ ] GUI utilities
- [ ] Security-focused desktop workflow

Phase 4 — Mani-Specific Features

Long-term:

- [ ] Custom Mani utilities
- [ ] Pentesting workspace
- [ ] Engagement management
- [ ] Evidence organization
- [ ] Reporting integration
- [ ] Custom system utilities
- [ ] Better automation


Development Requirements

Recommended development environment:

- Linux host
- x86_64 system
- Git
- QEMU or VirtualBox
- Internet connection
- Sufficient disk space for ISO builds

A VM is strongly recommended for development and testing.

---

Getting the Source

Clone the repository:
```
git clone https://github.com/12hrformat/ManiOS
cd mani-os
```
Create a development branch:
```
git checkout -b feature/<short-description>
```
Example:
```
git checkout -b feature/networking
```
---

Building Mani OS

The exact build command depends on the build system currently used by Mani OS.

Once the build system is finalized, the standard command should be:

./scripts/build.sh

If the script is not executable:

chmod +x scripts/build.sh
./scripts/build.sh

The resulting ISO should be placed in:

dist/

Example:

dist/
└── mani-os-x86_64.iso

Clean Build

Before testing a final build:

./scripts/clean.sh

Then rebuild:

./scripts/build.sh

Do not commit generated ISO files unless the repository explicitly requires release artifacts to be stored in Git.

---

VM Testing

Every contributor working on the base system should test changes in a VM before submitting a pull request.

QEMU

Example:

qemu-system-x86_64 \
    -enable-kvm \
    -m 4096 \
    -smp 2 \
    -cdrom dist/mani-os-x86_64.iso

If KVM is unavailable:

qemu-system-x86_64 \
    -m 4096 \
    -smp 2 \
    -cdrom dist/mani-os-x86_64.iso

Adjust memory and CPU allocation as required by your host.

---

Base System Test

After Mani OS boots, verify the architecture:

uname -m

Expected:

x86_64

Check the kernel:

uname -a

Check the filesystem:

pwd
ls

Test basic commands:

cd /
ls
pwd
mkdir test
cd test
touch test.txt
echo "Mani OS" > test.txt
cat test.txt
cd ..
rm -rf test

Verify process management:

ps

Verify available storage:

df -h

Verify memory:

free -h

---

Network Testing

Check network interfaces:

ip addr

Check routes:

ip route

Test raw connectivity:

ping -c 4 1.1.1.1

Test DNS:

ping -c 4 google.com

Both tests should succeed.

If:

ping 1.1.1.1

works but:

ping google.com

does not work, investigate DNS configuration.

---

Package Management

The package manager must be able to:

1. Access configured repositories.
2. Refresh package metadata.
3. Install packages.
4. Remove packages.
5. Update packages.

The exact commands depend on the base distribution.

For an Arch-based implementation, for example:

sudo pacman -Sy

Test installing a small package:

sudo pacman -S <package>

Then verify that the package works.

Do not perform a full system upgrade blindly while debugging a reproducibility issue. Record the package versions involved.

---

Security Tool Contributions

Mani OS is intended to provide a curated security toolkit.

Contributors may submit tools appropriate for:

- Authorized penetration testing
- Security research
- Network assessment
- Web application testing
- Digital forensics
- Reverse engineering
- OSINT
- Defensive security
- CTF/lab environments

Every tool PR should explain:

Tool:
Purpose:
Category:
Upstream:
License:
Installation method:
Dependencies:
Why Mani OS should include it:

Do not add proprietary, malicious, stolen, or intentionally backdoored software.

Security tools must be used only against systems where the user has authorization.

---

Tool PR Example

A security-tool contribution might look like:

packages/
└── security/
    └── example-tool/
        ├── package/
        ├── install.sh
        └── README.md

The exact structure depends on the Mani OS package system.

The contributor should document:

- Upstream project
- Version
- License
- Dependencies
- Installation procedure
- Update procedure
- Basic verification procedure

---

Pull Request Rules

1. Create a branch

Do not work directly on "main".

git checkout main
git pull
git checkout -b feature/<name>

---

2. Make one logical change

Prefer:

feat: add networking support

over one massive PR containing:

networking + GUI + 40 tools + installer + themes

Small, focused PRs are easier to review and merge.

---

3. Test before submitting

At minimum, verify the part of the system you changed.

For base-system changes:

- [ ] ISO builds
- [ ] ISO boots
- [ ] x86_64 detected
- [ ] Terminal works
- [ ] Basic commands work
- [ ] Network interface appears
- [ ] Internet works
- [ ] DNS works

For package/tool changes:

- [ ] Package installs
- [ ] Dependencies work
- [ ] Tool launches
- [ ] Tool version can be checked
- [ ] No unnecessary dependencies were introduced

---

Commit Messages

Use clear commit messages.

Recommended format:

type: short description

Examples:

feat: add base networking configuration
feat: add nmap package
fix: repair DNS configuration
fix: correct ISO build permissions
docs: update VM testing instructions
test: add package installation test
refactor: simplify build scripts

Avoid commits such as:

stuff
update
fixed
lol
changes
final final

---

Pull Request Template

Every PR should contain:

## What does this PR do?

<!-- Explain the change. -->

## Why is this needed?

<!-- Explain the reason for the change. -->

## Testing

- [ ] Tested locally
- [ ] Tested in VM
- [ ] ISO boots
- [ ] Basic commands work
- [ ] Networking works

## Additional Notes

<!-- Anything reviewers should know. -->

---

Code and Configuration Guidelines

Keep the base system:

- Minimal
- Reproducible
- Understandable
- Documented
- Easy to debug

Avoid adding unnecessary dependencies.

Avoid hardcoded paths when a configurable path is appropriate.

Avoid scripts that silently modify the host system.

Build scripts should clearly report errors.

---

VM First

If you are changing the operating system itself, test in a VM first.

Do not use your primary machine as the first test environment for:

- Kernel changes
- Bootloader changes
- Partitioning logic
- Filesystem changes
- Installer changes
- Network initialization
- Package-management changes

The VM should be the first validation environment.

---

Documentation

If your change introduces a new feature, package, command, configuration option, or workflow, update the relevant documentation.

Documentation is part of the implementation.

If you add:

feature

also explain:

what it does
how it works
how to test it
how to troubleshoot it

---

Current Definition of "Working"

For the current Phase 1 milestone, Mani OS is considered functionally working when:

┌─────────────────────────────┐
│       Mani OS ISO           │
├─────────────────────────────┤
│ ✓ Boots                     │
│ ✓ x86_64                    │
│ ✓ Kernel running            │
│ ✓ Root filesystem           │
│ ✓ Terminal                  │
│ ✓ Basic commands            │
│ ✓ Filesystem operations     │
│ ✓ Network interface         │
│ ✓ IP connectivity           │
│ ✓ DNS resolution            │
│ ✓ Internet access           │
│ ✓ Package management        │
└─────────────────────────────┘

The GUI comes after this foundation is stable.

---

Contributor Philosophy

Mani OS is a community project.

If you have an idea, open an issue or submit a PR.

If you find a bug, report it.

If you want to add a security tool, submit it.

If you can improve the build system, contribute.

If something is broken, document it.

The project grows through contributions, testing, review, and iteration.

Build the foundation first. Then build everything on top of it.

---

License

See ""LICENSE"" (LICENSE) for the project's licensing terms.
