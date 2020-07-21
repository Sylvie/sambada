/*************************************************************************
 * Copyright (©) 2011-2019 EPFL (Ecole Polytechnique fédérale de Lausanne)
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
//#include "shapelib-1.3.0/shapefil.h"
#include "shapefil.h"
#include "optimize.h"

using namespace std;
using namespace scythe;
using namespace sambada;


void RegressionLogistique::calculePonderation() CPPTHROW(Erreur)
{
	if (!analyseSpatiale)
	{
		erreurDetectee("MSG_noSpatialAnalysis", "No spatial analysis requested", false);
	}
	else
	{
		// calcul des distances
		const reel rayonTerre(6378137), rayonTerreCarre(rayonTerre *rayonTerre), carreBandePassante(bandePassante * bandePassante); // en mètres
		// Il faut les coordonnées en radians
		// crdBrutes= telles que stockées dans les var env (actives ou sup)
		// crd = crd en radian
		coordonneesBrutes.resize(nbPoints, 2);
		coordonneesBrutes = 0;
		coordonnees.resize(nbPoints, 2);
		coordonnees = 0;
		MatriceBools masqueCrd(nbPoints, 2, true, true);

		pointsGeo.masque.resize(nbPoints, 1);
		pointsGeo.masque = true;
		int indiceLongitude(specVarEnv.detailsVariables[longitude].localIndex), indiceLatitude(specVarEnv.detailsVariables[latitude].localIndex);
		istringstream lu;
		lu.precision(toolbox::precisionLecture);
		reel valeur(0);
		string dustbin("");

		// Longitude
		if (specVarEnv.detailsVariables[longitude].isActive) // Cas simple, il suffit de copier les données
		{
			coordonneesBrutes(_, 0) = dataEnv(_, indiceLongitude);
			masqueCrd(_, 0) = masqueX(_, indiceLongitude);
		}
		else    // Si la variable est passive, on doit récupérer les valeurs qui sont des strings
		{
			for (int i(0); i < nbPoints; ++i)
			{
				lu.clear();
				lu.precision(toolbox::precisionLecture);
				lu.str(dataSupEnv(i, indiceLongitude));
				lu >> valeur;
				if (lu.fail())
				{
					lu.clear();
					lu >> dustbin;
					// La coordonnée vaut 0 par défaut, pas besoin de la modifier
					masqueCrd(i, 0) = false;    // Mise à jour du masque
				}
				else
				{
					coordonneesBrutes(i, 0) = valeur;
				}
			}
		}

		// Latitude
		if (specVarEnv.detailsVariables[latitude].isActive) // Cas simple, il suffit de copier les données
		{
			coordonneesBrutes(_, 1) = dataEnv(_, indiceLatitude);
			masqueCrd(_, 1) = masqueX(_, indiceLatitude);
		}
		else    // Si la variable est passive, on doit récupérer les valeurs qui sont des strings
		{
			for (int i(0); i < nbPoints; ++i)
			{
				lu.clear();
				lu.precision(toolbox::precisionLecture);
				lu.str(dataSupEnv(i, indiceLatitude));
				lu >> valeur;
				if (lu.fail())
				{
					lu.clear();
					lu >> dustbin;
					// La coordonnée vaut 0 par défaut, pas besoin de la modifier
					masqueCrd(i, 1) = false;    // Mise à jour du masque
				}
				else
				{
					coordonneesBrutes(i, 1) = valeur;
				}
			}
		}

		// Calcul du masque total
		pointsGeo.masque = masqueCrd(_, 0) % masqueCrd(_, 1);

		// On repère tous les points ayant des coordonnées valides
		pointsGeo.miseAJour();

		// Coordonnees sphériques -> Transformation en radians
		if (!crdCartesiennes)
		{
			coordonnees(_, 0) = coordonneesBrutes(_, 0) * M_PI / 180.0;
			coordonnees(_, 1) = coordonneesBrutes(_, 1) * M_PI / 180.0;
		}
		else
		{
			coordonnees = coordonneesBrutes;
		}

		distances.resize(nbPoints, nbPoints);
		distances = 0;

		// Pour les points n'ayant pas de crd valides, la distance avec les autres points est considérée comme nulle
		// Leur pondération est également nulle, ce qui garantit la cohérence des résultats

		reel deltaLong, deltaLat, cosLatMoy;
		int pt1, pt2; // Pour repérer les indices des points considérés
		for (int i(0); i < pointsGeo.taille; ++i)
		{
			pt1 = pointsGeo.pointsValides[i];
			for (int j(i + 1); j < pointsGeo.taille; ++j)
			{
				pt2 = pointsGeo.pointsValides[j];
				deltaLong = coordonnees(pt1, 0) - coordonnees(pt2, 0);
				deltaLat = coordonnees(pt1, 1) - coordonnees(pt2, 1);

				if (crdCartesiennes)
				{
					distances(pt1, pt2) = (deltaLat * deltaLat + deltaLong * deltaLong);

				}
				else
				{
					cosLatMoy = cos((coordonnees(pt1, 1) + coordonnees(pt2, 1)) / 2);
					distances(pt1, pointsGeo.pointsValides[j]) = rayonTerreCarre * (deltaLat * deltaLat + deltaLong * deltaLong * cosLatMoy * cosLatMoy);
				}
				distances(pt2, pt1) = distances(pt1, pt2);

			}

		}

		// Pour la pondération, on prend w_ii = 0, un point n'est pas son propre voisin (pour l'autocorrélation)
		// Pour la GWR, le point sera rajouté parmi ses voisins
		// Les pondérations seront normées plus tard (pour tenir compte des valeurs manquantes)
		if (choixPonderation == pondDistanceMax)
		{
			for (int i(0); i < pointsGeo.taille; ++i)
			{
				pt1 = pointsGeo.pointsValides[i];
				for (int j(i + 1); j < pointsGeo.taille; ++j)
				{
					pt2 = pointsGeo.pointsValides[j];

					if (distances(pt1, pt2) < carreBandePassante)
					{
						pointsGeo.poids[pt1].push_back(make_pair(pt2, 1));
						pointsGeo.poids[pt2].push_back(make_pair(pt1, 1));
					}
				}

			}
		}
			// Pondération gaussienne: Faut-il tronquer?
		else if (choixPonderation == pondGaussienne)
		{
			double pondCourante(0);
			for (int i(0); i < pointsGeo.taille; ++i)
			{
				pt1 = pointsGeo.pointsValides[i];
				for (int j(i + 1); j < pointsGeo.taille; ++j)
				{
					pt2 = pointsGeo.pointsValides[j];
					//if (distances(pt1,pt2)<=carreBandePassante)	// Tronquage pour comparaison avec pysal
					//{
					pondCourante = exp(-distances(pt1, pt2) / (2 * carreBandePassante));
					pointsGeo.poids[pt1].push_back(make_pair(pt2, pondCourante));
					pointsGeo.poids[pt2].push_back(make_pair(pt1, pondCourante));
					//}
				}
				// Auto-voisinage pour comparaison avec pysal
				//pointsGeo.poids[pt1].push_back(make_pair(pt1, 1));

			}
		}
		else if (choixPonderation == pondBicarree)
		{
			double pondCourante(0);
			for (int i(0); i < pointsGeo.taille; ++i)
			{
				pt1 = pointsGeo.pointsValides[i];
				for (int j(i + 1); j < pointsGeo.taille; ++j)
				{
					pt2 = pointsGeo.pointsValides[j];

					if (distances(pt1, pt2) < carreBandePassante)
					{

						pondCourante = pow(1 - (distances(pt1, pt2) / carreBandePassante), 2);
						pointsGeo.poids[pt1].push_back(make_pair(pt2, pondCourante));
						pointsGeo.poids[pt2].push_back(make_pair(pt1, pondCourante));
					}

				}
				// Auto-voisinage pour comparaison avec pysal
				//pointsGeo.poids[pt1].push_back(make_pair(pt1, 1));

			}
		}
		else // Plus proches voisins (cas plus compliqué)
		{
			voisinage.resize(nbPoints);

			int nbVoisinsCourants(0);
			vector<Voisin>::iterator voisinCourant, voisinSuivant;
			// Pour chaque point, on enregistre les couples <voisin, distance> et on les trie
			for (int i(0); i < pointsGeo.taille; ++i)
			{
				pt1 = pointsGeo.pointsValides[i];

				for (int j(i + 1); j < pointsGeo.taille; ++j)
				{
					pt2 = pointsGeo.pointsValides[j];
					voisinage[pt1].push_back(make_pair(pt2, distances(pt1, pt2)));
					voisinage[pt2].push_back(make_pair(pt1, distances(pt1, pt2)));
				}

				sort(voisinage[pt1].begin(), voisinage[pt1].end(), ComparaisonVoisins::plusPetitQue);
			}

			double poids(1.); // On n'a normé personne pour le moment (et la norme change suivant le cas AC / GWR)
			//				double poids(1./nbPointsValides);

			// Cas où tous les points valides sont voisins
			if (pointsGeo.taille <= (nbPlusProchesVoisins + 1))
			{
				for (int i(0); i < pointsGeo.taille; ++i)
				{
					pt1 = pointsGeo.pointsValides[i];

					for (int j(i + 1); j < pointsGeo.taille; ++j)
					{
						pt2 = pointsGeo.pointsValides[j];
						voisinage[pt1].push_back(make_pair(pt2, poids));
						voisinage[pt2].push_back(make_pair(pt1, poids));
					}
					for (voisinCourant = voisinage[pt1].begin(); voisinCourant != voisinage[pt1].end(); ++voisinCourant)
					{
						pointsGeo.poids[pt1].push_back(make_pair(voisinCourant->first, poids));
					}
				}
			}
			else
			{
				// On sélectionne les points voisins
				// Il peut y en avoir plus que "nbPlusProchesVoisins" si certains points sont à égale distance

				reel plusPetiteDiffDist(pow(10., -4)); // En dessous d' 1cm ça compte pas! (Les distances sont au carré)
				for (int i(0); i < pointsGeo.taille; ++i)
				{
					pt1 = pointsGeo.pointsValides[i];
					nbVoisinsCourants = nbPlusProchesVoisins;
					voisinSuivant = voisinage[pt1].begin() + (nbPlusProchesVoisins); // Premier point hors limite
					voisinCourant = voisinage[pt1].begin() + (nbPlusProchesVoisins - 1); //dernier point à être pris automatiquement
					while (abs((voisinSuivant)->second - voisinCourant->second) < plusPetiteDiffDist)
					{
						++voisinCourant;
						++voisinSuivant;
						++nbVoisinsCourants;
					}
					//					double poids(1./nbVoisinsCourants);
					for (voisinCourant = voisinage[pt1].begin(); voisinCourant != voisinSuivant; ++voisinCourant)
					{
						pointsGeo.poids[pt1].push_back(make_pair(voisinCourant->first, poids));
					}
				}
			}
		}
		/*for (int chose(0); chose<pointsGeo.poids[0].size(); ++chose)
		 {
		 cout << pointsGeo.poids[0][chose].first << " " <<  pointsGeo.poids[0][chose].second<< "\n";
		 }*/
	}
}

bool RegressionLogistique::Domaine::miseAJour()
{
	if (masque.cols() != 1)
	{
		return false;
	}
	else
	{
		int ancienNbPoints(nbPoints);
		nbPoints = masque.rows();

		// Le poids est réinitialisé
		poids.clear();
		poids.resize(nbPoints, vector<Voisin>(0));

		pointsValides.clear();
		indices.resize(nbPoints);
		int indiceCourant(0);
		for (int i(0); i < nbPoints; ++i)
		{
			if (masque(i, 0)) // Si le point est valide
			{
				pointsValides.push_back(i);
				indices[i] = indiceCourant;
				++indiceCourant;
			}
			else
			{
				indices[i] = -1; // Les points invalides n'ont pas d'indice local
			}
		}
		taille = indiceCourant;
		return true;
	}
}


