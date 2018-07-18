Overview
========

- [*Test suite*] : Small tests intended to asses ability of Compiler.
  10 of which were done as assessed coursework.

- [*C-translator*](c_translator.md) : YTranslate a sub-set of C
  into equivalent Python. Demonstrate lexing and parsing functionality.

- [*C-compiler*](c_compiler.md) : This is a compiler from C to MIPS assembly.
  
In all cases, the source language is pre-processed C90. The target environment
is Ubuntu 16.04.


Compile
========

"make bin/c_compiler" - requires bison, flex and g++

Using: bin/c_compiler -MODE [source-file.c] -o [dest-file.s]

Mode "-o" - Compile
Mode "--translate" - Translate to python - limited C support - refer to spec
