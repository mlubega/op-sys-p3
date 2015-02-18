libmem.so: mem.o
	gcc -g -shared -o libmem.so mem.o;\
		    LD_LIBRARY_PATH=.;\
		    export LD_LIBRARY_PATH;\

mem.o: mem.c mem.h
	gcc -g -c -fpic -o mem.o mem.c -g

mymain: mymain.c libmem.so
	gcc -lmem -L. -o mymain mymain.c -Wall -Werror -g

clean:
	rm -rf *.o libmem.so
