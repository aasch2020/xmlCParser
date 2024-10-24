HEADERS = stateTree.h 
OBJECTS = stateTree.o

default: xmlParse

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

xmlParse: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f xmlParse