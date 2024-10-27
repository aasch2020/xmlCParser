HEADERS = stateTree.h  xmlParse.h
OBJECTS = stateTree.o  xmlParse.o

default: xmlParse

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

xmlParse: $(OBJECTS)
	gcc $(OBJECTS) -o $@

