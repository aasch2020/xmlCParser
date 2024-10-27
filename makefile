HEADERS = stateTree.h  xmlParse.h xml2Tree.h
OBJECTS = stateTree.o  xmlParse.o xml2Tree.o

default: xmlParse

%.o: %.c $(HEADERS)
	gcc -c -Wextra -Wall $< -o $@

xmlParse: $(OBJECTS)
	gcc $(OBJECTS) -o $@

