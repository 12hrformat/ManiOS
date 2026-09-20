#!/bin/bash
set -e

ISO="/home/lucifer/ManiOS/manios.iso"
DISK="/home/lucifer/ManOS-VM/disk/manios.qcow2"

echo "=== ManOS VM Installer ==="
echo "ISO: $ISO"
echo "Disk: $DISK"

# Check if disk exists
if [ ! -f "$DISK" ]; then
    echo "Creating 4GB qcow2 disk..."
    qemu-img create -f qcow2 "$DISK" 4G
fi

# Check if ISO exists
if [ ! -f "$ISO" ]; then
    echo "Building ISO first..."
    cd /home/lucifer/ManiOS && make iso-build
fi

echo "Disk size: $(qemu-img info "$DISK" | grep 'Virtual size')"
echo "ISO size: $(du -h "$ISO" | cut -f1)"

# Use qemu to install ISO onto the disk
# Boot the ISO and copy the filesystem to the disk
echo ""
echo "To complete installation, boot the VM and run the installer."
echo "The installer will copy ManOS to the hard disk."
echo ""
echo "Quick boot with: $0 --live"
echo "Install mode:    $0 --install"
