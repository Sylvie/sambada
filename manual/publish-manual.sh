#!/bin/bash

dossierCourant=$(pwd)

dossierSmb="../../Sambada/"

cd $dossierSmb
numVersion=$(./update-version-number.sh)
cd $dossierCourant

nomManuel="SamBadoc-"${numVersion}

rm manual.pdf
latexmk  -pdf -g -quiet -jobname=${nomManuel} -pdflatex="pdflatex  %O '\def\versionnum{$numVersion}\input{%S}'" manual.tex 
