CC := clang -ffreestanding -fno-stack-protector -mno-red-zone -fno-builtin
LD := ld.lld -nostdlib -no-pie

BOOT_SRC := $(wildcard boot/src/*.c)
BOOT_OBJ := $(patsubst boot/src/%.c, boot/obj/%.o, $(BOOT_SRC))

KERNEL_SRC := $(wildcard kernel/src/*.c)
KERNEL_OBJ := $(patsubst kernel/src/%.c, kernel/obj/%.o, $(KERNEL_SRC))

ASM_SRC := $(wildcard kernel/src/*.asm)
ASM_OBJ := $(patsubst kernel/src/%.asm, kernel/obj/%.o, $(ASM_SRC))

BOOT_TARGET   := esp/EFI/BOOT/bootx64.efi
KERNEL_TARGET := esp/EFI/BOOT/kernel.elf
ISO_TARGET	  := Veyra-x86_64-UEFI.iso

.PHONY: all clear

$(shell mkdir -p esp/EFI/BOOT)
$(shell mkdir -p kernel/obj)
$(shell mkdir -p boot/obj)

all: $(ISO_TARGET)
	qemu-system-x86_64 -bios boot/OVMF.fd \
		-drive if=virtio,format=raw,file=$(ISO_TARGET) \
		-machine q35 \
		-net none \
		-m 512M \
		-vnc :0 \
		-serial stdio

$(ISO_TARGET): $(BOOT_TARGET) $(KERNEL_TARGET)
	dd if=/dev/zero of=esp/efi.img bs=1M count=64
	mkfs.vfat -F 32 esp/efi.img
	mmd -i esp/efi.img ::/EFI
	mmd -i esp/efi.img ::/EFI/BOOT
	mcopy -i esp/efi.img $(BOOT_TARGET) ::/EFI/BOOT
	mcopy -i esp/efi.img $(KERNEL_TARGET) ::/EFI/BOOT
	xorriso -as mkisofs \
		-no-emul-boot \
		-eltorito-alt-boot \
		-isohybrid-gpt-basdat \
		-e efi.img \
		-o $(ISO_TARGET) esp/

$(BOOT_TARGET): $(BOOT_OBJ)
	$(CC) -target x86_64-unknown-windows \
		-Wl,-subsystem:efi_application \
		-Wl,-entry:efiMain \
		-nostdlib \
		$^ -o $@

boot/obj/%.o: boot/src/%.c
	$(CC) -target x86_64-unknown-windows \
		-fshort-wchar \
		-I boot/include \
		-c $< -o $@

$(KERNEL_TARGET): $(KERNEL_OBJ) $(ASM_OBJ)
	$(LD) $^ -o $@ \
		-T kernel/kernel.ld

kernel/obj/%.o: kernel/src/%.c
	$(CC) -target x86_64-unknown-elf \
		-I kernel/include \
		-fno-pie \
		-c $< -o $@

kernel/obj/%.o: kernel/src/%.asm
	nasm -f elf64 $< -o $@

clear:
	rm -rf esp \
		$(ISO_TARGET) \
		kernel/obj \
		boot/obj
