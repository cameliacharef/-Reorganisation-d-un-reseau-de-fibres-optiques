CC = gcc
FLAGS = -Wall
OBJECTS = ArbreQuat.o Chaine.o ChaineMain.o Hachage.o ReconstitueReseau.o Reseau.o SVGwriter.o ArbreQuat.o main.o
PROGRAMS = ChaineMain ReconstitueReseau main 

all: $(PROGRAMS)

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	$(CC) $(FLAGS) -o $@ $^ -lm

ReconstitueReseau : ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o Hachage.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm

main: main.o Reseau.o Chaine.o SVGwriter.o Hachage.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm

ArbreQuat.o: ArbreQuat.c
	$(CC) $(FLAGS) -c -o $@ $^

Chaine.o : Chaine.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

ChaineMain.o: ChaineMain.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

Hachage.o: Hachage.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

ReconstitueReseau.o : ReconstitueReseau.c
	$(CC) $(FLAGS) -c -o $@ $^

Reseau.o : Reseau.c
	$(CC) $(FLAGS) -c -o $@ $^

SVGwriter.o: SVGwriter.c
	$(CC) $(FLAGS) -c -o $@ $^

main.o: main.c
	$(CC) $(FLAGS) -c -o $@ $^

clean:
	rm -rf $(OBJECTS) $(PROGRAMS) *.o
