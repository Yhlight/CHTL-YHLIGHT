# CHTL Project Roadmap

This document outlines the development and refactoring route for the CHTL project. The project will be developed in phases, with a focus on Test-Driven Development (TDD) to ensure stability and correctness.

## Phase 1: Core Compiler Infrastructure (In Progress)

- [x] **Project Setup**
    - [x] Create `RoadMap.md`
    - [x] Create `.gitignore`
    - [x] Set up the recommended project directory structure
    - [x] Configure CMake for the C++ project
    - [x] Create a Python build script
- [x] **CHTL Lexer**
    - [x] Implement tokenization for basic CHTL syntax (comments, text nodes, elements, attributes)
    - [x] Write comprehensive tests for the lexer
- [ ] **CHTL Parser**
    - [ ] Implement a parser to build an Abstract Syntax Tree (AST) from the lexer's tokens
    - [x] Define node types for the AST
    - [x] Write tests for the parser

## Phase 2: CHTL Language Features

- [ ] **Element and Attribute Handling**
    - [ ] Implement generation of HTML from the AST
    - [ ] Support for all standard HTML elements
- [ ] **Local Style Blocks**
    - [ ] Implement parsing of local `style` blocks
    - [ ] Handle inline styles
    - [ ] Automatic class/id generation
    - [ ] Contextual `&` selector
- [ ] **Attribute Operations**
    - [ ] Implement arithmetic operations on attribute values
    - [ ] Implement attribute referencing
    - [ ] Implement conditional attribute expressions
- [ ] **Templates and Customization**
    - [ ] Implement `[Template]` and `[Custom]` blocks for styles, elements, and variables
    - [ ] Support for inheritance and specialization

## Phase 3: CHTL JS Integration

- [ ] **CHTL JS Preprocessor**
    - [ ] Implement a preprocessor to identify and isolate CHTL JS code blocks
- [ ] **CHTL JS Lexer and Parser**
    - [ ] Implement a lexer and parser for CHTL JS syntax
- [ ] **CHTL JS Features**
    - [ ] Implement enhanced selectors (`{{...}}`)
    - [ ] Implement event listeners (`Listen`, `&->`)
    - [ ] Implement animations (`Animate`)
    - [ ] Implement routing (`Router`)
- [ ] **Salt Bridge (Shared Core)**
    - [ ] Implement the communication bridge between the CHTL and CHTL JS compilers

## Phase 4: Modules and CLI

- [ ] **CMOD and CJMOD**
    - [ ] Implement the CMOD and CJMOD packaging and import system
- [ ] **CLI**
    - [ ] Implement the command-line interface for the CHTL compiler
- [ ] **VSCode IDE Extension**
    - [ ] Develop a VSCode extension for CHTL with the features outlined in `CHTL.md`

This roadmap will be updated as the project progresses.
