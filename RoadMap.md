# CHTL Project Roadmap

This document outlines the development roadmap for the CHTL project.

## Phase 1: Core Compiler Infrastructure

- [x] **Project Setup**
  - [x] Create `RoadMap.md`
  - [x] Set up `.gitignore`
  - [x] Create `build.py` script for CMake automation
  - [x] Establish basic project structure (`src`, `tests`, `Module`)
  - [x] Configure root `CMakeLists.txt`

- [x] **Lexer Implementation**
  - [x] Define `Token` structure
  - [x] Implement `Lexer` class
  - [x] Tokenize basic keywords, identifiers, and literals

- [x] **Testing Framework**
  - [x] Integrate Google Test
  - [x] Write initial Lexer tests

- [x] **Parser Implementation**
  - [x] Define AST Node hierarchy (`BaseNode`, `ElementNode`, `TextNode`, etc.)
  - [x] Implement basic recursive descent parser
  - [x] Parse simple element structures

- [x] **Generator Implementation**
  - [x] Create `Generator` class structure
  - [x] Implement visitor for `ProgramNode`
  - [x] Implement visitor for `ElementNode` (handles tags, attributes, and children)
  - [x] Implement visitor for `TextNode`
  - [x] Implement visitor for `OriginNode` (preserves raw content)

## Phase 2: Language Features

- [x] Comments (`//`, `/**/`, `#`)
- [x] Text Nodes (`text {}` and `text:` attribute)
- [x] Unquoted Literals
- [x] Element Attributes
- [x] Local Style Blocks (`style {}`)
- [x] Templates (`[Template]`)
- [x] Custom Templates (`[Custom]`)
- [x] Origin Blocks (`[Origin]`)
- [x] Imports (`[Import]`)
- [x] Namespaces (`[Namespace]`)
- [x] Configuration (`[Configuration]`)

## Phase 3: CHTL JS Integration

- [x] Shared Core (Salt Bridge)
- [x] CHTL JS Lexer
- [x] CHTL JS Parser
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