// La pondération est normée!
int RegressionLogistique::calculeAutocorrelations() CPPTHROW(Erreur)
{
	try
	{
		if (!analyseSpatiale || (!AS_autocorrVarEnv && !AS_autocorrMarq))
		{
			erreurDetectee("MSG_noAutocorr", "No autocorrelation computation requested", false);
		}
		calculePonderation();
	}
	catch (const Erreur& err)
	{
		if (err.estFatale())
		{
			throw;
		}
		else
		{
			return 1;
		}

	}

	//****
	// Initialisation du shapefile
	SHPHandle fichierSHP;
	DBFHandle fichierDBF;
	vector<int> indicesShpEnv, indicesShpMarq, indicesShpPoints;
	if (AS_shapefile)
	{
		indicesShpEnv.resize(nbEnvActives, -1);
		indicesShpMarq.resize(nbMarqActifs, -1);
		indicesShpPoints.resize(nbPoints);
		fichierSHP = SHPCreate(nomFichierResultats.first.c_str(), SHPT_POINT);
		fichierDBF = DBFCreate(nomFichierResultats.first.c_str());

		int precision(toolbox::precisionLecture), nbDecimales(precision / 3);

		// Création des colonnes Env
		if (AS_autocorrVarEnv && nbEnvActives > 0)
		{
			for (int i(0); i < nbEnvActives; ++i)
			{
				indicesShpEnv[i] = DBFAddField(fichierDBF, specVarEnv.detailsVariables[specVarEnv.variablesActives[i]].name.c_str(), FTDouble, precision, nbDecimales);
			}
		}

		// Création des colonnes Marq
		if (AS_autocorrMarq && nbMarqActifs > 0)
		{
			for (int i(0); i < nbMarqActifs; ++i)
			{
				indicesShpMarq[i] = DBFAddField(fichierDBF, specMarq.detailsVariables[specMarq.variablesActives[i]].name.c_str(), FTDouble, precision, nbDecimales);
			}
		}

		// Création de la couche vectorielle
		double x(0), y(0);
		for (int i(0); i < nbPoints; ++i)
		{
			// Si le point n'a pas de coordonnées valides, on ajoute un point nul dans le shp
			// Cela garantit la correspondance des données avec le dbf:
			// Un point manquant à cause des crd ou à cause de valeurs manquantes est traité de la même façon durant le calcul d'AC
			if (pointsGeo.masque(i, 0))
			{
				x = coordonneesBrutes(i, 0);
				y = coordonneesBrutes(i, 1);

				SHPObject *obj(SHPCreateSimpleObject(SHPT_POINT, 1, &x, &y, NULL));
				indicesShpPoints[i] = SHPWriteObject(fichierSHP, -1, obj);
				SHPDestroyObject(obj);
			}
			else
			{
				SHPObject *obj(SHPCreateSimpleObject(SHPT_NULL, 0, NULL, NULL, NULL));
				indicesShpPoints[i] = SHPWriteObject(fichierSHP, -1, obj);
				SHPDestroyObject(obj);
			}
		}
	}
	//****

	MatriceReels deviations(nbPoints, 1), deviationsCourantes(nbPoints, 1), autocorrLocale, autocorrGlobale,
			autocorrLocaleCourante, pValeurGlobale, pValeurLocale, historiqueAutocorrGlobale,
			autocorrTemp, autocorrTempCourante,
			tableDeviations, tableSpatialLags;

	// pointsAC : pondération-type où tous les points ayant des crd sont valides
	Domaine pointsAC, pointsCourants;
	//TableClassementsVoisins ponderationCourante(nbPoints);

	reel facteurEchelleLocal(0), facteurEchelleGlobal(0), moyenne(0), sommeCarresDeviations(0), autocorrGlobaleCourante(0);

	// Test de multiplication

	vector<int> listePointsValidesPerm, voisinagePerm, listePointsGlobauxPerm; // Liste des points sélectionnables lors de la permutations, voisinage temporaire pendant la permutation
	int pt1, pt2, nbVoisins; // Pour repérer les indices des points considérés || Pour compter les voisins d'un point
	reel limiteIndicesIdentiques(1e-6), moitieNbPerms(AS_nbPermutations / 2);
	int nbSimPlusGrandes, nbSimEgales, nbSimPlusPetites;

	// On calcule une pondération-type basée sur le masqueGeo pour les cas où tous les points ayant des crd sont valides
	pointsAC = pointsGeo;
	reel sommePond(0);
	for (int i(0); i < nbPoints; ++i)
	{
		if (pointsAC.masque(i, 0))
		{
			sommePond = 0;
			// Si un point est valide, tous ses voisins sont considérés, vu qu'on ne compte que le masque géo
			nbVoisins = pointsAC.poids[i].size();
			for (int j(0); j < nbVoisins; ++j)
			{
				sommePond += pointsAC.poids[i][j].second;
			}
			if (sommePond > 0)
			{
				for (int j(0); j < nbVoisins; ++j)
				{
					pointsAC.poids[i][j].second /= sommePond;
				}
			}
		}
	}

	ofstream sortieAS;

	//Initialisation de la graine du générateur pour les permutation
	srand(time(NULL));

	time_t t1(time(NULL));
	// Variables environnementales
	if (AS_autocorrVarEnv && nbEnvActives > 0)
	{
		autocorrLocale.resize(nbPoints, nbEnvActives);
		autocorrLocale = 0;
		autocorrLocaleCourante.resize(nbPoints, 1);
		autocorrLocaleCourante = 0;
		pValeurLocale.resize(nbPoints, nbEnvActives);
		pValeurLocale = 0;

		if (AS_autocorrGlobale)
		{
			autocorrGlobale.resize(1, nbEnvActives);
			autocorrGlobaleCourante = 0;
			pValeurGlobale.resize(1, nbEnvActives);
			pValeurGlobale = 0;
			historiqueAutocorrGlobale.resize(AS_nbPermutations, nbEnvActives);
		}

		for (int i(0); i < nbEnvActives; ++i)
		{
			pointsCourants.masque = pointsAC.masque % masqueX(_, i);
			pointsCourants.taille = toolbox::sommeNumerique(pointsCourants.masque);
			if (pointsCourants.taille == 0)
			{
				continue;
			}
			else if (pointsCourants.taille == pointsAC.taille)    // Cas où tous les points géographiquement valides ont une valeur env
			{
				pointsCourants = pointsAC;
			}
			else
			{
				pointsCourants.miseAJour();    // La liste des points valides, la pondération, etc, sont réinitialisées

				// Calcul de la pondératon
				if (choixPonderation != pondPlusProchesVoisins)
				{
					double somme(0);
					for (int u(0); u < pointsCourants.taille; ++u)
					{
						pt1 = pointsCourants.pointsValides[u];
						somme = 0;
						nbVoisins = pointsAC.poids[pt1].size();
						for (int v(0); v < nbVoisins; ++v)
						{
							// On copie le voisin s'il est lui-même valide
							if (pointsCourants.masque(pointsAC.poids[pt1][v].first, 0))
							{
								pointsCourants.poids[pt1].push_back(pointsAC.poids[pt1][v]);
								somme += (pointsAC.poids[pt1][v].second);
							}
						}
						nbVoisins = pointsCourants.poids[pt1].size();
						if (somme != 0)
						{
							for (int v(0); v < nbVoisins; ++v)
							{
								pointsCourants.poids[pt1][v].second /= somme;

							}
						}
					}
				}
				else    // Cas plus proches voisins
				{
					// Cas où tous les points valides sont voisins
					// On se base sur les points courants valides -> pas besoin de revérifier leur validité
					if (pointsCourants.taille <= (nbPlusProchesVoisins + 1))
					{
						double poids(1. / pointsCourants.taille);
						for (int u(0); u < pointsCourants.taille; ++u)
						{
							pt1 = pointsCourants.pointsValides[u];
							for (int v(u + 1); v < pointsCourants.taille; ++v)
							{
								pt2 = pointsCourants.pointsValides[v];
								pointsCourants.poids[pt1].push_back(make_pair(pt2, poids));
								pointsCourants.poids[pt2].push_back(make_pair(pt1, poids));
							}
						}
					}
					else
					{
						// On sélectionne les points voisins
						// Il peut y en avoir plus que "nbPlusProchesVoisins" si certains points sont à égale distance

						vector<Voisin>::iterator voisinCourant, voisinSuivant;

						for (int u(0); u < pointsCourants.taille; ++u)
						{
							pt1 = pointsCourants.pointsValides[u];
							nbVoisins = 0;
							voisinCourant = voisinage[pt1].begin();
							while (nbVoisins < nbPlusProchesVoisins && voisinCourant != voisinage[pt1].end())
							{
								if (pointsCourants.masque(voisinCourant->first, 0))
								{
									pointsCourants.poids[pt1].push_back(make_pair(voisinCourant->first, 1));
									++nbVoisins;
								}
								++voisinCourant;
							}

							if (voisinCourant != voisinage[pt1].end())
							{
								voisinSuivant = voisinCourant + 1;
								while (voisinSuivant != voisinage[pt1].end() && ((voisinSuivant)->second == voisinCourant->second))
								{
									if (pointsCourants.masque(voisinSuivant->first, 0))
									{
										pointsCourants.poids[pt1].push_back(make_pair(voisinSuivant->first, 1));
										++nbVoisins;
									}
									++voisinCourant;
									++voisinSuivant;
								}

							}

							double poids(1. / nbVoisins);
							for (voisinCourant = pointsCourants.poids[pt1].begin(); voisinCourant != pointsCourants.poids[pt1].end(); ++voisinCourant)
							{
								voisinCourant->second = poids;
							}
						}
					}
				}
			}


			// Calcul des déviations, moyenne et variance
			moyenne = 0;
			sommeCarresDeviations = 0;
			for (int k(0); k < nbPoints; ++k)
			{
				if (pointsCourants.masque(k, 0))
				{
					deviations(k, 0) = dataEnv(k, i);
					moyenne += dataEnv(k, i);
					sommeCarresDeviations += dataEnv(k, i) * dataEnv(k, i);
				}
				else
				{
					deviations(k, 0) = 0;
				}

			}

			moyenne /= pointsCourants.taille;
			//				sommeCarresDeviations= sommeCarresDeviations/pointsCourants.taille-moyenne*moyenne;	// ! Variance
			sommeCarresDeviations = sommeCarresDeviations - pointsCourants.taille * moyenne * moyenne;

			for (int u(0); u < pointsCourants.taille; ++u)
			{
				deviations(pointsCourants.pointsValides[u]) -= moyenne;
			}


			facteurEchelleLocal = (pointsCourants.taille - 1) / sommeCarresDeviations;
			facteurEchelleGlobal = 1. / (pointsCourants.taille - 1);

			// On calcule l'autocorrélation manuellement car la matrice des poids est creuse (sparse matrix)
			//autocorrTemp=0;
			double valeurIntermediaire(0);
			for (int j(0); j < pointsCourants.taille; ++j)
			{
				pt1 = pointsCourants.pointsValides[j];
				nbVoisins = pointsCourants.poids[pt1].size();
				valeurIntermediaire = 0;
				for (int k(0); k < nbVoisins; ++k)
				{
					valeurIntermediaire += pointsCourants.poids[pt1][k].second * deviations[pointsCourants.poids[pt1][k].first];
				}
				autocorrLocale(pt1, i) = valeurIntermediaire * deviations[pt1] * facteurEchelleLocal;
			}

			// Ecritude des valeurs dans le DBF
			if (AS_shapefile)
			{
				for (int j(0); j < nbPoints; ++j)
				{
					if (pointsCourants.masque(j, 0))
					{
						DBFWriteDoubleAttribute(fichierDBF, indicesShpPoints[j], indicesShpEnv[i], autocorrLocale(j, i));
					}
					else
					{
						DBFWriteNULLAttribute(fichierDBF, indicesShpPoints[j], indicesShpEnv[i]);

					}
				}
			}

			/*			autocorrTemp=(deviations%(ponderation*deviations))*facteurEchelleLocal;	// AC locale, elle est temporaire car il faut remettre les valeurs à leur place (tailleDonnees->nbPoints)
			 for (int j(0); j<tailleDonnees; ++j)
			 {
			 autocorrLocale(pointsValides[j], i) =autocorrTemp(j,0);
			 }
			 */
			if (AS_autocorrGlobale)
			{
				autocorrGlobale(0, i) = facteurEchelleGlobal * (sum(autocorrLocale(_, i)));
				//journal << specDataEnv[varEnvActives[i]].name << " " << moyenne << " " << autocorrGlobale(0,i)  << " " << sommeCarresDeviations<< "\n";
			}


			// Permutations!
			if (AS_autocorrLocale)
			{

				// Chaque fois qu'on prend un point, le nombre de points restants (=parmi lesquels on peut tirer un élément) diminue
				// Prop = facteur d'échelle pour le tirage d'un entier entre nbPointsValides, nbPointsValides-1, ..., 2,1,0
				// Subtilité : Le points courant reste fixe, il ne sera jamais choisi comme voisin -> nb voisins possibles = taille-1 !!!!
				vector<double> proportion(pointsCourants.taille - 1);
				int choix(0);
				for (int j(0); j < (pointsCourants.taille - 1); ++j)
				{
					// Comme 0<=rand<=RAND_MAX, on divise par RM+1 pour éviter les débordements de tableau
					proportion[j] = (double) (pointsCourants.taille - 1 - j) / ((double) RAND_MAX + 1);
				}


				// Itération sur les points -> tous les points n'ont pas le même nombre de voisins
				for (int k(0); k < pointsCourants.taille; ++k)
				{
					nbSimPlusGrandes = 0;
					nbSimEgales = 0;
					nbSimPlusPetites = 0;

					pt1 = pointsCourants.pointsValides[k]; // Numéro global du point considéré
					nbVoisins = pointsCourants.poids[pt1].size();
					voisinagePerm.resize(nbVoisins);    // Redimensionnement du voisinage temporaire

					for (int j(0); j < AS_nbPermutations; ++j)
					{

						// Les pondérations sont fixes, on réordonne les déviations
						listePointsValidesPerm = pointsCourants.pointsValides;
						// L'indice local du point courant est k -> on le permute avec le dernier point pour ne jamais le sélectionner
						swap(listePointsValidesPerm[k], listePointsValidesPerm[pointsCourants.taille - 1]);

						for (int l(0); l < nbVoisins; ++l)
						{
							choix = floor(rand() * proportion[l]);
							voisinagePerm[l] = listePointsValidesPerm[choix];
							swap(listePointsValidesPerm[choix], listePointsValidesPerm[pointsCourants.taille - 2 - l]);        // La dernière place est déjà occupée par le point
						}

						random_shuffle(voisinagePerm.begin(), voisinagePerm.end());
						// voisinagePerm contient les numéros globaux des voisins temporaires

						// On prend la position (et la pondération) du point d'origine et on utilise la valeur du point permuté

						//nbVoisins=pointsCourants.poids[pt1].size();
						valeurIntermediaire = 0;
						for (int l(0); l < nbVoisins; ++l)
						{
							valeurIntermediaire += pointsCourants.poids[pt1][l].second * deviations[voisinagePerm[l]];
						}
						// Le point courant reste le même
						autocorrLocaleCourante(pt1, 0) = valeurIntermediaire * deviations[pt1] * facteurEchelleLocal;



						// p-valeur
						/*if ( ( (autocorrLocale(pt1, i)>=0) && (autocorrLocaleCourante(pt1, 0)>=autocorrLocale(pt1, i)))
						 ||   ( (autocorrLocale(pt1, i)<0) && (autocorrLocaleCourante(pt1, 0)<=autocorrLocale(pt1, i)))    )
						 {
						 pValeurLocale(pt1, i)=pValeurLocale(pt1, i)+1;
						 }*/

						// Comparaison entre simulation et valeur réelle
						if (abs(autocorrLocaleCourante(pt1, 0) - autocorrLocale(pt1, i)) < limiteIndicesIdentiques)
						{
							++nbSimEgales;
						}
						else if (autocorrLocaleCourante(pt1, 0) > autocorrLocale(pt1, i))
						{
							++nbSimPlusGrandes;
						}

						// La p-valeur pour l'AC globale est calculée à part, tous les points sont permutés
					}

					// p-valeur
					// Discussions avec S. Rey et lexique ASU (pseudo p-valeurs)

					nbSimPlusPetites = AS_nbPermutations - nbSimPlusGrandes - nbSimEgales;

					if (nbSimPlusPetites >= nbSimPlusGrandes) // a >= c
					{
						if (nbSimPlusPetites >= moitieNbPerms)
						{
							pValeurLocale(pt1, i) = (nbSimPlusGrandes + nbSimEgales + 1.0) / (AS_nbPermutations + 1);
						}
						else    // cas où les I' == I incluent la médiane de la distribution
						{
							pValeurLocale(pt1, i) = 0.5;
						}
					}
					else    // a<c
					{
						if (nbSimPlusGrandes >= moitieNbPerms)
						{
							pValeurLocale(pt1, i) = (nbSimPlusPetites + nbSimEgales + 1.0) / (AS_nbPermutations + 1);
						}
						else // cas où les I' == I incluent la médiane de la distribution
						{
							pValeurLocale(pt1, i) = 0.5;
						}
					}
				}    // Fin calcul p-valeurs locales
			}
			if (AS_autocorrGlobale)
			{
				nbSimPlusGrandes = 0;
				nbSimEgales = 0;
				nbSimPlusPetites = 0;

				// La somme des carrés des déviations est la même pour chaque permutation
				// Idem pour le facteur d'échelle

				listePointsValidesPerm.resize(pointsCourants.taille);
				for (int j(0); j < pointsCourants.taille; ++j)
				{
					listePointsValidesPerm[j] = j;
				}

				for (int j(0); j < AS_nbPermutations; ++j)
				{
					//listePointsValidesPerm=pointsCourants.pointsValides;
					random_shuffle(listePointsValidesPerm.begin(), listePointsValidesPerm.end());
					listePointsGlobauxPerm = pointsCourants.indices; // Indices locaux des points valides (originaux)

					// On veut les nouveaux indices globaux des points permutés
					for (int k(0); k < pointsCourants.nbPoints; ++k)
					{
						if (listePointsGlobauxPerm[k] != -1)
						{
							// de droite à gauche: liste [N] des indices locaux -> permutations -> indices globaux correspondants
							listePointsGlobauxPerm[k] = pointsCourants.pointsValides[listePointsValidesPerm[listePointsGlobauxPerm[k]]];
						}
					}
					//	toolbox::affiche(listePointsGlobauxPerm);
					// On prend la position (et la pondération) du point d'origine et on utilise la valeur du point permuté
					for (int k(0); k < pointsCourants.taille; ++k)
					{
						pt1 = pointsCourants.pointsValides[k];
						nbVoisins = pointsCourants.poids[pt1].size();
						valeurIntermediaire = 0;
						for (int l(0); l < nbVoisins; ++l)
						{
							valeurIntermediaire += pointsCourants.poids[pt1][l].second * deviations[listePointsGlobauxPerm[pointsCourants.poids[pt1][l].first]];
						}

						autocorrLocaleCourante(pt1, 0) = valeurIntermediaire * deviations[listePointsGlobauxPerm[pt1]] * facteurEchelleLocal;
					}



					//autocorrGlobaleCourante=facteurEchelle*(sum(autocorrLocaleCourante(_,0))-sommeCarresDeviations);
					autocorrGlobaleCourante = facteurEchelleGlobal * (sum(autocorrLocaleCourante(_, 0)));


					// Historique
					historiqueAutocorrGlobale(j, i) = autocorrGlobaleCourante;

					// p-valeur
					/*					if ( ((autocorrGlobale(0, i) >= 0) && (autocorrGlobaleCourante >= autocorrGlobale(0, i)) ) || ((autocorrGlobale(0, i) < 0) && (autocorrGlobaleCourante <= autocorrGlobale(0, i)) ))
					 //if ( abs(autocorrGlobaleCourante) >= abs(autocorrGlobale(0, i) ))
					 {
					 pValeurGlobale(0, i)=pValeurGlobale(0, i)+1;
					 }*/
					// Comparaison entre simulation et valeur réelle
					if (abs(autocorrGlobaleCourante - autocorrGlobale(0, i)) < limiteIndicesIdentiques)
					{
						++nbSimEgales;
					}
					else if (autocorrGlobaleCourante > autocorrGlobale(0, i))
					{
						++nbSimPlusGrandes;
					}

				}

				nbSimPlusPetites = AS_nbPermutations - nbSimPlusGrandes - nbSimEgales;

				if (nbSimPlusPetites >= nbSimPlusGrandes) // a >= c
				{
					if (nbSimPlusPetites >= moitieNbPerms)
					{
						pValeurGlobale(0, i) = (nbSimPlusGrandes + nbSimEgales + 1.0) / (AS_nbPermutations + 1);
					}
					else    // cas où les I' == I incluent la médiane de la distribution
					{
						pValeurGlobale(0, i) = 0.5;
					}
				}
				else    // a<c
				{
					if (nbSimPlusGrandes >= moitieNbPerms)
					{
						pValeurLocale(0, i) = (nbSimPlusPetites + nbSimEgales + 1.0) / (AS_nbPermutations + 1);
					}
					else // cas où les I' == I incluent la médiane de la distribution
					{
						pValeurGlobale(0, i) = 0.5;
					}
				}


			}
		}

		// Calcul de la p-valeur
		/*pValeurLocale=(pValeurLocale+1.)/(AS_nbPermutations+1.);
		 if (AS_autocorrGlobale)
		 {
		 pValeurGlobale = (pValeurGlobale+1.)/(AS_nbPermutations+1.);
		 cout << "*** " << pValeurGlobale << "\n";
		 }*/





		time_t t2(time(NULL));
		cout << "Calcul autocorrélation : " << t2 - t1 << "\n";


		// Ecriture des résultats
		sortieAS.precision(toolbox::precisionLecture);
		t1 = time(NULL);
		// Ecriture de l'autocorrélation
		sortieAS.open((nomFichierResultats.first + "-AS-Env" + nomFichierResultats.second).c_str());
		if (sortieAS.fail())
		{
			erreurDetectee("MSG_errOpenFileACEnv", "Error while opening file for autocorrelation of environnemental variables.");
		}
		else
		{
			// Headers
			if (existeColID)
			{
				sortieAS << "ID ";
			}
			for (int j(0); j < nbEnvActives; ++j)
			{
				sortieAS << specVarEnv.detailsVariables[specVarEnv.variablesActives.at(j)].name << " ";
			}
			sortieAS << delimLignes;

			// AC Globale
			if (AS_autocorrGlobale)
			{
				if (existeColID)
				{
					sortieAS << "Global_AC ";
				}
				for (int j(0); j < nbEnvActives; ++j)
				{
					sortieAS << autocorrGlobale(0, j) << " ";
				}
				sortieAS << delimLignes;
			}


			// AC Locales
			if (AS_autocorrLocale)
			{

				for (int i(0); i < nbPoints; ++i)
				{
					if (existeColID)
					{
						sortieAS << dataSupEnv(i, specVarEnv.detailsVariables[colIDEnv].localIndex) << " ";
					}
					for (int j(0); j < nbEnvActives; ++j)
					{
						sortieAS << autocorrLocale(i, j) << " ";
					}
					sortieAS << delimLignes;

				}
			}
			sortieAS.close();
		}


		// Ecriture des p-valeurs
		sortieAS.open((nomFichierResultats.first + "-AS-Env-pVal" + nomFichierResultats.second).c_str());
		if (sortieAS.fail())
		{
			erreurDetectee("MSG_errOpenFileACSigEnv", "Error while opening file for autocorrelation significance of environnementales variables.");
		}
		else
		{
			// Headers
			if (existeColID)
			{
				sortieAS << "ID ";
			}
			for (int j(0); j < nbEnvActives; ++j)
			{
				sortieAS << specVarEnv.detailsVariables[specVarEnv.variablesActives.at(j)].name << " ";
			}
			sortieAS << delimLignes;

			// AC Globale
			if (AS_autocorrGlobale)
			{
				if (existeColID)
				{
					sortieAS << "Global_AC ";
				}
				for (int j(0); j < nbEnvActives; ++j)
				{
					sortieAS << pValeurGlobale(0, j) << " ";
				}
				sortieAS << delimLignes;
			}

			// AC Locales
			if (AS_autocorrLocale)
			{

				for (int i(0); i < nbPoints; ++i)
				{
					if (existeColID)
					{
						sortieAS << dataSupEnv(i, specVarEnv.detailsVariables[colIDEnv].localIndex) << " ";
					}
					for (int j(0); j < nbEnvActives; ++j)
					{
						sortieAS << pValeurLocale(i, j) << " ";
					}
					sortieAS << delimLignes;
				}
			}
			sortieAS.flush();
			sortieAS.close();
		}

		// Ecriture de l'historique des permutations (autocorr globale)
		if (AS_autocorrGlobale)
		{
			sortieAS.open((nomFichierResultats.first + "-AS-Env-Sim" + nomFichierResultats.second).c_str());
			if (sortieAS.fail())
			{
				erreurDetectee("MSG_errOpenFileACHistEnv", "Error while opening history file for autocorrelation of environnemental variables.");
			}
			else
			{
				// Headers
				for (int j(0); j < nbEnvActives; ++j)
				{
					sortieAS << specVarEnv.detailsVariables[specVarEnv.variablesActives.at(j)].name << " ";
				}
				sortieAS << delimLignes;


				for (int i(0); i < AS_nbPermutations; ++i)
				{
					for (int j(0); j < nbEnvActives; ++j)
					{
						sortieAS << historiqueAutocorrGlobale(i, j) << " ";
					}
					sortieAS << delimLignes;
				}
				sortieAS.flush();
				sortieAS.close();
			}

		}
		t2 = time(NULL);
		journal << "Ecriture autocorrélation : " << t2 - t1 << "s.\n";


	}

	// Marqueurs génétiques
	if (AS_autocorrMarq && nbMarqActifs > 0)
	{
		autocorrLocale.resize(nbPoints, nbMarqActifs);
		autocorrLocale = 0;
		autocorrLocaleCourante.resize(nbPoints, 1);
		autocorrLocaleCourante = 0;
		pValeurLocale.resize(nbPoints, nbMarqActifs);
		pValeurLocale = 0;

		tableDeviations.resize(nbPoints, nbMarqActifs);
		tableDeviations = toolbox::notANumber();
		tableSpatialLags.resize(nbPoints, nbMarqActifs);
		tableSpatialLags = toolbox::notANumber();

		if (AS_autocorrGlobale)
		{
			autocorrGlobale.resize(1, nbMarqActifs);
			autocorrGlobaleCourante = 0;
			pValeurGlobale.resize(1, nbMarqActifs);
			pValeurGlobale = 0;
			historiqueAutocorrGlobale.resize(AS_nbPermutations, nbMarqActifs);
		}

		for (int i(0); i < nbMarqActifs; ++i)
		{
			// There is no general mask for genetic data
			pointsCourants.masque = pointsAC.masque;
			for (set<int>::iterator iter(missingValuesMarq[i].begin()); iter != missingValuesMarq[i].end(); ++iter)
			{
				pointsCourants.masque[*iter] = false;
			}
			pointsCourants.taille = toolbox::sommeNumerique(pointsCourants.masque);
			if (pointsCourants.taille == 0)
			{
				continue;
			}
			else if (pointsCourants.taille == pointsAC.taille)    // Cas où tous les points géographiquement valides ont une valeur env
			{
				pointsCourants = pointsAC;
			}
			else
			{
				pointsCourants.miseAJour();    // La liste des points valides, la pondération, etc, sont réinitialisées

				// Calcul de la pondératon
				if (choixPonderation != pondPlusProchesVoisins)
				{
					double somme(0);
					for (int u(0); u < pointsCourants.taille; ++u)
					{
						pt1 = pointsCourants.pointsValides[u];
						somme = 0;
						nbVoisins = pointsAC.poids[pt1].size();
						for (int v(0); v < nbVoisins; ++v)
						{
							// On copie le voisin s'il est lui-même valide
							if (pointsCourants.masque(pointsAC.poids[pt1][v].first, 0))
							{
								pointsCourants.poids[pt1].push_back(pointsAC.poids[pt1][v]);
								somme += (pointsAC.poids[pt1][v].second);
							}
						}
						nbVoisins = pointsCourants.poids[pt1].size();
						if (somme != 0)
						{
							for (int v(0); v < nbVoisins; ++v)
							{
								pointsCourants.poids[pt1][v].second /= somme;

							}
						}
					}
				}
				else    // Cas plus proches voisins
				{
					// Cas où tous les points valides sont voisins
					// On se base sur les points courants valides -> pas besoin de revérifier leur validité
					if (pointsCourants.taille <= (nbPlusProchesVoisins + 1))
					{
						double poids(1. / pointsCourants.taille);
						for (int u(0); u < pointsCourants.taille; ++u)
						{
							pt1 = pointsCourants.pointsValides[u];
							for (int v(u + 1); v < pointsCourants.taille; ++v)
							{
								pt2 = pointsCourants.pointsValides[v];
								pointsCourants.poids[pt1].push_back(make_pair(pt2, poids));
								pointsCourants.poids[pt2].push_back(make_pair(pt1, poids));
							}
						}
					}
					else
					{
						// On sélectionne les points voisins
						// Il peut y en avoir plus que "nbPlusProchesVoisins" si certains points sont à égale distance

						vector<Voisin>::iterator voisinCourant, voisinSuivant;

						for (int u(0); u < pointsCourants.taille; ++u)
						{
							pt1 = pointsCourants.pointsValides[u];
							nbVoisins = 0;
							voisinCourant = voisinage[pt1].begin();
							while (nbVoisins < nbPlusProchesVoisins && voisinCourant != voisinage[pt1].end())
							{
								if (pointsCourants.masque(voisinCourant->first, 0))
								{
									pointsCourants.poids[pt1].push_back(make_pair(voisinCourant->first, 1));
									++nbVoisins;
								}
								++voisinCourant;
							}

							if (voisinCourant != voisinage[pt1].end())
							{
								voisinSuivant = voisinCourant + 1;
								while (voisinSuivant != voisinage[pt1].end() && ((voisinSuivant)->second == voisinCourant->second))
								{
									if (pointsCourants.masque(voisinSuivant->first, 0))
									{
										pointsCourants.poids[pt1].push_back(make_pair(voisinSuivant->first, 1));
										++nbVoisins;
									}
									++voisinCourant;
									++voisinSuivant;
								}

							}

							double poids(1. / nbVoisins);
							for (voisinCourant = pointsCourants.poids[pt1].begin(); voisinCourant != pointsCourants.poids[pt1].end(); ++voisinCourant)
							{
								voisinCourant->second = poids;
							}
						}
					}
				}
			}

			//toolbox::affiche(pointsCourants.poids);
			/*
			 for (int zut(0); zut<5; ++zut)
			 {
			 for (int prout(0); prout<voisinage[zut].size(); ++prout)
			 {
			 cout << voisinage[zut][prout].first << " " << voisinage[zut][prout].second << " " ;
			 }
			 cout << endl;
			 for (int prout(0); prout<pointsCourants.poids[zut].size(); ++ prout)
			 {
			 cout << pointsCourants.poids[zut][prout].first<< " " << pointsCourants.poids[zut][prout].second << " ";
			 }
			 cout << endl;
			 }*/
			// Calcul des déviations, moyenne et variance
			moyenne = 0;
			sommeCarresDeviations = 0;
			for (int k(0); k < nbPoints; ++k)
			{
				if (pointsCourants.masque(k, 0))
				{
					deviations(k, 0) = dataMarq(k, i);

					moyenne += dataMarq(k, i);
					sommeCarresDeviations += dataMarq(k, i) * dataMarq(k, i);
				}
				else
				{
					deviations(k, 0) = 0;
				}

			}

			moyenne /= pointsCourants.taille;
			//				sommeCarresDeviations= sommeCarresDeviations/pointsCourants.taille-moyenne*moyenne;	// ! Variance
			sommeCarresDeviations = sommeCarresDeviations - pointsCourants.taille * moyenne * moyenne;

			for (int u(0); u < pointsCourants.taille; ++u)
			{
				deviations(pointsCourants.pointsValides[u]) -= moyenne;
				tableDeviations(pointsCourants.pointsValides[u], i) = deviations(pointsCourants.pointsValides[u]);
			}


			facteurEchelleLocal = (pointsCourants.taille - 1) / sommeCarresDeviations;
			facteurEchelleGlobal = 1. / (pointsCourants.taille - 1);

			// On calcule l'autocorrélation manuellement car la matrice des poids est creuse (sparse matrix)
			//autocorrTemp=0;
			double valeurIntermediaire(0);
			for (int j(0); j < pointsCourants.taille; ++j)
			{
				pt1 = pointsCourants.pointsValides[j];
				nbVoisins = pointsCourants.poids[pt1].size();
				valeurIntermediaire = 0;
				for (int k(0); k < nbVoisins; ++k)
				{
					valeurIntermediaire += pointsCourants.poids[pt1][k].second * deviations[pointsCourants.poids[pt1][k].first];
				}
				autocorrLocale(pt1, i) = valeurIntermediaire * deviations[pt1] * facteurEchelleLocal;
				tableSpatialLags(pt1, i) = valeurIntermediaire;
			}

			// Ecritude des valeurs dans le DBF
			if (AS_shapefile)
			{
				for (int j(0); j < nbPoints; ++j)
				{
					if (pointsCourants.masque(j, 0))
					{
						DBFWriteDoubleAttribute(fichierDBF, indicesShpPoints[j], indicesShpMarq[i], autocorrLocale(j, i));
					}
					else
					{
						DBFWriteNULLAttribute(fichierDBF, indicesShpPoints[j], indicesShpMarq[i]);

					}
				}
			}

			/*			autocorrTemp=(deviations%(ponderation*deviations))*facteurEchelleLocal;	// AC locale, elle est temporaire car il faut remettre les valeurs à leur place (tailleDonnees->nbPoints)
			 for (int j(0); j<tailleDonnees; ++j)
			 {
			 autocorrLocale(pointsValides[j], i) =autocorrTemp(j,0);
			 }
			 */
			if (AS_autocorrGlobale)
			{

				autocorrGlobale(0, i) = facteurEchelleGlobal * (sum(autocorrLocale(_, i)));
				cout << specMarq.detailsVariables[specMarq.variablesActives[i]].name << " " << moyenne << " " << autocorrGlobale(0, i) << " " << sommeCarresDeviations << "\n";
			}


			// Permutations!
			if (AS_autocorrLocale)
			{
				// Chaque fois qu'on prend un point, le nombre de points restants (=parmi lesquels on peut tirer un élément) diminue
				// Prop = facteur d'échelle pour le tirage d'un entier entre nbPointsValides, nbPointsValides-1, ..., 2,1,0
				// Subtilité : Le points courant reste fixe, il ne sera jamais choisi comme voisin -> nb voisins possibles = taille-1 !!!!
				vector<double> proportion(pointsCourants.taille - 1);
				int choix(0);
				for (int j(0); j < (pointsCourants.taille - 1); ++j)
				{
					// Comme 0<=rand<=RAND_MAX, on divise par RM+1 pour éviter les débordements de tableau
					proportion[j] = (double) (pointsCourants.taille - 1 - j) / ((double) RAND_MAX + 1);
				}


				// Itération sur les points -> tous les points n'ont pas le même nombre de voisins
				for (int k(0); k < pointsCourants.taille; ++k)
				{
					nbSimPlusGrandes = 0;
					nbSimEgales = 0;
					nbSimPlusPetites = 0;

					pt1 = pointsCourants.pointsValides[k]; // Numéro global du point considéré
					nbVoisins = pointsCourants.poids[pt1].size();
					voisinagePerm.resize(nbVoisins);    // Redimensionnement du voisinage temporaire

					for (int j(0); j < AS_nbPermutations; ++j)
					{

						// Les pondérations sont fixes, on réordonne les déviations
						listePointsValidesPerm = pointsCourants.pointsValides;
						// L'indice local du point courant est k -> on le permute avec le dernier point pour ne jamais le sélectionner
						swap(listePointsValidesPerm[k], listePointsValidesPerm[pointsCourants.taille - 1]);

						for (int l(0); l < nbVoisins; ++l)
						{
							choix = floor(rand() * proportion[l]);
							voisinagePerm[l] = listePointsValidesPerm[choix];
							swap(listePointsValidesPerm[choix], listePointsValidesPerm[pointsCourants.taille - 2 - l]);        // La dernière place est déjà occupée par le point
						}

						random_shuffle(voisinagePerm.begin(), voisinagePerm.end());
						// voisinagePerm contient les numéros globaux des voisins temporaires

						// On prend la position (et la pondération) du point d'origine et on utilise la valeur du point permuté

						//nbVoisins=pointsCourants.poids[pt1].size();
						valeurIntermediaire = 0;
						for (int l(0); l < nbVoisins; ++l)
						{
							valeurIntermediaire += pointsCourants.poids[pt1][l].second * deviations[voisinagePerm[l]];
						}
						// Le point courant reste le même
						autocorrLocaleCourante(pt1, 0) = valeurIntermediaire * deviations[pt1] * facteurEchelleLocal;



						// p-valeur

						//if ( ((autocorrTempCourante(k, 0)*autocorrTemp(k, 0))>0) && (abs(autocorrTempCourante(k, 0)) >= abs(autocorrTemp(k, 0))))
						/*if ( ( (autocorrLocale(pt1, i)>=0) && (autocorrLocaleCourante(pt1, 0)>=autocorrLocale(pt1, i)))
						 ||   ( (autocorrLocale(pt1, i)<0) && (autocorrLocaleCourante(pt1, 0)<=autocorrLocale(pt1, i)))    )
						 //	if ( abs(autocorrLocaleCourante(pt1, 0))>=abs(autocorrLocale(pt1, i))   )
						 {
						 pValeurLocale(pt1, i)=pValeurLocale(pt1, i)+1;
						 }*/

						// Comparaison entre simulation et valeur réelle
						if (abs(autocorrLocaleCourante(pt1, 0) - autocorrLocale(pt1, i)) < limiteIndicesIdentiques)
						{
							++nbSimEgales;
						}
						else if (autocorrLocaleCourante(pt1, 0) > autocorrLocale(pt1, i))
						{
							++nbSimPlusGrandes;
						}

						// La p-valeur pour l'AC globale est calculée à part, tous les points sont permutés

					}
					// p-valeur
					// Discussions avec S. Rey et lexique ASU (pseudo p-valeurs)

					nbSimPlusPetites = AS_nbPermutations - nbSimPlusGrandes - nbSimEgales;

					if (nbSimPlusPetites >= nbSimPlusGrandes) // a >= c
					{
						if (nbSimPlusPetites >= moitieNbPerms)
						{
							pValeurLocale(pt1, i) = (nbSimPlusGrandes + nbSimEgales + 1.0) / (AS_nbPermutations + 1);
						}
						else    // cas où les I' == I incluent la médiane de la distribution
						{
							pValeurLocale(pt1, i) = 0.5;
						}
					}
					else    // a<c
					{
						if (nbSimPlusGrandes >= moitieNbPerms)
						{
							pValeurLocale(pt1, i) = (nbSimPlusPetites + nbSimEgales + 1.0) / (AS_nbPermutations + 1);
						}
						else // cas où les I' == I incluent la médiane de la distribution
						{
							pValeurLocale(pt1, i) = 0.5;
						}
					}

				}    // Fin calcul p-valeurs locales
			}
			if (AS_autocorrGlobale)
			{
				nbSimPlusGrandes = 0;
				nbSimEgales = 0;
				nbSimPlusPetites = 0;

				// La somme des carrés des déviations est la même pour chaque permutation
				// Idem pour le facteur d'échelle

				listePointsValidesPerm.resize(pointsCourants.taille);
				for (int j(0); j < pointsCourants.taille; ++j)
				{
					listePointsValidesPerm[j] = j;
				}

				for (int j(0); j < AS_nbPermutations; ++j)
				{
					//listePointsValidesPerm=pointsCourants.pointsValides;
					random_shuffle(listePointsValidesPerm.begin(), listePointsValidesPerm.end()); //indices locaux permutés
					listePointsGlobauxPerm = pointsCourants.indices; // Indices locaux des points valides (originaux)

					// On veut les nouveaux indices globaux des points permutés
					for (int k(0); k < pointsCourants.nbPoints; ++k)
					{
						if (listePointsGlobauxPerm[k] != -1)
						{
							// de droite à gauche: liste [N] des indices locaux -> permutations -> indices globaux correspondants
							listePointsGlobauxPerm[k] = pointsCourants.pointsValides[listePointsValidesPerm[listePointsGlobauxPerm[k]]];
						}
					}
					//	toolbox::affiche(listePointsGlobauxPerm);
					// On prend la position (et la pondération) du point d'origine et on utilise la valeur du point permuté
					for (int k(0); k < pointsCourants.taille; ++k)
					{
						pt1 = pointsCourants.pointsValides[k];
						nbVoisins = pointsCourants.poids[pt1].size();
						valeurIntermediaire = 0;
						for (int l(0); l < nbVoisins; ++l)
						{
							valeurIntermediaire += pointsCourants.poids[pt1][l].second * deviations[listePointsGlobauxPerm[pointsCourants.poids[pt1][l].first]];
						}

						autocorrLocaleCourante(pt1, 0) = valeurIntermediaire * deviations[listePointsGlobauxPerm[pt1]] * facteurEchelleLocal;
					}



					//autocorrGlobaleCourante=facteurEchelle*(sum(autocorrLocaleCourante(_,0))-sommeCarresDeviations);
					autocorrGlobaleCourante = facteurEchelleGlobal * (sum(autocorrLocaleCourante(_, 0)));


					// Historique
					historiqueAutocorrGlobale(j, i) = autocorrGlobaleCourante;

					// p-valeur
					/*if ( ((autocorrGlobale(0, i) >= 0) && (autocorrGlobaleCourante >= autocorrGlobale(0, i)) ) || ((autocorrGlobale(0, i) < 0) && (autocorrGlobaleCourante <= autocorrGlobale(0, i)) ))
					 //if ( abs(autocorrGlobaleCourante) >= abs(autocorrGlobale(0, i) ))
					 {
					 pValeurGlobale(0, i)=pValeurGlobale(0, i)+1;
					 }*/

					// Comparaison entre simulation et valeur réelle
					if (abs(autocorrGlobaleCourante - autocorrGlobale(0, i)) < limiteIndicesIdentiques)
					{
						++nbSimEgales;
					}
					else if (autocorrGlobaleCourante > autocorrGlobale(0, i))
					{
						++nbSimPlusGrandes;
					}

				}

				nbSimPlusPetites = AS_nbPermutations - nbSimPlusGrandes - nbSimEgales;

				if (nbSimPlusPetites >= nbSimPlusGrandes) // a >= c
				{
					if (nbSimPlusPetites >= moitieNbPerms)
					{
						pValeurGlobale(0, i) = (nbSimPlusGrandes + nbSimEgales + 1.0) / (AS_nbPermutations + 1);
					}
					else    // cas où les I' == I incluent la médiane de la distribution
					{
						pValeurGlobale(0, i) = 0.5;
					}
				}
				else    // a<c
				{
					if (nbSimPlusGrandes >= moitieNbPerms)
					{
						pValeurGlobale(0, i) = (nbSimPlusPetites + nbSimEgales + 1.0) / (AS_nbPermutations + 1);
					}
					else // cas où les I' == I incluent la médiane de la distribution
					{
						pValeurGlobale(0, i) = 0.5;
					}
				}

			}
		}

		// Calcul de la p-valeur
		/*pValeurLocale=(pValeurLocale+1.)/(AS_nbPermutations+1.);
		 if (AS_autocorrGlobale)
		 {
		 pValeurGlobale = (pValeurGlobale+1.)/(AS_nbPermutations+1.);
		 cout << "*** " << pValeurGlobale << "\n";
		 }*/

		time_t t2(time(NULL));
		cout << "Calcul autocorrélation : " << t2 - t1 << "s.\n";


		// Ecriture des résultats
		sortieAS.precision(toolbox::precisionLecture);
		t1 = time(NULL);
		// Ecriture de l'autocorrélation
		sortieAS.open((nomFichierResultats.first + "-AS-Mark" + nomFichierResultats.second).c_str());
		if (sortieAS.fail())
		{
			erreurDetectee("MSG_errOpenFileACEnv", "Error while opening file for autocorrelation of genetic markers.");
		}
		else
		{
			// Headers
			if (existeColID)
			{
				sortieAS << "ID ";
			}
			for (int j(0); j < nbMarqActifs; ++j)
			{
				sortieAS << specMarq.detailsVariables[specMarq.variablesActives.at(j)].name << " ";
			}
			sortieAS << delimLignes;

			// AC Globale
			if (AS_autocorrGlobale)
			{
				if (existeColID)
				{
					sortieAS << "Global_AC ";
				}
				for (int j(0); j < nbMarqActifs; ++j)
				{
					sortieAS << autocorrGlobale(0, j) << " ";
				}
				sortieAS << delimLignes;
			}


			// AC Locales
			if (AS_autocorrLocale)
			{
				for (int i(0); i < nbPoints; ++i)
				{
					if (existeColID)
					{
						// On lit les ID dans la liste des variables environnementales
						// S'il y a un seul fichier de données, l'ID n'est pas disponible parmis les marqueurs
						sortieAS << dataSupEnv(i, specVarEnv.detailsVariables[colIDEnv].localIndex) << " ";
					}
					for (int j(0); j < nbMarqActifs; ++j)
					{
						sortieAS << autocorrLocale(i, j) << " ";
					}
					sortieAS << delimLignes;

				}
			}

			sortieAS.close();
		}


		// Ecriture des p-valeurs
		sortieAS.open((nomFichierResultats.first + "-AS-Mark-pVal" + nomFichierResultats.second).c_str());
		if (sortieAS.fail())
		{
			erreurDetectee("MSG_errOpenFileACSigEnv", "Error while opening file for autocorrelation significance of genetic markers.");
		}
		else
		{
			// Headers
			if (existeColID)
			{
				sortieAS << "ID ";
			}
			for (int j(0); j < nbMarqActifs; ++j)
			{
				sortieAS << specMarq.detailsVariables[specMarq.variablesActives.at(j)].name << " ";
			}
			sortieAS << delimLignes;

			// AC Globale
			if (AS_autocorrGlobale)
			{
				if (existeColID)
				{
					sortieAS << "Global_AC ";
				}
				for (int j(0); j < nbMarqActifs; ++j)
				{
					sortieAS << pValeurGlobale(0, j) << " ";
				}
				sortieAS << delimLignes;
			}

			// AC Locales
			if (AS_autocorrLocale)
			{
				for (int i(0); i < nbPoints; ++i)
				{
					if (existeColID)
					{
						// On lit les ID dans la liste des variables environnementales
						// S'il y a un seul fichier de données, l'ID n'est pas disponible parmis les marqueurs
						sortieAS << dataSupEnv(i, specVarEnv.detailsVariables[colIDEnv].localIndex) << " ";
					}
					for (int j(0); j < nbMarqActifs; ++j)
					{
						sortieAS << pValeurLocale(i, j) << " ";
					}
					sortieAS << delimLignes;
				}
			}
			sortieAS.flush();
			sortieAS.close();

		}

		// Ecriture de l'historique des permutations (autocorr globale)
		if (AS_autocorrGlobale)
		{
			sortieAS.open((nomFichierResultats.first + "-AS-Mark-Sim" + nomFichierResultats.second).c_str());
			if (sortieAS.fail())
			{
				erreurDetectee("MSG_errOpenFileACHistEnv", "Error while opening history file for autocorrelation of genetic markers.");
			}
			else
			{
				// Headers
				for (int j(0); j < nbMarqActifs; ++j)
				{
					sortieAS << specMarq.detailsVariables[specMarq.variablesActives.at(j)].name << " ";
				}
				sortieAS << delimLignes;


				for (int i(0); i < AS_nbPermutations; ++i)
				{
					for (int j(0); j < nbMarqActifs; ++j)
					{
						sortieAS << historiqueAutocorrGlobale(i, j) << " ";
					}
					sortieAS << delimLignes;
				}
				sortieAS.flush();
				sortieAS.close();
			}

		}

		t2 = time(NULL);
		cout << "Ecriture autocorrélation : " << t2 - t1 << "s.\n";


		if (AS_spatialLag)
		{
			// Ecriture de l'autocorrélation
			sortieAS.open((nomFichierResultats.first + "-AS-Mark-deviations-spatial-lag" + nomFichierResultats.second).c_str());
			if (sortieAS.fail())
			{
				throw Erreur("MSG_errOpenFileACEnv", "Error while opening file for deviations and spatial lag of genetic markers.");
			}
			else
			{
				// Headers
				if (existeColID)
				{
					sortieAS << "ID ";
				}
				for (int j(0); j < nbMarqActifs; ++j)
				{
					sortieAS << specMarq.detailsVariables[specMarq.variablesActives.at(j)].name + "-dev" << " ";
					sortieAS << specMarq.detailsVariables[specMarq.variablesActives.at(j)].name + "-lag" << " ";
				}
				sortieAS << delimLignes;


				// Dev and spatial lag
				for (int i(0); i < nbPoints; ++i)
				{
					if (existeColID)
					{
						// On lit les ID dans la liste des variables environnementales
						// S'il y a un seul fichier de données, l'ID n'est pas disponible parmis les marqueurs
						sortieAS << dataSupEnv(i, specVarEnv.detailsVariables[colIDEnv].localIndex) << " ";
					}
					for (int j(0); j < nbMarqActifs; ++j)
					{
						sortieAS << tableDeviations(i, j) << " ";
						sortieAS << tableSpatialLags(i, j) << " ";
					}
					sortieAS << delimLignes;

				}
			}

			sortieAS.close();
		}
	}



	/*
	 // Marqueurs génétiques
	 if (AS_autocorrMarq && nbMarqActifs>0)
	 {
	 autocorrLocale.resize(nbPoints, nbMarqActifs);
	 autocorrLocale=0;
	 autocorrLocaleCourante.resize(nbPoints, 1);
	 autocorrLocaleCourante=0;
	 pValeurLocale.resize(nbPoints, nbMarqActifs);
	 pValeurLocale=0;

	 if (AS_autocorrGlobale)
	 {
	 autocorrGlobale.resize(1, nbMarqActifs);
	 autocorrGlobaleCourante=0;
	 pValeurGlobale.resize(1, nbMarqActifs);
	 pValeurGlobale=0;
	 historiqueAutocorrGlobale.resize(AS_nbPermutations, nbMarqActifs);
	 }

	 for (int i(0); i<nbMarqActifs; ++i)
	 {
	 masque=masqueGeo;
	 for (set< int >::iterator iter(missingValuesMarq[i].begin()); iter!=missingValuesMarq[i].end(); ++iter)
	 {
	 masque[*iter]=false;
	 }

	 tailleDonnees=(toolbox::sommeNumerique(masque));
	 if (tailleDonnees==0)
	 {
	 continue;
	 }

	 // Liste des points valides pour la variable i
	 pointsValides.clear();
	 for (int j(0); j<nbPoints; ++j)
	 {
	 if (masque(j, 0))
	 {
	 pointsValides.push_back(j);
	 }
	 }

	 // Mise à jour de la pondération
	 ponderationCourante.clear();
	 ponderationCourante.resize(tailleDonnees);

	 if (choixPonderation!=pondPlusProchesVoisins)
	 {
	 int somme(0);
	 int nbVoisins(0);
	 for (int u(0); u<tailleDonnees; ++u)
	 {
	 somme=0;
	 for (int v(0); v<tailleDonnees; ++v)
	 {
	 ponderationCourante[u].push_back(ponderation[u][v]);
	 somme += ponderation[u][v].second;
	 }
	 if (somme != 0)
	 {
	 nbVoisins=ponderationCourante[u].size();
	 for (int v(0); v<nbVoisins; ++v)
	 {
	 ponderationCourante[u][v].second /= somme;

	 }
	 }
	 }
	 }
	 else	// Cas plus proches voisins
	 {
	 // Cas où tous les points valides sont voisins
	 if (tailleDonnees<=nbPlusProchesVoisins)
	 {
	 double poids(1./tailleDonnees);
	 for (int u(0); u<tailleDonnees; ++u)
	 {
	 for (int v(u+1); v<tailleDonnees; ++v)
	 {
	 ponderationCourante[u].push_back(make_pair(v, poids));
	 ponderationCourante[v].push_back(make_pair(u, poids));
	 }
	 }
	 }
	 else
	 {
	 // On sélectionne les points voisins
	 // Il peut y en avoir plus que "nbPlusProchesVoisins" si certains points sont à égale distance

	 nbVoisinsCourants=0;
	 vector<Voisin>::iterator voisinCourant, voisinSuivant;

	 //cout << "! " << distances(9,0,9,384) << "\n";
	 for (int u(0); u<tailleDonnees; ++u)
	 {
	 pt1 = pointsValides[u];
	 nbVoisinsCourants=nbPlusProchesVoisins;
	 voisinSuivant=voisinage[pt1].begin()+(nbPlusProchesVoisins); // Premier point hors limite
	 voisinCourant=voisinage[pt1].begin()+(nbPlusProchesVoisins-1); //dernier point à être pris automatiquement
	 while((voisinSuivant)->second == voisinCourant->second)
	 {
	 ++voisinCourant;
	 ++voisinSuivant;
	 ++nbVoisinsCourants;
	 }
	 double poids(1./nbVoisinsCourants);
	 for (voisinCourant=voisinage[pt1].begin(); voisinCourant!=voisinSuivant; ++voisinCourant)
	 {
	 ponderationCourante[u].push_back(make_pair(voisinCourant->first, poids));
	 }
	 }
	 }
	 }

	 deviations.resize(tailleDonnees,1);
	 for (int u(0); u<tailleDonnees; ++u)
	 {
	 deviations(u, 0) = dataMarq(pointsValides[u], i);

	 }

	 moyenne=sum(deviations)/tailleDonnees;
	 deviations=(deviations-moyenne);

	 sommeCarresDeviations=sum(deviations%deviations);


	 facteurEchelleLocal=(tailleDonnees-1)/sommeCarresDeviations;
	 facteurEchelleGlobal=1./(tailleDonnees-1);

	 // On calcule l'autocorrélation manuellement car la matrice des poids est creuse (sparse matrix)
	 //autocorrTemp=0;
	 double valeurIntermediaire(0);
	 for (int  j(0); j<tailleDonnees; ++j)
	 {
	 nbVoisinsCourants=ponderationCourante[j].size();
	 valeurIntermediaire=0;
	 for (int k(0); k<nbVoisinsCourants; ++k)
	 {
	 valeurIntermediaire+=ponderationCourante[j][k].second * deviations[ponderationCourante[j][k].first];
	 }
	 autocorrLocale(pointsValides[j], i) = valeurIntermediaire*deviations[j]*facteurEchelleLocal;
	 }

	 // Ecritude des valeurs dans le DBF
	 if (AS_shapefile)
	 {
	 for (int j(0); j<tailleDonnees; ++j)
	 {
	 DBFWriteDoubleAttribute(fichierDBF, indicesShpPoints[pointsValides[j]], indicesShpMarq[i], autocorrLocale(pointsValides[j], i));
	 }
	 }

	 if (AS_autocorrGlobale)
	 {
	 //cout << "\n" << autocorrLocale(_,i);
	 //cout << tailleDonnees << " " << sum(ponderationCourante) << " " << sum(autocorrLocale(_,i)) << " " << sum(deviations%deviations) << "\n";

	 autocorrGlobale(0,i)=facteurEchelleGlobal*(sum(autocorrLocale(_,i)));
	 cout << specDataMarq[marqActifs[i]].name << " " << moyenne << " " << autocorrGlobale(0,i) << " "<< sommeCarresDeviations<< "\n";
	 }


	 // Permutations!
	 for (int j(0); j<AS_nbPermutations; ++j)
	 {

	 // Les pondérations sont fixes, on réordonne les déviation
	 deviationsCourantes=deviations;
	 random_shuffle(deviationsCourantes.begin(), deviationsCourantes.end());
	 for (int  k(0); k<tailleDonnees; ++k)
	 {
	 nbVoisinsCourants=ponderationCourante[k].size();
	 valeurIntermediaire=0;
	 for (int l(0); l<nbVoisinsCourants; ++l)
	 {
	 valeurIntermediaire+=ponderationCourante[k][l].second * deviationsCourantes[ponderationCourante[k][l].first];
	 }

	 autocorrLocaleCourante(pointsValides[k], 0) = valeurIntermediaire*deviationsCourantes[k]*facteurEchelleLocal;
	 }


	 // p-valeur
	 for (int k(0); k<tailleDonnees; ++k)
	 {
	 //if ( ((autocorrTempCourante(k, 0)*autocorrTemp(k, 0))>0) && (abs(autocorrTempCourante(k, 0)) >= abs(autocorrTemp(k, 0))))
	 if ( ( (autocorrLocale(pointsValides[k], i)>0) && (autocorrLocaleCourante(pointsValides[k], 0)>=autocorrLocale(pointsValides[k], i)))
	 ||   ( (autocorrLocale(pointsValides[k], i)<0) && (autocorrLocaleCourante(pointsValides[k], 0)<=autocorrLocale(pointsValides[k], i)))    )
	 //if ( (autocorrLocaleCourante(pointsValides[k], 0))>=abs(autocorrLocale(pointsValides[k], i))   )
	 {
	 pValeurLocale(pointsValides[k], i)=pValeurLocale(pointsValides[k], i)+1;
	 }

	 }

	 if (AS_autocorrGlobale)
	 {
	 // La somme des carrés des déviations est la même pour chaque permutation
	 // Idem pour le facteur d'échelle

	 //autocorrGlobaleCourante=facteurEchelle*(sum(autocorrLocaleCourante(_,0))-sommeCarresDeviations);
	 autocorrGlobaleCourante=facteurEchelleGlobal*(sum(autocorrLocaleCourante(_,0)));


	 // Historique
	 historiqueAutocorrGlobale(j, i)=autocorrGlobaleCourante;

	 // p-valeur
	 if ( ((autocorrGlobale(0, i) > 0) && (autocorrGlobaleCourante >= autocorrGlobale(0, i)) ) || ((autocorrGlobale(0, i) < 0) && (autocorrGlobaleCourante <= autocorrGlobale(0, i)) ))
	 {
	 pValeurGlobale(0, i)=pValeurGlobale(0, i)+1;
	 }
	 }
	 }
	 }

	 // Calcul de la p-valeur
	 pValeurLocale=(pValeurLocale+1.)/(AS_nbPermutations+1.);
	 if (AS_autocorrGlobale)
	 {
	 pValeurGlobale = (pValeurGlobale+1.)/(AS_nbPermutations+1.);
	 cout << "*** " << pValeurGlobale << "\n";
	 }

	 cout << autocorrLocale(0,0,10,2) << "\n"<< pValeurLocale(0,0,10,2) << "\n";

	 // Ecriture des résultats

	 sortieAS.precision(toolbox::precisionLecture);
	 sortieAS.open((nomFichierResultats.first+"-AS-Mark"+nomFichierResultats.second).c_str());
	 if (sortieAS.fail())
	 {
	 throw Erreur("MSG_errOpenFileACMark", "Error while opening file for autocorrelation of genetic markers.");
	 }
	 else
	 {
	 // Headers
	 if (existeColID)
	 {
	 sortieAS << "ID ";
	 }
	 for (int j(0); j<nbMarqActifs; ++j)
	 {
	 sortieAS << specDataMarq[marqActifs.at(j)].name << " ";
	 }
	 sortieAS << delimLignes;

	 // AC Globale
	 if (AS_autocorrGlobale)
	 {
	 if (existeColID)
	 {
	 sortieAS << "Global_AC ";
	 }
	 for (int j(0); j<nbMarqActifs; ++j)
	 {
	 sortieAS << autocorrGlobale(0, j) << " ";
	 }
	 sortieAS << delimLignes;
	 }


	 // AC Locales
	 for (int i(0); i<nbPoints; ++i)
	 {
	 if (existeColID)
	 {
	 // On lit les ID dans la liste des variables environnementales
	 // S'il y a un seul fichier de données, l'ID n'est pas disponible parmis les marqueurs
	 sortieAS << dataSupEnv(i, specDataEnv[colIDEnv].localIndex) << " ";
	 }
	 for (int j(0); j<nbMarqActifs; ++j)
	 {
	 sortieAS << autocorrLocale(i, j) << " ";
	 }
	 sortieAS << delimLignes;
	 }


	 sortieAS.close();
	 }

	 // Ecriture des p-valeurs
	 sortieAS.open((nomFichierResultats.first+"-AS-Mark-pVal"+nomFichierResultats.second).c_str());
	 if (sortieAS.fail())
	 {
	 throw Erreur("MSG_errOpenFileACSigMark", "Error while opening file for autocorrelation significance of genetic markers.");
	 }
	 else
	 {
	 // Headers
	 if (existeColID)
	 {
	 sortieAS << "ID ";
	 }
	 for (int j(0); j<nbMarqActifs; ++j)
	 {
	 sortieAS << specDataMarq[marqActifs.at(j)].name << " ";
	 }
	 sortieAS << delimLignes;

	 // AC Globale
	 if (AS_autocorrGlobale)
	 {
	 if (existeColID)
	 {
	 sortieAS << "Global_AC ";
	 }
	 for (int j(0); j<nbMarqActifs; ++j)
	 {
	 sortieAS <<  pValeurGlobale(0, j) << " ";
	 }
	 sortieAS << delimLignes;
	 }

	 // AC Locales
	 for (int i(0); i<nbPoints; ++i)
	 {
	 if (existeColID)
	 {
	 // On lit les ID dans la liste des variables environnementales
	 // S'il y a un seul fichier de données, l'ID n'est pas disponible parmis les marqueurs
	 sortieAS << dataSupEnv(i, specDataEnv[colIDEnv].localIndex) << " ";
	 }
	 for (int j(0); j<nbMarqActifs; ++j)
	 {
	 sortieAS << pValeurLocale(i, j) << " ";
	 }
	 sortieAS << delimLignes;
	 }

	 sortieAS.close();
	 }

	 // Ecriture de l'historique des permutations (autocorr globale)
	 if (AS_autocorrGlobale)
	 {
	 sortieAS.open((nomFichierResultats.first+"-AS-Mark-Sim"+nomFichierResultats.second).c_str());
	 if (sortieAS.fail())
	 {
	 throw Erreur("MSG_errOpenFileACHistMark", "Error while opening history file for autocorrelation of genetic markers.");
	 }
	 else
	 {
	 // Headers
	 for (int j(0); j<nbMarqActifs; ++j)
	 {
	 sortieAS << specDataMarq[marqActifs.at(j)].name << " ";
	 }
	 sortieAS << delimLignes;


	 for (int i(0); i<AS_nbPermutations; ++i)
	 {
	 for (int j(0); j<nbMarqActifs; ++j)
	 {
	 sortieAS << historiqueAutocorrGlobale(i, j) << " ";
	 }
	 sortieAS << delimLignes;
	 }
	 sortieAS.flush();
	 sortieAS.close();
	 }
	 }
	 }
	 */
	// Fermeture des fichier shp et dbf
	if (AS_shapefile)
	{
		SHPClose(fichierSHP);
		DBFClose(fichierDBF);
	}
	return 0;
}

