#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

is_wsl() {
    grep -qi "microsoft" /proc/sys/kernel/osrelease 2>/dev/null
}

confirm() {
    local prompt="$1"
    local response
    read -rp "$prompt" response

    # empty = yes for [Y/n]
    if [[ -z "$response" ]]; then
        echo "yes"
        return 0
    fi

    response="${response,,}"   # lowercase
    case "$response" in
        y|yes) echo "yes"; return 0 ;;
        n|no)  echo "no";  return 1 ;;
        *)     echo "no";  return 1 ;;  # default
    esac
}

main() {
    echo "This will install: build-essential, mtools, and optionally qemu-system-x86_64."
    echo

    if ! confirm "Continue? [Y/n]: "; then
        echo "Aborted."
        exit 1
    fi

    echo
    echo "Updating apt..."
    sudo apt update

    echo "Installing base packages..."
    sudo apt install -y build-essential mtools clang

    if is_wsl; then
        echo
        echo "WSL detected — qemu will not be installed automatically."
        echo "If you wish, you can install native Windows QEMU from:"
        echo "  https://qemu.weilnetz.de/w64/"
        echo
        if confirm "Install Linux QEMU inside WSL anyway? [y/N]: "; then
            sudo apt install -y qemu-system-x86
        else
            echo "Skipping QEMU installation."
        fi
    else
        echo "Installing qemu-system-x86 (includes qemu-system-x86_64)..."
        sudo apt install -y qemu-system-x86
    fi
	
	if confirm "Automaticlly allow all scripts? [y/N]: "; then
        cd .. && make chmod_all
	fi

    echo
    echo "Done!"
}

main "$@"
