qemu-system-x86_64 -pflash OVMF.fd -hda fat:rw:hda-contents/ -net none -debugcon file:debug.log -global isa-debugcon.iobase=0x402
