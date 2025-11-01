# CHTL Project Roadmap

This document outlines the development plan for the CHTL compiler and its ecosystem.

## Phase 1: Core Compiler Infrastructure

- [x] **Project Setup**: Initialize the repository with the basic directory structure, build scripts, and configuration files.
- [x] **Lexer Implementation**: Create the lexical analyzer to tokenize the CHTL source code.
- [ ] **Parser & AST**: Implement the parser to build an Abstract Syntax Tree (AST) from the token stream. Define the core AST nodes.
- [ ] **Basic Generator**: Implement a simple generator to traverse the AST and produce basic HTML output.
- [ ] **Testing Framework**: Set up the Google Test framework and create initial tests for the core components.

## Phase 2: Basic CHTL Features

- [ ] **Text and Element Nodes**: Implement parsing and generation for basic text and HTML element nodes with attributes.
- [ ] **Local Style Blocks**: Support for `style {}` blocks within elements for inline styles.
- [ ] **Comments**: Handle `//`, `/**/`, and `#` comments correctly.
- [ ] **CE Equivalence**: Ensure `:` and `=` are treated as equivalent for property assignments.

## Phase 3: Advanced Styling and Expressions

- [ ] **Property Expressions**: Implement support for arithmetic and conditional expressions in style properties.
- [ ] **Property Access**: Implement the ability to reference other element properties (e.g., `#box.width`).
- [ ] **Advanced Selectors**: Support for automated class/ID generation and `&` context identifier in local style blocks.

## Phase 4: Templating and Modularity

- [ ] **Templates**: Implement `[Template]` for styles, elements, and variables.
- [ ] **Template Inheritance**: Support both implicit and explicit (`inherit`) template inheritance.
- [ ] **Custom Templates**: Implement `[Custom]` templates with placeholders and specialization features (delete, insert).
- [ ] **Imports**: Implement `[Import]` for CHTL, HTML, CSS, and JS files.
- [ ] **Namespaces**: Implement `[Namespace]` to manage scope and prevent conflicts.
- [ ] **Origin Blocks**: Implement `[Origin]` for embedding raw, unprocessed code.

## Phase 5: CHTL JS Integration & Tooling

- [ ] **CHTL JS Bridge**: Set up the basic infrastructure for the CHTL JS compiler to interact with the CHTL compiler.
- [ ] **Enhanced Selectors**: Implement `{{...}}` selectors in `script` blocks.
- [ ] **Module System**: Design and implement the CMOD and CJMOD module systems.
- [ ] **CLI**: Develop the command-line interface for compiling CHTL files.
- [ ] **Configuration**: Implement `[Configuration]` blocks for customizing compiler behavior.

## Phase 6: Ecosystem and Finalization

- [ ] **VSCode Extension**: Develop a VSCode extension for syntax highlighting, code completion, and other IDE features.
- [ ] **Official Modules**: Develop the official `Chtholly` and `Yuigahama` modules.
- [ ] **Documentation**: Finalize all user and developer documentation.
- [ ] **Performance Optimization**: Profile and optimize the compiler.
