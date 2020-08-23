CC = 'g++'
LIBS = '-lcurl'

main: main.o
	$(CC) -g -o $@ $< $(LIBS)

main.o: main.cpp
	$(CC) -g -c $<

clean:
	rm -f main.o main
