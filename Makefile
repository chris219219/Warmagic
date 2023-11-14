EX_NAME = warmagic
CC = gcc
CCFLAGS = -Os -Wall -std=c17
LIBFLAGS = -L ./lib/ -lraylib
SRCS = $(wildcard ./src/*.c)

CLEAN_CMD =
COPY_RES_CMD =

ifeq ($(OS), WINDOWS_NT)
	EX_NAME += .exe
	LIBFLAGS += -lopengl32 -lgdi32 -lwinmm
	CLEAN_CMD += del /F /Q ./bin/*
	COPY_RES_CMD += xcopy ./src/res ./bin /E
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		LIBFLAGS += -lGL -lm -lpthread -ldl -lrt -lX11
		CLEAN_CMD += rm -rf ./bin/*
		COPY_RES_CMD += cp -r ./src/res ./bin
	endif
endif

all:
	$(CC) -o ./bin/$(EX_NAME) $(SRCS) $(CCFLAGS) $(LIBFLAGS)
	$(COPY_RES_CMD)

clean:
	$(CLEAN_CMD)