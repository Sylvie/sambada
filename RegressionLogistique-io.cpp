/*************************************************************************
* Copyright (©) 2011-2014 EPFL (Ecole Polytechnique fédérale de Lausanne)
* Laboratory of Geographic information systems (LaSIG)
* 
* This file is part of Sambada.
*  
* Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.
* Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
* You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.
* 
* Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), Stéphane Joost (stephane.joost@epfl.ch) 
* Laboratory of Geographic information systems
* EPFL ENAC IIE LASIG
* Station 18
* CH-1015 Lausanne
* Web site : http://lasig.epfl.ch/sambada
* 
* Sambada includes two libraries: Scythe Statistical Library (under GPL 3) and Shapefile C Library (under LGPL 2.1, courtesy of Frank Warmerdam).
* 
* Scythe Statistical Library
* Copyright (C) 2000-2002 Andrew D. Martin and Kevin M. Quinn;
* 2002-2012 Andrew D. Martin, Kevin M. Quinn, and Daniel Pemstein.  All Rights Reserved.
* 
* Shapefile C Library
* Copyright (c) 1999, Frank Warmerdam
*************************************************************************/



#include "RegressionLogistique.h"
#include "optimize.h"
#include <limits>
#include <list>
#include <ctime>

using namespace std;
using namespace scythe;

