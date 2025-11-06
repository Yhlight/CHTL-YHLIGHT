# CHTL Compiler Development Roadmap

This document outlines the development plan for the CHTL compiler.

## Milestone 1: Project Initialization and Core Lexer

- [x] Set up the basic project structure with `src`, `tests`, and `Module` directories.
- [x] Configure the CMake build system and create a `build.py` script.
- [x] Implement the initial Lexer to handle basic tokens, including single-quoted strings.
- [x] Write unit tests to verify the lexer's functionality.

## Milestone 2: Advanced Lexer Features

- [x] Handle unterminated strings gracefully.
- [x] Add support for double-quoted strings.
- [x] Add support for basic symbols (`{`, `}`, `:`, `;`).
- [x] Add support for identifiers and unquoted literals.
- [x] Add support for single-line and multi-line comments.

## Milestone 3: Basic Parser and AST

- [x] Define the core Abstract Syntax Tree (AST) nodes.
- [x] Implement the basic `Parser` class structure.
- [x] Implement parsing for simple and nested elements.
- [x] Write unit tests for the parser.

## Milestone 4: Parser - Attributes and Text Nodes

- [x] Enhance the lexer to recognize the `=` symbol for attributes.
- [x] Enhance the AST to support attributes and text nodes.
- [x] Implement attribute parsing in the parser.
- [x] Implement text node parsing in the parser.
- [x] Write unit tests for attribute and text node parsing.

## Milestone 5: Parser - Style Blocks

- [x] Enhance the AST to represent `style` blocks and their content.
- [x] Refactor the parser to correctly handle behavior blocks vs. child elements.
- [x] Implement parsing for `style` blocks with CSS properties.
- [x] Implement parsing for CSS rules within `style` blocks.
- [x] Write unit tests for `style` block parsing.

## Milestone 6: Basic Code Generator

- [x] Implement the `Generator` class structure.
- [x] Implement visitor methods for `Program`, `Element`, and `Text` nodes.
- [x] Implement generation of element attributes.
- [x] Implement generation of inline styles from `StylePropertyNode`.
- [x] Implement generation of global stylesheets from `StyleRuleNode`.
- [x] Write unit tests for the generator.

## Milestone 7: Automatic Class/ID Generation

- [x] Enhance the `Lexer` to recognize `.` and `#` in identifiers.
- [x] Enhance the `Parser` to correctly parse `StyleRuleNode` within `style` blocks.
- [x] Enhance the `Generator` to automatically add `class` or `id` attributes to elements.
- [x] Write unit tests for the automatic class/id generation feature.

## Milestone 8: Context Deduction in Style Blocks

- [x] Enhance the `Lexer` to recognize the `&` symbol.
- [x] Enhance the `Parser` to correctly parse style rules that use the `&` symbol.
- [x] Refactor the `Generator` to resolve `&` selectors based on the parent element's primary class or ID.
- [x] Write unit tests for the context deduction feature.

## Milestone 9: Script Blocks

- [x] Add a `ScriptNode` to the AST.
- [x] Refactor the `Lexer` to handle escaped quotes in strings.
- [x] Update the `Parser` to recognize and parse `script` blocks.
- [x] Update the `Generator` to produce `<script>` tags.
- [x] Write unit tests for `script` block parsing and generation.
