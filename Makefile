# Makefile pour le TD4 - Comparaison d'algorithmes de tri

CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = benchmark
SRCDIR = src
OBJDIR = obj
DATADIR = data
RESULTSDIR = results


SOURCES = $(SRCDIR)/main.c $(SRCDIR)/tri.c $(SRCDIR)/benchmark.c
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/tri.o $(OBJDIR)/benchmark.o


all: directories $(TARGET)

# Créer les dossiers nécessaires
directories:
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	@if not exist $(DATADIR) mkdir $(DATADIR)
	@if not exist $(RESULTSDIR) mkdir $(RESULTSDIR)
	@echo Dossiers créés

# Compilation du programme principal
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "✓ Compilation terminée : ./$(TARGET)"

# Compilation des fichiers objets
$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/tri.h $(SRCDIR)/benchmark.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o

$(OBJDIR)/tri.o: $(SRCDIR)/tri.c $(SRCDIR)/tri.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/tri.c -o $(OBJDIR)/tri.o

$(OBJDIR)/benchmark.o: $(SRCDIR)/benchmark.c $(SRCDIR)/benchmark.h $(SRCDIR)/tri.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/benchmark.c -o $(OBJDIR)/benchmark.o

# Exécuter le programme
run: all
	@echo "\n════════════════════════════════════════"
	@echo "  Exécution du benchmark"
	@echo "════════════════════════════════════════\n"
	./$(TARGET)

# Nettoyer les fichiers compilés
clean:
	if exist obj rmdir /s /q obj
	if exist benchmark.exe del benchmark.exe
	@echo "✓ Fichiers compilés supprimés"


# Nettoyer tout (compilation + résultats)
cleanall: clean
	rm -rf $(DATADIR)/*
	rm -rf $(RESULTSDIR)/*
	@echo "✓ Tous les fichiers supprimés"

# Afficher l'aide
help:
	@echo "Makefile pour TD4 - Comparaison d'algorithmes de tri"
	@echo ""
	@echo "Commandes disponibles :"
	@echo "  make           - Compile le projet"
	@echo "  make run       - Compile et exécute le benchmark"
	@echo "  make clean     - Supprime les fichiers compilés"
	@echo "  make cleanall  - Supprime tout (compilation + résultats)"
	@echo "  make help      - Affiche cette aide"

.PHONY: all directories run clean cleanall help