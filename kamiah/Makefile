# Kamiah is the document engine of PapayaIDE.
#
# Author: Victor Marmol (vmarmol@gmail.com)

CC=g++-4.6
DOC=doxygen
FLAGS=-Wall -Wextra -Werror -ansi -pedantic -O2 -g
LIBS=-lpthread

all:

clean:
	rm -f *.o *.gch
	rm -rf doc/

document_test: document_test.cc document.o diff.o
	${CC} ${LIBS} ${FLAGS} document.o diff.o document_test.cc -o document_test

%.o: %.cc
	${CC} ${FLAGS} -c $^ -o $@

docs:
	${DOC} doxygen.conf
