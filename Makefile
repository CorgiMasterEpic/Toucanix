all: build reset_ovmf_bin run

run:
	./scripts/run.sh
	
build:
	./scripts/build.sh

clean:
	./scripts/clean.sh

install_required:
	./scripts/install_required.sh

chmod_all:
	chmod +x scripts/*

ultra: clean build reset_ovmf_bin run

reset_ovmf_bin:
	./scripts/reset_ovmf_bin.sh