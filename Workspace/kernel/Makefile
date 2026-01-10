OSNAME = Toucanix

GNUEFI = ../bootloader
STARTUPNSH = ../../Build/startup.nsh
OVMFDIR = ../../RuntimeAssets/OVMFbin
LDS = ../../Build/kernel.ld
CXX = clang++
LD = ld.lld

SRCDIR := src
OBJDIR := ../../Build/objs
BUILDDIR = ../../Build/bin
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

KERNEL_CXX_BASE = \
  -O2 -ffreestanding -mno-red-zone -fshort-wchar -std=c++17

KERNEL_CXX_NO_RUNTIME = \
  -fno-exceptions -fno-rtti -fno-use-cxa-atexit

KERNEL_CXX_NO_LIBC = \
  -nostdlib -nostdinc++ -fno-builtin

KERNEL_CXX_SAFETY = \
  -fno-strict-aliasing -fno-stack-protector -fno-pie

KERNEL_CXX_QOL = \
  -Wimplicit-fallthrough -I$(SRCDIR)/Goat

WARNINGS = \
  -Wall -Wextra -Wpedantic -Werror=return-type

CXXFLAGS = \
  $(KERNEL_CXX_BASE) \
  $(KERNEL_CXX_NO_RUNTIME) \
  $(KERNEL_CXX_NO_LIBC) \
  $(KERNEL_CXX_SAFETY) \
  $(KERNEL_CXX_QOL) \
  $(WARNINGS)

LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib -no-pie

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
DIRS = $(wildcard $(SRCDIR)/*)

kernel:	$(OBJS) link

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "-> [Compiling $^] <-"
	@echo "> [Using $(CXX)]"
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

link:
	@echo "-> [Linking Project] <-"
	@echo "> [Using $(LD)]"
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/$(OSNAME).elf $(OBJS)

buildQemuIMG:
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	mformat -i $(BUILDDIR)/$(OSNAME).img -f 1440 ::
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/Shrimp
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(STARTUPNSH) ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/$(OSNAME).elf ::/Shrimp
	mcopy -i $(BUILDDIR)/$(OSNAME).img ../../Build/default.psf ::/Shrimp

clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(BUILDDIR)/*

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
