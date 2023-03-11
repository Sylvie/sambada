/*************************************************************************
 * Copyright (©) 2011-2023 EPFL (Ecole Polytechnique fédérale de Lausanne)
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


#include "RegressionLogistique.hpp"
#include "Chronometre.hpp"
#include "optimize.h"
#include <limits>
#include <list>
#include <ctime>
#include <histograms/StoreyHistogramsStreamWriter.hpp>

using namespace std;
using namespace scythe;
using namespace sambada;


RegressionLogistique::RegressionLogistique()
		: dataEnv(0, 0), missingValuesEnv(0), dataMarq(0, 0), missingValuesMarq(0), existeColID(false), /*headerEnv(0), headerMarq(0),*/
		  sauvegardeTempsReel(true), selModeles(all),
		  analyseSpatiale(false), longitude(0), latitude(0), choixPonderation(pondDistanceMax), bandePassante(0), AS_nbPermutations(0), nbPlusProchesVoisins(0),
		  eps(sqrt(epsilon<reel>())), convCrit(1e-6), seuilPValeur(0.01), seuilScore(0), seuilScoreMultivarie(0), limiteNaN(1000000), limiteExp(min((reel) 700, log(numeric_limits<reel>::max() / 2))), nbModelesParMarqueur(1),
		  limiteIter(100), limiteEcartType(7), nbPseudosRcarres(7), nbStats(11), nbStatsAvecPop(13), nbStatsSansPseudos(4),
		  numPremierMarq(0), tailleEtiquetteInvar(4),
		  storey(nullptr),
		  sortie(flotSortieFichierFactory), delimLignes("\n"), delimMots(' '),
		  AS_GWR(false), AS_autocorrGlobale(false), AS_autocorrLocale(false), AS_autocorrVarEnv(false), AS_autocorrMarq(false), AS_shapefile(false),
		  AS_spatialLag(false)
{
}

RegressionLogistique::~RegressionLogistique()
{}

int RegressionLogistique::calculeCorrelations() const
{
	int nbPaires(nbEnv * (nbEnv - 1) / 2.);
	Matrix<> taillesDonnees(toolbox::sommeNumeriqueCol(masqueX));

	// Calcul des moyennes et variances
	Matrix<> moyennes(sumc(dataEnv)), variances(sumc(dataEnv % dataEnv));
	moyennes /= taillesDonnees;
	variances = variances / taillesDonnees - moyennes % moyennes;

	vector<vector<reel> > correlations(0);
	vector<reel> correlationCourante(3);

	for (int i(0); i < nbEnv; ++i)
	{
		correlationCourante[0] = i;
		for (int j(i + 1); j < nbEnv; ++j)
		{
			correlationCourante[1] = j;
			correlationCourante[2] = (sum(dataEnv(_, i) % dataEnv(_, j)) / toolbox::sommeNumerique(masqueX(_, i) % masqueX(_, j)) - moyennes[i] * moyennes[j]) / sqrt(variances[i] * variances[j]);
			correlations.push_back(correlationCourante);
		}
	}

	for (int i(0); i < nbPaires; ++i)
	{
		cout << specDataEnv[correlations[i][0]].name << "-" << specDataEnv[correlations[i][1]].name << " ";
	}
	cout << endl;
	for (int i(0); i < nbPaires; ++i)
	{
		cout << correlations[i][2] << " ";
	}
	cout << endl << CLOCKS_PER_SEC << endl;

	ComparaisonVecteurs::setCase(0);
	sort(correlations.begin(), correlations.end(), ComparaisonVecteurs::plusPetitQue<reel>);

	cout << correlations.size() << " " << nbPaires << "\n";
	for (int i(0); i < nbPaires; ++i)
	{
		//		cout << correlations[i][0] << " " << correlations[i][1] << " " << correlations[i][2] << "\n";
	}
	return 0;
}

