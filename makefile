all:	compiler.o error_handler.o lille_exception.o scanner.o id_table_entry.o id_table.o symbol.o token.o parser.o
	g++ -o compiler compiler.o error_handler.o lille_exception.o scanner.o symbol.o token.o parser.o
	echo Compilation complete.

compiler.o:	id_table.o error_handler.o lille_exception.o scanner.o symbol.o compiler.cpp
	g++ -std=c++2a -c compiler.cpp

error_handler.o: lille_exception.o token.o error_handler.h error_handler.cpp
	g++ -std=c++2a -c error_handler.cpp

id_table_entry.o: 

id_table.o:: id_table.h id_table.cpp
	g++ -std=c++2a -c id_table.cpp

lille_exception.o:	lille_exception.h lille_exception.cpp
	g++ -std=c++2a -c lille_exception.cpp

scanner.o: error_handler.o lille_exception.o token.o symbol.o id_table.o scanner.h scanner.cpp
	g++ -std=c++2a -c scanner.cpp


symbol.o: symbol.h symbol.cpp
	g++ -std=c++2a -c symbol.cpp

token.o: lille_exception.o symbol.o token.h token.cpp
	g++ -std=c++2a -c token.cpp

parser.o: error_handler.o token.o symbol.o id_table.o scanner.o compiler.o parser.h parser.cpp
	g++ -std=c++2a -c parser.cpp

clean:
	rm *.o 
	echo Clean complete
