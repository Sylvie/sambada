#!/bin/bash
set -v
echo "TODO: measuring code coverage"
${GCOV} -r ../src/Archiviste.cpp -o src/binaries_sambada-Archiviste.o
${GCOV} -r ../src/Chronometre.cpp -o src/binaries_sambada-Chronometre.o
${GCOV} -r ../src/Duree.cpp -osrc/binaries_sambada-Duree.o
${GCOV} -r ../src/FluxSortie.cpp -osrc/binaries_sambada-FluxSortie.o
${GCOV} -r ../src/Journal.cpp -osrc/binaries_sambada-Journal.o
${GCOV} -r ../src/JournalTemporaire.cpp -osrc/binaries_sambada-JournalTemporaire.o
${GCOV} -r ../src/LecteurCheminAcces.cpp -osrc/binaries_sambada-LecteurCheminAcces.o
${GCOV} -r ../src/RegressionLogistique.cpp -osrc/binaries_sambada-RegressionLogistique-calc.o
${GCOV} -r ../src/RegressionLogistique.cpp -osrc/binaries_sambada-RegressionLogistique-io.o
${GCOV} -r ../src/mainSambada.cpp -osrc/binaries_sambada-mainSambada.o
${GCOV} -r ../src/Erreur.cpp -osrc/libintermediate_a-Erreur.o
${GCOV} -r ../src/RegressionLogistique-bienvenue.cpp -osrc/libintermediate_a-RegressionLogistique-bienvenue.o
${GCOV} -r ../src/Toolbox.cpp -osrc/libintermediate_a-Toolbox.o
