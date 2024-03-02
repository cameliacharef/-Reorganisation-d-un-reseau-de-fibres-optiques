CC: gcc
FLAGS: -Wall
OBJECTS:ArbreQuat.o Chaine.o Hachage.o SVGwriter.o
PROGRAMS :

#all: $(PROGRAMS)

#main: $(OBJECTS)
	#$(CC) $(FLAGS) -o $@ $^

ArbreQuat.o: ArbreQuat.c
	$(CC) $(FLAGS) -c -o $@ $<

Chaine.o : Chaine.c
	$(CC) $(FLAGS) -c -o $@ $<

Hachage.o: Hachage.c
	$(CC) $(FLAGS) -c -o $@ $<

SVGwriter.o: SVGwriter.c
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJECTS) $(PROGRAMS)
