.PHONY: buildBootloader buildKernel buildGoatStdlib clean resetOVMFbin run build all ultra

all: build resetOVMFbin run
	clear

buildBootloader:
	@echo "> [Building Bootloader]"
	cd Workspace/bootloader && make && make bootloader

buildKernel:
	@echo "> [Building Kernel]"
	cd Workspace/kernel && make kernel && make buildQemuIMG

clean:
	@echo "> [Cleaning Project]"
	cd Workspace/bootloader && make clean && \
	cd ../kernel && make clean

resetOVMFbin:
	@echo "> [Resetting OVMFbin]"
	cd RuntimeAssets && rm -rf OVMFbin && cp -r OVMFbinCopy OVMFbin

run:
	@echo "> [Running]"
	./Build/Scripts/run.sh

build: buildBootloader buildKernel

ultra:
	$(MAKE) clean
	$(MAKE) all
	clear
