CC=g++
CFLAGS=-std=c++11 
TARGET=lib
OBJECTS=library.o resource.o member.o date.o

$(TARGET): $(OBJECTS) main.cpp
		$(CC) $(CFLAGS) -o $@ $^

library.o: library.cpp
		$(CC) $(CFLAGS) -c $^

resource.o: resource.cpp
		$(CC) $(CFLAGS) -c $^

member.o: member.cpp
		$(CC) $(CFLAGS) -c $^

date.o: date.cpp
		$(CC) $(CFLAGS) -c $^

clean:
		rm *.o $(TARGET)
