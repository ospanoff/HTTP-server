#vars
CC = g++
CFLAGS = -c -Wall
LDFLAGS =
SOURCES = server.cpp lib/sock_create.cpp lib/http_ans.cpp lib/Script_parser.cpp lib/Interpreter/Executer.cpp lib/Interpreter/Ident.cpp lib/Interpreter/Interpreter.cpp lib/Interpreter/Lex.cpp lib/Interpreter/lexems.cpp lib/Interpreter/Parser.cpp lib/Interpreter/Poliz.cpp lib/Interpreter/Scanner.cpp lib/Interpreter/Stack.cpp lib/Interpreter/Table_Ident.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = s

#template
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS)