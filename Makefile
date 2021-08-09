all:
	g++ -c elf_function.cpp dis.cpp disassembler.cpp readelf.cpp
	g++ -o readelf elf_function.o readelf.o
	g++ -o disassembler elf_function.o dis.o disassembler.o
readelf:
	g++ -c elf_function.cpp readelf.cpp
	g++ -o readelf elf_function.o readelf.o
disassembler:
	g++ -c elf_function.cpp dis.cpp disassembler.cpp
	g++ -o disassembler elf_function.o dis.o disassembler.o
clean:
	rm -f readelf disassembler readelf.o disassembler.o elf_function.o dis.o