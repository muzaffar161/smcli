# SmCLI

![C++](https://img.shields.io/badge/language-C%2B%2B-blue)
![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)
![Type](https://img.shields.io/badge/type-CLI%20Tool-purple)
![Beginner Friendly](https://img.shields.io/badge/good%20for-beginners-brightgreen)
![Design](https://img.shields.io/badge/design-simple-orange)
![Performance](https://img.shields.io/badge/performance-lightweight-yellow)
![Cross Platform](https://img.shields.io/badge/cross--platform-supported-success)

![GitHub stars](https://img.shields.io/github/stars/muzaffar161/smcli)
![GitHub forks](https://img.shields.io/github/forks/muzaffar161/smcli)
![GitHub issues](https://img.shields.io/github/issues/muzaffar161/smcli)


A simple and lightweight cross-platform CLI tool for file management and directory exploration.


SmCLI is a cross-platform command line tool written in C++ for file management, search, and directory visualization.

It provides fast CLI utilities for copying, moving, and searching files, as well as displaying directory trees directly from the terminal.

Supports:
- macOS
- Linux
- Windows

## Quick Example

```bash
smcli show
smcli search report
smcli du /Users/user/Movies
smcli copy file.txt to backup/
smcli move file.txt to archive/
```

## Project Structure

- `build.sh`: The main build script for the project.
- `src/`: Contains all the C++ source code (`.cpp` files).
- `include/`: Contains all the C++ header files (`.h`, `.hpp` files).
- `completions/`: Contains shell completion scripts.
- `dist/`: Contains installers and other distribution-related files.
- `bin/`: Contains the compiled executable.
- `.smcliignore`: Local configuration file for ignoring specific files and directories.
- `.smcliignore-global`: Global ignore file (located next to the `smcli` binary).

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

**Smart Ignoring:** Automatically reads local `.smcliignore` (searching up the tree) and global `.smcliignore-global` to skip unnecessary files.

**Usage:**

```bash
# Show map of the current directory
smcli show

# Show map of a specific directory
smcli show /path/to/directory

# Limit tree display to 2 levels deep
smcli show /path/to/directory --depth 2

# Exclude multiple items manually
smcli show --exclude node_modules --exclude .git --exclude "*.log"

# Show all files, bypassing all ignore rules
smcli show --no-ignore
```

**Options:**
*   `--depth <N>`: Limit tree display to N levels deep.
*   `--exclude <name>`: Exclude specific files or directories (can be used multiple times).
*   `--no-ignore`: Show all files, bypassing `.smcliignore` and global ignore.

### `du` (Disk Usage)

Visualizes disk space usage with beautiful progress bars, showing which folders take up the most space.

**Example output:**
`[████████░░] 80% (2GB) /Users/user/Movies`

**Usage:**

```bash
# Analyze current directory
smcli du

# Analyze a specific path
smcli du /path/to/folder
```

### Global Ignore Management

Manage a global list of patterns to ignore across all your projects.

```bash
# Add a pattern (e.g., all temp files) to global ignore list
smcli --exclude-add-global "*.tmp"

# Add a specific folder to global ignore
smcli --exclude-add-global "build/"
```

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

Searches for files and folders matching a query with advanced filtering and content search.

**Usage:**

```bash
# Search for items with "report" in their name
smcli search report
smcli find report

# Search INSIDE file contents (Content Search)
smcli search "TODO" --content

# Search with multiple manual excludes
smcli search "fix" --exclude "dist" --exclude "bin" --exclude "*.bak"

# Search with size filters (e.g., find large files)
smcli search "" --min-size 100MB --max-size 2GB

# Search by modification date
# (Find files modified in the last 3 days)
smcli search "" --newer-than 3

# Search for image files only
smcli search vacation -img

# Search for video files only
smcli search family -vid

# Search for exact name match (case-insensitive)
smcli search my_document.pdf --exact-name

# Search bypassing all ignore rules (local and global)
smcli search "API_KEY" --content --no-ignore
```

**Flags:**
*   `-f`: Search for files only.
*   `-fl`: Search for folders only.
*   `-c, --content`: Search inside file contents.
*   `-img`: Search for image files (e.g., .jpg, .png, .gif).
*   `-vid`: Search for video files (e.g., .mp4, .mov, .avi).
*   `--min-size <S>`: Search files larger than size S (e.g., 10MB).
*   `--max-size <S>`: Search files smaller than size S.
*   `--newer-than <N>`: Modified in the last N days.
*   `--older-than <N>`: Modified more than N days ago.
*   `--exclude <name>`: Exclude specific name (can be used multiple times).
*   `--exact-name`: Search for exact name match (case-insensitive).
*   `--depth <N>`: Limit search to N levels deep.
*   `--no-ignore`: Search all files, bypassing ignore files.

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


## Keywords

cli tool  
command line utility  
cpp cli tool  
file manager cli  
terminal file search  
cross platform cli  
disk usage visualizer