int RegressionLogistique::initialisation(int argc, char *argv[]) throw(Erreur)
{
    /*	cout << numeric_limits < double >::max() << " " << numeric_limits < reel >::max() << "\n";
    cout << log(numeric_limits < double >::max()) << " " << log(numeric_limits < reel >::max()) << "\n"; */

    // Gestion temporaire du journal
    JournalTemporaire journalTemp;

    if (argc==1)
    {
        messageBienvenue(journalTemp, true);
    }
    else
    {
        messageBienvenue(journalTemp);
    }

	journalTemp << "Program call:" << nl;
	for (int i(0); i<argc; ++i)
	{
		journalTemp << argv[i] << " ";
	}
	journalTemp << nl;
	
    // Conteneur pour les paramètres
    ParameterSet listeParam;

    // Création de l'index
    ParameterSetIndex indexParam;

    // Initialisation des paramètres
    initialisationParametres(listeParam, indexParam);

    // Comptage des arguments
    // 2 si le(s) noms(s) de fichiers d'entrée sont dans les paramètres
    // 3 ou 4 si les noms sont données en arguments
    if (argc > 4 || argc < 2)
    {
		Erreur e("MSG_falseArgNum", "Incorrect number of arguments");
		dumpJournalTemporaire(journalTemp, e);
        throw e;
    }

    string nomFichierParam(argv[1]);
    journalTemp << "Fetching parameter file: " << nomFichierParam << nl;
    ifstream entree(nomFichierParam.c_str());

    if (entree.fail())
    {
		Erreur e("MSG_errReadParams", "Error while reading parameters.");
		dumpJournalTemporaire(journalTemp, e);
        throw e;
    }

    // Lecture des paramètres
    try
    {
        // repérage du type de retour lignes pour formater les fichiers de résultats (et les logs)
        toolbox::chercheRetourLigne(entree, delimLignes);
        sortie.setRetourLigne(delimLignes);
		journal.setDelimLignes(delimLignes);
        lectureParametres(entree, indexParam, listeParam, journalTemp);
    }
    catch (Erreur & e)
    {	
		dumpJournalTemporaire(journalTemp, e);
        throw;
    }

    // Fin de la lecture des paramètres
    entree.close();

    // Analyse des paramètres
    ParameterSet::iterator paramCourant(listeParam.begin());

    //INPUTFILE
    // Le(s) nom(s) du/des fichier(s) d'entrées sont copiés dans le vecteur nomFichierInput
    // S'il y a 3 ou 4 arguments, le fichier de paramètres ne contient pas les noms des fichiers de données
    bool entete(false), uniqueFichierDonnees(true);
    vector<string> nomFichierInput(0);
    if (argc==3)
    {
        nomFichierInput.push_back(argv[2]); // Marqueurs
    }
    else if (argc==4)
    {
        uniqueFichierDonnees=false;
        nomFichierInput.push_back(argv[2]); // Env
        nomFichierInput.push_back(argv[3]); // Marqueurs
    }
    else // Le/les fichier(s) de données sont indiqués dans le fichier de paramètres
    {
        // FILENAME
        int nombreFichiers(paramCourant->contents.size());
        if (!paramCourant->present || nombreFichiers==0)
        {
			Erreur e("MSG_missDataFiles", "Missing data files.");
			dumpJournalTemporaire(journalTemp, e);
			throw e;
		}

        if (nombreFichiers==0 || nombreFichiers>2)
        {
            Erreur e("MSG_falseFilesNum", "Incorrect number of data files on line FILENAME.");
			dumpJournalTemporaire(journalTemp, e);
			throw e;
		}
        else if(nombreFichiers==2)
        {
            uniqueFichierDonnees=false;
        }
        // Copie des noms des fichiers
        nomFichierInput.insert(nomFichierInput.end(), paramCourant->contents.begin(), paramCourant->contents.end());
    }
    // Rem: le nom des fichiers de sortie est traité juste en dessous
    ++paramCourant;

    // OUTPUTFILE
    if (paramCourant->present && paramCourant->contents.size()==1)	// Cas où l'utilisateur a fourni un nom de fichier de sortie
    {
        int position(paramCourant->contents[0].rfind("."));
        if (position==string::npos)
        {
            nomFichierResultats.first=paramCourant->contents[0];
            nomFichierResultats.second="";
        }
        else
        {
            nomFichierResultats.first=paramCourant->contents[0].substr(0, position);
            nomFichierResultats.second=paramCourant->contents[0].substr(position);
        }
    }
    // Recherche du nom du fichier et de l'extension
    // S'il y a un unique fichier de données, on place les résultats dans le même dossier
    // S'il y en a deux, on place les résultats dans le dossier des marqueurs //variables environnementales
    // //-> Même code
    else if (uniqueFichierDonnees)
    {
        int position(nomFichierInput[0].rfind("."));
        nomFichierResultats.first=nomFichierInput[0].substr(0, position);
        nomFichierResultats.second=nomFichierInput[0].substr(position);

    }
    else
    {
        int position(nomFichierInput[1].rfind("."));
        nomFichierResultats.first=nomFichierInput[1].substr(0, position);
        nomFichierResultats.second=nomFichierInput[1].substr(position);
    }
    ++paramCourant;

    // WORDDELIM
    delimMots=' ';
    if (paramCourant->present && paramCourant->contents.size()==1)
    {
        int tailleDelim((paramCourant->contents[0]).size());
        // The delimiter might be surrounded by backets
        // If size is zero, the delimiter stays set to ' '
        journalTemp << "Size of word delimitor: " << tailleDelim<< nl;
        for (int i(0); i<tailleDelim; ++i)
        {
            journalTemp << "*" << (paramCourant->contents)[0][i] << "*" << nl;
        }
        if (tailleDelim==1)
        {
            delimMots=(paramCourant->contents)[0][0];
        }
        else if (tailleDelim>1)
        {
			journalTemp << "Word delimitor set to char: *" << (paramCourant->contents)[0][1] << "*" << nl;
            delimMots=(paramCourant->contents)[0][1];
        }
    }
    sortie.setDelimMots(delimMots);
	journal.setDelimMots(&delimMots);
    ++paramCourant;
	
	/* Word and line delimiters are known, as well as output file name.
	 The actual journal can be set up.*/
	journal.setNomFichier(nomFichierResultats.first+"-log"+nomFichierResultats.second);
	journal.ouvertureFichier();
	if (!journal.testeValiditeFichier())
	{
		Erreur e("MSG_errOpenLog", "Error while opening the log file.");
		dumpJournalTemporaire(journalTemp, e);
        throw e;
	}
	// Log transfer
	journal << journalTemp;
	
	modelesDivergents.setNomFichier(nomFichierResultats.first+"-unconvergedModels"+nomFichierResultats.second);
	modelesDivergents.ouvertureFichier();
	if (!modelesDivergents.testeValiditeFichier())
	{
		Erreur e("MSG_errOpenUnconvergedModelsFile", "Error while opening the file for unconverged models.");
		journal << e.getPhrase() << nl;
        throw e;
	}

    // HEADERS
    if (!paramCourant->present || paramCourant->contents.size()==0)
    {
        entete=false;
    }
    else
    {
        string lu(paramCourant->contents[0]);
        if (lu == "0" || lu[0] == 'N' || lu[0] == 'n')
        {
            entete=false;
        }
        else
        {
            entete=true;
        }
    }
    ++paramCourant;

    // NUMVARENV
    nbEnv=toolbox::conversion<int>(paramCourant->contents[0]);
    ++paramCourant;

    journal << "Number of environmental variables in file: " << nbEnv << nl;
    nbEnvActives=nbEnv;

    /*
     // De base, tous les param env sont des variables et tous sont actifs
     for (int i(0); i<nbEnv; ++i)
     {
     variables.insert(i);
     }
     varActives=variables;
     */
    // NUMMMARK
    nbMarq=toolbox::conversion<int>(paramCourant->contents[0]);
    if (paramCourant->contents.size()==2)
    {
        nbMarqTot=toolbox::conversion<int>(paramCourant->contents[1]);
        // S'il y a plusieurs fichiers de marqueurs, il faut repérer le numéro du premier qu'on a
        int position(nomFichierResultats.first.rfind("-"));
        if (position==string::npos)
        {
            Erreur e("MSG_missing-first-mark", "Missing number of the first marker. \n It must be provided in the name of the input marker file (as done by Supervision) or in the name given in the optional OUTPUTFILE entry.");
			journal << e.getPhrase() << nl;
			throw e;
        }
        else
        {

            journal << "Number of the first marker: "<< nomFichierResultats.first.substr(position+1) << nl;

            istringstream iss((nomFichierResultats.first.substr(position+1)));
            iss >> numPremierMarq;
            if (iss.fail()) 
			{
                Erreur e("MSG_non-integer-first-mark-number", "Non-integer entry for the number of the first marker.");
				journal << e.getPhrase() << nl;
				throw e;
			}
            else
            {
                journal << iss.str() << "->" << numPremierMarq << nl;
            }
        }
    }
    else
    {

        nbMarqTot=nbMarq;
        numPremierMarq=0;
    }
    ++paramCourant;

    // NUMINDIV
    nbPoints=toolbox::conversion<int>(paramCourant->contents[0]);
    ++paramCourant;


    // ***Lecture des en-têtes***
    // L'analyse des paramètres est mise en attente:
    // Les noms des colonnes nous seront utiles pour la suite!


    int nbCols(nbEnv+nbMarq);
    vector< string > headerEnv, headerMarq;
    //	headerEnv.clear();
    //	headerMarq.clear();

    if (!entete)
    {
        // Construction de l'entête
        for (int i(0); i<nbEnv; ++i)
        {
            // créer un flux de sortie
            std::ostringstream oss;
            oss << i+1;
            headerEnv.push_back("P"+oss.str());
        }
        for (int i(0); i<nbMarq; ++i)
        {
            // créer un flux de sortie
            std::ostringstream oss;
            if (nbMarqTot == nbMarq)
            {
                oss << (i+1);

            }
            else
            {
                // On ajoute le numéro du premier marqueur pour tenir compte des calculs partagés
                oss << (i+numPremierMarq);
            }

            headerMarq.push_back("M"+oss.str());
        }
    }
    else if (uniqueFichierDonnees)
    {
		journal << "Fetching data:" << nomFichierInput[0].c_str() <<nl;
        entree.open((nomFichierInput[0]).c_str());
        if (entree.fail())
        {
            Erreur e("MSG_errOpenFiles", "Error while opening data files.");
			journal << e.getPhrase() << nl;
			throw e;

        }

        // Lecture de l'en-tête
        vector<string> header(0);
        toolbox::lectureLigne(entree, header, delimMots);

        entree.close();


        nbCols = header.size();
        if (nbEnv+nbMarq != nbCols)
        {
            journal << "Header size: " << nbCols << nl;
            for (int q(0); q<nbCols; ++q)
                journal << q+1 << " " << header[q] << nl;
            Erreur e("MSG_falseDimNum", "Incorrect data dimension: NUMVARENV and NUMMARK do not sum up to the header size ("+toolbox::conversion(nbCols)+").");
			journal << e.getPhrase() << nl;
			throw e;

        }
        headerEnv.resize(nbEnv, "");
        headerMarq.resize(nbMarq, "");
        for (int i(0); i<nbEnv; ++i)
        {
            headerEnv[i]=header[i];
        }
        for (int i(0); i<nbMarq; ++i)
        {
            headerMarq[i]=header[nbEnv+i];
        }
    }
    else // Deux fichiers de données
    {
        // Lecture des données environnementales
        journal << "Fetching environmental data:" << nomFichierInput[0].c_str() <<nl;
        entree.open(nomFichierInput[0].c_str());
        if (entree.fail())
        {
            Erreur e("MSG_errOpenEnv", "Error while opening environnemental data file.");
			journal << e.getPhrase() << nl;
			throw e;
        }

        // Lecture de l'en-tête
        toolbox::lectureLigne(entree, headerEnv, delimMots);

        entree.close();

        if (headerEnv.size()!=nbEnv)
        {
            journal << "NUMVARENV (" <<nbEnv<< ") does not match the header size (" << headerEnv.size() << ")." <<nl;
            Erreur e("MSG_falseEnvNum", "Incorrect number of environnemental data.");
			journal << e.getPhrase() << nl;
			throw e;
        }


        // Lecture des données génétiques
		journal << "Fetching genetic data:" << nomFichierInput[1].c_str() <<nl;
		entree.open(nomFichierInput[1].c_str());
        if (entree.fail())
        {
            Erreur e("MSG_errOpenMark", "Error while opening genetic data file.");
			journal << e.getPhrase() << nl;
			throw e;
        }

        // Lecture de l'en-tête
        toolbox::lectureLigne(entree, headerMarq, delimMots);

        entree.close();

        if (headerMarq.size()!=nbMarq)
        {
            journal << "NUMMARK (" <<nbMarq<< ") does not match the header size (" << headerMarq.size() << ")." <<nl;
            Erreur e("MSG_falseMarkNum", "Incorrect number of genetic markers.");
			journal << e.getPhrase() << nl;
			throw e;
		}
    }

    journal << "Headers sizes : " << headerEnv.size() << " " << headerMarq.size() << nl;

    // Initialisation des tableaux de spécifications
    // Le classement des variables actives et passives se fera quand tous les paramètres auront été lus
    DetailsVariable details={0, "", true, true, 0};
    specDataEnv.resize(nbEnv);
    for (int i(0); i<nbEnv; ++i)
    {
        details.number=i;
        details.name=headerEnv[i];
        specDataEnv[i]=details;
    }
    specDataMarq.resize(nbMarq);
    for (int i(0); i<nbMarq; ++i)
    {
        details.number=i;
        details.name=headerMarq[i];
        specDataMarq[i]=details;
    }


    // Suite de l'analyse du fichier de paramètres

    // INDIVID
    if (paramCourant->present && paramCourant->contents.size()>0)
    {
        existeColID=true;
        int nbCas(paramCourant->contents.size());
        // On stocke le numéro global de la colonne!
        if (!entete) // Cas facile : il suffit de lire les numéros
        {
            if (nbCas==1 || uniqueFichierDonnees)
            {
                colIDEnv=toolbox::conversion<int>(paramCourant->contents[0]);
                colIDMarq=colIDEnv;
            }
            else
            {
                colIDEnv=toolbox::conversion<int>(paramCourant->contents[0]);
                colIDMarq=toolbox::conversion<int>(paramCourant->contents[1]);
            }

        }
        else
        {
            // Il faut trouver les headers correspondants
            if (uniqueFichierDonnees)
            {
                colIDEnv=find(headerEnv.begin(), headerEnv.end(), paramCourant->contents[0])-headerEnv.begin();
                colIDMarq=colIDEnv;
            }
            else if (nbCas==1)
            {
                colIDEnv=find(headerEnv.begin(), headerEnv.end(), paramCourant->contents[0])-headerEnv.begin();
                colIDMarq=find(headerMarq.begin(), headerMarq.end(), paramCourant->contents[0])-headerMarq.begin();
            }
            else
            {
                colIDEnv=find(headerEnv.begin(), headerEnv.end(), paramCourant->contents[0])-headerEnv.begin();
                colIDMarq=find(headerMarq.begin(), headerMarq.end(), paramCourant->contents[1])-headerMarq.begin();
            }

        }
        if ((uniqueFichierDonnees && (colIDEnv>=nbEnv || colIDMarq>=(nbEnv+nbMarq))) || (!uniqueFichierDonnees && (colIDEnv>=nbEnv || colIDMarq>=nbMarq)))
        {
            Erreur e("MSG_errIDcol", "Incorrect definition of ID columns.");
			journal << e.getPhrase() << nl;
			throw e;
        }
        specDataEnv[colIDEnv].isNumeric=false;
        specDataEnv[colIDEnv].isActive=false;
        if(!uniqueFichierDonnees)
        {
            specDataMarq[colIDMarq].isNumeric=false;
            specDataMarq[colIDMarq].isActive=false;
        }
    }
    ++paramCourant;

    // COLSUPENV
    if (paramCourant->present && paramCourant->contents.size()>0)
    {
        int nbCas(paramCourant->contents.size());
        int colCourante(0);
        for (int i(0); i<nbCas; ++i)
        {
            if (!entete) // Cas facile : il suffit de lire les numéros, les colonnes sont numérotées de P1 à PN
            {
                colCourante=toolbox::conversion<int>(paramCourant->contents[i]);
                if (colCourante>=nbEnv)
                {
                    Erreur e("MSG_missIdleEnv", "Error: Missing idle environnemental variable : "+colCourante);
					journal << e.getPhrase() << nl;
					throw e;
				}
            }
            else
            {
                // Il faut trouver le header correspondant, puis enlever son numéro de la liste des variables actives

                colCourante=find(headerEnv.begin(), headerEnv.end(), paramCourant->contents[i])-headerEnv.begin();
                if (colCourante>=nbEnv)
                {
                    Erreur e("MSG_missIdleEnv", "Error: Missing idle environnemental variable : "+paramCourant->contents[i]);
					journal << e.getPhrase() << nl;
					throw e;
				}

            }

            specDataEnv[colCourante].isNumeric=false;
            specDataEnv[colCourante].isActive=false;
        }
    }
    ++paramCourant;

    // COLSUPMARK
    // Attention aux nombre total de marqueurs et au numéro du premier marqueur
    if (paramCourant->present && paramCourant->contents.size()>0)
    {
        int nbCas(paramCourant->contents.size());
        int colCourante(0);
        for (int i(0); i<nbCas; ++i)
        {
            if (!entete) // Cas facile : il suffit de lire les numéros // Attention à la première colonne
            {
                colCourante=toolbox::conversion<int>(paramCourant->contents[i]);
                if (colCourante>=nbMarq)
                {
                    Erreur e("MSG_missIdleMark", "Error: Missing idle genetic marker : "+colCourante);
					journal << e.getPhrase() << nl;
					throw e;
                }
                else
                {
                    // Correction pour numéro du premier marqueur
                    if (nbMarqTot == nbMarq)
                    {
                        colCourante-=1;
                    }
                    else
                    {
                        colCourante-=numPremierMarq;
                    }
                }

            }
            else
            {
                // Il faut trouver le header correspondant, puis enlever son numéro de la liste des variables actives

                colCourante=find(headerMarq.begin(), headerMarq.end(), paramCourant->contents[i])-headerMarq.begin();
                if (colCourante>=nbMarq)
                {
                    Erreur e("MSG_missIdleMark", "Error: Missing idle genetic marker : "+paramCourant->contents[i]);
					journal << e.getPhrase() << nl;
					throw e;
				}

            }

            specDataMarq[colCourante].isNumeric=false;
            specDataMarq[colCourante].isActive=false;
        }
    }
    ++paramCourant;

    // Sélection de données géographiques
    // SUBSETVARENV
    if (paramCourant->present && paramCourant->contents.size()>0)
    {
        int nbCas(paramCourant->contents.size());
        int colCourante(0);
        // Principe: On repère les numéros des colonnes concernées
        // On remplit un tableau de taille nbEnv avec la liste des variables à garder (les numéros repérés)
        // On parcourt le tableau, toutes les colonnes qui ont la valeur 0 sont désactivées
        vector<bool> listeConservation(nbEnv, false);
        for (int i(0); i<nbCas; ++i)
        {
            if (!entete) // Cas facile : il suffit de lire les numéros
            {
                colCourante=toolbox::conversion<int>(paramCourant->contents[i]);
                if (colCourante>=nbEnv)
                {
                    Erreur e("MSG_missActiveEnv", "Error: Missing active environnemental variable : "+colCourante);
					journal << e.getPhrase() << nl;
					throw e;
                }
            }
            else
            {
                // Il faut trouver le header correspondant, puis enlever son numéro de la liste des variables actives

                colCourante=find(headerEnv.begin(), headerEnv.end(), paramCourant->contents[i])-headerEnv.begin();
                if (colCourante>=nbEnv)
                {
                    Erreur e("MSG_missActiveEnv", "Error: Missing active environnemental variable : "+paramCourant->contents[i]);
					journal << e.getPhrase() << nl;
					throw e;
				}

            }

            listeConservation[colCourante]=true;
        }

        for (int i(0); i<nbEnv; ++i)
        {
            if (!listeConservation[i])
            {
                specDataEnv[i].isNumeric=false;
                specDataEnv[i].isActive=false;
            }
        }

    }
    ++paramCourant;

    // Sélection de marqueurs génétiques
    // SUBSETMARK
    if (paramCourant->present && paramCourant->contents.size()>0)
    {
        int nbCas(paramCourant->contents.size());
        int colCourante(0);
        // Principe: On repère les numéros des colonnes concernées
        // On remplit un tableau de taille nbMarq avec la liste des variables à garder (les numéros repérés)
        // On parcourt le tableau, toutes les colonnes qui ont la valeur 0 sont désactivées
        vector<bool> listeConservation(nbMarq, false);
        for (int i(0); i<nbCas; ++i)
        {
            if (!entete) // Cas facile : il suffit de lire les numéros
            {
                colCourante=toolbox::conversion<int>(paramCourant->contents[i]);
                if (colCourante>=nbMarq)
                {
                    Erreur e("MSG_missActiveMark", "Error: Missing active genetic marker : "+colCourante);
					journal << e.getPhrase() << nl;
					throw e;
				}
            }
            else
            {
                // Il faut trouver le header correspondant, puis enlever son numéro de la liste des variables actives

                colCourante=find(headerMarq.begin(), headerMarq.end(), paramCourant->contents[i])-headerMarq.begin();
                if (colCourante>=nbMarq)
                {
                    Erreur e("MSG_missActiveMark", "Error: Missing active genetic marker : "+paramCourant->contents[i]);
					journal << e.getPhrase() << nl;
					throw e;
				}

            }

            listeConservation[colCourante]=true;
        }

        for (int i(0); i<nbMarq; ++i)
        {
            if (!listeConservation[i])
            {
                specDataMarq[i].isNumeric=false;
                specDataMarq[i].isActive=false;
            }
        }

    }
    ++paramCourant;




    // DIMMAX
    // HEADERS
    if (!paramCourant->present || paramCourant->contents.size()==0)
    {
        dimensionMax=1;
    }
    else
    {
        bool echec(false);
        dimensionMax = toolbox::conversion<int>(paramCourant->contents[0], echec);
        if (echec)
        {
            Erreur e("MSG_invalidDimMax", "Invalid maximal dimension: "+paramCourant->contents[0]);
			journal << e.getPhrase() << nl;
			throw e;
		}
    }
    ++paramCourant;


    // SPATIAL
    if (paramCourant->present)
    {
        if (paramCourant->contents.size()!=5)
        {
            Erreur e("MSG_spatialArgNum", "Incorrect number of arguments for SPATIAL parameter : 5 required.");
			journal << e.getPhrase() << nl;
			throw e;
		}
        else
        {
            analyseSpatiale=true;
            // Colonnes pour longitude et latitude
            // On stocke le numéro global de la colonne!
            if (!entete)
            {
                longitude=toolbox::conversion<int>(paramCourant->contents[0]);
                latitude=toolbox::conversion<int>(paramCourant->contents[1]);
            }
            else
            {
                longitude=find(headerEnv.begin(), headerEnv.end(), paramCourant->contents[0])-headerEnv.begin();
                latitude=find(headerEnv.begin(), headerEnv.end(), paramCourant->contents[1])-headerEnv.begin();
            }
            if (longitude >= nbEnv || latitude >=nbEnv)
            {
                Erreur e("MSG_falseCrdDef", "Incorrect definition for longitude or latitude.");
				journal << e.getPhrase() << nl;
				throw e;
            }

            // De base, la longitude et la latitude sont considérées comme des variables de régression
            // Si l'utilisateur les a marquées comme inactives, alors elles ne seront pas utilisées pour construire des modèles
            // En conséquence, les coordonnées peuvent être stockés soit avec les variables actives soit avec les variables passives (strings!)

            // Type de coordonnées
            if (paramCourant->contents[2]=="CARTESIAN")
            {
                crdCartesiennes=true;
            }
            else
            {
                crdCartesiennes=false;
            }

            // Traitement du type de voisinage
            string ponderationLue(paramCourant->contents[3]);
            if (ponderationLue=="DISTANCE" || ponderationLue=="D" || ponderationLue=="d")
            {
                choixPonderation=pondDistanceMax;
            }
            else if (ponderationLue=="GAUSSIAN" || ponderationLue=="G" || ponderationLue=="g")
            {
                choixPonderation=pondGaussienne;
            }
            else if (ponderationLue=="BISQUARE" || ponderationLue=="B" || ponderationLue=="b")
            {
                choixPonderation = pondBicarree;
            }
            else
            {
                choixPonderation = pondPlusProchesVoisins;
            }

            if (choixPonderation != pondPlusProchesVoisins)
            {
                bandePassante=toolbox::conversion<reel>(paramCourant->contents[4]);
            }
            else
            {
                nbPlusProchesVoisins=toolbox::conversion<int>(paramCourant->contents[4]);
            }
        }
    }
    ++paramCourant;

    // AUTOCORR
    if (paramCourant->present)
    {
        if (paramCourant->contents.size()<2 || paramCourant->contents.size()>3)
        {
            Erreur e("MSG_autocorrArgNum", "Incorrect number of arguments for AUTOCORR parameter : 2 required.");
			journal << e.getPhrase() << nl;
			throw e;
		}
        else
        {
            // Autocorrélation globale ou locale
            string lu(paramCourant->contents[0]);
            AS_autocorrGlobale=false;
            AS_autocorrLocale=false;
            if (lu == "GLOBAL" || lu == "BOTH")
            {
                AS_autocorrGlobale=true;
            }
            if (lu == "LOCAL" || lu=="BOTH")
            {
                AS_autocorrLocale=true;
            }

            // Autocorrélation des variables environnementales ou des marqueurs;
            lu=paramCourant->contents[1];
            AS_autocorrVarEnv=false;
            AS_autocorrMarq=false;
            if (lu == "ENV" || lu == "BOTH")
            {
                AS_autocorrVarEnv=true;
            }
            if (lu == "MARK" || lu=="BOTH")
            {
                AS_autocorrMarq=true;
            }

            // Nombre de permutations pour le test de significativité
            if (paramCourant->contents.size()==3)
            {
                AS_nbPermutations=toolbox::conversion<int>(paramCourant->contents[2]);
            }
            else
            {
                AS_nbPermutations=9999;
            }
        }
    }
    ++paramCourant;

    // GWR
    if (!paramCourant->present)
    {
        AS_GWR=false;
    }
    else if(paramCourant->contents.size()==0)
    {
        AS_GWR=true;
    }
    else
    {
        string lu(paramCourant->contents[0]);
        if (lu == "0" || lu[0] == 'N' || lu[0] == 'n')
        {
            AS_GWR=false;
        }
        else
        {
            AS_GWR=true;
        }
    }
    ++paramCourant;

    // SHAPEFILE
    if (!paramCourant->present)
    {
        AS_shapefile=false;
    }
    else if(paramCourant->contents.size()==0)
    {
        AS_shapefile=true;
    }
    else
    {
        string lu(paramCourant->contents[0]);
        if (lu == "0" || lu[0] == 'N' || lu[0] == 'n')
        {
            AS_shapefile=false;
        }
        else
        {
            AS_shapefile=true;
        }
    }
    ++paramCourant;


    // DISCRETEVAR
    ++paramCourant;

    // Gestion des sauvegardes
    // SAVETYPE
    journal << paramCourant->name << nl;
    int nbParamSauvegarde(paramCourant->contents.size());
    if (nbParamSauvegarde<2 || nbParamSauvegarde>3)
    {
        Erreur e("MSG_savetypeArgNum", "Incorrect number of arguments for SAVETYPE parameter.");
		journal << e.getPhrase() << nl;
        throw e;
	}
    else
    {
        if (paramCourant->contents[0]=="REAL")
        {
            sauvegardeTempsReel=true;
        }
        else
        {
            sauvegardeTempsReel=false;
        }

        if (paramCourant->contents[1]=="ALL")
        {
            journal << "all!" << nl;
            selModeles=all;
        }
        else
        {
            if (paramCourant->contents[1]=="BEST")
            {
                journal << "best!" << nl;
                selModeles=best;
            }
            else
            {
                journal << "signif!" << nl;
                selModeles=signif;
            }

            if(nbParamSauvegarde!=3)
            {
                Erreur e("MSG_savetypeThreshold", "SAVETYPE : Specify the significance threshold for best models.");
           		journal << e.getPhrase() << nl;
				throw e;
			}
            else
            {
                seuilPValeur = toolbox::conversion<reel>(paramCourant->contents[2]);
                seuilScore.resize(dimensionMax+1, 0);
                seuilScoreMultivarie.resize(dimensionMax+1, 0);
                seuilScore[0]=0;
                seuilScoreMultivarie[0]=0;
                // Il faut repérer quelles colonnes sont actives pour calculer les seuils de Bonferroni

            }
        }


    }

    /* FIN DU TRAITEMENT DES PARAMETRES */

    // Quelques vérifications
    if (existeColID && analyseSpatiale)
    {
        if (colIDEnv==latitude || colIDEnv==longitude)
        {
            Erreur e("MSG_longLatID", "Longitude and latitude cannot be chosen as ID.");
			journal << e.getPhrase() << nl;
			throw e;
		}
    }

    // Décompte et repérage des colonnes actives et inactives
    // Environnement
    nbEnvActives=0;
    int nbPassives(0);
    for (int i(0); i<nbEnv; ++i)
    {
        if (specDataEnv[i].isActive)
        {
            varEnvActives.insert(make_pair(nbEnvActives, i));
            specDataEnv[i].localIndex=nbEnvActives; // On peut ainsi connaître la position de la variable dans les sous-tableaux
            ++nbEnvActives;
        }
        else
        {
            varEnvPassives.insert(make_pair(nbPassives, i));
            specDataEnv[i].localIndex=nbPassives;
            ++nbPassives;
        }
        //cout << specDataEnv[i].number << " "<< specDataEnv[i].name << " "<< specDataEnv[i].isNumeric << " "<< specDataEnv[i].isActive << " "<< specDataEnv[i].localIndex << endl;

    }
    // Marqueurs
    nbMarqActifs=0;
    nbPassives=0;
    for (int i(0); i<nbMarq; ++i)
    {
        if (specDataMarq[i].isActive)
        {
            marqActifs.insert(make_pair(nbMarqActifs, i));
            specDataMarq[i].localIndex=nbMarqActifs;	// On peut ainsi connaître la position de la variable dans les sous-tableaux
            ++nbMarqActifs;
        }
        else
        {
            marqPassifs.insert(make_pair(nbPassives, i));
            specDataMarq[i].localIndex=nbPassives;
            ++nbPassives;
        }
    }
    // Ici il faut mettre nbMarkTot à jour si les calculs ne sont pas distribués
    if (nbMarqTot==nbMarq)
    {
        nbMarqTot=nbMarqActifs;
    }

    if(selModeles!=all)
    {
        // Calcul des seuils de p-valeurs
        journal << "Computing p-value thresholds..." << nl;
		journal << "Dimension" << "nbModels" << "p-value" << "univariateThreshold" << "multivariateThreshold" << nl;
        reel nbModeles, pValeur;
        for (int i(1); i<=dimensionMax; ++i)
        {
            nbModeles = toolbox::combinaisons(nbEnvActives, i)*nbMarqTot;
            pValeur=seuilPValeur/nbModeles;
            seuilScore[i] = toolbox::invCDF_ChiSquare(1-pValeur, 1, sqrt(epsilon < reel > ()));
            seuilScoreMultivarie[i] = toolbox::invCDF_ChiSquare(1-pValeur, i, sqrt(epsilon < reel > ()));
            journal << "Dim "<< i << " " << nbModeles << " " << pValeur << " " << seuilScore[i] << " " << seuilScoreMultivarie[i] << nl;
        }
		journal << nl;
	}


    /*	LECTURE DES DONNEES */

    if (uniqueFichierDonnees)
    {
        journal << "Reading data in " << nomFichierInput[0] << nl;

        entree.open((nomFichierInput[0]).c_str());
        entree.precision(toolbox::precisionLecture);
        if (entree.fail())
        {
            Erreur e("MSG_errOpenFiles", "Error while opening data files.");
			journal << e.getPhrase() << nl;
			throw e;
        }

        // Lecture des données
        vector< vector< int > > validation;
        unsigned int rows(0), caseCourante(0), nombreRecuperes(0); // Nombre de lignes lues & la case actuelle dans la liste de validation

        // Variables temporaires
        vector<reel> line(0);
        vector<int> lineValidation(0), listeErreurs(0);
        vector<string> ligneOriginale(0);

        // S'il y a un en-tête, on passe la première ligne
        if (entete)
        {
            toolbox::lectureLigne(entree, ligneOriginale, delimMots);
        }


        dataEnv.resize(nbPoints, nbEnvActives);
        if (nbEnvActives>0)
        {
            dataEnv=0;
        }
        dataMarq.resize(nbPoints, nbMarqActifs);
        if (nbMarqActifs>0)
        {
            dataMarq=0;
        }
        dataSupEnv.resize(nbPoints, nbEnv-nbEnvActives);
        if (nbEnv>nbEnvActives)
        {
            dataSupEnv="";
        }
        dataSupMarq.resize(nbPoints, nbMarq-nbMarqActifs);
        if (nbMarq>nbMarqActifs)
        {
            dataSupMarq=0;
        }
        while (!entree.eof())
        {
            if (rows%100==0)
            {
                journal <<"Row " << rows << nl;
            }
            line.clear();
            lineValidation.clear();
            listeErreurs.clear();
            caseCourante=0;
            nombreRecuperes=0;
            ligneOriginale.clear();

            // ligneOriginale contient tous les mots lus
            toolbox::lectureLigne(entree, line, lineValidation, ligneOriginale, delimMots);


            if(nbCols != line.size())
            {
                //for (int compteur(0); compteur<line.size(); ++compteur) cout << compteur << " " << line[compteur] << " " << ligneOriginale[compteur] << endl;
                std::ostringstream oss;
                oss << "Row " << (rows + 1) << " of input file has " << line.size() << " elements, but should have " << nbCols << ".";
                Erreur e("MSG_falseLineSize", oss.str());
				journal << e.getPhrase() << nl;
				throw e;
            }

            if (rows >= nbPoints)
            {
                Erreur e("MSG_tooManyPoints", "Incorrect points number, too many lines in the data file.");
				journal << e.getPhrase() << nl;
				throw e;
            }

            // Validation des résultats
            caseCourante=0;
            nombreRecuperes=lineValidation.size();
            // Variables environnementales
            for (int i(0); i<nbEnv; ++i)
            {
                // Variable active
                if (specDataEnv[i].isActive)
                {
                    if (caseCourante>=nombreRecuperes || i<lineValidation[caseCourante]) // Cas sans erreur
                    {
                        dataEnv(rows,specDataEnv[i].localIndex)=line[i];
                    }
                    else	// Erreur détectée -> on la note dans la ligne de validation
                    {
                        //cout << " $ " << i << " " << lineValidation[caseCourante] << "\n";
                        // Si la variable est active et illisible, on ne s'intéresse pas à la valeur lue
                        listeErreurs.push_back(lineValidation[caseCourante]);
                        ++caseCourante;
                    }

                }
                // Variable passive (string)
                // Pour toutes les variables passives, il suffit de copier la valeur lue (un string)
                else
                {
                    dataSupEnv(rows,specDataEnv[i].localIndex)=ligneOriginale[i];
                    // Si la case était illisble, il faut passer à la prochaine case récupérée
                    if ((caseCourante<nombreRecuperes) && (i==lineValidation[caseCourante]))
                    {
                        ++caseCourante;
                    }
                }

            }
            //cout << "\n";
            //cout << "!" << dataSupEnv(rows, _);
            // Marqueurs
            for (int i(0); i<nbMarq; ++i)
            {
                // Variable active
                if (specDataMarq[i].isActive)
                {
                    if (caseCourante>=nombreRecuperes || (i+nbEnv)<lineValidation[caseCourante]) // Cas sans erreur
                    {
                        dataMarq(rows,specDataMarq[i].localIndex)=line[i+nbEnv];
                    }
                    else	// Erreur détectée -> on la note dans la ligne de validation
                    {
                        //cout << " % " << i << " " << lineValidation[caseCourante] << "\n";
                        // Si la variable est active et illisible, on ne s'intéresse pas à la valeur lue
                        listeErreurs.push_back(lineValidation[caseCourante]);
                        ++caseCourante;
                    }

                }
                // Variable passive (string)
                // Pour toutes les variables passives, il suffit de copier la valeur lue (un string)
                else
                {
                    dataSupMarq(rows,specDataMarq[i].localIndex)=ligneOriginale[i+nbEnv];
                    // Si la case était illisble, il faut passer à la prochaine case récupérée
                    if ((caseCourante<nombreRecuperes) && (i==lineValidation[caseCourante]))
                    {
                        ++caseCourante;
                    }
                }
            }

            validation.push_back(listeErreurs);
            ++rows;

            entree >> ws;
        }

        entree.close();


        if (rows < nbPoints)
        {
            std::ostringstream oss;
            oss << "Data file contains " << (rows) << " samples, while there should be " <<  nbPoints << ".";
			Erreur e("MSG_tooFewPoints", "Incorrect point number: "+oss.str());
			journal << e.getPhrase() << nl;
			throw e;
        }


        missingValuesEnv.resize(nbEnvActives, std::set< int > ());
        missingValuesMarq.resize(nbMarqActifs, std::set< int > ());

        for (unsigned int i(0); i < rows; ++i)
        {
            // Pour chaque ligne de data, validation contient les colonnes manquantes
            // On note l'indice dans le tableau des variables actives!
            //cout << "*" << validation[i].size() << "\n";
            for (int j(0); j<(validation[i].size()); ++j)
            {
                //cout << i << " " << j << " : " << validation[i][j] << "\n";
                if (validation[i][j]<nbEnv)
                {
                    missingValuesEnv[specDataEnv[validation[i][j]].localIndex].insert(i);
                }
                else
                {
                    //cout << specDataMarq[validation[i][j]-nbEnv].localIndex << "\n";
                    missingValuesMarq[specDataMarq[validation[i][j]-nbEnv].localIndex].insert(i);
                }
            }
        }

        /*cout << "Env : " << "\n" << dataEnv(0,0,3,10) << "\n";

         cout << "Marqueurs :" << dataMarq.rows() << " " << dataMarq.cols()<< "\n"<<(dataMarq(0,0, 1,138)) << "\n";*/
        //	cout << toolbox::sommeNumerique(dataMarq(0,138,384,138)) << " " << toolbox::sommeNumerique(dataMarq(0,139,384,139)) << "\n";
    }
    else // deux fichiers pour les données
    {
        // Lecture des données environnementales
        journal << "Reading data in " << nomFichierInput[0] << nl;		
        entree.open(nomFichierInput[0].c_str());
        entree.precision(toolbox::precisionLecture);
        if (entree.fail())
        {
            Erreur e("MSG_errOpenEnv", "Error while opening environnemental data file.");
			journal << e.getPhrase() << nl;
			throw e;
        }


        nbCols = headerEnv.size();

        // Lecture des données
        vector< vector< int > > validation;
        unsigned int rows(0), caseCourante(0), nombreRecuperes(0);

        // Variables temporaires
        vector<reel> line(0);
        vector<int> lineValidation(0), listeErreurs(0);
        vector<string> ligneOriginale(0);


        // S'il y a un en-tête, on passe la première ligne
        if (entete)
        {
            toolbox::lectureLigne(entree, ligneOriginale, delimMots);
        }


        dataEnv.resize(nbPoints, nbEnvActives);
        if (nbEnvActives>0)
        {
            dataEnv=0;
        }
        dataSupEnv.resize(nbPoints, nbEnv-nbEnvActives);
        if (nbEnv>nbEnvActives)
        {
            dataSupEnv="";
        }

        while (!entree.eof())
        {
            if (rows%100==0)
            {
                journal <<"Row " << rows << nl;
            }

            line.clear();
            lineValidation.clear();
            listeErreurs.clear();
            caseCourante=0;
            nombreRecuperes=0;
            ligneOriginale.clear();

            // ligneOriginale contient tous les mots lus
            toolbox::lectureLigne(entree, line, lineValidation, ligneOriginale, delimMots);

            if(nbCols != line.size())
            {
                std::ostringstream oss;
                oss << "Row " << (rows + 1) << " of environmental input file has " << line.size() << " elements, but should have " << nbCols << ".";
                Erreur e("MSG_falseLineSize", oss.str());
				journal << e.getPhrase() << nl;
				throw e;
			}

            if (rows >= nbPoints)
            {
                Erreur e("MSG_tooManyPoints", "Incorrect number of points, too many lines in environmental data file.");
				journal << e.getPhrase() << nl;
				throw e;
            }

            // Validation des résultats
            caseCourante=0;
            nombreRecuperes=lineValidation.size();	// Nombre de mots inconvertibles
            // Variables environnementales
            for (int i(0); i<nbEnv; ++i)
            {
                // Variable active
                if (specDataEnv[i].isActive)
                {
                    if (caseCourante>=nombreRecuperes || i<lineValidation[caseCourante]) // Cas sans erreur
                    {
                        dataEnv(rows,specDataEnv[i].localIndex)=line[i];
                    }
                    else	// Erreur détectée -> on la note dans la ligne de validation
                    {
                        // Si la variable est active et illisible, on ne s'intéresse pas à la valeur lue
                        listeErreurs.push_back(lineValidation[caseCourante]);
                        ++caseCourante;
                    }

                }
                // Variable passive (string)
                // Pour toutes les variables passives, il suffit de copier la valeur lue (un string)
                else
                {
                    dataSupEnv(rows,specDataEnv[i].localIndex)=ligneOriginale[i];
                    // Si la case était illisble, il faut passer à la prochaine case récupérée
                    if ((caseCourante<nombreRecuperes) && (i==lineValidation[caseCourante]))
                    {
                        ++caseCourante;
                    }
                }


            }


            validation.push_back(listeErreurs);
            ++rows;

            entree >> ws;
        }

        /*		for (int compteur(0); compteur<3;++compteur)
         {
         cout << validation[compteur].size() << endl;
         for (int ccompt(0); ccompt<validation[compteur].size(); ++ccompt)
         {
         cout << validation[compteur][ccompt] << " " ;
         }
         cout << endl;
         }*/

        if (rows < nbPoints)
        {
            std::ostringstream oss;
            oss << "Environmental file contains " << (rows) << " samples, while there should be " <<  nbPoints << ".";
            Erreur e("MSG_tooFewPoints", "Incorrect number of points. "+oss.str());
			journal << e.getPhrase() << nl;
			throw e;
        }



        missingValuesEnv.resize(nbEnvActives, std::set< int > ());
       // cout << missingValuesEnv.size() << "\n";

        for (unsigned int i(0); i < rows; ++i)
        {
            /*	Matrix<double> line(1, nbCols, vals[i].begin());
             dataEnv(i, _) = line(0, 0, 0, nbEnv-1);
             dataMarq(i, _) = line(0, nbEnv, 0, nbCols-1);*/
            // Pour chaque ligne de data, validation contient les colonnes manquantes
            for (int j(0); j<(validation[i].size()); ++j)
            {
                missingValuesEnv[specDataEnv[validation[i][j]].localIndex].insert(i);
            }
        }
        entree.close();


        // Lecture des données génétiques
		journal << "Reading data in " << nomFichierInput[1] << nl;
        entree.open(nomFichierInput[1].c_str());
        entree.precision(toolbox::precisionLecture);
        if (entree.fail())
        {
            Erreur e("MSG_errOpenMark", "Error while opening genetic data file.");
			journal << e.getPhrase() << nl;
			throw e;

        }

        nbCols = headerMarq.size();

        // Lecture des données
        //std::vector<std::vector<double> > vals;
        //std::vector<std::vector< int > > validation;
        //unsigned int rows(0);
        validation.clear();
        rows=0;
        caseCourante=0;
        nombreRecuperes=0;


        // S'il y a un en-tête, on passe la première ligne
        if (entete)
        {
            toolbox::lectureLigne(entree, ligneOriginale, delimMots);
        }


        dataMarq.resize(nbPoints, nbMarqActifs);
        if (nbMarqActifs>0)
        {
            dataMarq=0;
        }
        dataSupMarq.resize(nbPoints, nbMarq-nbMarqActifs);
        if (nbMarq>nbMarqActifs)
        {
            dataSupMarq="";
        }

        while (!entree.eof())
        {
            if (rows%100==0)
            {
                journal  <<"Row " <<rows<< nl;
            }

            line.clear();
            lineValidation.clear();
            listeErreurs.clear();
            caseCourante=0;
            nombreRecuperes=0;
            ligneOriginale.clear();

            toolbox::lectureLigne(entree, line, lineValidation, ligneOriginale, delimMots);



            if(nbCols != line.size())
            {
                std::ostringstream oss;
                oss << "Row " << (rows + 1) << " of markers input file has " << line.size() << " elements, but should have " << nbCols << ".";
                Erreur e("MSG_falseLineSize", oss.str());
				journal << e.getPhrase() << nl;
				throw e;
            }

            if (rows >= nbPoints)
            {
                Erreur e("MSG_tooManyPoints", "Incorrect points number, too many lines in data file.");
				journal << e.getPhrase() << nl;
				throw e;
            }

            // Validation des résultats
            caseCourante=0;
            nombreRecuperes=lineValidation.size();


            for (int i(0); i<nbMarq; ++i)
            {
                // Variable active
                if (specDataMarq[i].isActive)
                {
                    if (caseCourante>=nombreRecuperes || i<lineValidation[caseCourante]) // Cas sans erreur
                    {
                        dataMarq(rows,specDataMarq[i].localIndex)=line[i];
                    }
                    else	// Erreur détectée -> on la note dans la ligne de validation
                    {
                        // Si la variable est active et illisible, on ne s'intéresse pas à la valeur lue
                        listeErreurs.push_back(lineValidation[caseCourante]);
                        ++caseCourante;
                    }

                }
                // Variable passive (string)
                // Pour toutes les variables passives, il suffit de copier la valeur lue (un string)
                else
                {
                    dataSupMarq(rows,specDataMarq[i].localIndex)=ligneOriginale[i];
                    // Si la case était illisble, il faut passer à la prochaine case récupérée
                    if ((caseCourante<nombreRecuperes) && (i==lineValidation[caseCourante]))
                    {
                        ++caseCourante;
                    }
                }

            }
            //cout << dataMarq(rows, _) << endl;

            validation.push_back(listeErreurs);
            ++rows;

            entree >> ws;
        }

        entree.close();



        if (rows < nbPoints)
        {
            std::ostringstream oss;
            oss << "Molecular file contains " << (rows) << " samples, while there should be " <<  nbPoints << ".";
            Erreur e("MSG_tooFewPoints", "Nombre de points incorrect. "+oss.str());
			journal << e.getPhrase() << nl;
			throw e;
        }


        // Réunion des valeurs manquantes
        missingValuesMarq.resize(nbMarqActifs, std::set< int > ());

        for (unsigned int i(0); i < rows; ++i)
        {

            // Pour chaque ligne de data, validation contient les colonnes manquantes
            for (int j(0); j<(validation[i].size()); ++j)
            {
                missingValuesMarq[specDataMarq[validation[i][j]].localIndex].insert(i);
            }
        }

    }

    //Creation des masques pour les variables environnemntales et les marqueurs
    masqueX.resize(nbPoints, nbEnvActives);
    masqueX=1;
    for (int i(0); i<nbEnvActives; ++i)
    {
        //cout << "?" << missingValuesEnv[i].size() << "\n";
        for (set<int>::iterator iter(missingValuesEnv[i].begin()); iter!=missingValuesEnv[i].end(); ++iter)
        {
            masqueX((*iter), i)=0;
        }
    }
//    cout << toolbox::sommeNumerique(masqueX) << " " << nbEnv*nbPoints << "\n";
  
	masqueY.resize(nbPoints, 1);

    //	cout << dataEnv(nbPoints-10,0,nbPoints-1,8) << endl;
    //	cout << dataMarq(nbPoints-10,0,nbPoints-1,9) << endl;

    // FIN DE LA LECTURE DES DONNEES

    // S'il y a deux fichiers d'entrée et des identifiants d'individus, on test pour chaque ligne si les noms correspondent
    if (!uniqueFichierDonnees && existeColID)
    {
        /*cout <<"%"<< colIDEnv << colIDMarq << endl;
         cout << dataSupEnv.rows() << "  "<< dataSupEnv.cols()<< endl;
         cout << dataSupEnv(0,_) << dataSupEnv(1,_) << dataSupEnv(2,_) << endl;
         cout << dataSupMarq(0,_) << dataSupMarq(1,_) << dataSupMarq(2,_) << endl;*/
        for (int i(0); i<nbPoints; ++i)
        {
            //cout << specDataEnv[colIDEnv].localIndex << " & " << dataSupEnv(i, specDataEnv[colIDEnv].localIndex) << endl;
            if (dataSupEnv(i, specDataEnv[colIDEnv].localIndex) != dataSupMarq(i, specDataMarq[colIDMarq].localIndex))
            {
                std::ostringstream oss;
                oss << "IDs do not match on line " << i << " : "
                    << dataSupEnv(i, specDataEnv[colIDEnv].localIndex) << " != " << dataSupMarq(i, specDataMarq[colIDMarq].localIndex);
                Erreur e("MSG_unmatchIDs", oss.str());
				journal << e.getPhrase() << nl;
				throw e;
            }
        }

    }


    return 0;
}


