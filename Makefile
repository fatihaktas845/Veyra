CC := clang++ @flags.txt
LD := ld.lld -T kernel/kernel.ld -nostdlib -no-pie

OUTPUT_BINARY := part_esp/kernel.elf
OUTPUT_IMAGE := Veyra-x86_64-UEFI.img

CPP_SOURCES := $(wildcard kernel/src/*.cpp)
ASM_SOURCES := $(wildcard kernel/src/*.asm)

OBJS := $(patsubst kernel/src/%.cpp, kernel/obj/%.o, $(CPP_SOURCES))
OBJS += $(patsubst kernel/src/%.asm, kernel/obj/%.o, $(ASM_SOURCES))

.PHONY: all clean

all: $(OUTPUT_IMAGE)
	qemu-system-x86_64 \
		-machine q35 \
		-net none \
		-enable-kvm \
		-cpu host \
		-drive if=pflash,format=raw,readonly=true,file=code.fd \
		-drive if=pflash,format=raw,file=vars.fd \
		-drive format=raw,file=$< \
		-monitor stdio

$(OUTPUT_IMAGE): $(OUTPUT_BINARY)
	dd if=/dev/zero of=$@ bs=1M count=64
	dd if=/dev/zero of=esp.img bs=1M count=40
	mkfs.fat -F 32 esp.img
	mcopy -i esp.img -s part_esp/* ::/
	@parted -s $@ mklabel gpt
	@parted -s $@ mkpart "ESP" 2MiB 42MiB
	@parted -s $@ set 1 boot on
	dd if=esp.img of=$@ bs=1M count=40 oseek=2 conv=notrunc
	-rm -rf esp.img

$(OUTPUT_BINARY): $(OBJS)
	$(LD) $^ -o $@

kernel/obj/%.o: kernel/src/%.cpp
	@mkdir -p kernel/obj
	$(CC) -c $< -o $@

kernel/obj/%.o: kernel/src/%.asm
	@mkdir -p kernel/obj
	nasm -f elf64 -i kernel/src $< -o $@

clean:
	rm -rf \
		$(OUTPUT_IMAGE) \
		$(OUTPUT_BINARY) \
		kernel/obj