int RegressionLogistique::creeModelesGlobaux()
{
	journal << "Detection of selection signatures" << nl;
	journal << "Highest dimension : " << dimensionMax << nl;
	journal << "List of possible errors : " << nl
	        << "1: exponential divergence" << nl
	        << "2: J_info is singular" << nl
	        << "3: divergence of beta" << nl
	        << "4: maximum number of iterations" << nl
	        << "5: monomorphic marker" << nl
	        << "6: non-significant parents models" << nl;


	// Initialisation du conteneur de résultats et des flots de sortie
	resultats.clear();
	resultats.resize(dimensionMax + 1);
	vector<string> nomsFichiers(dimensionMax + 1);
	for (int i(0); i <= dimensionMax; ++i)
	{
		ostringstream oss;
		oss << i;
		nomsFichiers[i] = (nomFichierResultats.first) + "-Out-" + oss.str() + (nomFichierResultats.second);
	}

	sortie.initialise(nomsFichiers, delimLignes, delimMots, toolbox::precisionLecture);

	// Ecriture des noms de colonnes pour s'y repérer
	vector<vector<string> > names(4);
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

	names[3].push_back("GscorePop");
	names[3].push_back("WaldScorePop");

	sortie.ecriture(0, names[0][0]);
	sortie.ecriture(0, names[1], true);

	for (int i(1); i <= dimensionMax; ++i)
	{
		sortie.ecriture(i, names[0][0]);
		for (int j(1); j <= i; ++j)
		{
			sortie.ecriture(i, names[0][1] + toolbox::conversion(j));
		}
		if (!calculeStructurePop(i))
		{
			sortie.ecriture(i, names[2]);
		}
		else
		{
			sortie.ecriture(i, names[2], false);
			sortie.ecriture(i, names[3]);
		}
		for (int j(0); j < i; ++j)
		{
			sortie.ecriture(i, names[0][2] + toolbox::conversion(j));
		}
		sortie.ecriture(i, names[0][2] + toolbox::conversion(i), true);
		//sortie.ecriture(i, "", true);
	}


	// Variables locales
	// int colMarq(0);
	reel tailleY(0), sommeY(0); //, tailleX(0), beta_zero(0), val_loglikelihood_zero(0);
	// pseudosRcarresCourants(nbPseudosRcarres), statsCourantes(nbStats), betaCourant(0); */
	vector<reel> loglikelihood(dimensionMax + 1, 0.0);//, resultatsCourants(tailleEtiquetteInvar, 0.0); // Il y a le modèle sans paramètres!

	Modele resultatCourant;
	resultatCourant.second.resize(tailleEtiquetteInvar, 0.0);

	// Ces matrices ne changent pas de taille
	Y.resize(nbPoints, 1);
	Xb.resize(nbPoints, 1);
	nouv_Xb.resize(nbPoints, 1);
	exp_Xb.resize(nbPoints, 1);
	pi_hat.resize(nbPoints, 1);
	interm.resize(nbPoints, 1);
	intermScores.resize(nbPoints, 1);

	Domaine pointsTot;//, pointsMarq, pointsCourants;

	if (AS_GWR)
	{
		if (!AS_autocorrVarEnv && !AS_autocorrMarq)
		{
			calculePonderation();
		}
		// Calcul de la pondération type si aucun marqueur ou variable env ne manque
		// La pondération géo prend en compte le point lui-même
		// On fait le même calcul quel que soit le type de pondération (y compris les plus proches voisins)
		pointsTot = pointsGeo;
		reel sommePond(0);
		int nbVoisins(0);
		for (int i(0); i < nbPoints; ++i)
		{
			if (pointsTot.masque(i, 0))
			{
				// Si un point est valide, tous ses voisins sont considérés, vu qu'on ne compte que le masque géo
				nbVoisins = pointsTot.poids[i].size();
				for (int j(0); j < nbVoisins; ++j)
				{
					sommePond += pointsTot.poids[i][j].second;
				}
				sommePond += 1.; // Pour le point i
				for (int j(0); j < nbVoisins; ++j)
				{
					pointsTot.poids[i][j].second /= sommePond;
				}
				pointsTot.poids[i].push_back(make_pair(i, 1. / sommePond));
				//ponderationGeo[i].push_back(make_pair(i, 1.));

			}
		}

		// On redimensionne aussi les matrices locales
		// Ces matrices ne changent pas de taille
		Y_l.resize(nbPoints, 1);
		nouv_Xb_l.resize(nbPoints, 1);
		Xb_l.resize(nbPoints, 1);
		exp_Xb_l.resize(nbPoints, 1);
		pi_hat_l.resize(nbPoints, 1);
		interm_l.resize(nbPoints, 1);
		intermScores_l.resize(nbPoints, 1);

	}

	storey = std::unique_ptr<sambada::StoreyHistograms>(new sambada::StoreyHistograms(dimensionMax));

	//	clock_t t1, t2;


	// Mesure du temps de calcul
	Chronometre chrono;
	int prochaineMesure(chrono.initialisation(&journal, nbMarqActifs, ceil(1000. / nbModelesParMarqueur), " | "));

	// On ne prend en compte que les marqueurs actifs
	for (int i(0); i < nbMarqActifs; ++i)
	{
		if (sauvegardeTempsReel)
		{
			resultats.clear();
			resultats.resize(dimensionMax + 1);
		}
		if (i == prochaineMesure)
		{
			prochaineMesure = chrono.mesureEtAffiche();
		}
		//colMarq = nbEnv+i;
		resultatCourant.first.marqueur = i;
		resultatCourant.first.environnement.clear();

		// Gestion des valeurs manquantes -> On ne prend que les points dont toutes les valeurs sont connues
		// Remplissage du masque
		masqueY = true;
		// Les valeurs manquantes sont un ensemble!
		for (set<int>::iterator iter(missingValuesMarq[i].begin()); iter != missingValuesMarq[i].end(); ++iter)
		{
			masqueY[*iter] = false;
		}

		sommeY = toolbox::sommeNumerique(dataMarq(_, i));
		tailleY = toolbox::sommeNumerique(masqueY);
		if (sommeY == 0 || tailleY == 0 || sommeY == tailleY)    // si tous les marqueurs sont nuls ou manquants ou si tous les marqueurs non-nuls sont égaux à 1
		{
			// On veut afficher le numéro global du marqueur
			modelesDivergents << 5 << " | " << i + numPremierMarq << " : " << nl;
			//journal << "Y[" << marqActifs[i]+numPremierMarq << "] is constant ! " << dataMarq(0,i) << nl;
			// Paramètres du modèle
			resultatCourant.second[1] = (sommeY > 0 ? 1 : 0);
			resultatCourant.second[0] = 0;
			resultatCourant.second[2] = 0;
			resultatCourant.second[3] = 5;
			resultats[0].insert(resultatCourant);
			if (sauvegardeTempsReel)
			{
				ecritResultat(0, resultatCourant);
			}
			continue;
		}

		// Calcul du modèle avec seulement la constante

		// beta_zero =log((double)sommeY / (tailleY - sommeY)); // beta_zero = ln( Y / (N-Y))
		// val_loglikelihood_zero = sommeY*beta_zero - tailleY*log(tailleY / (tailleY - sommeY));
		// loglikelihood_zero[i] = val_loglikelihood_zero;
		// Probabilité
		resultatCourant.second[1] = (sommeY / tailleY);
		// beta_zero
		resultatCourant.second[2] = log((double) sommeY / (tailleY - sommeY)); // beta_zero = ln( Y / (N-Y))
		// loglikelihood
		resultatCourant.second[0] = sommeY * resultatCourant.second[2] - tailleY * log(tailleY / (tailleY - sommeY));
		// Pas d'erreur détectée
		resultatCourant.second[3] = 0;

		resultats[0].insert(resultatCourant);

		if (sauvegardeTempsReel)
		{
			ecritResultat(0, resultatCourant);
		}

		// Copie de la loglikelihood dans le tableau
		loglikelihood[0] = resultatCourant.second[1];

		if (nbEnvActives == 0 || dimensionMax == 0)
		{
			continue;
		}

		// Pour toutes les combinaisons de paramètres environnementaux

		// On commence par les modèles univariés
		//for (set<int>::iterator variableCourante(varActives.begin()); variableCourante!=varActives.end(); ++variableCourante)
		int nbParam(2);
		beta_hat.resize(nbParam, 1);
		nouv_beta_hat.resize(nbParam, 1);
		diff_beta_hat.resize(nbParam, 1);
		scores.resize(nbParam, 1);
		J_info.resize(nbParam, nbParam);
		inv_J_info.resize(nbParam, nbParam);
		X.resize(nbPoints, nbParam);

		// Sauvegarde des paramètres globaux si calcul de modèles locaux -> redimensionnement
		if (AS_GWR)
		{
			beta_hat_l.resize(nbParam, 1);
			nouv_beta_hat_l.resize(nbParam, 1);
			diff_beta_hat_l.resize(nbParam, 1);
			scores_l.resize(nbParam, 1);
			J_info_l.resize(nbParam, nbParam);
			inv_J_info_l.resize(nbParam, nbParam);
			X_l.resize(nbPoints, nbParam);

			// Calcul de la pondération type si masqueGeoMarq=masque (X*Y)
			pointsMarq.masque = pointsTot.masque % masqueY;
			pointsMarq.taille = toolbox::sommeNumerique(pointsMarq.masque);
			if (pointsMarq.taille == pointsTot.taille)    // Si on n'a pas de nouvelles valeurs manquantes (par rapport à masqueGeo)
			{
				pointsMarq = pointsTot;
			}
			else
			{
				pointsMarq.miseAJour();

				reel sommePond(0);
				int nbVoisins(0);
				if (choixPonderation != pondPlusProchesVoisins)
				{
					for (int i(0); i < nbPoints; ++i)
					{
						if (pointsMarq.masque(i, 0))
						{
							nbVoisins = pointsTot.poids[i].size();
							for (int j(0); j < nbVoisins; ++j)
							{
								if (pointsMarq.masque(j, 0))
								{
									sommePond += pointsTot.poids[i][j].second;
									pointsMarq.poids[i].push_back(pointsTot.poids[i][j]);
								}
							}
							sommePond += 1.; // Pour le point i
							nbVoisins = pointsMarq.poids[i].size();

							for (int j(0); j < nbVoisins; ++j)
							{
								pointsMarq.poids[i][j].second /= sommePond;
							}
							pointsMarq.poids[i].push_back(make_pair(i, 1. / sommePond));
						}
					}
				}
				else    // Cas plus proches voisins, on reprend la liste du voisinage
				{

					// Cas où tous les points valides sont voisins
					if (pointsMarq.taille <= (nbPlusProchesVoisins + 1))
					{
						double poids(1. / pointsMarq.taille);
						for (int i(0); i < nbPoints; ++i)
						{
							if (pointsMarq.masque(i, 0))
							{

								pointsMarq.poids[i].push_back(make_pair(i, poids));

								for (int j(i + 1); j < nbPoints; ++j)
								{
									if (pointsMarq.masque(j, 0))
									{
										pointsMarq.poids[i].push_back(make_pair(j, poids));
										pointsMarq.poids[j].push_back(make_pair(i, poids));
									}
								}
							}
						}
					}
					else
					{
						// On sélectionne les points voisins
						// Il peut y en avoir plus que "nbPlusProchesVoisins" si certains points sont à égale distance

						int nbVoisinsCourants(0);
						vector<Voisin>::iterator voisinCourant, voisinSuivant;

						reel plusPetiteDiffDist(pow(10., -4)); // En dessous d' 1cm ça compte pas! (Les distances sont au carré)
						for (int i(0); i < nbPoints; ++i)
						{
							if (pointsMarq.masque(i, 0))
							{
								nbVoisinsCourants = 0;
								pointsMarq.poids[i].push_back(make_pair(i, 1));
								voisinCourant = voisinage[i].begin();
								while ((nbVoisinsCourants < nbPlusProchesVoisins) && (voisinCourant != voisinage[i].end()))
								{
									if (pointsMarq.masque(voisinCourant->first, 0))
									{
										pointsMarq.poids[i].push_back(make_pair(voisinCourant->first, 1));
										++voisinCourant;
										++nbVoisinsCourants;
									}
								}
								// On cherche si des points sont à la même distance que le dernier
								voisinSuivant = voisinCourant + 1;
								while ((voisinSuivant != voisinage[i].end()) && abs((voisinSuivant)->second - voisinCourant->second) < plusPetiteDiffDist)
								{
									if (pointsMarq.masque(voisinSuivant->first, 0))
									{
										pointsMarq.poids[i].push_back(make_pair(voisinSuivant->first, 1));

										++voisinCourant;
										++voisinSuivant;
										++nbVoisinsCourants;
									}
								}
								sommePond = nbVoisinsCourants + 1; // +1 pour le point lui-même
								double poids(1. / sommePond);
								for (int j(0); j < sommePond; ++j)
								{
									pointsMarq.poids[i][j].second = poids;
								}
							}
						}
					}


				}

			}
		}

		for (sambada::GenerationVariables::const_iterator variableCourante(familleVariables[1].cbegin()); variableCourante != familleVariables[1].cend(); ++variableCourante)
		{
			construitModele(i, variableCourante->first);
		}

		// Calcul des modèles multivariés

		// Ici la dim est celle de la génération courante (le cas dim=1 est traité avant)
		for (int dim(2); dim <= dimensionMax; ++dim)
		{
			// On peut déjà redimensionner les matrices pour les calculs
			// On connaît la taille de l'échantillon pour la regression -> construction des matrices
			// Matrice des paramètres
			nbParam = dim + 1;
			beta_hat.resize(nbParam, 1);
			nouv_beta_hat.resize(nbParam, 1);
			diff_beta_hat.resize(nbParam, 1);
			scores.resize(nbParam, 1);
			J_info.resize(nbParam, nbParam);
			inv_J_info.resize(nbParam, nbParam);

			X.resize(nbPoints, nbParam);


			beta_hat_l.resize(nbParam, 1);
			nouv_beta_hat_l.resize(nbParam, 1);
			diff_beta_hat_l.resize(nbParam, 1);
			scores_l.resize(nbParam, 1);
			J_info_l.resize(nbParam, nbParam);
			inv_J_info_l.resize(nbParam, nbParam);

			X_l.resize(nbPoints, nbParam);



			// Parcours des modèles de dimension dim
			for (sambada::GenerationVariables::const_iterator generationCourante(familleVariables[dim].cbegin()); (generationCourante != familleVariables[dim].cend()); ++generationCourante)
			{
				//if (dim < (dimensionMax - 2) || (structurePop == pasStructurePop) || (inclutToutesVariablesPop(varContinues)))
				if (estModeleEligiblePourStructurePopulation(generationCourante->first))
				{
					construitModele(i, generationCourante->first);
				}
			}
		}
	}

	chrono.fin();

	if (!sauvegardeTempsReel)
	{
		// Si on fait la sauvegarde à la fin, on prend le temps de trier les résultats
		trieEtEcritResultats();
	}

	if (calculeStorey)
	{
		// Storey
		std::vector<int> nbModelesValides(storey->getNumValidModels());
		for (int i(1); i < (dimensionMax + 1); ++i)
		{
			cout << "Nombre de modèles valides (Storey) dim=" << i << " : " << nbModelesValides[i] << "\n";
		}
		//	ofstream sortieStorey("res-Storey.txt");
		ofstream sortieStorey((nomFichierResultats.first + "-storey" + nomFichierResultats.second).c_str());
		sambada::StoreyHistogramsStreamWriter writer;
		writer.write(*storey, sortieStorey, structurePop != pasStructurePop, delimMots);

		sortieStorey.close();
	}

	return 0;
}


