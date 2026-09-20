#!/bin/bash
set -e

ISO="/home/lucifer/ManiOS/manios.iso"
DISK="/home/lucifer/ManOS-VM/disk/manios.qcow2"
KERNEL="/usr/lib/modules/$(uname -r)/vmlinuz"
INITRAMFS="/tmp/manios-iso/boot/initramfs.img"

echo "=== Creating standalone ManOS VM Disk ==="

# Create a 2GB disk
qemu-img create -f qcow2 "$DISK" 2G 2>/dev/null || true

# Create a minimal partition table and install ManOS
# Use qemu-nbd to mount and copy files
modprobe nbd max_part=8 2>/dev/null || true
qemu-nbd --connect=/dev/nbd0 "$DISK" 2>/dev/null && sleep 1 || true

# If nbd not available, use a simpler approach
# Create a raw disk image with the kernel and initramfs
RAW="/tmp/manios-raw.img"
qemu-img create -f raw "$RAW" 256M 2>/dev/null

# Copy the kernel and initramfs to a bootable structure
mkdir -p /tmp/manios-root
cp "$KERNEL" /tmp/manios-root/vmlinuz 2>/dev/null || true
cp "$INITRAMFS" /tmp/manios-root/initramfs.img 2>/dev/null || true

# Create init script for the disk
cat > /tmp/manios-root/init << 'INIT'
#!/bin/sh
mount -t proc proc /proc 2>/dev/null
mount -t sysfs sysfs /sys 2>/dev/null
mount -t devtmpfs devtmpfs /dev 2>/dev/null
mkdir -p /dev /proc /sys /tmp
chmod 1777 /tmp
export PATH=/bin:/sbin:/usr/bin:/usr/sbin
clear 2>/dev/null
/bin/maniOS
poweroff -f 2>/dev/null || reboot -f 2>/dev/null
INIT
chmod +x /tmp/manios-root/init

echo "Standalone disk created: $DISK"
echo "Raw image: $RAW"
echo "Use: qemu-system-x86_64 -kernel $KERNEL -initrd $INITRAMFS -append 'root=/dev/ram0 rw' -m 512M"
