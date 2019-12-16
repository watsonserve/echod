#! /bin/make

BIN = ..
OBJ = /tmp
CC = gcc
CFLAGS = -c -O3 -Wall --std=c11

$(BIN)/echo_cli : $(OBJ)/client.o $(OBJ)/utils.o $(OBJ)/main.o
	$(CC) -o $@ $^

$(OBJ)/client.o : client.c
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/utils.o : utils.c
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/main.o : main.c
	$(CC) $(CFLAGS) -o $@ $^
