# Custom Shell Mini Project

## Overview
This project involves building a custom shell in C. The shell supports basic system commands, custom commands, and handles both foreground and background processes.

## Directory Structure
- `README.md` - This file.
- `makefile` - Used to compile the project and generate the executable `a.out`.
- `main.c` - The entry point for the shell.
- `shell_utils.c` - Contains utility functions for the shell.
- `commands.c` - Implements custom commands like `hop`, `reveal`, `log`, etc.
- `process.c` - Handles process management (foreground/background).

## Features
- **Shell Prompt**: Displays `<Username@SystemName:~>` and updates with directory changes.
- **Input Handling**: Supports multiple commands separated by `;` or `&`, handles spaces and tabs.
- **Custom Commands**: 
  - `hop`: Change directories, support `~`, `.`, `-` and `..`.
  - `reveal`: List files and directories with flags `-a` and `-l`.
  - `log`: Track command history, execute commands from history.
  - `proclore`: Display process information.
  - `seek`: Search for files or directories.
- **Process Management**: 
  - Foreground: Waits for process completion, logs time if > 2s.
  - Background: Executes without blocking the shell, notifies upon completion.

## Assumptions
- The command length will have a length of less that 4096 characters.
- The maximum number of aliases that can be stored is 256 and its maximum size is 256.
- Few assumptions being made during the definition of the shrc file is that any comment starts in a new line, there is no excessive spacing during the definition of aliases and functions
- Aliases follow the following format of definition `alias al = command` correct upto the spacing.
- Paths/names do not contain whitespace.
- Background process handling is not required for custom commands.
- Piping and redirection are supported for both user and system commands.

## Compilation
Run `make` to compile the project. This will generate an executable named `shell`.

## Execution
Start the shell by running `shell`.

## Error Handling
- Errors do not crash the shell.

## Prompts
https://chatgpt.com/share/fb63b982-92d6-4027-9a88-aaac772038ec
