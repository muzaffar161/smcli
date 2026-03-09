# Changelog

All notable changes to the **smartTerm (smcli)** project will be documented in this file.

## [1.0.0] - 2026-03-09

### Added
- **Windows UTF-8 Support**: Added `SetConsoleOutputCP(65001)` in `main.cpp` to ensure correct display of tree symbols (`├──`, `└──`) and Cyrillic characters in the Windows console.
- **Accurate Folder Size**: Modified `ShowCommand` to recursively calculate the total size of a directory, including items excluded by `--exclude` or hidden by `--depth` limits.
- **Improved Windows Installer**: Updated `setup.iss` (Inno Setup) to:
    - Install to `C:\smcli\bin` by default.
    - Automatically add the installation path to the system `Path` environment variable.
    - Remove the unnecessary desktop shortcut for a cleaner CLI experience.
- **GitHub Actions Automation**: 
    - Automated build process for Windows (x64), macOS (ARM64), and Linux (x64).
    - Cleaned up artifact structure: each platform now has a dedicated folder containing the binary archive and installers.
    - Added support for existing `install.sh` scripts for macOS and Linux in the build pipeline.
- **Enhanced Search**: Implemented `u8string()` support in `SearchCommand` for proper handling of Unicode filenames.

### Fixed
- Fixed a bug where the "Total size" in the `show` command would decrease when files were excluded from the visual tree.
- Resolved "garbled text" issues in the Windows terminal by switching to UTF-8 output mode.
- Fixed GitHub Actions build errors on Windows by switching to the `bash` shell for all runner steps.

### Initial Features (Pre-release)
- `show`: Recursive directory tree visualization with size formatting.
- `copy` / `cp`: Robust file and directory copying with "copy" suffix for collisions.
- `move` / `mv`: File and directory moving/renaming.
- `import` / `get`: Quick import of files to the current working directory.
- `search` / `find`: Multi-threaded file searching with filters for images, videos, and depth limits.
- **Bash Completion**: Auto-completion script for faster command-line usage.

---
*Project started: March 2026*
