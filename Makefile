graph: main.o graph.o
	$(CC) -o graph main.o graph.o -lm
main.o: main.c graph.h -lm
graph.o: graph.c graph.h -lm
test1: 
	./graph --file dane.txt --from 0 --to 3 --output sciezka.txt
test2: 
	./graph --grow 2 --gcol 3 --gfrom 4 --gto 12 --goutput gen.txt
