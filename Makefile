all: leitor_pai escritor_paralelo monitor 

monitor: monitor.o
	gcc -o monitor monitor.o

monitor.o: monitor.c leitor_escritor.h
	gcc -g -c monitor.c



leitor_pai: leitor_pai.o leitor.o
	gcc -o leitor_pai leitor_pai.o leitor.o -lpthread

leitor_pai.o: leitor_pai.c leitor_escritor.h leitor.h
	gcc -g -c leitor_pai.c

leitor.o: leitor.c leitor_escritor.h leitor.h
	gcc -g -c leitor.c




escritor_paralelo: escritor_paralelo.o escritor.o
	gcc -o escritor_paralelo escritor_paralelo.o escritor.o -lpthread

escritor_paralelo.o: escritor_paralelo.c leitor_escritor.h escritor.h escritor_paralelo.h
	gcc -g -c escritor_paralelo.c

escritor.o: escritor.c leitor_escritor.h escritor.h
	gcc -g -c escritor.c

clean:
	rm -f *.o leitor_pai escritor_paralelo monitor
