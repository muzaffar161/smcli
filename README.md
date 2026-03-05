# SmCLI

A smcli command-line interface.

## Project Structure

- `build.sh`: The main build script for the project.
- `src/`: Contains all the C++ source code (`.cpp` files).
- `include/`: Contains all the C++ header files (`.h`, `.hpp` files).
- `completions/`: Contains shell completion scripts.
- `dist/`: Contains installers and other distribution-related files.
- `bin/`: Contains the compiled executable.

## Building

To build the project, run the following command from the root directory:

```bash
./build.sh
```

This will compile the source code and create the `smcli` executable in the `bin/` directory.

## Features

### `show`

Displays a tree-like map of the specified directory or the current directory if no path is provided. File and directory sizes are displayed in a human-readable format (B, KB, MB, GB, TB).

**Usage:**

```bash
# Show map of the current directory
./bin/smcli show

# Show map of a specific directory
./bin/smcli show /path/to/directory

# Limit tree display to 2 levels deep
./bin/smcli show /path/to/directory --depth 2

# Exclude specific files or directories from the tree
./bin/smcli show /path/to/directory --exclude node_modules --exclude .git
```

**Options:**
*   `--depth <N>`: Limit tree display to N levels deep.
*   `--exclude <name>`: Exclude files or directories with the given name.

### `copy` (Alias: `cp`)

Copies a file or directory from a source to a destination. Optionally, you can specify a new name for the copied item. If a conflict occurs (item with the same name already exists and no new name is provided), a " copy" suffix will be added to the new item's name.

**Usage:**

```bash
# Copy 'file.txt' to 'destination_dir/'
./bin/smcli copy file.txt to destination_dir/
./bin/smcli cp file.txt to destination_dir/

# Copy 'folder_a' to 'destination_dir/' as 'new_folder_name'
./bin/smcli copy folder_a to destination_dir/ as new_folder_name
./bin/smcli cp folder_a to destination_dir/ as new_folder_name
```

### `move` (Alias: `mv`)

Moves (renames) a file or directory from a source to a destination. Optionally, you can specify a new name for the moved item. If a conflict occurs (item with the same name already exists and no new name is provided), a " copy" suffix will be added to the new item's name.

**Usage:**

```bash
# Move 'file.txt' to 'destination_dir/'
./bin/smcli move file.txt to destination_dir/
./bin/smcli mv file.txt to destination_dir/

# Move 'folder_a' to 'destination_dir/' as 'new_folder_name'
./bin/smcli move folder_a to destination_dir/ as new_folder_name
./bin/smcli mv folder_a to destination_dir/ as new_folder_name
```

### `import` (Alias: `get`)

Copies a file or directory from a source to the current working directory where `smcli` is executed. Optionally, you can specify a new name for the imported item. If a conflict occurs (item with the same name already exists and no new name is provided), a " copy" suffix will be added to the new item's name.

**Usage:**

```bash
# Import 'path/to/file.txt' to the current directory
./bin/smcli import /path/to/file.txt
./bin/smcli get /path/to/file.txt

# Import 'path/to/folder_a' to the current directory as 'imported_folder'
./bin/smcli import /path/to/folder_a as imported_folder
./bin/smcli get /path/to/folder_a as imported_folder
```

### `search` (Alias: `find`)

Searches for files and folders matching a query in the current directory and its subdirectories.

**Usage:**

```bash
# Search for items with "report" in their name
./bin/smcli search report
./bin/smcli find report

# Search for files with "document" in their name
./bin/smcli search document -f
./bin/smcli find document -f

# Search for folders with "project" in their name
./bin/smcli search project -fl
./bin/smcli find project -fl

# Search for image files with "vacation" in their name
./bin/smcli search vacation -img
./bin/smcli find vacation -img

# Search for video files with "family" in their name
./bin/smcli search family -vid
./bin/smcli find family -vid

# Search for a file with the exact name "my_document.pdf"
./bin/smcli search my_document.pdf --exact-name
./bin/smcli find my_document.pdf --exact-name

# Search for items only in the current directory (depth 0)
./bin/smcli search my_file --depth 0
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
