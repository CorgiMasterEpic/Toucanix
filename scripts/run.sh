#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

is_wsl() {
    grep -qi "microsoft" /proc/sys/kernel/osrelease 2>/dev/null
}

main() {
    if is_wsl; then
        cd kernel

        # Convert batch filenames to Windows paths for cmd.exe
        bat1=$(wslpath -w "$(pwd)/run.bat")
        bat2=$(wslpath -w "$(pwd)/run2.bat")

        echo "Running run.bat..."
        if ! cmd.exe /c "$bat1"; then
            echo "run.bat failed — running fallback run2.bat..."
            cmd.exe /c "$bat2"
        fi

    else
        ./scripts/build.sh
        cd kernel && make run
    fi
}

main "$@"
