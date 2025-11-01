# CHTL Development Roadmap

This document outlines the development plan for the CHTL compiler. The project will be developed in phases, following a Test-Driven Development (TDD) approach to ensure stability and correctness.

## Phase 1: Core Compiler Infrastructure

The primary goal of this phase is to establish the fundamental architecture of the CHTL compiler.

- [ ] **Project Setup**: Initialize the C++ project using CMake, set up the directory structure, and configure the build system.
- [ ] **Lexer (Tokenizer)**: Implement the lexer to scan CHTL source code and convert it into a stream of tokens.
    - [ ] Support for comments (`//`, `/**/`, `#`).
    - [ ] Support for keywords, identifiers, literals (string, unquoted), and symbols.
- [ ] **Parser**: Implement a parser to consume the token stream and build an Abstract Syntax Tree (AST).
    - [ ] Define AST node structures for basic elements, text, and attributes.
- [ ] **Generator**: Implement a basic code generator that traverses the AST and produces a simple HTML output.
- [ ] **Command-Line Interface (CLI)**: Create a basic CLI to compile a `.chtl` file and output the result.
- [ ] **Testing Framework**: Set up a testing framework (e.g., Google Test) and write initial tests for the lexer and parser.

## Phase 2: Core CHTL Language Features

This phase focuses on implementing the essential features of the CHTL language for generating static HTML.

- [ ] **Text Nodes and Literals**: Implement `text {}` blocks and unquoted string literals.
- [ ] **Element Nodes and Attributes**: Support for all standard HTML elements and attribute syntax (`key: value;`).
- [ ] **[Origin] Blocks**: Implement `[Origin]` blocks to allow embedding raw HTML, CSS, and JavaScript.

## Phase 3: Advanced CHTL Features

This phase introduces more complex and powerful features for styling and code organization.

- [ ] **Local Style Blocks**:
    - [ ] Inline style generation from `style {}` blocks.
    - [ ] Automatic class/ID attachment.
    - [ ] Context deduction with `&`.
- [ ] **Style Expressions**:
    - [ ] Attribute arithmetic (`width: 100px + 50px`).
    - [ ] Attribute referencing (`width: .box.width`).
    - [ ] Conditional expressions (`background-color: width > 50px ? "red" : "blue"`).
- [ ] **Templates and Customization**:
    - [ ] Implement `[Template]` blocks for styles, elements, and variables.
    - [ ] Implement `[Custom]` blocks with specialization features (e.g., `delete`, `insert`).
- [ ] **Imports**:
    - [ ] Implement `[Import]` for CHTL, HTML, CSS, and JS files.
- [ ] **Namespaces**:
    - [ ] Implement `[Namespace]` to prevent naming conflicts.

## Phase 4: CHTL JS Integration

This phase focuses on integrating the CHTL JS language and its dynamic features.

- [ ] **Shared Core (Salt Bridge)**: Develop the communication layer between the CHTL and CHTL JS compilers.
- [ ] **CHTL JS Lexer & Parser**: Implement the lexer and parser for CHTL JS syntax.
- [ ] **CHTL JS Generator**: Implement the generator to transpile CHTL JS into standard JavaScript.
- [ ] **Enhanced Selectors**: Implement `{{selector}}` syntax.
- [ ] **Dynamic Features**:
    - [ ] Dynamic conditional rendering.
    - [ ] Responsive values (`$jsVariable$`).
    - [ ] Event listeners (`Listen {}`, `&->`).

## Phase 5: Tooling and Ecosystem

This phase involves building the surrounding ecosystem and developer tools.

- [ ] **Module System**:
    - [ ] Implement the CMOD packaging system.
    - [ ] Implement the CJMOD system and API.
- [ ] **Build and Packaging**: Create scripts to build the compiler and package modules.
- [ ] **VSCode Extension**: Develop a VSCode extension providing syntax highlighting, code completion, and other IDE features.
