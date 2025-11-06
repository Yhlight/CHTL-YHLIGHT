# CHTL Project Roadmap

This document outlines the development plan for the CHTL language and its associated tooling.

## Phase 1: Core Compiler Infrastructure (Completed)

- [x] **Project Setup**
  - [x] Create `RoadMap.md`
  - [x] Create `.gitignore`
  - [x] Establish directory structure (`src`, `tests`, `src/SharedCore`, `src/CHTL`, `src/CHTLJS`)
  - [x] Implement Python build script (`build.py`)
  - [x] Set up root `CMakeLists.txt`

- [x] **Lexer (CHTL)**
  - [x] Implement basic tokenization (identifiers, keywords, symbols, literals)
  - [x] Add support for comments (`//`, `/**/`, `#`)
  - [ ] Differentiate between block keywords (`[Template]`) and regular identifiers

- [x] **Parser (CHTL)**
  - [x] Implement basic AST node structure (`ASTNode`, `ProgramNode`)
  - [x] Parse text nodes (`text {}`)
  - [x] Parse element nodes (`div {}`)
  - [x] Parse attributes (`id: "box";`)

- [x] **Testing**
  - [x] Set up Google Test framework
  - [x] Write unit tests for the Lexer
  - [x] Write unit tests for the Parser

## Phase 2: Basic Language Features (In Progress)

- [x] **Generator (CHTL)**
  - [x] Implement a basic generator to traverse the AST
  - [x] Generate HTML from element and text nodes
  - [x] Handle attributes

- [ ] **Style Blocks**
  - [ ] Parse `style {}` blocks
  - [ ] Parse style properties (`width: 100px;`)
  - [ ] Generate inline styles

- [ ] **Templates**
  - [ ] Parse `[Template]` blocks (`@Style`, `@Element`)
  - [ ] Implement Analyser pass to collect templates
  - [ ] Implement Analyser pass to resolve template usage

- [ ] **Imports**
  - [ ] Parse `[Import]` statements
  - [ ] Implement file loading and parsing for CHTL files

## Phase 3: Advanced CHTL and CHTLJS Integration

- [ ] **CHTLJS Core**
  - [ ] Implement CHTLJS Lexer
  - [ ] Implement CHTLJS Parser (Pratt Parser)
  - [ ] Implement CHTLJS Generator

- [ ] **Enhanced Selectors**
  - [ ] Parse `{{...}}` syntax
  - [ ] Generate corresponding JavaScript DOM queries

- [ ] **Event Handling**
  - [ ] Parse `Listen` and `Delegate` blocks
  - [ ] Generate JavaScript `addEventListener` calls

- [ ] **Salt Bridge (SharedCore)**
  - [ ] Design and implement the communication bridge between CHTL and CHTLJS compilers

## Phase 4: Tooling and Modules

- [ ] **CLI**
  - [ ] Implement command-line interface for the compiler
  - [ ] Add support for `--inline` and other flags

- [ ] **CMOD System**
  - [ ] Implement module packaging
  - [ ] Implement module importing and resolution

- [ ] **VSCode Extension**
  - [ ] Develop basic syntax highlighting
  - [ ] Implement code formatting
