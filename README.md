

### CSCI-3334 Compiler Construction
# ILOC Partial Compiler

Implementations of three components of a compiler for a subset of the ILOC assembly language. It takes a branch-free block of ILOC code and produces an equivalent block that is register-allocated and instruction-scheduled. There are three main components:

1. **Lexer and Parser**: Processes the input ILOC code into a structured intermediate representation.
2. **Bottom-Up Local Register Allocator**: Maps virtual registers to physical registers, inserting spill and restore instructions when necessary.
3. **Instruction Scheduler**: Builds a depedency graph and reorders instructions to optimize performance of generated code.

## Limitations
This project is a **partial compiler**, focusing on a specific subset of ILOC and excluding certain features typical of full compilers, such as:
- Global optimizations and register allocation.
- Support for control flow (branches, loops, etc.).
- Code generation for a target machine.
