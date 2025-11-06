# CHTL Compiler Development Roadmap

This document outlines the development plan for the CHTL compiler.

## Milestone 1: Project Initialization and Core Lexer

- [x] Set up the basic project structure with `src`, `tests`, and `Module` directories.
- [x] Configure the CMake build system and create a `build.py` script.
- [x] Implement the initial Lexer to handle basic tokens, including single-quoted strings.
- [x] Write unit tests to verify the lexer's functionality.

## Milestone 2: Advanced Lexer Features

- [x] Handle unterminated strings gracefully.
- [x] Add support for double-quoted strings.
- [x] Add support for basic symbols (`{`, `}`, `:`, `;`).
- [x] Add support for identifiers and unquoted literals.
- [x] Add support for single-line and multi-line comments.

## Milestone 3: Basic Parser and AST

- [x] Define the core Abstract Syntax Tree (AST) nodes.
- [x] Implement the basic `Parser` class structure.
- [x] Implement parsing for simple and nested elements.
- [x] Write unit tests for the parser.

## Milestone 4: Parser - Attributes and Text Nodes

- [ ] Enhance the lexer to recognize the `=` symbol for attributes.
- [ ] Enhance the AST to support attributes and text nodes.
- [ ] Implement attribute parsing in the parser.
- [ ] Implement text node parsing in the parser.
- [ ] Write unit tests for attribute and text node parsing.