void RegressionLogistique::ecritResultat(int numFichier, const resModele& r) const
{
    //On écrit les numéros/noms globaux des marqueurs
    // Le décalage du numéro de marqueur dans les jobs multiples a déjà été pris en compte

    // No de marqueur
    sortie.ecriture(numFichier, specDataMarq[marqActifs.at(r.first.first)].name, false);

    // Liste des variables
    for (set<int>::iterator i(r.first.second.begin()); i!=r.first.second.end(); ++i)
    {
        sortie.ecriture(numFichier, specDataEnv[varEnvActives.at(*i)].name, false);
    }
    // Résultats
    sortie.ecriture(numFichier, r.second, true);

}

void RegressionLogistique::dumpJournalTemporaire(JournalTemporaire& journalTemp, const string& erreur)
{
    journalTemp.synchronise();
    if (!erreur.empty())
    {
        journalTemp << erreur << nl;
    }
    string nomFichierDump("Sambada-error-log-"+toolbox::timestamp()+".txt");
    FluxSortie errorLog;
    errorLog.setNomFichier(nomFichierDump);
    errorLog.ouvertureFichier();
    errorLog << journalTemp << flush;
    errorLog.fermetureFichier();
}

void RegressionLogistique::dumpJournalTemporaire(JournalTemporaire& journalTemp, const Erreur& e)
{
	string erreur(e.getPhrase());
	dumpJournalTemporaire(journalTemp, erreur);
}

