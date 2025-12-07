#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

main() {
    echo "-> [Building Toucanix]"
    cd gnu-efi && make && make bootloader
    cd ../kernel && make setup && make kernel && make buildimg
}

main "$@"
