CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I$(INC)
INC = include/
SRC = src/
BIN = bin/

OBJ = $(BIN)main.o $(BIN)helpers.o $(BIN)cJSON.o

all: cod_league

cod_league: $(OBJ)
	$(CC) $(CFLAGS) -o cod_league $(OBJ)

$(BIN)main.o: $(SRC)main.c $(INC)match.h $(INC)cJSON.h
	$(CC) $(CFLAGS) -c $(SRC)main.c -o $(BIN)main.o

$(BIN)helpers.o: $(SRC)helpers.c $(INC)match.h $(INC)cJSON.h
	$(CC) $(CFLAGS) -c $(SRC)helpers.c -o $(BIN)helpers.o

$(BIN)cJSON.o: $(SRC)cJSON.c $(INC)cJSON.h
	$(CC) $(CFLAGS) -c $(SRC)cJSON.c -o $(BIN)cJSON.o

clean:
	rm -f $(BIN)*.o cod_league