void RegressionLogistique::construitModele(int numMarq, const set<int>& varContinues)//, const reel loglike_zero, reel& loglike_courante)
{
	int nbVarCont(varContinues.size());
	int dim(nbVarCont);

	if (dim == 0)
	{
		return;
	}

	Modele resultat;
	resultat.first.marqueur = numMarq;
	resultat.first.environnement = varContinues;

	sambada::CombinaisonVariables combinaisonVariables(familleVariables[varContinues.size()][varContinues]);

	masque = masqueY % combinaisonVariables.masque;

	taille = toolbox::sommeNumerique(masque);
	double somme(toolbox::sommeNumerique(dataMarq(_, numMarq) % masque));

	if (somme == taille || somme == 0)
	{
		// Y est constant!
		resultat.second.resize(nbStatsSansPseudos, 0); // loglike, Gscore et Wald -> Model non-significatif
		resultat.second[validiteModele] = 5;
		if (somme == 0)
		{
			resultat.second.push_back(0); // prob
		}
		else
		{
			resultat.second.push_back(1);
		}

		if (selModeles == all)
		{
			if (!appliqueSeuilScoreStorey ||
			    (dim < dimensionMax) ||
			    dim == dimensionMax && structurePop != pasStructurePop && (resultat.second[GscorePop] >= seuilScoreStorey || resultat.second[WaldScorePop] >= seuilScoreStorey) ||
			    dim == dimensionMax && structurePop == pasStructurePop && (resultat.second[Gscore] >= seuilScoreStorey || resultat.second[WaldScore] >= seuilScoreStorey)
					)
			{
				resultats[dim].insert(resultat);
			}
			// Cas où on sauvegarde les résultats au fur et à mesure
			if (sauvegardeTempsReel)
			{
				// Test storey
				if (!appliqueSeuilScoreStorey ||
				    (dim < dimensionMax && (resultat.second[Gscore] >= seuilScoreStorey || resultat.second[WaldScore] >= seuilScoreStorey)) ||
				    dim == dimensionMax && structurePop != pasStructurePop && (resultat.second[GscorePop] >= seuilScoreStorey || resultat.second[WaldScorePop] >= seuilScoreStorey) ||
				    dim == dimensionMax && structurePop == pasStructurePop && (resultat.second[Gscore] >= seuilScoreStorey || resultat.second[WaldScore] >= seuilScoreStorey)
						)
				{
					ecritResultat(dim, resultat);
				}

				/*
				 // No de marqueur
				 sortie.ecriture(dim, resultat.first.first, false);

				 // Liste des variables
				 sortie.ecriture(dim, resultat.first.second, false);

				 // Résultats
				 sortie.ecriture(dim, resultat.second, true);
				 */
			}

		}
	}
	else
	{
		nbParam = dim + 1;
		resultat.second.resize(nbStats + nbParam);

		if (calculeStructurePop(dim))
		{
			resultat.second.resize(nbStatsAvecPop + nbParam);
		}
		// On connaît la taille de l'échantillon pour la regression -> construction des matrices
		// Matrice des paramètres
		/*beta_hat.resize(nbParam, 1);
		 nouv_beta_hat.resize(nbParam, 1);
		 diff_beta_hat.resize(nbParam, 1);
		 scores.resize(nbParam, 1);
		 J_info.resize(nbParam, nbParam);
		 inv_J_info.resize(nbParam, nbParam);



		 X.resize(taille, nbParam);
		 X(_, 0) =1; // Initialisation à 1 -> constante ok
		 Y.resize(taille, 1);
		 Xb.resize(taille, 1);
		 exp_Xb.resize(taille, 1);
		 pi_hat.resize(taille, 1);
		 interm.resize(taille, 1);
		 intermScores.resize(taille, 1);*/

		X(_, 0) = 1; // Initialisation à 1 -> constante ok
		if (taille < nbPoints)
		{
			X(taille, 0, nbPoints - 1, nbParam - 1) = 0;
			Y(taille, 0, nbPoints - 1, 0) = 0;
		}
		// Copie des valeurs dans la 2e colonne de X et dans Y
		int position(0);
		for (int k(0); k < nbPoints; ++k)
		{
			if (masque(k, 0) == true)
			{
				set<int>::iterator iter(varContinues.begin());
				for (int l(0); l < nbVarCont; ++l)
				{
					X(position, l + 1) = dataEnv(k, *iter);
					++iter;
				}
				Y(position, 0) = dataMarq(k, numMarq);
				++position;
			}

		}

		bool modeleRetenu(false);
		if (sum(Y) == taille || sum(Y) == 0)
		{
			// Y est constant!
			resultat.second.resize(nbStatsSansPseudos, 0); // loglike, Gscore et Wald -> Model non-significatif
			resultat.second[validiteModele] = 5;
			resultat.second.push_back(Y(0, 0)); // prob
			resultat.second.push_back(1);
			modeleRetenu = false;
		}
		else
		{
			// Initialisation de beta_hat
			beta_hat = 0;

			int typeErreur(0);
			typeErreur = calculeRegression(resultat.second[valloglikelihood], resultat.second[Efron]);

			if (typeErreur > 0)
			{
				resultat.second[validiteModele] = typeErreur;
				modelesDivergents << typeErreur << " | " << numMarq + numPremierMarq << " : ";
				for (set<int>::iterator l(varContinues.begin()); l != varContinues.end(); ++l)
				{
					modelesDivergents << *l << " ";
				}
				modelesDivergents << nl;


			}

			else    // On ne calcule pas de stats pour les modèles qui présentent une erreur de calcul
			{

				// Storey!
				storey->addValidModel(dim);

				reel denominateur(somme * (1. - somme / taille) * (1. - somme / taille) + (taille - somme) * (somme / taille) * (somme / taille));
				resultat.second[Efron] = 1. - (resultat.second[Efron] / denominateur);

				//loglike_courante=resultat.second[valloglikelihood];


				// Wald test
				// Méthode matricielle
				/*	Matrix<> testWald(t(beta_hat) * J_info * beta_hat);
				 cout << beta_hat << J_info << inv_J_info << testWald ;*/
				//statsCourantes[WaldScore] = (t(beta_hat(1,0,dim,0)) * J_info(1,1,dim,dim) * beta_hat(1,0,dim,0))(0,0);

				/*
				 // TRICHE
				 MatriceReels matInterm(dim, dim);
				 try {
				 matInterm=invpd(inv_J_info(1,1,dim,dim));
				 resultat.second[WaldScore] = (t(beta_hat(1,0,dim,0)) * matInterm * beta_hat(1,0,dim,0))(0,0);

				 }
				 catch (scythe_exception& error) {
				 cerr << error.message() << "\n";
				 resultat.second[WaldScore]=0;
				 }
				 //statsCourantes[WaldScore] = beta_hat(1, 0)/sqrt(inv_J_info(1,1));
				 */





				// Traitements des stats et comparaison du modèle avec ses "parents"
				modeleRetenu = calculeStats(resultat, nbParam - 1);

				// Copie de la valeur de beta
				int nbStatCourantes(nbStats);
				if (calculeStructurePop(nbParam - 1))
				{
					nbStatCourantes = nbStatsAvecPop;
				}
				for (int i(0); i < nbParam; ++i)
				{
					resultat.second[nbStatCourantes + i] = beta_hat(i, 0);
				}

				if (AS_GWR)
				{
					calculeGWR(numMarq, varContinues, resultat);
				}
			}

		}

		// On traite la mise en mémoire et la sauvegarde différemment
		if ((selModeles == all || modeleRetenu) && sauvegardeTempsReel)
		{
			// Test storey
			if (!appliqueSeuilScoreStorey ||
			    (dim < dimensionMax && (resultat.second[Gscore] >= seuilScoreStorey || resultat.second[WaldScore] >= seuilScoreStorey)) ||
			    dim == dimensionMax && structurePop != pasStructurePop && (resultat.second[GscorePop] >= seuilScoreStorey || resultat.second[WaldScorePop] >= seuilScoreStorey) ||
			    dim == dimensionMax && structurePop == pasStructurePop && (resultat.second[Gscore] >= seuilScoreStorey || resultat.second[WaldScore] >= seuilScoreStorey)
					)
			{
				ecritResultat(dim, resultat);
			}
		}

		// Il faut garder le modèle même s'il n'est pas signif dans le cas signif, pour les comparaison ultérieures
		if ((dim < dimensionMax) || ((selModeles == all || modeleRetenu) && !sauvegardeTempsReel))
		{
			if (!appliqueSeuilScoreStorey ||
			    (dim < dimensionMax) ||
			    dim == dimensionMax && structurePop != pasStructurePop && (resultat.second[GscorePop] >= seuilScoreStorey || resultat.second[WaldScorePop] >= seuilScoreStorey) ||
			    dim == dimensionMax && structurePop == pasStructurePop && (resultat.second[Gscore] >= seuilScoreStorey || resultat.second[WaldScore] >= seuilScoreStorey)
					)
			{
				resultats[dim].insert(resultat);
			}
		}


	}


}


