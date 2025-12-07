#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

main() {
    echo "-> [Resetting OVMFbin]"
	rm -rf OVMFbin
	cp -r scripts/OVMFbin_Copy OVMFbin
}

main "$@"
