// # Nom du programme
// PROGRAM = mon_programme

// # Répertoire des fichiers sources
// SRC_DIR = src

// # Répertoire des fichiers objets
// OBJ_DIR = obj

// # Fichiers sources
// SOURCES = $(wildcard $(SRC_DIR)/*.c)

// # Fichiers objets
// OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

// # Compilateur et options de compilation
// CC = gcc
// CFLAGS = -Wall -Wextra -O2

// all: $(PROGRAM)

// $(PROGRAM): $(OBJECTS)
//     $(CC) $(CFLAGS) -o $@ $^

// $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
//     $(CC) $(CFLAGS) -c $< -o $@

// clean:
//     rm -rf $(OBJ_DIR) $(PROGRAM)

// .PHONY: all clean