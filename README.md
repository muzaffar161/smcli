# SmCLI
![C++](https://img.shields.io/badge/language-C%2B%2B-blue)
![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)


A smcli command-line interface.

SmCLI is a cross-platform command line tool written in C++ for file management, search, and directory visualization.

It provides fast CLI utilities for copying, moving, searching files and displaying directory trees directly from the terminal.

Supports:
- macOS
- Linux
- Windows

## Project Structure

- `build.sh`: The main build script for the project.
- `src/`: Contains all the C++ source code (`.cpp` files).
- `include/`: Contains all the C++ header files (`.h`, `.hpp` files).
- `completions/`: Contains shell completion scripts.
- `dist/`: Contains installers and other distribution-related files.
- `bin/`: Contains the compiled executable.

## Installation

Choose the installer for your operating system from the `dist/installers/` directory.

### 🍎 macOS

1.  Open Terminal.
2.  Navigate to the macOS installer directory:
    ```bash
    cd dist/installers/smcliMac
    ```
3.  Run the installer:
    ```bash
    chmod +x install.sh
    ./install.sh
    ```
4.  Restart your terminal or run `source ~/.zshrc` (for Zsh) or `source ~/.bash_profile` (for Bash) to enable auto-completion.

### 🐧 Linux

1.  Open Terminal.
2.  Navigate to the Linux installer directory:
    ```bash
    cd dist/installers/smcliLinux
    ```
3.  Run the installer:
    ```bash
    chmod +x install.sh
    ./install.sh
    ```
4.  Restart your terminal to activate the command and auto-completion.

### 🪟 Windows

1.  Ensure you have a C++ compiler (like MinGW/g++) installed and added to your `PATH`.
2.  Open **Command Prompt** or **PowerShell** as **Administrator**.
3.  Navigate to the Windows installer directory:
    ```cmd
    cd dist\installers\smcliWin
    ```
4.  Run the installer:
    ```cmd
    install.bat
    ```

---

## Manual Building (Alternative)

If you prefer to build manually, you can use the provided `Makefile` or `build.sh` script:

```bash
# Using Makefile
make clean all

# Using build.sh
./build.sh
```

The compiled binary will be located in the `bin/` directory.

## Features

### `show`

Displays a tree-like map of the specified directory or the current directory if no path is provided. File and directory sizes are displayed in a human-readable format (B, KB, MB, GB, TB).

**Usage:**

```bash
# Show map of the current directory
smcli show

# Show map of a specific directory
smcli show /path/to/directory

# Limit tree display to 2 levels deep
smcli show /path/to/directory --depth 2

# Exclude specific files or directories from the tree
smcli show /path/to/directory --exclude node_modules --exclude .git
```

**Options:**
*   `--depth <N>`: Limit tree display to N levels deep.
*   `--exclude <name>`: Exclude files or directories with the given name.

### `copy` (Alias: `cp`)

Copies a file or directory from a source to a destination. Optionally, you can specify a new name for the copied item. If a conflict occurs (item with the same name already exists and no new name is provided), a " copy" suffix will be added to the new item's name.

**Usage:**

```bash
# Copy 'file.txt' to 'destination_dir/'
smcli copy file.txt to destination_dir/
smcli cp file.txt to destination_dir/

# Copy 'folder_a' to 'destination_dir/' as 'new_folder_name'
smcli copy folder_a to destination_dir/ as new_folder_name
smcli cp folder_a to destination_dir/ as new_folder_name
```

### `move` (Alias: `mv`)

Moves (renames) a file or directory from a source to a destination. Optionally, you can specify a new name for the moved item. If a conflict occurs (item with the same name already exists and no new name is provided), a " copy" suffix will be added to the new item's name.

**Usage:**

```bash
# Move 'file.txt' to 'destination_dir/'
smcli move file.txt to destination_dir/
smcli mv file.txt to destination_dir/

# Move 'folder_a' to 'destination_dir/' as 'new_folder_name'
smcli move folder_a to destination_dir/ as new_folder_name
smcli mv folder_a to destination_dir/ as new_folder_name
```

### `import` (Alias: `get`)

Copies a file or directory from a source to the current working directory where `smcli` is executed. Optionally, you can specify a new name for the imported item. If a conflict occurs (item with the same name already exists and no new name is provided), a " copy" suffix will be added to the new item's name.

**Usage:**

```bash
# Import 'path/to/file.txt' to the current directory
smcli import /path/to/file.txt
smcli get /path/to/file.txt

# Import 'path/to/folder_a' to the current directory as 'imported_folder'
smcli import /path/to/folder_a as imported_folder
smcli get /path/to/folder_a as imported_folder
```

### `search` (Alias: `find`)

Searches for files and folders matching a query in the current directory and its subdirectories.

**Usage:**

```bash
# Search for items with "report" in their name
smcli search report
smcli find report

# Search for files with "document" in their name
smcli search document -f
smcli find document -f

# Search for folders with "project" in their name
smcli search project -fl
smcli find project -fl

# Search for image files with "vacation" in their name
smcli search vacation -img
smcli find vacation -img

# Search for video files with "family" in their name
smcli search family -vid
smcli find family -vid

# Search for a file with the exact name "my_document.pdf"
smcli search my_document.pdf --exact-name
smcli find my_document.pdf --exact-name

# Search for items only in the current directory (depth 0)
smcli search my_file --depth 0
```

**Flags:**
*   `-f`: Search for files only.
*   `-fl`: Search for folders only.
*   `-img`: Search for image files (e.g., .jpg, .png, .gif).
*   `-vid`: Search for video files (e.g., .mp4, .mov, .avi).
*   `--exact-name`: Search for exact name match (case-insensitive).
*   `--depth <N>`: Limit search to N levels deep (0 for current directory only).

**Note on macOS Permissions for `search` command:**
On macOS, for the `search` command to access all directories (especially personal folders like `Documents`, `Downloads`, `Desktop`, etc.), you might need to grant "Full Disk Access" to your terminal application (e.g., Terminal.app, iTerm2.app). You can do this in `System Settings > Privacy & Security > Full Disk Access`. If permissions are not granted, the `search` command will skip inaccessible directories and report them at the end.

## Tab Completion

Tab completion for `smcli` now supports commands, aliases, flags, and path completion for arguments that expect a file or directory path.

To enable tab completion for `smcli`, you need to source the completion script for your shell.

For Bash, add the following to your `~/.bashrc`:

```bash
source /path/to/smcli-completion.sh
```

For Zsh, add the following to your `~/.zshrc`:

```bash
source /path/to/smcli-completion.sh
```

You will need to replace `/path/to/smcli-completion.sh` with the actual path to the `smcli-completion.sh` file in this project.
