# CHTL Project Roadmap

This document outlines the development roadmap for the CHTL project. The development will be broken down into several phases, focusing on a Test-Driven Development (TDD) approach to ensure stability and correctness.

## Phase 1: Core Compiler Infrastructure

- [x] **Lexer (Tokenizer):** Implement the lexical analyzer to convert CHTL source code into a stream of tokens.
- [ ] **Parser:** Implement the parser to build an Abstract Syntax Tree (AST) from the token stream.
- [ ] **AST Node Definitions:** Define the structure of the AST nodes for all CHTL grammar constructs.
- [ ] **Basic Code Generation:** Implement a basic code generator to traverse the AST and produce HTML.
- [ ] **CMake Build System:** Establish the initial CMake build system for the C++ project.
- [ ] **Python Build Script:** Create a Python script to automate the build process.
- [ ] **Testing Framework:** Set up the Google Test framework and write initial tests for the lexer and parser.

## Phase 2: CHTL Language Features

- [ ] **Text and Element Nodes:** Implement parsing and code generation for `text` and standard HTML element nodes.
- [ ] **Attributes:** Add support for element attributes.
- [ ] **Local Style Blocks (`style {}`):**
    - [ ] Inline styles.
    - [ ] Automatic class/id generation.
    - [ ] Context-aware `&` selector.
- [ ] **Advanced Style Features:**
    - [ ] Attribute arithmetic (`width: 100px + 50px`).
    - [ ] Attribute referencing (`width: .box.width`).
    - [ ] Conditional expressions (`background-color: width > 50px ? "red" : "blue"`).
- [ ] **Templates:**
    - [ ] Style group templates (`[Template] @Style`).
    - [ ] Element templates (`[Template] @Element`).
    - [ ] Variable group templates (`[Template] @Var`).
    - [ ] Template inheritance.
- [ ] **Customization (`[Custom]`):**
    - [ ] Custom style groups with value placeholders.
    - [ ] Specialization (deleting properties, inserting/deleting elements).
- [ ] **Imports (`[Import]`):**
    - [ ] Import CHTL, HTML, CSS, and JS files.
    - [ ] Namespace support.
- [ ] **Configuration (`[Configuration]`):**
    - [ ] Implement support for custom configurations.
    - [ ] Keyword renaming (`[Name]`).

## Phase 3: CHTL JS Integration

- [ ] **Salt Bridge (Shared Core):** Design and implement the communication bridge between the CHTL and CHTL JS compilers.
- [ ] **CHTL JS Lexer and Parser:** Implement the lexer and parser for the CHTL JS syntax.
- [ ] **Enhanced Selectors (`{{...}}`):** Implement the enhanced DOM element selector syntax.
- [ ] **Local Script Blocks (`script {}`):** Handle local script blocks and their interaction with the global script context.
- [ ] **Dynamic Features:**
    - [ ] Dynamic attribute conditional expressions.
    - [ ] Dynamic conditional rendering.
    - [ ] Responsive values (`$jsVariable$`).

## Phase 4: Module System (CMOD and CJMOD)

- [ ] **CMOD Packaging:** Create the tooling to package CHTL files into `.cmod` modules.
- [ ] **CJMOD API:** Implement the C++ API for creating `.cjmod` extensions.
- [ ] **Module Resolution and Loading:** Implement the logic for resolving and loading CMOD and CJMOD modules from different paths.
- [ ] **Official Modules:** Begin development of the official `Chtholly` and `Yuigahama` modules.

## Phase 5: Tooling and IDE Support

- [ ] **Command-Line Interface (CLI):**
    - [ ] Basic CLI for compiling files.
    - [ ] Advanced features like `--inline` and `--default-struct`.
- [ ] **VSCode Extension:**
    - [ ] Syntax highlighting.
    - [ ] Code formatting.
    - [ ] Snippets and code completion.
    - [ ] Real-time preview.

## Phase 6: Documentation and Refinement

- [ ] **User Documentation:** Write comprehensive documentation for the CHTL language and its features.
- [ ] **API Documentation:** Document the CJMOD API for module developers.
- [ ] **Performance Optimization:** Profile and optimize the compiler.
- [ ] **Community Feedback:** Incorporate feedback from the community to refine the language and tools.
