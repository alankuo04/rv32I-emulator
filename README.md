# RV32I emulator

RV32I emulator is a visual emulator built for **RV32I Base Integer Instruction Set**

<center><img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/emulator.png?raw=true"/></center>

## Usage
RV32I emulator can be used to do such as:
- How an executable file of RV32I be disassembled.
- How machine code is executed in a single-cycle processor of RV32I machine.
- How register and memory modified by machine.
- How elf header, program header and section header look like in an ELF file.

## The Assembly View
The text editor in left hand side show the disassembled RISC-V instruction in ELF file compiled for RV32I machine. The file will be opened in read-only mode, you can't modify the ELF file by yourself. Also, there will show **pc** (program counter) in the left of the editor.

<center><img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/textEditor.png?raw=true"/></center>

## The Register View
The register in this emulator will show the temporary value of running ELF file. While running the program, the value changing in the same time.

<center><img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/register.png?raw=true"/></center>

## The Memory View
The memory in this emulator will show the temporary value of running ELF file. Memory can be allocated in a wide range, so it can't show the whole memory value at the same time. When you scroll your scroll bar to the temporary upper or lower bound, it will change the upper or lower bound to show more value of memory. In addition, there are two combo box for going to specified memory position by selecting different register or section.

<center><img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/memory.png?raw=true"/></center>

## The Console View
There is a console for showing the stdout in this RV32I emulator. In the top-right, a delete icon can clear the console output directly.

<center><img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/console.png?raw=true"/></center>

## Controlling of emulator
The toolbar have five icons for controlling the RV32I emulator.
|icon|Name|Usage|
|----|----|-----|
|<img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/Reset.png?raw=true" width=50px/>|Reset|Reset the emulator, set the program counter to the entry point and reset the register and memory.|
|<img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/Step.png?raw=true" width=50px/>|Step|Run next step of instruction, showing the register and memory value.|
|<img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/Run.png?raw=true" width=50px/>|Run|Start running the emulator with the specified interval setting by user.|
|<img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/End.png?raw=true" width=50px/>|End|Execute the emulator without showing register and memory modified in each step, the ecall functions will still be printed to console. It will stop at an exit ecall.|
|<img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/Stop.png?raw=true" width=50px/>|Stop|Stop running the emulator.|

## ELF File Header View
In the menu bar Help >> About, It will show the elf header, program header and section header by pressed each button.

<center><img src="https://github.com/alankuo04/rv32I-emulator/blob/main/images/header.png?raw=true"/></center>

## Downloading
There is a prebuilt binary for Windows in [Releases](https://github.com/alankuo04/rv32I-emulator/releases), or just downloading the source code and build it in Qt Creator.