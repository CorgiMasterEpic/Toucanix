#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

main() {
    echo "-> [Cleaning Toucanix]"
	cd gnu-efi && make clean
    cd ../kernel && make clean
}

main "$@"
