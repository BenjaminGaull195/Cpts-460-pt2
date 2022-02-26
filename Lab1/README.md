# Lab 1

This assignment has us create a bootloader for the MTX OS

### Lab Checklist
- [ ] Does it boot the MTX OS?
- [ ] Lab1.1: Can boot up and gets{}/prints()
- [ ] Lab1.2: can display the file names under / dir

### Lab Questions
On Intel x86 based systems, during booting, what does the BIOS do?
  - The bios collects system configuration data and typically loads a bootloader from the boot device

How many sectors does the BIOS load from the boot device?
  - the BIOS loads only the first sector

Where in memory does BIOS load the booter?
  - the bios loads the sector to segment 0x0000 with offset 0x7C00

Bios loads only 512 bytes of a booter into memory, what is only the beginning part of the booter. How does the booter load the remaining part of the booter into memory?
  - the first part of the booter contains instructions at the beginning to load the first 2 sectors of the boot device to a location in memory then jumps to a that location

Assume a complete booter is loaded at the segment 0x9000. why do we have to set the CPU's segment registers CS, DS, SS, ES to 0x9000?

How do you find the file /boot/mtx?
  - you first retrieve the inode for the root /. Then you iterate over the first data blocks in the root inode until you find the dir_entry for the directory /boot. you then retrieve the inode for /boot. then you iterate over the data blocks to find the dir_entry for /boot/mtx. you then retrieve the inode for /boot/mtx

How to load the )disk) blocks of /boot/mtx to the segment 0x1000?
  - first you iterate over and read the direct data blocks in the inode, where you set ES and increment ES by 1KB after each block.  The same process is repeated for the indirect blocks in the inode. 

