PROJ_NAME=kilo

SRC_DIR=./src
C_SRC=$(wildcard $(SRC_DIR)/*.c)

OBJS=$(C_SRC:.c=.o)

CC=gcc
CCFLAGS=-I$(SRC_DIR) -Wall -Wextra -pedantic -std=c99 -g

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJS)
	$(CC) -o $(PROJ_NAME) $(OBJS) $(CCFLAGS)

run:
	@ ./$(PROJ_NAME)

clear:
	@ rm -rf $(SRC_DIR)/*.o $(PROJ_NAME)
