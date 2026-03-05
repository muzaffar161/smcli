#!/bin/bash

# Linux SmCLI Installer
# -------------------------

set -e

# ANSI Colors
GREEN='\033[0;32m'
BOLD='\033[1m'
NC='\033[0m'

echo -e "${GREEN}${BOLD}--- SmCLI Installer for Linux ---${NC}\n"

# 1. Check for Dependencies
for cmd in g++ make; do
    if ! command -v $cmd &> /dev/null; then
        echo -e "Error: $cmd is not installed. Please install 'build-essential' or similar."
        exit 1
    fi
done

# 2. Build from root
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$SCRIPT_DIR/../../.."
cd "$PROJECT_ROOT"

echo "Building SmCLI..."
make clean all

# 3. Install binary
echo "Installing to /usr/bin..."
sudo cp bin/smcli /usr/bin/
sudo chmod +x /usr/bin/smcli

# 4. Completion setup
echo "Installing shell completion to /etc/bash_completion.d/..."
sudo cp completions/smcli-completion.sh /etc/bash_completion.d/smcli

echo -e "\n${GREEN}${BOLD}✓ Installation complete!${NC}"
echo -e "Type 'smcli help' to start.\n"
