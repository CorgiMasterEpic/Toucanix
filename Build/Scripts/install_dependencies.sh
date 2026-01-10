#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

main() {
    echo "> [Installing Dependencies]"
    sudo apt update
    sudo apt-get install build-essential mtools llvm clang lld nasm
}

main "$@"