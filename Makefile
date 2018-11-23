CC	:= gcc
CFLAGS	:= -Wall
INC	:= include

SRC	:= src/main.c

all:	$(SRC)
	$(CC) $(CFLAGS) -I$(INC) $< -o build/version

clean:
	

cleanall:
	rm -rf build/version

install: build/version
	cp build/version /usr/local/bin/
