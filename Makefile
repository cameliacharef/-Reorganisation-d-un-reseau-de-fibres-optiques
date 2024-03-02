CC: gcc
FLAGS: -Wall
OBJECTS:ArbreQuat.o Chaine.o ChaineMain.o Hachage.o Reseau.o SVGwriter.o
PROGRAMS : ChaineMain

all: $(PROGRAMS)

ChaineMain: ChaineMain.o SVGwriter.o
	#$(CC) $(FLAGS) -o $@ $^

ArbreQuat.o: ArbreQuat.c
	$(CC) $(FLAGS) -c -o $@ $^

Chaine.o : Chaine.c
	$(CC) $(FLAGS) -c -o $@ $^

ChaineMain.o: ChaineMain.c
	$(CC) $(FLAGS) -c -o $@ $^

Hachage.o: Hachage.c
	$(CC) $(FLAGS) -c -o $@ $^

ReconstitueReseau.o : ReconstitueReseau.c
	$(CC) $(FLAGS) -c -o $@ $^

Reseau.o : Reseau.c
	$(CC) $(FLAGS) -c -o $@ $^

SVGwriter.o: SVGwriter.c
	$(CC) $(FLAGS) -c -o $@ $^

clean:
	rm -rf $(OBJECTS) $(PROGRAMS)