// Liste des erreurs:
// 1: divergence exponentielle
// 2: J_info singulière
// 3: divergence beta
// 4: max iterations atteint
// 5: marqueur constant
// 6: modèle avec parents non-significatifs
int RegressionLogistique::calculeRegression(reel& loglikeCourante, reel& composantEfron)
{

	// Test de convergence
	bool continueCalcul(true), singularMatrix(false), divergentCalculation(false);
	int nbIterations(0), typeErreur(0);

	// Iteration
	while (continueCalcul && !singularMatrix && !divergentCalculation && (nbIterations < limiteIter))
	{
		// Calcul pi
		nouv_Xb = X * beta_hat; // Test avant l'exp

		if (max(nouv_Xb) > limiteExp)
		{
			continueCalcul = false;
			composantEfron = sum((intermScores % intermScores)(0, 0, taille - 1, 0));
			loglikeCourante = sum((Y % (Xb) - log(1. + exp_Xb))(0, 0, taille - 1, 0));
			typeErreur = 1;

		}
		else
		{
			++nbIterations;

			// Calcul pi
			Xb = nouv_Xb;
			exp_Xb = exp(Xb);

			pi_hat = exp_Xb / (1 + exp_Xb);

			// Calcul ni * pi * (1 - pi)
			interm = pi_hat % (1 - pi_hat);

			// Calcul des scores U
			intermScores = (Y - pi_hat);
			for (int k(0); k < nbParam; ++k)
			{
				scores(k, 0) = sum(intermScores % X(_, k));
			}

			// Calcul de J, qui est symétrique
			for (int k(0); k < nbParam; ++k)
			{
				J_info(k, k) = sum(interm % X(_, k) % X(_, k));
				for (int l(k + 1); l < nbParam; ++l)
				{
					J_info(k, l) = sum(interm % X(_, k) % X(_, l));
					J_info(l, k) = J_info(k, l);
				}
			}

			/*if ( det(J_info)<=0)
			 {
			 singularMatrix = true;
			 }
			 else */
			{
				// Calcul de l'inverse de J
				try
				{
					inv_J_info = invpd(J_info);
				}
				catch (scythe_exception& error)
				{
					singularMatrix = true;
					continueCalcul = false;
					typeErreur = 2;
				}

				if (!singularMatrix)
				{
					// Mise à jour de beta_hat
					nouv_beta_hat = beta_hat + inv_J_info * scores;

					for (int l(0); l < nbParam; ++l)
					{
						if (abs(nouv_beta_hat(l, 0)) > limiteNaN)
						{
							continueCalcul = false;
							divergentCalculation = true;
							typeErreur = 3;
							break;
						}
					}

					if (continueCalcul)
					{
						// Test de convergence
						diff_beta_hat = nouv_beta_hat - beta_hat;
						continueCalcul = false;
						for (int l(0); l < nbParam; ++l)
						{
							if (abs(diff_beta_hat(l, 0)) > convCrit * max(eps, abs(beta_hat(l, 0))))
							{
								continueCalcul = true;
								break;
							}
						}

						beta_hat = nouv_beta_hat;
					}
				}
			}
			// Si le calcul est terminé, on calcule l'indice d'Effron
			if (!continueCalcul)
			{
				composantEfron = sum((intermScores % intermScores)(0, 0, taille - 1, 0));
				loglikeCourante = sum((Y % (Xb) - log(1. + exp_Xb))(0, 0, taille - 1, 0));
			}
		}
	}
	if (nbIterations == limiteIter)
	{
		composantEfron = sum((intermScores % intermScores)(0, 0, taille - 1, 0));
		loglikeCourante = sum((Y % (Xb) - log(1. + exp_Xb))(0, 0, taille - 1, 0));
		typeErreur = 4;
	}
	return typeErreur;
}

