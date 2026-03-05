#!/bin/bash

# macOS SmCLI Installer
# -------------------------

set -e

# ANSI Colors
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

echo -e "${CYAN}${BOLD}--- SmCLI Installer for macOS ---${NC}\n"

# 1. Check for Xcode Command Line Tools
if ! command -v g++ &> /dev/null; then
    echo -e "Installing Command Line Tools..."
    xcode-select --install
    exit 1
fi

# 2. Build from root
cd ../../../
echo "Building SmCLI..."
make clean all

# 3. Install binary
echo "Installing to /usr/local/bin..."
sudo cp bin/smcli /usr/local/bin/
sudo chmod +x /usr/local/bin/smcli

# 4. Completion setup
echo "Setting up shell completions..."
ABS_PATH=$(pwd)
COMP_FILE="$ABS_PATH/completions/smcli-completion.sh"

# Bash completion
if [ -f "$HOME/.bash_profile" ]; then
    if ! grep -q "smcli-completion.sh" "$HOME/.bash_profile"; then
        echo -e "\nsource $COMP_FILE" >> "$HOME/.bash_profile"
    fi
fi

# Zsh completion (macOS default)
if [ -f "$HOME/.zshrc" ]; then
    if ! grep -q "smcli-completion.sh" "$HOME/.zshrc"; then
        echo -e "\nsource $COMP_FILE" >> "$HOME/.zshrc"
    fi
fi

echo -e "\n${CYAN}${BOLD}✓ Installation complete!${NC}"
echo -e "Type 'smcli help' to start.\n"