void RegressionLogistique::calculeDomaineGlobal(RegressionLogistique::Domaine& pointsTot)
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
}

void RegressionLogistique::calculeGWR(int numMarq, const set<int>& varContinues, Modele& resultat)
{
	cout << "%" << numMarq << endl;
	int nbVarCont(varContinues.size());
	int dim(nbVarCont);

	if (dim == 0)
	{
		return;
	}
	nbParam = dim + 1;

	int tailleResDepart(resultat.second.size());
	int nbStatsGWR(9 + 2 * dim), nbStatsGWRbase(7);
	resultat.second.resize(tailleResDepart + nbStatsGWR, 0);

	bool recalculeLoglikeGlobale(false);    // Si des points inclus dans le modèle global n'ont pas été inclus dans le modèle local, il faut recalculer la loglike globale pour la cohérence du nombre de points

	Domaine pointsGWR;
	pointsGWR.masque = pointsMarq.masque;

	for (set<int>::iterator i(varContinues.begin()); i != varContinues.end(); ++i)
	{
		pointsGWR.masque %= masqueX(_, *i);
	}

	pointsGWR.miseAJour();
	double sommeGWR(toolbox::sommeNumerique(dataMarq(_, numMarq) % pointsGWR.masque));


	if (pointsGWR.taille == taille)    // Cas où les var env n'avaient pas de valeurs manquantes supplémentaires par rapport aux crd et au marqueur
	{
		pointsGWR.poids = pointsMarq.poids;
	}
	else
	{
		if (sommeGWR == pointsGWR.taille || sommeGWR == 0) // Y est constant!
		{
			resultat.second[tailleResDepart + nbStatsGWR - 1] = 1;    // La p-valeur vaut 1
			return;
		}


		recalculeLoglikeGlobale = true;
		reel sommePond(0);
		int nbVoisins(0);
		if (choixPonderation != pondPlusProchesVoisins)
		{
			cout << "ICI" << endl;
			for (int i(0); i < nbPoints; ++i)
			{
				if (pointsGWR.masque(i, 0))
				{
					nbVoisins = pointsMarq.poids[i].size();
					for (int j(0); j < nbVoisins; ++j)
					{
						if (pointsGWR.masque(pointsMarq.poids[i][j].first, 0))
						{
							sommePond += pointsMarq.poids[i][j].second;
							pointsGWR.poids[i].push_back(pointsMarq.poids[i][j]);
						}
					}
					sommePond += 1.; // Pour le point i
					nbVoisins = pointsGWR.poids[i].size();
					for (int j(0); j < nbVoisins; ++j)
					{
						pointsGWR.poids[i][j].second /= sommePond;
					}
					pointsGWR.poids[i].push_back(make_pair(i, 1. / sommePond));

				}
			}
		}
		else    // Cas plus proches voisins, on reprend la liste du voisinage
		{

			// Cas où tous les points valides sont voisins
			if (pointsGWR.taille <= (nbPlusProchesVoisins + 1))
			{
				double poids(1. / pointsGWR.taille);
				for (int i(0); i < nbPoints; ++i)
				{
					if (pointsGWR.masque(i, 0))
					{
						for (int j(i); j < nbPoints; ++j)
						{
							if (pointsGWR.masque(j, 0))
							{
								pointsGWR.poids[i].push_back(make_pair(j, poids));
								pointsGWR.poids[j].push_back(make_pair(i, poids));
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
					if (pointsGWR.masque(i, 0))
					{
						nbVoisinsCourants = 0;
						voisinCourant = voisinage[i].begin();
						while (nbVoisinsCourants < nbPlusProchesVoisins && voisinCourant != voisinage[i].end())
						{
							if (pointsGWR.masque(voisinCourant->first, 0))
							{
								pointsGWR.poids[i].push_back(make_pair(voisinCourant->first, 1));
								++nbVoisinsCourants;
							}
							++voisinCourant;
						}
						// On cherche si des points sont à la même distance que le dernier
						voisinSuivant = voisinCourant + 1;
						while ((voisinSuivant != voisinage[i].end()) && abs((voisinSuivant)->second - voisinCourant->second) < plusPetiteDiffDist)
						{
							if (pointsGWR.masque(voisinSuivant->first, 0))
							{
								pointsGWR.poids[i].push_back(make_pair(voisinSuivant->first, 1));

								++voisinCourant;
								++voisinSuivant;
								++nbVoisinsCourants;
							}
						}
						pointsGWR.poids[i].push_back(make_pair(i, 1));

						sommePond = nbVoisinsCourants + 1; // +1 pour le point lui-même
						double poids(1. / sommePond);
						for (int j(0); j < sommePond; ++j)
						{
							pointsGWR.poids[i][j].second = poids;
						}
					}
				}
			}
		}
	}

	// On a la pondération qui tient compte des coordonnées, du marqueur et des variables env
	// Initialisation des calculs
	MatriceBools pointsValides(pointsGWR.masque);  // Liste des points pour lesquels les calculs ont abouti
	MatriceReels liste_beta_hat(nbParam, nbPoints, true, 0); // On garde les beta pour faire des stats
	reel loglikeGWR(0), traceS(0);

	for (int i(0); i < nbPoints; ++i)
	{
		if (pointsGWR.masque(i, 0) == false)
		{
			continue;
		}

		tableauNoirLocal.X = 0;
		tableauNoirLocal.Y = 0;

		// Copie des valeurs dans  X et dans Y
		// On a déjà tenu compte des voisins géographiques à exclure du calcul avec masqueGWR dans la création de la pondération
		int nbVoisins(pointsGWR.poids[i].size());
		/*for (int compteur(0); compteur<nbVoisins; ++compteur)
		 {
		 cout << "*"<< ponderationCourante[i][compteur].first << " "<<ponderationCourante[i][compteur].second << endl;
		 }
		 for (int compteur(0); compteur<ponderation[i].size(); ++compteur)
		 {
		 cout << "!"<< ponderation[i][compteur].first << " "<<ponderation[i][compteur].second << endl;
		 }*/

		int voisinCourant(0);
		MatriceReels listePoids(nbVoisins, 1, true, 0);    // Sert à multiplier les pi * (1-pi) dans la boucle
		for (int k(0); k < nbVoisins; ++k)
		{
			voisinCourant = pointsGWR.poids[i][k].first;
			listePoids(k, 0) = pointsGWR.poids[i][k].second;
			//if (masqueGWR(voisinCourant,0) == true)
			//{
			tableauNoirLocal.X(k, 0) = 1;
			set<int>::iterator iter(varContinues.begin());
			for (int l(0); l < nbVarCont; ++l)
			{
				tableauNoirLocal.X(k, l + 1) = dataEnv(voisinCourant, *iter);
				++iter;
			}
			tableauNoirLocal.Y(k, 0) = dataMarq(voisinCourant, numMarq);
			//			}

		}

		//cout << "!" << numMarq << " " << i << " " << nbVoisins; //<< " " << X_l << " " << Y_l << endl;

		if (sum(tableauNoirLocal.Y) == 0 || sum(tableauNoirLocal.Y) == pointsGWR.taille)    // Y est constant
		{
			cout << "P " << i << " " << 5 << " " << sum(tableauNoirLocal.Y) << t(tableauNoirLocal.X(0, 1, nbVoisins - 1, 1)) << endl;

			recalculeLoglikeGlobale = true;
			pointsValides(i, 0) = false;
			continue;
		}

		// Initialisation de beta_hat_local, on prend l'estimation globale
		tableauNoirLocal.beta_hat = tableauNoir.beta_hat;

		int limiteLignes(nbVoisins - 1), limiteCols(nbParam - 1);

		// Test de convergence
		bool continueCalcul(true), singularMatrix(false), divergentCalculation(false);
		int nbIterations(0), typeErreur(0);
		int limiteIterGWR(40);

		// Iteration
		while (continueCalcul && !singularMatrix && !divergentCalculation && (nbIterations < limiteIterGWR))
		{
			cout << "%" << i << " " << nbIterations << " " << sum(tableauNoirLocal.nouv_Xb(0, 0, limiteLignes, 0)) << " " << (tableauNoirLocal.J_info)[2, 2] << " " << tableauNoirLocal.beta_hat << endl;
			// Calcul pi
			//cout << X_l(0,0,limiteLignes,limiteCols)<< beta_hat_l << X_l(0,0,limiteLignes,limiteCols) * beta_hat_l;
			tableauNoirLocal.nouv_Xb(0, 0, limiteLignes, 0) = (tableauNoirLocal.X(0, 0, limiteLignes, limiteCols) * tableauNoirLocal.beta_hat)(0, 0, limiteLignes, 0); // Test avant l'exp

			if (max(tableauNoirLocal.nouv_Xb(0, 0, limiteLignes, 0)) > limiteExp)
			{
				continueCalcul = false;
				//loglikeCourante = sum((Y_l%(Xb_l) - log(1.+exp_Xb_l))(0,0,nbVoisinsValides-1,0));
				typeErreur = 1;

			}
			else
			{
				++nbIterations;

				// Calcul pi
				tableauNoirLocal.Xb(0, 0, limiteLignes, 0) = tableauNoirLocal.nouv_Xb(0, 0, limiteLignes, 0);
				tableauNoirLocal.exp_Xb(0, 0, limiteLignes, 0) = exp(tableauNoirLocal.Xb(0, 0, limiteLignes, 0));

				//cout << nbIterations << "\n" << beta_hat << "\n" << Xb << "\n" << exp_Xb << "\n";

				tableauNoirLocal.pi_hat(0, 0, limiteLignes, 0) = tableauNoirLocal.exp_Xb(0, 0, limiteLignes, 0) / (1 + tableauNoirLocal.exp_Xb(0, 0, limiteLignes, 0));
				//cout << endl << pi_hat_l(0,0,limiteLignes,0) << endl<< endl;
				/*if (nbIterations==26)
				 {
				 cout << exp_Xb << "\n" << pi_hat << "\n";
				 }*/

				//cout << pi_hat<< "\n";
				// Calcul ni * pi * (1 - pi)
				tableauNoirLocal.interm(0, 0, limiteLignes, 0) = (tableauNoirLocal.pi_hat(0, 0, limiteLignes, 0) % (1 - tableauNoirLocal.pi_hat(0, 0, limiteLignes, 0))) % listePoids(0, 0, limiteLignes, 0);

				// Calcul des scores U
				tableauNoirLocal.intermScores(0, 0, limiteLignes, 0) = (tableauNoirLocal.Y(0, 0, limiteLignes, 0) - tableauNoirLocal.pi_hat(0, 0, limiteLignes, 0)) % listePoids(0, 0, limiteLignes, 0);
				//cout << "ç" << endl << intermScores_l(0,0,limiteLignes,0) << "ç" << X_l(0,0,limiteLignes,limiteCols) << endl;
				for (int k(0); k < nbParam; ++k)
				{
					//cout << "£"<< intermScores_l(0,0,limiteLignes,0) % X_l(0,k,limiteLignes,k) << endl;
					tableauNoirLocal.scores(k, 0) = sum(tableauNoirLocal.intermScores(0, 0, limiteLignes, 0) % tableauNoirLocal.X(0, k, limiteLignes, k));
					//cout<< "&" << intermScores_l(0,0,1,0) << " " << X_l(0,k,1,k) << " " << scores_l(k,0) << endl;
				}

				// Calcul de J, qui est symétrique
				for (int k(0); k < nbParam; ++k)
				{
					tableauNoirLocal.J_info(k, k) = sum(tableauNoirLocal.interm(0, 0, limiteLignes, 0) % tableauNoirLocal.X(0, k, limiteLignes, k) % tableauNoirLocal.X(0, k, limiteLignes, k));
					for (int l(k + 1); l < nbParam; ++l)
					{
						tableauNoirLocal.J_info(k, l) = sum(tableauNoirLocal.interm(0, 0, limiteLignes, 0) % tableauNoirLocal.X(0, k, limiteLignes, k) % tableauNoirLocal.X(0, l, limiteLignes, l));
						tableauNoirLocal.J_info(l, k) = tableauNoirLocal.J_info(k, l);
					}
				}

				//cout << "# " << nbIterations << " " << min(nouv_Xb_l) << " " << max(nouv_Xb_l) << " " << exp(max(nouv_Xb_l)) << " " << max(exp_Xb_l)<<  "\n" << scores_l << "\n" <<J_info_l<< "\n";

				/*if ( det(J_info)<=0)
				 {
				 singularMatrix = true;
				 }
				 else */
				{
					// Calcul de l'inverse de J
					try
					{
						tableauNoirLocal.inv_J_info = invpd(tableauNoirLocal.J_info);
					}
					catch (scythe_exception& error)
					{
						// cerr << error.message() << "\n";
						singularMatrix = true;
						continueCalcul = false;
						typeErreur = 2;
					}

					/*			for (int m(0); m<nbParam; ++m)
					 {
					 if (inv_J_info(m, m) <= 0)
					 cerr << i << " " << j << " " << inv_J_info << "\n";
					 }
					 */

					if (!singularMatrix)
					{
						// Mise à jour de beta_hat
						tableauNoirLocal.nouv_beta_hat = tableauNoirLocal.beta_hat + tableauNoirLocal.inv_J_info * tableauNoirLocal.scores;

						//cout << inv_J_info_l << det(J_info_l) << "\n" << nouv_beta_hat_l << "\n" << "\n";

						for (int l(0); l < nbParam; ++l)
						{
							if (abs(tableauNoirLocal.nouv_beta_hat(l, 0)) > limiteNaN)
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
							tableauNoirLocal.diff_beta_hat = tableauNoirLocal.nouv_beta_hat - tableauNoirLocal.beta_hat;
							continueCalcul = false;
							for (int l(0); l < nbParam; ++l)
							{
								if (abs(tableauNoirLocal.diff_beta_hat(l, 0)) > convCrit * max(eps, abs(tableauNoirLocal.beta_hat(l, 0))))
								{
									continueCalcul = true;
									break;
								}
							}

							tableauNoirLocal.beta_hat = tableauNoirLocal.nouv_beta_hat;
						}
					}
					//cout << nbIterations << "\n" << beta_hat << "\n";
				}
				// Si le calcul est terminé, on calcule l'indice d'Effron
				//if (!continueCalcul) {
				//cout << nbIterations << "\n" << beta_hat << "\n";

				//double ratio(sum(Y)*1.0 / taille);
				//cout << ratio << "\n";
				//loglikelihood = sum(Y%log(pi_hat) + (1-Y)%log(1-pi_hat));
				//loglikeCourante += sum((Y%(Xb) - log(1.+exp_Xb))(0,0,taille-1,0));
				//}
			}
		}
		if (nbIterations == limiteIterGWR)
		{
			typeErreur = 4;
		}

		if (typeErreur > 0)
		{
			cout << "PQ " << i << " " << typeErreur << " " << t(tableauNoirLocal.nouv_beta_hat) << " " << t(tableauNoirLocal.beta_hat) << endl << t(listePoids(0, 0, limiteLignes, 0)) << endl;
			pointsValides(i, 0) = false;
			recalculeLoglikeGlobale = true;

		}
		else
		{
			//Copie des betas
			liste_beta_hat(_, i) = tableauNoirLocal.beta_hat;
			MatriceReels invXtWX(nbParam, nbParam, true, 0), XtW(nbParam, nbVoisins);
			for (int j(0); j < nbVoisins; ++j)
			{
				XtW(_, j) = t(tableauNoirLocal.X(j, _)) * tableauNoir.interm(j, 0);
			}
			try
			{
				invXtWX = invpd(XtW * tableauNoirLocal.X(0, 0, limiteLignes, nbParam - 1));
			}
			catch (scythe_exception& error)
			{
				pointsValides(i, 0) = false;
				recalculeLoglikeGlobale = true;
				continue;
			}
			// Le point lui-même est le dernier de la liste
			//cout << "^" << i << " " << invXtWX.rows() << " " << invXtWX.cols() << " " << (Y_l(limiteLignes,0,limiteLignes,0)%(Xb_l(limiteLignes,0,limiteLignes,0)) - log(1.+exp_Xb_l(limiteLignes,0,limiteLignes,0)))<< endl;
			traceS += (tableauNoirLocal.X(limiteLignes, 0, limiteLignes, limiteCols) * ((invXtWX * XtW)))(0, limiteLignes);
			// Le point i est la dernière ligne
			loglikeGWR += sum(listePoids(0, 0, limiteLignes, 0) % (tableauNoirLocal.Y(limiteLignes, 0, limiteLignes, 0) % (tableauNoirLocal.Xb(limiteLignes, 0, limiteLignes, 0)) - log(1. + tableauNoirLocal.exp_Xb(limiteLignes, 0, limiteLignes, 0))));

		}
	}
	reel loglikeGlobale(resultat.second[valloglikelihood]);
	if (recalculeLoglikeGlobale)
	{
		loglikeGlobale = 0;
		int indiceVariablesModeleGlobal(0);

		for (int i(0); i < nbPoints; ++i)
		{
			if (pointsValides(i, 0))
			{
				//On fait le calcul avec la variable du modèle global
				loglikeGlobale += tableauNoir.Y(indiceVariablesModeleGlobal, 0) * tableauNoir.Xb(indiceVariablesModeleGlobal, 0) - log(1 + tableauNoir.exp_Xb(indiceVariablesModeleGlobal, 0));
				++indiceVariablesModeleGlobal;
			}
			else if (masque(i, 0))
			{
				++indiceVariablesModeleGlobal;
			}

		}
	}
	int nbPointsValides(toolbox::sommeNumerique(pointsValides));
	cout << " " << nbPointsValides << endl;

	for (int i(0); i < nbPoints; ++i)
	{
		cout << i << " " << liste_beta_hat(0, i) << " " << liste_beta_hat(1, i) << endl;
	}
	if (nbPointsValides > 0)
	{
		//On peut calculer AIC
		reel nbParamCorrige((1. * taille * nbParam) / (taille - nbParam - 1)), scoreAIC(2 * ((-loglikeGWR + traceS) / nbPointsValides + (loglikeGlobale - nbParamCorrige) / taille));
		//reel pvalue(pchisq(scoreAIC, traceS-nbParamCorrige));

		MatriceReels resumeBeta(nbParam, 2, true, 0);
		// Calcul des moyennes et ecarts-types beta
		for (int i(0); i < nbParam; ++i)
		{
			resumeBeta(_, 0) += liste_beta_hat(_, i);
			resumeBeta(_, 1) += liste_beta_hat(_, i) % liste_beta_hat(_, i);
		}
		resumeBeta(_, 0) /= nbPointsValides;
		resumeBeta(_, 1) = resumeBeta(_, 1) / nbPointsValides - resumeBeta(_, 0) % resumeBeta(_, 0);
		//copie des résultats

		resultat.second[tailleResDepart] = nbPointsValides;
		resultat.second[tailleResDepart + 1] = loglikeGlobale;
		resultat.second[tailleResDepart + 2] = loglikeGWR;
		resultat.second[tailleResDepart + 3] = traceS;
		resultat.second[tailleResDepart + 4] = -loglikeGWR + traceS;
		resultat.second[tailleResDepart + 5] = scoreAIC;
		//resultat.second[tailleResDepart+6] = pvalue;
		for (int i(0); i < nbParam; ++i)
		{
			resultat.second[tailleResDepart + nbStatsGWRbase + 2 * i] = resumeBeta(i, 0);
			resultat.second[tailleResDepart + nbStatsGWRbase + 2 * i + 1] = resumeBeta(i, 1);
		}
	}
	else
	{
		resultat.second[tailleResDepart + nbStatsGWR - 1] = 1;
	}

}