vector<string> RegressionLogistique::messageBienvenue(bool versionLongue)
{
    vector<string> message;
    message.push_back("| Welcome to Sambada!");
    message.push_back("| Copyright (C) 2011-2014 EPFL (Ecole Polytechnique federale de Lausanne)");
    message.push_back("| Laboratory of Geographic information systems (LaSIG)");
    if (!versionLongue)
    {

        message.push_back("| Contact: sylvie.stucki@a3.epfl.ch, stephane.joost@epfl.ch");
        message.push_back("| Sambada is free software and comes with ABSOLUTELY NO WARRANTY.");
        message.push_back("| Please refer to the file AUTHORS for details.");
    }
    else
    {
        message.push_back("| Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.");
        message.push_back("| Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.");
        message.push_back("| You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.");
        message.push_back("|");

        message.push_back("| Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), Stephane Joost (stephane.joost@epfl.ch)");
        message.push_back("| Laboratory of Geographic information systems");
        message.push_back("| EPFL ENAC IIE LASIG");
        message.push_back("| Station 18");
        message.push_back("| CH-1015 Lausanne");
        message.push_back("| Web site : http://lasig.epfl.ch/sambada");
        message.push_back("|");

        message.push_back("| Sambada includes two libraries: Scythe Statistical Library (under GPL 3) and Shapefile C Library (under LGPL 2.1, courtesy of Frank Warmerdam).");
        message.push_back("|");

        message.push_back("| Scythe Statistical Library");
        message.push_back("| Copyright (C) 2000-2002 Andrew D. Martin and Kevin M. Quinn;");
        message.push_back("| 2002-2012 Andrew D. Martin, Kevin M. Quinn, and Daniel Pemstein.  All Rights Reserved.");
        message.push_back("|");

        message.push_back("| Shapefile C Library");
        message.push_back("| Copyright (c) 1999, Frank Warmerdam");
    }
	return message;
}


