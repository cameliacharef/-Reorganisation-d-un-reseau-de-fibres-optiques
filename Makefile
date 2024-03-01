CC: gcc
FLAGS: -Wall
OBJECTS:ArbreQuat.o Chaine.o Hachage.o SVGwriter.o
#PROGRAMS : 

#all: $(PROGRAMS)

#main: $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $^

ArbreQuat.o: ArbreQuat.c ArbreQuat.h
	$(CC) $(FLAGS) -c -o $@ $<

Chaine.o : Chaine.c Chaine.h
	$(CC) $(FLAGS) -c -o $@ $<

Hachage.o: Hachage.c Hachage.h
	$(CC) $(FLAGS) -c -o $@ $<

SVGwriter.o: SVGwriter.c SVGwriter.h
	$(CC) $(FLAGS) -c -o $@ $<

clean: 
	rm -rf $(OBJECTS) #$(PROGRAMS)