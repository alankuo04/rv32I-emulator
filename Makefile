readelf:
	g++ -c elf_function.cpp readelf.cpp
	g++ -o readelf elf_function.o readelf.o
disassembler:
	g++ -c elf_function.cpp dis_function.cpp disassembler.cpp
	g++ -o disassembler elf_function.o dis_function.o disassembler.o
clean:
	rm -f readelf disassembler readelf.o disassembler.o elf_function.o dis_function.o