ostream& RegressionLogistique::messageBienvenue(ostream& out, bool versionLongue)
{
    vector<string> message(messageBienvenue(versionLongue));
    for (int i(0); i<message.size(); ++i)
    {
        cout << message[i] << "\n";
    }
    cout << flush;
}

JournalTemporaire& RegressionLogistique::messageBienvenue(JournalTemporaire& jt, bool versionLongue)
{
    vector<string> message(messageBienvenue(versionLongue));
    jt.synchronise();
    for (int i(0); i<message.size(); ++i)
    {
        jt << message[i] << nl;
    }
}



/*
 void RegressionLogistique::ecritResultats() const
 {
 if (resultats.size()>0)
 {
 for (int i(0); i<=dimensionMax; ++i)
 {
 //sortie << "Dimension = " << i << "\n";
 for (groupeResultats::const_iterator fleche=(resultats[i]).begin(); fleche!=(resultats[i]).end(); ++fleche)
 {
 
 //On écrit les numéros/noms globaux des marqueurs
 // Le décalage du numéro de marqueur dans les jobs multiples a déjà été pris en compte
 
 // No de marqueur
 sortie.ecriture(i, specDataMarq[marqActifs.at(fleche->first.first)].name, false);
 
 // Liste des variables
 for (set<int>::iterator iter(fleche->first.second.begin()); iter!=fleche->first.second.end(); ++iter)
 {
 //cout << *iter << " " << varEnvActives.size() << "\n";
 sortie.ecriture(i, specDataEnv[varEnvActives.at(*iter)].name, false);
 }
 
 // Résultats
 sortie.ecriture(i, fleche->second, true);
 
 }
 }
 }
 }
 
 
 // Ecrit les résultats dans un fichier (ou dans la console)
 void RegressionLogistique::ecritResultats(string nomFichier) const
 {
 ofstream sortie(nomFichier.c_str());
 if (sortie.fail())
 {
 ecritResultats(cout);
 }
 else
 {
 ecritResultats(sortie);
 }
 sortie.close();
 }
 
 // Ecrit les résultats dans un flot
 ostream& RegressionLogistique::ecritResultats(ostream& sortie) const
 {
 // Ecriture des noms de colonnes pour s'y repérer
 vector< vector<string> > names(3);
 names[0].push_back("Marker");
 names[0].push_back("Env_");
 names[0].push_back("Beta_");
 names[1].push_back("Loglikelihood");
 names[1].push_back("AverageProb");
 names[1].push_back("Beta_0");
 names[1].push_back("NumError");
 names[2].push_back("Loglikelihood");
 names[2].push_back("Gscore");
 names[2].push_back("WaldScore");
 names[2].push_back("NumError");
 names[2].push_back("Efron");
 names[2].push_back("McFadden");
 names[2].push_back("McFaddenAdj");
 names[2].push_back("CoxSnell");
 names[2].push_back("Nagelkerke");
 names[2].push_back("AIC");
 names[2].push_back("BIC");
 
 if (resultats.size()>0)
 {
 int precisionRes(toolbox::precisionLecture), anciennePrecision(sortie.precision());;
 sortie.precision(precisionRes);
 for (int i(0); i<=dimensionMax;++i)
 {
 sortie << "Dimension = " << i << delimLignes;
 if (i==0)
 {
 sortie << names[0][0] << delimMots;
 for (int j(0); j< names[1].size(); ++j)
 {
 sortie << names[1][j]<< delimMots;
 }
 sortie << delimLignes;
 }
 else
 {
 sortie << names[0][0] << delimMots;
 for (int j(1); j<=i; ++j)
 {
 sortie << names[0][1]+toolbox::conversion(j) << delimMots;
 }
 for (int j(0); j< names[2].size(); ++j)
 {
 sortie << names[2][j]<< delimMots;
 }
 for (int j(0); j<=i; ++j)
 {
 sortie << names[0][2]+toolbox::conversion(j) << delimMots;
 }
 sortie << delimLignes;
 
 }
 
 
 for (groupeResultats::const_iterator fleche=(resultats[i]).begin(); fleche!=(resultats[i]).end(); ++fleche)
 {
 // No de marqueur
 sortie << specDataMarq[marqActifs.at(fleche->first.first)].name << delimMots;
 
 // Liste des variables
 for (set<int>::const_iterator set_iter(fleche->first.second.begin()); set_iter!=fleche->first.second.end(); ++set_iter)
 {
 sortie << specDataEnv[varEnvActives.at(*set_iter)].name << delimMots;
 }
 // Résultats
 int nbRes(fleche->second.size());
 for (int j(0); j<nbRes; ++j)
 {
 sortie << fleche->second[j] << delimMots;
 }
 sortie << delimLignes;
 }
 sortie << delimLignes;
 }
 cout << sortie.precision() << delimLignes;
 sortie.precision(anciennePrecision);
 }
 return sortie;
 }
 */
