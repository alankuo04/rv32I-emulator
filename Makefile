all:
	g++ -c elf_function.cpp dis_function.cpp disassembler.cpp readelf.cpp
	g++ -o readelf elf_function.o readelf.o
	g++ -o disassembler elf_function.o dis_function.o disassembler.o
readelf:
	g++ -c elf_function.cpp readelf.cpp
	g++ -o readelf elf_function.o readelf.o
disassembler:
	g++ -c elf_function.cpp dis_function.cpp disassembler.cpp
	g++ -o disassembler elf_function.o dis_function.o disassembler.o
emulator:
	g++ -c elf_function.cpp dis_function.cpp simple_emulator.cpp
	g++ -o emulator elf_function.o dis_function.o simple_emulator.o
clean:
	rm -f readelf disassembler emulator readelf.o disassembler.o simple_emulator.o elf_function.o dis_function.o