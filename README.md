# 42sh

42sh est un interpréteur shell codé en C suivant le stadard POSIX
(Portable Operating System Interface).
C'est un projet développé en janvier 2023 par Zoé Bréniaux, Louis Garnier,
Julien Delbosc et Sacha Hibon dans le cadre de nos études à epita sur un
environnement Linux.

# Fonctionnalités

42sh interprète :
- commandes simples tel que "ls -a"
- single et double quotes, le backslash
- les boucles (for, while, until)
- if, elif, else
- certains builtins (echo, true, false, break, continue, exit)
- la négation !
- la pipeline |
- certaines redirections (>, >>, <, <>, <&, >|)
- les variables
- les fonctions
- "case" command

Vous pouvez trouver des exemples de script shell dans le dossier
"tests/python_test/test_in/".

# Prérequis

- Cmake version 3.22
- C version C99

# Générer le binaire build/42sh

Taper ces commandes à la racine du projet :

cmake -S . -B build
make -C build

# Tester

3 méthodes pour utiliser le binaire :

- avec une string
./build/42sh -c "echo toto"

- avec un fichier
./build/42sh script.sh

- avec le standard input de manière interactive
./build/42sh