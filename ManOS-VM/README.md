# ManOS Virtual Machine

## Quick Start

```bash
cd /home/lucifer/ManOS-VM
./start              # Boot with KVM + GUI
./start-nographic    # Boot with KVM + terminal
./start-soft         # Boot without KVM + GUI
```

## VM Specifications
- **Memory:** 512MB
- **CPUs:** 2
- **Disk:** 4GB qcow2 virtual disk
- **Boot:** Linux kernel + initramfs with ManOS terminal
- **Accelerated:** KVM (hardware virtualization)

## Files
- `disk/manios-standalone.qcow2` — Standalone ManOS disk image
- `disk/manios.qcow2` — Installation disk
- `manios.iso` — Bootable ISO image
- `start` — Launch VM (KVM + GUI)
- `start-nographic` — Launch VM (KVM + terminal)
- `start-soft` — Launch VM (software emulation)

## Build
```bash
cd /home/lucifer/ManiOS
make iso-build    # Rebuild the ISO
make clean && make # Rebuild the terminal
```

## Features
- Full ManOS terminal (C + readline)
- Built-in commands: cd, pwd, exit, help, history, alias, etc.
- Pipe support: `ls -la | head -5`
- Background processes: `command &`
- Persistent history and aliases
- Tab completion, ↑/↓ history
- Color-coded prompt and output
