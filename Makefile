graph: main.o graph.o
	$(CC) -o graph main.o graph.o -lm
main.o: main.c graph.h
	$(CC) main.c -c -lm 
graph.o: graph.c graph.h
	$(CC) graph.c -c -lm
test1: graph
	./graph --file dane.txt --from 0 --to 3 --output sciezka.txt
test2: graph
	./graph --grow 2 --gcol 3 --gfrom 4 --gto 12 --goutput gen.txt
.PHONY: clean
clean:
	-rm *.o graph
