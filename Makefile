graph: main.o graph.o
	$(CC) -o graph main.o graph.o -lm
main.o: main.c graph.h
	$(CC) main.c -c -lm 
graph.o: graph.c graph.h
	$(CC) graph.c -c -lm
help: graph
	./graph -h
test1: graph
	./graph --file dane.txt --from 0 --to 3 --output sciezka.txt
testg1: graph
	./graph --grow 2 --gcol 3 --gfrom 4 --gto 12 --gconnect 1 --goutput gen.txt
testg2: graph
	./graph --grow 3 --gcol 3 --gfrom 1 --gto 10 --gconnect 0 --goutput gen.txt
testg3: graph
	./graph --grow 3 --gcol 3 --gfrom 1 --gto 10 --goutput gen.txt
test2: graph
	./graph --file gen.txt --from 0 --output sciezka.txt
test3: graph
	./graph --file gen.txt
test4: graph
	./graph --from 0 --to 6 --output sciezka.txt
test5: graph
	./graph --from 0 --to 15 --output sciezka.txt
test6: graph
	./graph --from -5 --to 5 --output sciezka.txt
test7: graph
	./graph --from 0 --to 3 --output sciezka.txt --grow 2 --gcol 3 --gfrom 4 --gto 12 --gconnect 1 --goutput gen.txt
test8: graph
	./graph
.PHONY: clean
clean:
	-rm *.o graph
