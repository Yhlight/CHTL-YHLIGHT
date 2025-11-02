# CHTL Project Roadmap

This document outlines the development roadmap for the CHTL project. The project will be developed following a Test-Driven Development (TDD) methodology to ensure stability and correctness.

## Phase 1: Core Compiler Implementation

- **[X] Lexer:** Implement the lexical analyzer to tokenize the CHTL source code.
  - [X] Basic tokenization (keywords, identifiers, literals)
  - [X] Comment handling (`//`, `/**/`, `#`)
  - [X] Support for all specified token types in `CHTL.md`
- **[ ] Parser:** Implement the parser to build an Abstract Syntax Tree (AST) from the token stream.
   - [X] Basic AST structure
   - [X] Parsing of core language features (elements, attributes, text)
  - [ ] Support for all grammatical constructs in `CHTL.md`
- **[ ] Generator:** Implement the code generator to produce HTML from the AST.
  - [ ] Basic HTML generation
  - [ ] Generation of all CHTL features into corresponding HTML and CSS

## Phase 2: Advanced Language Features

- **[ ] CHTL JS Support:** Implement the necessary components to support CHTL JS.
  - [ ] Salt bridge for communication between CHTL and CHTL JS compilers
  - [ ] CHTL JS preprocessor
  - [ ] CHTL JS parser and generator
- **[ ] Module System:** Implement the CMOD and CJMOD module systems.
  - [ ] CMOD packaging and unpacking
  - [ ] CJMOD support
  - [ ] Module import and resolution logic
- **[ ] CLI and Tooling:** Develop the command-line interface and other development tools.
  - [ ] Basic compiler CLI
  - [ ] File watching and recompilation
  - [ ] VSCode extension support

## Phase 3: Official Modules

- **[ ] Chtholly Module:** Implement the official "Chtholly" module.
- **[ ] Yuigahama Module:** Implement the official "Yuigahama" module.
