all:
	g++ -c readelf.cpp -o readelf.o
readelf:
	g++ readelf.cpp -o readelf
clean:
	rm -f readelf readelf.o