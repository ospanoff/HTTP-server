#vars
CC = g++
CFLAGS = -c -Wall
LDFLAGS =
SOURCES = server.cpp lib/sock_create.cpp lib/http_ans.cpp lib/Script_parser.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = server

#template
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS)