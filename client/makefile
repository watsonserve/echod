#! /bin/make

BIN = ..
OBJ = /tmp
CC = gcc
CFLAGS = -c -O3 -Wall

$(BIN)/echo_cli : $(OBJ)/client.o $(OBJ)/utils.o $(OBJ)/main.o
	$(CC) -o $@ $^ -lpthread

$(OBJ)/client.o : client.c
	$(CC) $(CFLAGS) -o $@ $^ --std=c11

$(OBJ)/utils.o : utils.c
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/main.o : main.c
	$(CC) $(CFLAGS) -o $@ $^
