# CHTL Compiler Development Roadmap

This document outlines the development plan for the CHTL compiler.

## Milestone 1: Project Initialization and Core Lexer

- [x] Set up the basic project structure with `src`, `tests`, and `Module` directories.
- [x] Configure the CMake build system and create a `build.py` script.
- [x] Implement the initial Lexer to handle basic tokens, including single-quoted strings.
- [x] Write unit tests to verify the lexer's functionality.

## Milestone 2: Advanced Lexer Features

- [ ] Handle unterminated strings gracefully.
- [ ] Add support for double-quoted strings.
- [ ] Add support for basic symbols (`{`, `}`, `:`, `;`).
- [ ] Add support for identifiers and unquoted literals.
- [ ] Add support for single-line and multi-line comments.