bool RegressionLogistique::calculeStats(Modele& resultat, int nbParamEstimes)
{
	bool modeleRetenu(true);

	// Il faut trouver les meilleurs modèles "parents"
	int dimParents(resultat.first.environnement.size() - 1); // Taille de l'étiquette - 1

	GenerationModeles::iterator modeleCourant, bestLoglike;

	// Calcul des pseudos R carrés
	reel loglikeCourante(0), loglikeZero(0);

	EtiquetteModele etiquetteCourante;

	// Il faut trouver un modèle valide pour la comparaison
	bool parentValide(false);

	// Calcul des scores G et Wald avec les variables de pop
	reel scoreWaldPop(0);
	GenerationModeles::iterator modeleParentPop;

	if (dimParents == 0)    // Modèle univarié -> un seul parent
	{
		parentValide = true;
		etiquetteCourante = resultat.first;
		etiquetteCourante.environnement.clear();
		modeleCourant = resultats[dimParents].find(etiquetteCourante);
		bestLoglike = modeleCourant;
		modeleParentPop = modeleCourant;
	}
	else // Recherche d'un parent valide et sélection du parent avec la meilleure loglike
	{
		sambada::CombinaisonVariables combinaisonVariables(familleVariables[dimParents + 1][resultat.first.environnement]);
		for (std::set<sambada::EtiquetteCombinaisonVariables>::const_iterator parentCourant(combinaisonVariables.parents.cbegin()); parentCourant != combinaisonVariables.parents.cend(); ++parentCourant)
		{
			// Initialisation avec les valeurs du premier modèle
			etiquetteCourante = resultat.first;
			etiquetteCourante.environnement = *parentCourant;    // On corrige l'étiquette pour le modèle à considérer
			modeleCourant = resultats[dimParents].find(etiquetteCourante);    // Parent courant

			// On teste si le parent existe et s'il n'est pas dans un état d'erreur
			if (modeleCourant != resultats[dimParents].end() && ((modeleCourant->second[validiteModele]) == 0 || (modeleCourant->second[validiteModele]) == 6 || (modeleCourant->second[validiteModele]) == 7 && calculeStorey)) // Storey!!
			{
				if (parentValide) //  Test si un parent valide a déjà été trouvé
				{
					if ((modeleCourant->second[valloglikelihood]) > (bestLoglike->second[valloglikelihood]))
					{
						bestLoglike = modeleCourant;
					}
				}
				else
				{
					// Ce modèle est le premier modèle valide trouvé
					bestLoglike = modeleCourant;
					parentValide = true;
				}
			}
		}
	}

	// On a trouvé un premier modèle pour comparer
	if (parentValide == true)
	{
		loglikeZero = bestLoglike->second[valloglikelihood];

		// Calcul des scores
		resultat.second[Gscore] = 2.0 * (resultat.second[valloglikelihood] - bestLoglike->second[valloglikelihood]);

		if (resultat.second[Gscore] < 0.)
		{
			resultat.second[Gscore] = 0;
		}

		// Mise à jour du compteur pour la FDR

		if (structurePop == pasStructurePop || resultat.first.environnement.size() != dimensionMax - 1 || inclutToutesVariablesPop(resultat.first.environnement))
		{
			storey->addValue(sambada::StoreyHistograms::ScoreType::G, resultat.first.environnement.size(), resultat.second[Gscore]);
		}
		// Si on sauve tous les modèles, on cherche le plus petit score de Wald
		int tailleModele(dimParents + 1);

		// Au cas où on choisit les modèles significatifs, on commence par tester le score G
		if (selModeles != all && (resultat.second[Gscore] < seuilScore[tailleModele]))
		{
			// rejet car G trop petit
			modeleRetenu = false;
			resultat.second[validiteModele] = 7;
		}
		//		else	// STOREY!
		//		{
		// On fait un test de Wald par variable (pas la constante)
		// Simplification : on calcule le score de Wald final (= le plus petit score de Wald, un par variable)
		// On teste la significativité du score à la fin.

		// Initialisation du score de Wald
		reel WaldCourant(beta_hat(1, 0) * beta_hat(1, 0) / inv_J_info(1, 1));
		resultat.second[WaldScore] = WaldCourant;

		//affiche(resultat.first);
		//cout << "	" << WaldCourant;
		for (int paramCourant(2); paramCourant <= tailleModele; ++paramCourant)
		{
			WaldCourant = beta_hat(paramCourant, 0) * beta_hat(paramCourant, 0) / inv_J_info(paramCourant, paramCourant);
			if (WaldCourant < resultat.second[WaldScore])
			{
				resultat.second[WaldScore] = WaldCourant;
			}
		}

		// Mise à jour du compteur pour la FDR
		if (structurePop == pasStructurePop || resultat.first.environnement.size() != dimensionMax - 1 || inclutToutesVariablesPop(resultat.first.environnement))
		{
			storey->addValue(sambada::StoreyHistograms::ScoreType::Wald, resultat.first.environnement.size(), resultat.second[WaldScore]);
		}
		// Test du score de Wald
		if (selModeles != all && (resultat.second[WaldScore] < seuilScore[tailleModele]))
		{
			modeleRetenu = false;
			resultat.second[validiteModele] = 7;

		}
		//		} // Storey!

	}
	else //if (selModeles!=best)	// STOREY !
		// Aucun modèle parent n'est valide -> on compare avec le modèle constant 
		// Le modèle constant est de toute façon valide
		//-> Attention au changement de seuil pour la p-valeur
	{
		// Aucun parent valide -> erreur type 6
		resultat.second[validiteModele] = 6;

		//STOREY!
		if (selModeles == best)
		{
			modeleRetenu = false;
		}

		etiquetteCourante = resultat.first;
		etiquetteCourante.environnement.clear();
		modeleCourant = resultats[0].find(etiquetteCourante);

		loglikeZero = modeleCourant->second[valloglikelihood];


		// Calcul des scores
		resultat.second[Gscore] = 2.0 * (resultat.second[valloglikelihood] - loglikeZero);
		if (resultat.second[Gscore] < 0.)
		{
			resultat.second[Gscore] = 0;
		}
		// Mise à jour du compteur pour la FDR
		if (structurePop == pasStructurePop || resultat.first.environnement.size() != dimensionMax - 1 || inclutToutesVariablesPop(resultat.first.environnement))
		{
			storey->addValue(sambada::StoreyHistograms::ScoreType::GOrphelins, resultat.first.environnement.size(), resultat.second[Gscore]);
		}
		// Test de Wald si le modèle passe le test G ou si on sauve tous les modèles
		if (selModeles != all && (resultat.second[Gscore] < seuilScoreMultivarie[dimParents + 1]))    // STOREY! (sinon selModeles==signif)
		{
			modeleRetenu = false;
			resultat.second[validiteModele] = 7;

		}
		//		else	//STOREY !
		//		{
		// Test avec Wald individuel
		/*
		 // Calcul du score de Wald -> On prend la sous-matrice (1:n, 1:n) de inv_J_info et on l'inverse
		 //cout << nbParamEstimes << endl;
		 MatriceReels matInterm(nbParamEstimes, nbParamEstimes);
		 try {
		 matInterm=invpd(inv_J_info(1,1,nbParamEstimes,nbParamEstimes));
		 resultat.second[WaldScore] = (t(beta_hat(1,0,nbParamEstimes,0)) * matInterm * beta_hat(1,0,nbParamEstimes,0))(0,0);

		 }
		 catch (scythe_exception& error) {
		 //cerr << error.message() << "\n";
		 resultat.second[WaldScore]=0;
		 }
		 */

		// On fait un test de Wald par variable (pas la constante)
		// Simplification : on calcule le score de Wald final (= le plus petit score de Wald, un par variable)
		// On teste la significativité du score à la fin.

		// Si on sauve tous les modèles, on cherche le plus petit score de Wald
		int tailleModele(dimParents + 1);

		// Initialisation du score de Wald
		reel WaldCourant(beta_hat(1, 0) * beta_hat(1, 0) / inv_J_info(1, 1));
		resultat.second[WaldScore] = WaldCourant;

		for (int paramCourant(2); paramCourant <= tailleModele; ++paramCourant)
		{
			WaldCourant = beta_hat(paramCourant, 0) * beta_hat(paramCourant, 0) / inv_J_info(paramCourant, paramCourant);

			if (WaldCourant < resultat.second[WaldScore])
			{
				resultat.second[WaldScore] = WaldCourant;
			}
		}

		// Mise à jour du compteur pour la FDR
		if (structurePop == pasStructurePop || resultat.first.environnement.size() != dimensionMax - 1 || inclutToutesVariablesPop(resultat.first.environnement))
		{
			storey->addValue(sambada::StoreyHistograms::ScoreType::WaldOrphelins, resultat.first.environnement.size(), resultat.second[WaldScore]);
		}
		// STOREY
		//			if (selModeles==signif && (resultat.second[WaldScore]<seuilScoreMultivarie[dimParents+1]))
		if (selModeles == !all && (resultat.second[WaldScore] < seuilScore[dimParents + 1]))
		{
			modeleRetenu = false;
			resultat.second[validiteModele] = 7;

		}


		//}




	}
	//STOREY
	/*else // Aucun parent n'est valide et on ne prend que les meilleurs modèles
	 {
	 modeleRetenu=false;
	 resultat.second[validiteModele]=7;

	 }*/

	// Selection du modèle parent pour la structure de pop
	if (calculeStructurePop(resultat.first.environnement.size()))
	{
		etiquetteCourante = resultat.first;
		if (structurePop == structurePopPremier)
		{
			// La variable env est à la fin
			set<int>::reverse_iterator variableCourante(resultat.first.environnement.rbegin());
			etiquetteCourante.environnement.erase(*variableCourante);

			resultat.second[WaldScorePop] = beta_hat((dimParents + 1), 0) * beta_hat((dimParents + 1), 0) / inv_J_info((dimParents + 1), (dimParents + 1));
		}
		else
		{
			// La variable env est au début
			set<int>::iterator variableCourante(resultat.first.environnement.begin());
			etiquetteCourante.environnement.erase(*variableCourante);

			resultat.second[WaldScorePop] = beta_hat(1, 0) * beta_hat(1, 0) / inv_J_info(1, 1);
		}
		modeleParentPop = resultats[dimParents].find(etiquetteCourante);

		resultat.second[GscorePop] = 2 * (resultat.second[valloglikelihood] - modeleParentPop->second[valloglikelihood]);

		// Mise à jour du compteur pour la FDR
		storey->addValue(sambada::StoreyHistograms::ScoreType::GPop, resultat.first.environnement.size(), resultat.second[GscorePop]);
		storey->addValue(sambada::StoreyHistograms::ScoreType::WaldPop, resultat.first.environnement.size(), resultat.second[WaldScorePop]);
	}







	// Ici pas besoin de calculer ceci pour les modèles non signif dans le cas signif
	if (modeleRetenu || selModeles == all)
	{

		// Calcul des pseudos R carrés
		loglikeCourante = resultat.second[valloglikelihood];

		// McFadden
		resultat.second[McFadden] = 1 - (loglikeCourante / loglikeZero);

		// McFadden adjusted ->TRICHE
		resultat.second[McFaddenAdj] = 1 - ((loglikeCourante - nbParamEstimes - 1) / loglikeZero);

		reel exposant(2. / taille);

		// Cox & Snell
//		resultat.second[CoxSnell] = 1 - pow((exp(loglikeZero) / exp(loglikeCourante)), exposant);
		resultat.second[CoxSnell] = 1 - exp(exposant * (loglikeZero - loglikeCourante));

		// Nagelkerke / Cragg & Uhler
//		resultat.second[Nagelkerke] = resultat.second[CoxSnell] / (1 - pow(exp(loglikeZero), exposant));
		resultat.second[Nagelkerke] = resultat.second[CoxSnell] / (1 - exp(exposant * loglikeZero));

		// AIC
		resultat.second[AIC] = -2 * loglikeCourante + 2 * nbParam;

		// BIC
		resultat.second[BIC] = -2 * loglikeCourante + nbParam * log(taille);
	}
	return modeleRetenu;

}

