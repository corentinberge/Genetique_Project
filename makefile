# Specific part of the Makefile
# EXEC contient le nom des exécutables à générer, le nom est indépendant des noms de fichiers auquels le makefile fait appel
EXEC = geneticMain
# Begin generic part of the Makefile
# CC indique le compilateur utilisé
CC=gcc
# CFLAGS regroupe les options de compilation (par exemple "-Werror" transforme les Warning en Error, ne produit pas d'exécutable)
CFLAGS= -std=c99 -Wextra -pedantic
# LDFLAGS regroupe les options de l'édition de liens
LDFLAGS= -lm

# SRC contient la liste des fichiers sources et OBJ contient la liste des fichiers objets
SRC= $(wildcard *.c)
OBJ= $(SRC:.c =.o)

ifeq ($(DEBUG), yes)
	CFLAGS += -g
	LDFLAGS +=
else
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS +=
endif

all : $(EXEC)

$(EXEC): main.o
	$(CC) $(CFLAGS) main.o $(LDFLAGS) -o $(EXEC)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# On utilise .PHONY au cas où clean ou mrproper soient également des nom de fichiers présents dans le répertoire
.PHONY: clean mrproper

# Permet de supprimer tous les fichiers intermédiaires
clean:
	@rm -rf *.o

#Permet de supprimer tout ce qui a été généré par le makefile
mrproper: clean
	@rm -rf $(EXEC)
# End generic part of the makefile

# List of target dependencies
#main.o: block.h blockchain.h transaction.h merkel.h sha256.h sha256_utils.h cheater.h process.h config.h
