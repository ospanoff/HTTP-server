#vars
CC = g++
CFLAGS = -c -Wall
LDFLAGS =
SOURCES = server.cpp lib/sock_create.cpp lib/http_ans.cpp
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