void RegressionLogistique::initialisationParametres(ParameterSet& listeParam, ParameterSetIndex& indexParam) const
{
	ParameterSetData paramCourant;

	// INPUTFILE
	paramCourant.name = "INPUTFILE";
	paramCourant.mandatory = false;
	paramCourant.present = false;
	//paramCourant.tokenize=false;
	listeParam.push_back(paramCourant);

	// OUTPUTFILE
	paramCourant.name = "OUTPUTFILE";
	listeParam.push_back(paramCourant);

	// WORDDELIM
	paramCourant.name = "WORDDELIM";
	listeParam.push_back(paramCourant);

	// LOG
	paramCourant.name = "LOG";
	listeParam.push_back(paramCourant);

	// UNCONVERGEDMODELS
	paramCourant.name = "UNCONVERGEDMODELS";
	listeParam.push_back(paramCourant);

	// HEADERS
	paramCourant.name = "HEADERS";
	//paramCourant.tokenize=true;
	listeParam.push_back(paramCourant);

	// NUMVARENV
	paramCourant.name = "NUMVARENV";
	paramCourant.mandatory = true;
	listeParam.push_back(paramCourant);

	// NUMMARK
	paramCourant.name = "NUMMARK";
	listeParam.push_back(paramCourant);

	// NUMINDIV
	paramCourant.name = "NUMINDIV";
	listeParam.push_back(paramCourant);

	// IDINDIV
	paramCourant.name = "IDINDIV";
	paramCourant.mandatory = false;
	listeParam.push_back(paramCourant);

	// COLSUPENV
	paramCourant.name = "COLSUPENV";
	listeParam.push_back(paramCourant);

	// COLSUPMARK
	paramCourant.name = "COLSUPMARK";
	listeParam.push_back(paramCourant);

	// SUBSETVARENV
	paramCourant.name = "SUBSETVARENV";
	listeParam.push_back(paramCourant);

	// SUBSETMARK
	paramCourant.name = "SUBSETMARK";
	listeParam.push_back(paramCourant);

	// DIMMAX
	paramCourant.name = "DIMMAX";
	listeParam.push_back(paramCourant);

	// SPATIAL
	paramCourant.name = "SPATIAL";
	listeParam.push_back(paramCourant);

	// Pré-requis: autocorr, GWR et SHP requièrent le paramètre SPATIAL
	// AUTOCORR
	paramCourant.name = "AUTOCORR";
	paramCourant.prereq.push_back("SPATIAL");
	listeParam.push_back(paramCourant);

	// GWR
	paramCourant.name = "GWR";
	listeParam.push_back(paramCourant);

	// SHAPEFILE
	paramCourant.name = "SHAPEFILE";
	listeParam.push_back(paramCourant);

	// DISCRETEVAR
	paramCourant.name = "DISCRETEVAR";
	paramCourant.prereq.clear();
	listeParam.push_back(paramCourant);

	// SAVETYPE
	paramCourant.name = "SAVETYPE";
	paramCourant.mandatory = true;
	listeParam.push_back(paramCourant);

	// POPULATIONVAR
	paramCourant.name = "POPULATIONVAR";
	paramCourant.mandatory = false;
	listeParam.push_back(paramCourant);

	// STOREY
	paramCourant.name = "STOREY";
	paramCourant.mandatory = false;
	listeParam.push_back(paramCourant);

	int nbTotParam(listeParam.size());
	for (int i(0); i < nbTotParam; ++i)
	{
		indexParam.insert(make_pair(listeParam[i].name, i));
	}
}

