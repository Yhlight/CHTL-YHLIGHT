# CHTL and CHTL JS Rust Implementation Roadmap

This document outlines the development roadmap for the Rust implementation of the CHTL and CHTL JS compiler.

## Milestone 1: Core CHTL Language Features

- [ ] **Comments**
    - [ ] Single-line comments (`//`)
    - [ ] Multi-line comments (`/**/`)
    - [ ] Generator-aware comments (`#`)
- [ ] **Text Nodes**
    - [ ] `text {}` blocks
    - [ ] `text:` attribute
- [ ] **Literals**
    - [ ] Unquoted string literals
    - [ ] Double-quoted strings (`""`)
    - [ ] Single-quoted strings (`''`)
- [ ] **CE Equality**
    - [ ] `:` and `=` equivalency
- [ ] **Element Nodes**
    - [ ] Support for all standard HTML elements
    - [ ] Nested element structure
- [ ] **Attributes**
    - [ ] Standard attribute syntax (`name: "value";`)
- [ ] **Local Style Blocks (`style {}`)**
    - [ ] Inline styles
    - [ ] Automatic class/id generation (`.class`, `#id`)
    - [ ] Contextual `&` selector for pseudo-classes/elements
    - [ ] Attribute arithmetic (`+`, `-`, `*`, `/`, `%`, `**`)
    - [ ] Referenced attributes (`selector.property`)
    - [ ] Attribute conditional expressions (`property > value ? ... : ...`)
- [ ] **Origin Blocks (`[Origin]`)**
    - [ ] Raw HTML embedding (`[Origin] @Html`)
    - [ ] Raw CSS embedding (`[Origin] @Style`)
    - [ ] Raw JavaScript embedding (`[Origin] @JavaScript`)
    - [ ] Named origin blocks

## Milestone 2: Advanced CHTL Features

- [ ] **Templates (`[Template]`)**
    - [ ] Style Group Templates (`@Style`)
    - [ ] Element Templates (`@Element`)
    - [ ] Variable Group Templates (`@Var`)
    - [ ] Template inheritance (composition and `inherit`)
- [ ] **Custom Components (`[Custom]`)**
    - [ ] Custom Style Groups (`@Style`) with valueless properties
    - [ ] Style Group specialization (delete properties, delete inheritance)
    - [ ] Custom Elements (`@Element`)
    - [ ] Element specialization (add styles, index access, insert, delete)
    - [ ] Custom Variable Groups (`@Var`) with specialization
    - [ ] Fully qualified names for templates and custom components
- [ ] **Imports (`[Import]`)**
    - [ ] Import HTML, CSS, JS files
    - [ ] Import CHTL files (precise, by type, wildcard)
    - [ ] `./` and `.` path equivalence
- [ ] **Namespaces (`[Namespace]`)**
    - [ ] Namespace creation
    - [ ] Nested namespaces
    - [ ] Automatic namespacing on import
- [ ] **Constraints (`except`)**
    - [ ] Precise constraints (elements, custom components)
    - [ ] Type constraints (@Html, [Custom], etc.)
    - [ ] Global constraints within namespaces
- [ ] **Configuration Groups (`[Configuration]`)**
    - [ ] Global configuration settings
    - [ ] `[Name]` block for keyword customization
    - [ ] Named configuration groups
    - [ ] Import configuration groups
    - [ ] Rules for local style block automation
    - [ ] Default namespace configuration
    - [ ] Custom origin type definitions
- [ ] **`use` keyword**
    - [ ] `use html5;`
    - [ ] `use @Config ...;`

## Milestone 3: CHTL JS Implementation

- [ ] **CHTL JS Preprocessor**
    - [ ] `[__CHTLJS__]` and `[__CHTLJSEND__]` block handling
- [ ] **Script Loader**
    - [ ] AMD-style `ScriptLoader {}`
- [ ] **Local `script` blocks**
    - [ ] Isolate local scripts into a non-polluting global script
- [ ] **Enhanced Selectors (`{{...}}`)**
    - [ ] DOM object creation from CSS selectors
    - [ ] Automatic class/id generation
- [ ] **`->` operator** for explicit CHTL JS syntax
- [ ] **Enhanced Listeners (`Listen`)**
    - [ ] Declarative event listener binding
- [ ] **Event Binding Operator (`&->`)**
    - [ ] Shortcut for event binding
    - [ ] Multi-event binding
    - [ ] Chained binding
    - [ ] Binding blocks
- [ ] **Event Delegation (`Delegate`)**
    - [ ] `Delegate {}` block for robust event handling
- [ ] **Animation (`Animate`)**
    - [ ] `Animate {}` block for `requestAnimationFrame`-based animations
- [ ] **Virtual Objects (`Vir`)**
    - [ ] Compile-time object introspection
- [ ] **Router**
    - [ ] `Router {}` for SPA routing
- [ ] **Dynamic Attribute Condition Expressions**
    - [ ] Conditions based on dynamic values
- [ ] **Dynamic Conditional Rendering**
    - [ ] `if` blocks with dynamic conditions
- [ ] **Reactive Values (`$...$`)**
    - [ ] Bind CHTL attributes and styles to JS variables

## Milestone 4: CHTL and CHTL JS Modules

- [ ] **CMOD Module System**
    - [ ] CMOD packaging tool
    - [ ] CMOD file structure (`src`, `info`)
    - [ ] `[Info]` and `[Export]` blocks
    - [ ] Sub-module support
    - [ ] CMOD import and path resolution
- [ ] **CJMOD Module System**
    - [ ] CJMOD file structure (`src`, `info`)
    - [ ] `[Info]` block for CJMOD
    - [ ] CJMOD import and path resolution
    - [ ] CJMOD API (Syntax, Arg, CJMODScanner, etc.)
- [ ] **Mixed CMOD + CJMOD modules**
    - [ ] Support for combined module structure

## Milestone 5: CLI and Tooling

- [ ] **Command Line Interface (CLI)**
    - [ ] Basic command-line tool
    - [ ] Advanced command-line program with rendering
    - [ ] Event loop support
- [ ] **CLI Flags**
    - [ ] `--default-struct`
    - [ ] `--inline`, `--inline-css`, `--inline-js`
- [ ] **Compilation Monitor**
    - [ ] Track compilation time and memory usage

## Milestone 6: IDE Support (VSCode)

- [ ] **Basic IDE Features**
    - [ ] Code Highlighting
    - [ ] Code Formatting (JS and C++ styles)
    - [ ] Code Completion
- [ ] **Live Development**
    - [ ] Page Preview
    - [ ] Real-time Preview
- [ ] **Integration**
    - [ ] Right-click to open in browser
    - [ ] Right-click to open documentation
    - [ ] Right-click to export HTML/CSS/JS
    - [ ] Built-in Compiler and Official Modules
    - [ ] Right-click to compile
- [ ] **Advanced Features**
    - [ ] Automatic Module Unpacking and JSON Query Tables for completion
    - [ ] Resolve `[]` autocompletion conflicts
    - [ ] Module Syntax Highlighting

## Milestone 7: Official Modules

- [ ] **Chtholly Module (CMOD + CJMOD)**
    - [ ] CMOD components (Accordion, Gallery, etc.)
    - [ ] CJMOD extensions (PrintMylove, INeverAway, util...end)
- [ ] **Yuigahama Module (CMOD)**
    - [ ] CMOD components (Music Player, etc.)
