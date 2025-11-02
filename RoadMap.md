# CHTL Project Roadmap

This document outlines the development roadmap for the CHTL project.

## Phase 1: Core Compiler Infrastructure

- [ ] **Project Setup**
  - [x] Create `RoadMap.md`
  - [x] Set up `.gitignore`
  - [x] Create `build.py` script for CMake automation
  - [x] Establish basic project structure (`src`, `tests`, `Module`)
  - [x] Configure root `CMakeLists.txt`

- [ ] **Lexer Implementation**
  - [x] Define `Token` structure
  - [x] Implement `Lexer` class
  - [ ] Tokenize basic keywords, identifiers, and literals

- [ ] **Testing Framework**
  - [x] Integrate Google Test
  - [x] Write initial Lexer tests

- [x] **Parser Implementation**
  - [x] Define AST Node hierarchy (`BaseNode`, `ElementNode`, `TextNode`, etc.)
  - [x] Implement basic recursive descent parser
  - [x] Parse simple element structures

- [ ] **Generator Implementation**
  - [ ] Implement `Generator` class
  - [ ] Traverse AST and generate basic HTML output

## Phase 2: Language Features

- [x] Comments (`//`, `/**/`, `#`)
- [x] Text Nodes (`text {}` and `text:` attribute)
- [x] Unquoted Literals
- [x] Element Attributes
- [x] Local Style Blocks (`style {}`)
- [x] Templates (`[Template]`)
- [x] Custom Templates (`[Custom]`)
- [ ] Origin Blocks (`[Origin]`)
- [ ] Imports (`[Import]`)
- [ ] Namespaces (`[Namespace]`)
- [ ] Configuration (`[Configuration]`)

## Phase 3: CHTL JS Integration

- [ ] Shared Core (Salt Bridge)
- [ ] CHTL JS Lexer
- [ ] CHTL JS Parser
- [ ] CHTL JS Generator
- [ ] Enhanced Selectors (`{{...}}`)
- [ ] Event Handling (`Listen`, `&->`, `Delegate`)
- [ ] Animations (`Animate`)
- [ ] Virtual Objects (`Vir`)
- [ ] Routing (`Router`)

## Phase 4: Tooling and Ecosystem

- [ ] CLI (Command-Line Interface)
- [ ] VSCode Extension
- [ ] CMOD/CJMOD Module System
- [ ] Official Modules (Chtholly, Yuigahama)