bool RegressionLogistique::calculeStructurePop(int dimensionCourante) const
{
	return (dimensionCourante == dimensionMax) && (structurePop == structurePopPremier || structurePop == structurePopDernier);
}

bool RegressionLogistique::estVariablePop(string variable) const
{
	if (variable.size() < 3)
	{
		return false;
	}

	string prefixe("");
	for (int i(0); i < 3; ++i)
	{
		prefixe += tolower(variable[i]);
	}

	return prefixe == "pop";
}

bool RegressionLogistique::inclutToutesVariablesPop(set<int> variables) const
{
	for (set<int>::iterator i(variablesPop.begin()); i != variablesPop.end(); ++i)
	{
		if (variables.find(*i) == variables.end())
		{
			return false;
		}
	}

	return true;
}

bool RegressionLogistique::estModeleEligiblePourStructurePopulation(set<int> variables) const
{
	int dimension(variables.size());
	if (dimension < (dimensionMax - 1) || structurePop == pasStructurePop)
	{
		return true;
	}

	int nombreVariablesPop = count_if(variables.begin(), variables.end(), [this](int i) { return (this->variablesPop.find(i) != this->variablesPop.end()); });

	if (dimension == dimensionMax - 1)
	{
		return (nombreVariablesPop == dimension || nombreVariablesPop == dimension - 1);
	}

	return nombreVariablesPop == (dimension - 1);
}


