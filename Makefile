CC = gcc
FLAGS = -Wall
OBJECTS = ArbreQuat.o Chaine.o ChaineMain.o Hachage.o ReconstitueReseau.o Reseau.o SVGwriter.o ArbreQuat.o main.o GenerationAleatoireMain.o ReorganiseReseau.o
PROGRAMS = ChaineMain ReconstitueReseau main GenerationAleatoireMain ReorganiseReseau

all: $(PROGRAMS)

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	$(CC) $(FLAGS) -o $@ $^ -lm

main: main.o Reseau.o Chaine.o SVGwriter.o Hachage.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm

GenerationAleatoireMain: GenerationAleatoireMain.o Reseau.o Chaine.o SVGwriter.o Hachage.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm

ReconstitueReseau : ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o Hachage.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm

ReorganiseReseau: ReorganiseReseau.o Graphe.o Reseau.o Struct_File.o SVGwriter.o Chaine.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm
	
ArbreQuat.o: ArbreQuat.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

Chaine.o : Chaine.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

Graphe.o : Graphe.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

Hachage.o: Hachage.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm	

Reseau.o : Reseau.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

SVGwriter.o: SVGwriter.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm	

Struct_File.o: Struct_File.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm	

ChaineMain.o: ChaineMain.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

GenerationAleatoireMain.o: GenerationAleatoireMain.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

ReconstitueReseau.o : ReconstitueReseau.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

main.o: main.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

ReorganiseReseau.o: ReorganiseReseau.c
	$(CC) $(FLAGS) -c -o $@ $^ -lm

clean:
	rm -rf $(OBJECTS) $(PROGRAMS) *.o