void RegressionLogistique::affiche(const etiquetteModele& label)
{
    cout << label.first << " : ";
    for (set<int>::iterator i(label.second.begin()); i!=label.second.end(); ++i)
    {
        cout << *i << " ";
    }
    cout << "\n";
}

void RegressionLogistique::affiche(const resModele& res)
{
    affiche(res.first);
    cout << "    ";
    for (int i(0); i<res.second.size(); ++i)
    {
        cout << res.second[i] << " " ;
    }
    cout << "\n";
}

void RegressionLogistique::affiche(const groupeResultats::iterator res)
{
    affiche(res->first);
    cout << "    (" << res->second.size() << ") "<< flush;
    for (int i(0); i<res->second.size(); ++i)
    {
        cout << res->second[i] << " " ;
    }
    cout << "\n";
}




void RegressionLogistique::trieEtEcritResultats()
{
    cout << "Coucou"<< "\n";
    vector<  groupeResultats::value_type*  > listeModeles(0);
    int tailleListe(0);
    // On ne trie pas les modèles constants
    // On ne peut pas trier une map! Il faut trier un vecteur

    // Vu la structure des résultats, la valeur du Gscore est la colonne 1 (loglike=0, Gscore=1...)
    // On fait le tri sur le score Wald -> case=2
    ComparaisonResultats::setCase(2);


    if (resultats.size()>0)
    {
        for (int i(0); i<=dimensionMax; ++i)
        {
            //sortie << "Dimension = " << i << "\n";
            tailleListe=resultats[i].size();
            listeModeles.resize(tailleListe);
            int position(0);
            for (groupeResultats::iterator iter_set(resultats[i].begin()); iter_set!=resultats[i].end(); ++iter_set)
            {
                listeModeles[position]=&(*iter_set);
                ++position;
            }

            // On ne trie pas les modèles constants
            if (i>0)
            {
                sort(listeModeles.begin(), listeModeles.end(), ComparaisonResultats::plusGrandQue);
            }

            //On écrit les numéros/noms globaux des marqueurs
            // Le décalage du numéro de marqueur dans les jobs multiples a déjà été pris en compte

            //for (groupeResultats::const_iterator fleche=(resultats[i]).begin(); fleche!=(resultats[i]).end(); ++fleche)
            for (int j(0); j<tailleListe; ++j)
            {
                if (listeModeles[j]->second[validiteModele]==0 || (listeModeles[j]->second[validiteModele]==6 && selModeles==signif) || selModeles==all)
                {

                    // No de marqueur
                    sortie.ecriture(i, specDataMarq[marqActifs.at(listeModeles[j]->first.first)].name, false);

                    // Liste des variables
                    for (set<int>::iterator iter(listeModeles[j]->first.second.begin()); iter!=listeModeles[j]->first.second.end(); ++iter)
                    {
                        //cout << *iter << " " << varEnvActives.size() << "\n";
                        sortie.ecriture(i, specDataEnv[varEnvActives.at(*iter)].name, false);

                        //cout << specDataEnv[varEnvActives.at(*iter)].name<< " ";

                    }

                    // Résultats
                    sortie.ecriture(i, listeModeles[j]->second, true);
                    //cout << listeModeles[j]->second[0] << " " << listeModeles[j]->second[1] << " " << listeModeles[j]->second[2] << " " << listeModeles[j]->second[3] << " " << endl;
                }

            }
        }
    }
}	

