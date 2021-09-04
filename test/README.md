# RV32I Testing Program
+ a.out is an executable file compiled by riscv32-unknown-elf-gcc.
+ instruction.output and no_instruction.output are the emulator running output in different mode.
---
### A readelf program
+ A readelf program for reading the ELF file.
+ Compile the readelf.cpp to executable file. (```make readelf```)
+ Enter ```./readelf ./a.out``` to show the ELF file headers.
---
### A RV32I disassembler 
+ Disassemble the text section of ELF file.
+ Compile the disassembler.cpp to executable file. (```make disassembler```)
+ Enter ```./disassembler ./a.out``` to get the assembly with register name. (ra, sp, gp...)
---
### A simple RV32I emulator
1. Using riscv32-unknown-elf-gcc compile the hello.c file.
2. Get the "a.out" elf file.
3. Compile the emulator.cpp to executable file. (```make emulator```)
4. Enter ```./emulator ./a.out``` to get the elf file running output.
5. Enter ```./emulator ./a.out --show-instruction``` to run the elf file with instruction sequence.