HEADERS = stateTree.h  xmlParse.h xml2Tree.h
OBJECTS = stateTree.o  xmlParse.o xml2Tree.o
binaries = xmlParse

default: xmlParse

%.o: %.c $(HEADERS)
	gcc -c -g -Wextra -Wall $< -o $@

xmlParse: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	rm -f $(binaries) *.o