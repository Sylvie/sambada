#!/bin/bash
set -v
echo "TODO: measuring code coverage"
${GCOV} -r ../src/Archiviste.cpp -o src/binaries_sambada-Archiviste.o
${GCOV} -r ../src/Chronometre.cpp -o src/binaries_sambada-Chronometre.o
${GCOV} -r ../src/Duree.cpp -o src/binaries_sambada-Duree.o
${GCOV} -r ../src/FluxSortie.cpp -o src/binaries_sambada-FluxSortie.o
${GCOV} -r ../src/Journal.cpp -o src/binaries_sambada-Journal.o
${GCOV} -r ../src/JournalTemporaire.cpp -o src/binaries_sambada-JournalTemporaire.o
${GCOV} -r ../src/LecteurCheminAcces.cpp -o src/binaries_sambada-LecteurCheminAcces.o
${GCOV} -r ../src/RegressionLogistique.cpp -o src/binaries_sambada-RegressionLogistique-calc.o
${GCOV} -r ../src/RegressionLogistique.cpp -o src/binaries_sambada-RegressionLogistique-io.o
${GCOV} -r ../src/RegressionLogistique.cpp -o src/binaries_sambada-RegressionLogistique-as.o
${GCOV} -r ../src/mainSambada.cpp -o src/binaries_sambada-mainSambada.o
${GCOV} -r ../src/Erreur.cpp -o src/libintermediate_a-Erreur.o
${GCOV} -r ../src/RegressionLogistique-bienvenue.cpp -o src/libintermediate_a-RegressionLogistique-bienvenue.o
${GCOV} -r ../src/Toolbox.cpp -o src/libintermediate_a-Toolbox.o
${GCOV} -r ../src/mainSupervision.cpp -o src/binaries_supervision-mainSupervision.o
${GCOV} -r ../src/Supervision.cpp -o src/binaries_supervision-Supervision.o