// Cette méthode lit le fichier de paramètres et remplit la liste
// Elle vérifie aussi si les paramètres obligatoires (et les pré-requis) sont présents
ifstream& RegressionLogistique::lectureParametres(ifstream& entree, const ParameterSetIndex& index, ParameterSet& parametres, JournalTemporaire& journalTemp) throw()
{
    entree >> ws;
    string nomParam, lu;
    vector<string> ligne;
    ParameterSetIndex::const_iterator paramCourant; // L'itérateur doit pointer une valeur constante (index est const)
	
	string filler("    ");	// Indentation for parameters in the log
	journalTemp << "Reading parameter file..." << nl;
    while(!entree.eof())
    {
        toolbox::lectureLigne(entree, ligne, ' ', true);
        if (ligne.size()==0)	// Vérification de la taille de ligne
        {
            continue;
        }
		
		// Une ligne valide doit commencer par le nom d'un tag
        nomParam=ligne[0];
        if (!isupper(nomParam[0]))
        {
            journalTemp << "This ligne was ignored: ";
        }
		journalTemp << nomParam << nl; 
        for (int i(1); i<ligne.size(); ++i)
        {
            journalTemp << filler << ligne[i]  << nl;
        }
        if (!isupper(nomParam[0]))
        {
            continue;
        }
		
        paramCourant=index.find(nomParam);
        if (paramCourant==index.end())
        {
			Erreur e("MSG_unknownParam", "Unknown parameter : '"+nomParam+"'");
			journal << e.getPhrase() << nl;
			throw e;
        }
        else
        {
            /*cout << nomParam << " " << parametres[paramCourant->second].name << "\n";*/
            // Le paramètre existe
            parametres[paramCourant->second].present=true;
            parametres[paramCourant->second].contents.insert(parametres[paramCourant->second].contents.end(),ligne.begin()+1, ligne.end());
            entree >> ws;
            /*cout << parametres[paramCourant->second].contents.size() << "\n";
            for (int compteur(0); compteur<parametres[paramCourant->second].contents.size(); ++compteur)
            {
                cout << parametres[paramCourant->second].contents[compteur] << "\n";
            }*/
        }
    }
    // Vérification des paramètres obligatoires et des prérequis
    int nbParam(parametres.size()), nbPrereq(0);
    for (int i(0); i<nbParam; ++i)
    {
        if (parametres[i].mandatory && (!parametres[i].present || parametres[i].contents.size()==0))
        {
			Erreur e("MSG_mandParam", "Parameter "+parametres[i].name+" is mandatory ! ");
			journal << e.getPhrase() << nl;
			throw e;
			
        }
        nbPrereq=parametres[i].prereq.size();
        for (int j(0); j<nbPrereq; ++j)
        {
            paramCourant=index.find(parametres[i].prereq[j]);
            if (parametres[i].present && !parametres[paramCourant->second].present) // Cas où le paramètre est présent et où un de ses pré-requis manque
            {
				Erreur e("MSG_reqParam", "Parameter "+parametres[paramCourant->second].name+" is required by "+parametres[i].name);
				journal << e.getPhrase() << nl;
				throw e;
				
            }
        }
    }

    return entree;
}
