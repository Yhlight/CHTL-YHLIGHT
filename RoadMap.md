# CHTL Project Roadmap

This document outlines the development roadmap for the CHTL project, guided by the official `CHTL.md` specification. Development is broken down into phases, adhering to a Test-Driven Development (TDD) approach to ensure stability and correctness.

## Phase 1: Core Compiler Infrastructure (Completed)

- [x] **Lexer (Tokenizer):** Implement the lexical analyzer to convert CHTL source code into a stream of tokens.
- [x] **Parser:** Implement the parser to build an Abstract Syntax Tree (AST) from the token stream.
- [x] **AST Node Definitions:** Define the structure of the AST nodes for all CHTL grammar constructs.
- [x] **Basic Code Generation:** Implement a basic code generator to traverse the AST and produce HTML.
- [x] **CMake Build System:** Establish the initial CMake build system for the C++ project.
- [x] **Python Build Script:** Create a Python script to automate the build process.
- [x] **Testing Framework:** Set up the Google Test framework and write initial tests for the lexer and parser.

## Phase 2: CHTL Language Features

- [x] **Text and Element Nodes:** Implement parsing and code generation for `text` and standard HTML element nodes.
- [x] **Attributes:** Add support for element attributes.
- [x] **Local Style Blocks (`style {}`):**
    - [x] Inline styles.
    - [x] Automatic class/id generation.
    - [x] Context-aware `&` selector (pseudo-classes and pseudo-elements).
- [ ] **Advanced Style Features:**
    - [x] **Attribute Arithmetic:** Implement support for arithmetic operations (`+`, `-`, `*`, `/`, `%`, `**`) on style property values.
        - [x] Update Lexer to tokenize arithmetic operators.
        - [x] Refactor AST to support expression trees for style values.
        - [x] Implement an expression parser to handle operator precedence.
        - [x] Create an expression evaluator to compute final values and handle unit rules.
    - [x] **Attribute Referencing:** Implement the ability for style properties to reference the values of other properties using CSS selectors (`width: .box.width`).
        - [x] Create a `SymbolTable` to store element properties.
        - [x] Add `PropertyAccessNode` to the AST.
        - [x] Update the parser to handle property access expressions.
        - [x] Implement an `Analyser` class to populate the symbol table and resolve references.
    - [x] **Conditional Expressions:** Implement support for ternary conditional expressions in style properties (`background-color: width > 50px ? "red" : "blue"`).
- [x] **Templates (`[Template]`):**
    - [x] Style group templates (`@Style`).
    - [x] Element templates (`@Element`).
    - [x] Variable group templates (`@Var`).
    - [x] Template inheritance (`@Style OtherStyle;` and `inherit @Style OtherStyle;`).
- [x] **Customization (`[Custom]`):**
    - [x] Custom style groups with value placeholders.
    - [x] Specialization (deleting properties).
    - [x] Specialization (inserting/deleting elements).
- [x] **Imports (`[Import]`):**
    - [x] Import CHTL files.
    - [x] Import HTML, CSS, and JS files.
- [x] **Namespaces (`[Namespace]`):**
    - [x] Define and use namespaces.
    - [x] Handle automatic namespacing on import.
- [x] **Other Features:**
    - [x] Raw embedding (`[Origin]`).
    - [ ] Constraints (`except`).
    - [ ] Configuration (`[Configuration]`).

## Phase 3: CHTL JS Integration

- [ ] **Salt Bridge (Shared Core):** Design and implement the communication bridge between the CHTL and CHTL JS compilers.
- [ ] **CHTL JS Lexer and Parser:**
    - [ ] Implement preprocessor for `[__CHTLJS__]` blocks.
    - [ ] Implement lexer and parser for the CHTL JS syntax.
- [ ] **CHTL JS Features:**
    - [ ] Enhanced Selectors (`{{...}}`).
    - [ ] Local Script Blocks (`script {}`).
    - [ ] Enhanced Listeners (`Listen {}` and `&->`).
    - [ ] Event Delegation (`Delegate {}`).
    - [ ] Animation (`Animate {}`).
    - [ ] Virtual Objects (`Vir`).
    - [ ] Routing (`Router {}`).
- [ ] **Dynamic/Responsive Features:**
    - [ ] Dynamic attribute conditional expressions.
    - [ ] Dynamic conditional rendering.
    - [ ] Responsive values (`$jsVariable$`).

## Phase 4: Module System (CMOD and CJMOD)

- [ ] **CMOD Packaging:** Create the tooling to package CHTL files into `.cmod` modules.
- [ ] **CJMOD API:** Implement the C++ API for creating `.cjmod` extensions.
- [ ] **Module Resolution and Loading:** Implement the logic for resolving and loading CMOD and CJMOD modules.
- [ ] **Official Modules:** Begin development of the official `Chtholly` and `Yuigahama` modules.

## Phase 5: Tooling and IDE Support

- [ ] **Command-Line Interface (CLI):**
    - [ ] Basic CLI for compiling files from paths.
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