ComparaisonVoisins::ComparaisonVoisins()
{}

ComparaisonVoisins::~ComparaisonVoisins()
{}

bool ComparaisonVoisins::plusPetitQue(const Voisin& v1, const Voisin& v2)
{
	return (v1.second < v2.second);
}

bool ComparaisonVoisins::plusGrandQue(const Voisin& v1, const Voisin& v2)
{
	return (v1.second > v2.second);
}

ComparaisonVoisins::ComparaisonVoisins(const ComparaisonVoisins& c)
{}

int ComparaisonResultats::caseComparaisonResultats = 0;

ComparaisonResultats::ComparaisonResultats()
{}

ComparaisonResultats::~ComparaisonResultats()
{}

int ComparaisonResultats::getCase()
{
	return caseComparaisonResultats;
}

void ComparaisonResultats::setCase(int i)
{
	caseComparaisonResultats = i;
}

bool ComparaisonResultats::plusPetitQue(const GenerationModeles::value_type *const& r1, const GenerationModeles::value_type *const& r2)
{
	return ((r1->second[caseComparaisonResultats]) < (r2->second[caseComparaisonResultats]));
}

bool ComparaisonResultats::plusGrandQue(const GenerationModeles::value_type *const& r1, const GenerationModeles::value_type *const& r2)
{
	return ((r1->second[caseComparaisonResultats]) > (r2->second[caseComparaisonResultats]));
}


ComparaisonResultats::ComparaisonResultats(ComparaisonResultats& c)
{}
