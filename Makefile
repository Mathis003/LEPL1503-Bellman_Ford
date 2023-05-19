# Variables
CC = gcc
CFLAGS = -Wall -Werror -g
LIBS = -lpthread
INCLUDE_HEADERS_DIRECTORY = -Iinclude

FILES_MAIN = ./src/display.c \
			 ./src/files.c \
			 ./src/bellman-ford.c \
			 ./src/struct_creater.c \
			 ./src/struct_initializer.c \
			 ./src/struct_free.c \
			 ./src/threads.c

FILES_MAIN_TEST = ./tests/src/display_test.c \
				  ./tests/src/bellman_test.c \
				  ./tests/src/files_test.c \
				  ./tests/src/verifyOutput_test.c \
				  ./tests/src/struct_initializer_test.c \
				  ./tests/src/struct_creater_test.c \
				  ./tests/src/struct_free_test.c \
				  ./tests/src/threads_test.c \
				  ./tests/src/final_test.c \
				  ./tests/src/main_test.c

FILES_HELPER_TEST = ./tests/src/helper/verifyOutput.c \
					./tests/src/helper/sort_bin_files.c


# Compile le fichier sp.c (et toutes les dépendances nécessaires se trouvant dans
# $(FILES_MAIN)) et créé l'exécutable ./main.  
main: ./sp.c $(FILES_MAIN)
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS)

	rm -rf main.dSYM


# Compile et exécute directement les tests unitaires (= l'exécutable ./unit_test).
# Supprime également les fichiers .dSYM générés par le compilateur (sur MacOS).
tests: $(FILES_MAIN) $(FILES_HELPER_TEST) $(FILES_MAIN_TEST)
	
	if [ ! -d "binary_files/general_case/output_C" ]; then mkdir binary_files/general_case/output_C; fi
	if [ ! -d "binary_files/limits_case/output_C" ]; then mkdir binary_files/limits_case/output_C; fi

	$(CC) $(CFLAGS) $(LIBS) -I$(HOME)/local/include -std=gnu99 $^ -L$(HOME)/local/lib -lcunit -o unit_test
	LD_LIBRARY_PATH=$(HOME)/local/lib ./unit_test

	rm -rf unit_test.dSYM


# Compile et exécute directement les tests unitaires (= l'exécutable ./unit_test).
# Génère égalmement un rapport de coverage.
# Supprime également les fichiers .dSYM générés par le compilateur (sur MacOS)
# et d'autres fichiers non nécessaire générés par la commande.
coverage: $(FILES_MAIN) $(FILES_HELPER_TEST) $(FILES_MAIN_TEST)
	
	if [ ! -d "binary_files/general_case/output_C" ]; then mkdir binary_files/general_case/output_C; fi
	if [ ! -d "binary_files/limits_case/output_C" ]; then mkdir binary_files/limits_case/output_C; fi

	$(CC) $(CFLAGS) $(LIBS) -I$(HOME)/local/include --coverage -std=gnu99 $^ -L$(HOME)/local/lib -lcunit -o unit_test
	LD_LIBRARY_PATH=$(HOME)/local/lib ./unit_test
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '*/tests/**' '*/usr/**' -o coverage.info
	genhtml coverage.info --output-directory coverage

	rm -rf unit_test.dSYM
	rm -rf *.gcda
	rm -rf *.gcno
	rm coverage.info
	rm coverage/index-sort-f.html
	rm coverage/index-sort-l.html


# Clean les fichiers .o et .exe + les exécutables ./unit_test et ./main +
# le dossier coverage + les dossiers contenant les outputs des tests finaux de notre programme C.
clean:
	rm -rf *.o *.exe *.bin
	rm -rf binary_files/limits_case/output_C binary_files/general_case/output_C
	rm -rf main unit_test
	rm -rf coverage


# La cible .PHONY force les commandes à s'exécuter même si la cible existe déjà dans le répértoire.
.PHONY: main tests coverage clean