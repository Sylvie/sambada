#include "RegressionLogistique.h"
#include "optimize.h"
//#include "shapelib-1.3.0/shapefil.h"
#include "shapefil.h"
#include <limits>
#include <list>
#include <ctime>

using namespace std;
using namespace scythe;


RegressionLogistique::RegressionLogistique()
:dataEnv(0, 0), missingValuesEnv(0), dataMarq(0, 0), missingValuesMarq(0), existeColID(false), /*headerEnv(0), headerMarq(0),*/
sauvegardeTempsReel(true), selModeles(all),
analyseSpatiale(false), longitude(0), latitude(0), choixPonderation(pondDistanceMax), bandePassante(0), AS_nbPermutations(0), nbPlusProchesVoisins(0),
eps(sqrt(epsilon < reel > ())), convCrit(1e-6), seuilPValeur(0.01), seuilScore(0), seuilScoreMultivarie(0), limiteNaN(1000000), limiteExp(min((reel)700,log(numeric_limits < reel >::max()/2))),
limiteIter(100), limiteEcartType(7), nbPseudosRcarres(7), nbStats(11), nbStatsSansPseudos(4),
tailleEtiquetteInvar(4), numPremierMarq(0),
delimLignes("\n")
{}

RegressionLogistique::~RegressionLogistique()
{}

bool RegressionLogistique::calculePonderation() throw(Erreur)
{
	if (!analyseSpatiale)
	{
		throw Erreur("MSG_noSpatialAnalysis", "No spatial analysis requested", false);
	}
	else 
	{
		// calcul des distances
		const reel rayonTerre(6378137), rayonTerreCarre(rayonTerre*rayonTerre), carreBandePassante(bandePassante*bandePassante); // en mètres
		// Il faut les coordonnées en radians
		// crdBrutes= telles que stockées dans les var env (actives ou sup)
		// crd = crd en radian
		coordonneesBrutes.resize(nbPoints, 2);
		coordonneesBrutes=0;
		coordonnees.resize(nbPoints, 2);
		coordonnees=0;
		MatriceBools masqueCrd(nbPoints,2, true, true);
		
		pointsGeo.masque.resize(nbPoints,1);
		pointsGeo.masque=true;
		int indiceLongitude(specDataEnv[longitude].localIndex), indiceLatitude(specDataEnv[latitude].localIndex);
		istringstream lu;
		lu.precision(toolbox::precisionLecture);
		reel valeur(0);
		string dustbin("");
		
		// Longitude
		if (specDataEnv[longitude].isActive) // Cas simple, il suffit de copier les données
		{
			coordonneesBrutes(_, 0) =dataEnv(_, indiceLongitude);
			masqueCrd(_, 0) = masqueX(_, indiceLongitude);
		}
		else	// Si la variable est passive, on doit récupérer les valeurs qui sont des strings
		{
			for (int i(0); i<nbPoints; ++i)
			{
				lu.clear();
				lu.precision(toolbox::precisionLecture);
				lu.str(dataSupEnv(i, indiceLongitude));
				lu >> valeur;
				if(lu.fail())
				{
					lu.clear();
					lu>>dustbin;
					// La coordonnée vaut 0 par défaut, pas besoin de la modifier
					masqueCrd(i, 0)=false;	// Mise à jour du masque
				}
				else 
				{
					coordonneesBrutes(i, 0)=valeur;
				}
			}
		}
		
		// Latitude
		if (specDataEnv[latitude].isActive) // Cas simple, il suffit de copier les données
		{
			coordonneesBrutes(_, 1) =dataEnv(_, indiceLatitude);
			masqueCrd(_, 1) = masqueX(_, indiceLatitude);
		}
		else	// Si la variable est passive, on doit récupérer les valeurs qui sont des strings
		{
			for (int i(0); i<nbPoints; ++i)
			{
				lu.clear();
				//	cout << i << " " << dataSupEnv(i, indiceLatitude).size() << " " << setprecision(toolbox::precisionLecture)<< dataSupEnv(i, indiceLatitude) << "\n";
				lu.precision(toolbox::precisionLecture);
				lu.str(dataSupEnv(i, indiceLatitude));
				lu >> valeur;
				if(lu.fail())
				{
					lu.clear();
					lu>>dustbin;
					// La coordonnée vaut 0 par défaut, pas besoin de la modifier
					masqueCrd(i, 1)=false;	// Mise à jour du masque
				}
				else 
				{
					coordonneesBrutes(i, 1)=valeur;
				}
			}
		}		
		//cout.precision(22);
		//cout << coordonnees(25,0,29,1) << "\n";
		
		// Calcul du masque total
		pointsGeo.masque = masqueCrd(_,0) % masqueCrd(_,1);
		
		// On repère tous les points ayant des coordonnées valides
		pointsGeo.miseAJour();
		
		// Coordonnees sphériques -> Transformation en radians
		if (!crdCartesiennes) 
		{
			coordonnees(_, 0) = coordonneesBrutes(_,0) * M_PI / 180.0;
			coordonnees(_, 1) = coordonneesBrutes(_,1) * M_PI / 180.0;
		}
		else 
		{
			coordonnees=coordonneesBrutes;
		}
		
		distances.resize(nbPoints, nbPoints);
		distances=0;
		
		// Pour les points n'ayant pas de crd valides, la distance avec les autres points est considérée comme nulle
		// Leur pondération est également nulle, ce qui garantit la cohérence des résultats
		
		reel deltaLong, deltaLat, cosLatMoy;
		int pt1, pt2; // Pour repérer les indices des points considérés
		for (int i(0); i<pointsGeo.taille; ++i)
		{
			pt1 = pointsGeo.pointsValides[i];
			for (int j(i+1); j<pointsGeo.taille; ++j)
			{
				pt2 = pointsGeo.pointsValides[j];
				deltaLong = coordonnees(pt1, 0) - coordonnees(pt2, 0);
				deltaLat=coordonnees(pt1, 1) - coordonnees(pt2, 1);
				
				if (crdCartesiennes)
				{
					distances(pt1, pt2) = (deltaLat*deltaLat + deltaLong*deltaLong);
					
				}
				else
				{
					cosLatMoy= cos((coordonnees(pt1, 1) + coordonnees(pt2, 1))/2);
					distances(pt1, pointsGeo.pointsValides[j]) = rayonTerreCarre*(deltaLat*deltaLat + deltaLong*deltaLong*cosLatMoy*cosLatMoy);
				}
				distances(pt2, pt1) = distances(pt1, pt2);
				
			}
			
		}
		
		//cout << distances(0,0,5,5)<<"\n";
		
		// Pour la pondération, on prend w_ii = 0, un point n'est pas son propre voisin (pour l'autocorrélation)
		// Pour la GWR, le point sera rajouté parmi ses voisins
		// Les pondérations seront normées plus tard (pour tenir compte des valeurs manquantes)
		if (choixPonderation==pondDistanceMax) 
		{
			for (int i(0); i<pointsGeo.taille; ++i)
			{
				pt1 = pointsGeo.pointsValides[i];
				for (int j(i+1); j<pointsGeo.taille; ++j)
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
		else if (choixPonderation==pondGaussienne)
		{
			double pondCourante(0);
			for (int i(0); i<pointsGeo.taille; ++i)
			{
				pt1 = pointsGeo.pointsValides[i];
				for (int j(i+1); j<pointsGeo.taille; ++j)
				{
					pt2 = pointsGeo.pointsValides[j];
					//if (distances(pt1,pt2)<=carreBandePassante)	// Tronquage pour comparaison avec pysal
					//{
					pondCourante=exp(-distances(pt1,pt2)/(2*carreBandePassante));
					//cout << "*"<< i << " " << j << " " << pondCourante << endl;
					pointsGeo.poids[pt1].push_back(make_pair(pt2, pondCourante));
					pointsGeo.poids[pt2].push_back(make_pair(pt1, pondCourante));
					//}
				}
				// Auto-voisinage pour comparaison avec pysal
				//pointsGeo.poids[pt1].push_back(make_pair(pt1, 1));
				
			}
		}
		else if(choixPonderation==pondBicarree)
		{
			double pondCourante(0);
			for (int i(0); i<pointsGeo.taille; ++i)
			{
				pt1 = pointsGeo.pointsValides[i];
				for (int j(i+1); j<pointsGeo.taille; ++j)
				{
					pt2 = pointsGeo.pointsValides[j];
					
					if (distances(pt1, pt2)<carreBandePassante)
					{
						
						pondCourante=pow( 1-(distances(pt1,pt2)/carreBandePassante), 2);
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
			for (int i(0); i<pointsGeo.taille; ++i)
			{
				pt1 = pointsGeo.pointsValides[i];
				
				for (int j(i+1); j<pointsGeo.taille; ++j)
				{
					pt2=pointsGeo.pointsValides[j];
					voisinage[pt1].push_back(make_pair(pt2, distances(pt1, pt2)));
					voisinage[pt2].push_back(make_pair(pt1, distances(pt1, pt2)));
				}
				
				sort(voisinage[pt1].begin(), voisinage[pt1].end(), ComparaisonVoisins::plusPetitQue);
				/*for (vector<Voisin>::iterator iter(voisinage[pt1].begin()); iter!=voisinage[pt1].end(); ++iter)
				 {
				 cout << "*"<< pt1+1 << " " << (iter->first)+1 << " " << iter->second << " " << sqrt(iter->second)<< "\n";
				 }*/
			}
			
			double poids(1.); // On n'a normé personne pour le moment (et la norme change suivant le cas AC / GWR)
			//				double poids(1./nbPointsValides);
			
			// Cas où tous les points valides sont voisins
			if (pointsGeo.taille<=(nbPlusProchesVoisins+1))
			{
				for (int i(0); i<pointsGeo.taille; ++i)
				{
					pt1 = pointsGeo.pointsValides[i];
					
					for (int j(i+1); j<pointsGeo.taille; ++j)
					{
						//cout << i<< " " << j << endl;
						pt2=pointsGeo.pointsValides[j];
						voisinage[pt1].push_back(make_pair(pt2, poids));
						voisinage[pt2].push_back(make_pair(pt1, poids));
					}
					for (voisinCourant=voisinage[pt1].begin(); voisinCourant!=voisinage[pt1].end(); ++voisinCourant)
					{
						pointsGeo.poids[pt1].push_back(make_pair(voisinCourant->first, poids));						
					}
				}	
			}
			else
			{
				// On sélectionne les points voisins
				// Il peut y en avoir plus que "nbPlusProchesVoisins" si certains points sont à égale distance
				
				//cout << "! " << distances(9,0,9,384) << "\n";
				reel plusPetiteDiffDist(pow(10.,-4)); // En dessous d' 1cm ça compte pas! (Les distances sont au carré)
				for (int i(0); i<pointsGeo.taille; ++i)
				{
					pt1 = pointsGeo.pointsValides[i];
					nbVoisinsCourants=nbPlusProchesVoisins;
					voisinSuivant=voisinage[pt1].begin()+(nbPlusProchesVoisins); // Premier point hors limite
					voisinCourant=voisinage[pt1].begin()+(nbPlusProchesVoisins-1); //dernier point à être pris automatiquement
					while(abs((voisinSuivant)->second - voisinCourant->second) < plusPetiteDiffDist) 
					{
						++voisinCourant;
						++voisinSuivant;
						++nbVoisinsCourants;
					}
					//					double poids(1./nbVoisinsCourants);
					for (voisinCourant=voisinage[pt1].begin(); voisinCourant!=voisinSuivant; ++voisinCourant)
					{
						pointsGeo.poids[pt1].push_back(make_pair(voisinCourant->first, poids));						
					}
				}
			}
		}
		//cout.precision(toolbox::precisionLecture);
		//cout << coordonnees(0,0,10,1) << "\n" << "\n" <<sqrt( distances(0,0,10,10)) << " " << pointsGeo.poids.size()<< " "<<pointsGeo.poids[0].size() << " " << pointsGeo.poids[384].size() <<"\n" ;
		for (int chose(0); chose<pointsGeo.poids[0].size(); ++chose)
		{
			cout << pointsGeo.poids[0][chose].first << " " <<  pointsGeo.poids[0][chose].second<< "\n";
		}
		
		
	}	
}

bool RegressionLogistique::Domaine::miseAJour()
{
	if (masque.cols()!=1)
	{
		return false;
	}
	else
	{
		int ancienNbPoints(nbPoints);
		nbPoints = masque.rows();
		
		// Le poids est réinitialisé
		poids.clear();
		poids.resize(nbPoints, vector< Voisin > (0));
		
		pointsValides.clear();
		indices.resize(nbPoints);
		int indiceCourant(0);
		for (int i(0); i<nbPoints; ++i)
		{
			if (masque(i,0)) // Si le point est valide
			{
				pointsValides.push_back(i);
				indices[i]=indiceCourant;
				++indiceCourant;
			}
			else 
			{
				indices[i]=-1; // Les points invalides n'ont pas d'indice local
			}
		}
		taille=indiceCourant;
		return true;
	}
}

int RegressionLogistique::calculeCorrelations() const
{
	int nbPaires(nbEnv * (nbEnv-1) / 2.);
	Matrix<> taillesDonnees(toolbox::sommeNumeriqueCol(masqueX));
	
	// Calcul des moyennes et variances
	Matrix<> moyennes(sumc(dataEnv)), variances(sumc(dataEnv%dataEnv));
	moyennes /= taillesDonnees;
	variances = variances / taillesDonnees - moyennes % moyennes;
	
	vector< vector <reel> > correlations(0);
	vector< reel > correlationCourante(3);
	
	for (int i(0); i< nbEnv; ++i)
	{
		correlationCourante[0] = i;
		for (int j(i+1); j<nbEnv; ++j)
		{
			correlationCourante[1] = j;
			correlationCourante[2] = ( sum(dataEnv(_, i)%dataEnv(_, j))/toolbox::sommeNumerique(masqueX(_, i)%masqueX(_, j)) - moyennes[i]*moyennes[j] ) / sqrt(variances[i] * variances[j]) ;
			correlations.push_back(correlationCourante);
			
			//cout << i << " " << j << " " << correlationCourante[2] << "\n";
		}
	}
	
	for (int i(0); i<nbPaires; ++i)
	{
		cout  <<specDataEnv[ correlations[i][0]].name<<"-"<<specDataEnv[correlations[i][1]].name << " ";
	}
	cout <<endl;
	for (int i(0); i<nbPaires; ++i)
	{
		cout  << correlations[i][2] << " ";
	}
	cout << endl << CLOCKS_PER_SEC << endl;
	
	ComparaisonVecteurs::setCase(0);
	sort(correlations.begin(), correlations.end(), ComparaisonVecteurs::plusPetitQue<reel>);
	
	cout << correlations.size() << " " << nbPaires << "\n";
	for (int i(0); i<nbPaires; ++i)
	{
		//		cout << correlations[i][0] << " " << correlations[i][1] << " " << correlations[i][2] << "\n";
	}
	return 0;
}

// La pondération est normée!
int RegressionLogistique::calculeAutocorrelations() throw(Erreur)
{
	try
	{
		
		if (!analyseSpatiale || (!AS_autocorrVarEnv && !AS_autocorrMarq))
		{
			throw Erreur("MSG_noAutocorr", "No autocorrelation computation requested", false);
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
			cout << err.what() << "\n";
		}
		return 1;
	}
	//cout << ponderation<<"\n";
	
	//****
	// Initialisation du shapefile
	SHPHandle fichierSHP;
	DBFHandle fichierDBF;
	vector< int > indicesShpEnv, indicesShpMarq, indicesShpPoints;
	if (AS_shapefile)
	{
		indicesShpEnv.resize(nbEnvActives, -1);
		indicesShpMarq.resize(nbMarqActifs, -1);
		indicesShpPoints.resize(nbPoints);
		fichierSHP = SHPCreate(nomFichierResultats.first.c_str(), SHPT_POINT);
		fichierDBF = DBFCreate(nomFichierResultats.first.c_str());
		
		int precision(toolbox::precisionLecture), nbDecimales(precision/3);
		
		// Création des colonnes Env
		if (AS_autocorrVarEnv && nbEnvActives>0)
		{
			for (int i(0); i<nbEnvActives; ++i)
			{
				indicesShpEnv[i] = DBFAddField(fichierDBF, specDataEnv[varEnvActives[i]].name.c_str(), FTDouble, precision, nbDecimales);
			}
		}
		
		// Création des colonnes Marq
		if (AS_autocorrMarq && nbMarqActifs>0)
		{
			for (int i(0); i<nbMarqActifs; ++i)
			{
				indicesShpMarq[i] = DBFAddField(fichierDBF, specDataMarq[marqActifs[i]].name.c_str(), FTDouble, precision, nbDecimales);
			}
		}
		
		// Création de la couche vectorielle
		double x(0), y(0);		
		for (int i(0); i<nbPoints; ++i)
		{
			// Si le point n'a pas de coordonnées valides, on ajoute un point nul dans le shp 
			// Cela garantit la correspondance des données avec le dbf:
			// Un point manquant à cause des crd ou à cause de valeurs manquantes est traité de la même façon durant le calcul d'AC
			if (pointsGeo.masque(i,0))
			{
				x=coordonneesBrutes(i,0);
				y=coordonneesBrutes(i,1);
				
				SHPObject* obj(SHPCreateSimpleObject(SHPT_POINT, 1, &x, &y, NULL));
				indicesShpPoints[i]=SHPWriteObject(fichierSHP, -1, obj);
				SHPDestroyObject(obj);
			}
			else	
			{
				SHPObject* obj(SHPCreateSimpleObject(SHPT_NULL, 0, NULL, NULL, NULL));
				indicesShpPoints[i]=SHPWriteObject(fichierSHP, -1, obj);
				SHPDestroyObject(obj);
			}
		}
	}
	//****
	
	MatriceReels deviations(nbPoints,1), deviationsCourantes(nbPoints, 1), autocorrLocale, autocorrGlobale, 
	autocorrLocaleCourante, pValeurGlobale, pValeurLocale, historiqueAutocorrGlobale,
	autocorrTemp, autocorrTempCourante;
	
	// pointsAC : pondération-type où tous les points ayant des crd sont valides
	Domaine pointsAC, pointsCourants;
	//TableClassementsVoisins ponderationCourante(nbPoints);
	
	reel facteurEchelleLocal(0), facteurEchelleGlobal(0), moyenne(0), sommeCarresDeviations(0), autocorrGlobaleCourante(0);
	
	// Test de multiplication
	
	vector<int> listePointsValidesPerm, voisinagePerm, listePointsGlobauxPerm; // Liste des points sélectionnables lors de la permutations, voisinage temporaire pendant la permutation
	int pt1, pt2, nbVoisins; // Pour repérer les indices des points considérés || Pour compter les voisins d'un point
	reel limiteIndicesIdentiques(1e-6), moitieNbPerms(AS_nbPermutations/2);
	int nbSimPlusGrandes, nbSimEgales, nbSimPlusPetites;
	
	// On calcule une pondération-type basée sur le masqueGeo pour les cas où tous les points ayant des crd sont valides
	pointsAC=pointsGeo;
	reel sommePond(0);
	for (int i(0); i<nbPoints; ++i)
	{
		if (pointsAC.masque(i,0))			
		{
			sommePond=0;
			// Si un point est valide, tous ses voisins sont considérés, vu qu'on ne compte que le masque géo
			nbVoisins=pointsAC.poids[i].size();
			for (int j(0); j<nbVoisins; ++j)
			{
				sommePond += pointsAC.poids[i][j].second;	
			}
			if (sommePond>0)
			{
				for (int j(0); j<nbVoisins; ++j)
				{
					pointsAC.poids[i][j].second /= sommePond;	
				}
			}
		}
	}
	
	//cout << distances << endl;
	//toolbox::affiche(voisinage);
	//toolbox::affiche(pointsGeo.poids);
	//toolbox::affiche(pointsAC.poids);
	
	ofstream sortieAS;
	
	//Initialisation de la graine du générateur pour les permutation
	srand(time(NULL));
	
	time_t t1(time(NULL));
	// Variables environnementales
	if (AS_autocorrVarEnv && nbEnvActives>0)
	{
		autocorrLocale.resize(nbPoints, nbEnvActives);
		autocorrLocale=0;
		autocorrLocaleCourante.resize(nbPoints, 1);
		autocorrLocaleCourante=0;
		pValeurLocale.resize(nbPoints, nbEnvActives);
		pValeurLocale=0;
		
		if (AS_autocorrGlobale)
		{
			autocorrGlobale.resize(1, nbEnvActives);
			autocorrGlobaleCourante=0;
			pValeurGlobale.resize(1, nbEnvActives);
			pValeurGlobale=0;
			historiqueAutocorrGlobale.resize(AS_nbPermutations, nbEnvActives);
		}
		
		for (int i(0); i<nbEnvActives; ++i)
		{
			pointsCourants.masque=pointsAC.masque%masqueX(_,i);
			pointsCourants.taille=toolbox:: sommeNumerique(pointsCourants.masque);
			if (pointsCourants.taille==0)
			{
				continue;
			}
			else if (pointsCourants.taille==pointsAC.taille)	// Cas où tous les points géographiquement valides ont une valeur env
			{
				pointsCourants=pointsAC;
			}
			else
			{
				pointsCourants.miseAJour();	// La liste des points valides, la pondération, etc, sont réinitialisées
				
				// Calcul de la pondératon
				if (choixPonderation!=pondPlusProchesVoisins)
				{
					double somme(0);
					for (int u(0); u<pointsCourants.taille; ++u)
					{
						pt1=pointsCourants.pointsValides[u];
						somme=0;
						nbVoisins=pointsAC.poids[pt1].size();
						for (int v(0); v<nbVoisins; ++v)
						{
							// On copie le voisin s'il est lui-même valide
							if(pointsCourants.masque(pointsAC.poids[pt1][v].first,0))
							{
								pointsCourants.poids[pt1].push_back(pointsAC.poids[pt1][v]);
								//cout << "*"<< pointsAC.poids[pt1][v].first << " " << pointsAC.poids[pt1][v].second << endl;
								somme += (pointsAC.poids[pt1][v].second);
							}
						}
						nbVoisins=pointsCourants.poids[pt1].size();
						if (somme != 0)
						{
							for (int v(0); v<nbVoisins; ++v)
							{
								pointsCourants.poids[pt1][v].second /= somme;
								
							}
						}
					}
				}
				else	// Cas plus proches voisins
				{
					// Cas où tous les points valides sont voisins
					// On se base sur les points courants valides -> pas besoin de revérifier leur validité
					if (pointsCourants.taille<=(nbPlusProchesVoisins+1))
					{
						double poids(1./pointsCourants.taille);
						for (int u(0); u<pointsCourants.taille; ++u)
						{						
							pt1=pointsCourants.pointsValides[u];
							for (int v(u+1); v<pointsCourants.taille; ++v)
							{
								pt2=pointsCourants.pointsValides[v];
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
						
						//cout << "! " << distances(9,0,9,384) << "\n";
						for (int u(0); u<pointsCourants.taille; ++u)
						{
							pt1 = pointsCourants.pointsValides[u];
							nbVoisins=0;
							voisinCourant=voisinage[pt1].begin();
							while (nbVoisins<nbPlusProchesVoisins && voisinCourant!=voisinage[pt1].end())
							{
								if (pointsCourants.masque(voisinCourant->first,0))
								{
									pointsCourants.poids[pt1].push_back(make_pair(voisinCourant->first, 1));
									++nbVoisins;
								}
								++voisinCourant;
							}
							
							if (voisinCourant!=voisinage[pt1].end())
							{
								voisinSuivant=voisinCourant+1;
								while (voisinSuivant!=voisinage[pt1].end() && ((voisinSuivant)->second == voisinCourant->second))
								{
									if(pointsCourants.masque(voisinSuivant->first,0))
									{
										pointsCourants.poids[pt1].push_back(make_pair(voisinSuivant->first, 1));
										++nbVoisins;									
									}
									++voisinCourant;
									++voisinSuivant;
								}
								
							}
							
							double poids(1./nbVoisins);
							for (voisinCourant=pointsCourants.poids[pt1].begin(); voisinCourant!=pointsCourants.poids[pt1].end(); ++voisinCourant)
							{
								voisinCourant->second=poids;						
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
			moyenne=0;
			sommeCarresDeviations=0;
			for (int k(0); k<nbPoints; ++k)
			{
				if (pointsCourants.masque(k,0))
				{
					deviations(k, 0) = dataEnv(k, i);
					moyenne+=dataEnv(k,i);
					sommeCarresDeviations+=dataEnv(k, i)*dataEnv(k, i);
				}
				else 
				{
					deviations(k,0)=0;
				}
				
			}
			
			moyenne/=pointsCourants.taille;
			//				sommeCarresDeviations= sommeCarresDeviations/pointsCourants.taille-moyenne*moyenne;	// ! Variance
			sommeCarresDeviations= sommeCarresDeviations-pointsCourants.taille*moyenne*moyenne;
			
			for(int u(0); u<pointsCourants.taille; ++u)
			{
				deviations(pointsCourants.pointsValides[u])-=moyenne;
			}
			
			
			facteurEchelleLocal=(pointsCourants.taille-1)/sommeCarresDeviations;
			facteurEchelleGlobal=1./(pointsCourants.taille-1);
			
			// On calcule l'autocorrélation manuellement car la matrice des poids est creuse (sparse matrix)
			//autocorrTemp=0;
			double valeurIntermediaire(0);
			for (int  j(0); j<pointsCourants.taille; ++j)
			{
				pt1=pointsCourants.pointsValides[j];
				nbVoisins=pointsCourants.poids[pt1].size();
				valeurIntermediaire=0;
				for (int k(0); k<nbVoisins; ++k)
				{
					valeurIntermediaire+=pointsCourants.poids[pt1][k].second * deviations[pointsCourants.poids[pt1][k].first];
				}
				autocorrLocale(pt1, i) = valeurIntermediaire*deviations[pt1]*facteurEchelleLocal;
			}
			
			// Ecritude des valeurs dans le DBF
			if (AS_shapefile)
			{
				for (int j(0); j<nbPoints; ++j)
				{
					if(pointsCourants.masque(j,0))
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
				//cout << "\n" << autocorrLocale(_,i);
				//cout << tailleDonnees << " " << sum(ponderationCourante) << " " << sum(autocorrLocale(_,i)) << " " << sum(deviations%deviations) << "\n";
				
				autocorrGlobale(0,i)=facteurEchelleGlobal*(sum(autocorrLocale(_,i)));				
				cout << specDataEnv[varEnvActives[i]].name << " " << moyenne << " " << autocorrGlobale(0,i)  << " " << sommeCarresDeviations<< "\n";
			}
			
			
			// Permutations!
			if (AS_autocorrLocale)
			{
				
				// Chaque fois qu'on prend un point, le nombre de points restants (=parmi lesquels on peut tirer un élément) diminue
				// Prop = facteur d'échelle pour le tirage d'un entier entre nbPointsValides, nbPointsValides-1, ..., 2,1,0
				// Subtilité : Le points courant reste fixe, il ne sera jamais choisi comme voisin -> nb voisins possibles = taille-1 !!!!
				vector<double> proportion(pointsCourants.taille-1);
				int choix(0);
				for (int j(0); j<(pointsCourants.taille-1); ++j)
				{
					// Comme 0<=rand<=RAND_MAX, on divise par RM+1 pour éviter les débordements de tableau
					//cout << "!" <<  pointsCourants.taille-1-j << endl;
					proportion[j]=(double)(pointsCourants.taille-1-j)/((double)RAND_MAX+1);
				}
				
				
				// Itération sur les points -> tous les points n'ont pas le même nombre de voisins
				for (int  k(0); k<pointsCourants.taille; ++k)
				{
					nbSimPlusGrandes=0;
					nbSimEgales=0;
					nbSimPlusPetites=0;
					
					pt1 = pointsCourants.pointsValides[k]; // Numéro global du point considéré
					nbVoisins = pointsCourants.poids[pt1].size();
					voisinagePerm.resize(nbVoisins);	// Redimensionnement du voisinage temporaire
					
					for (int j(0); j<AS_nbPermutations; ++j)
					{
						
						// Les pondérations sont fixes, on réordonne les déviations
						listePointsValidesPerm=pointsCourants.pointsValides;
						// L'indice local du point courant est k -> on le permute avec le dernier point pour ne jamais le sélectionner
						swap(listePointsValidesPerm[k], listePointsValidesPerm[pointsCourants.taille-1]);
						
						for (int l(0); l<nbVoisins; ++l)
						{
							choix=floor(rand()*proportion[l]);
							voisinagePerm[l]=listePointsValidesPerm[choix];
							swap(listePointsValidesPerm[choix], listePointsValidesPerm[pointsCourants.taille-2-l]);		// La dernière place est déjà occupée par le point
						}
						
						random_shuffle(voisinagePerm.begin(), voisinagePerm.end());
						// voisinagePerm contient les numéros globaux des voisins temporaires
						
						// On prend la position (et la pondération) du point d'origine et on utilise la valeur du point permuté
						
						//nbVoisins=pointsCourants.poids[pt1].size();
						valeurIntermediaire=0;
						for (int l(0); l<nbVoisins; ++l)
						{
							valeurIntermediaire+=pointsCourants.poids[pt1][l].second * deviations[ voisinagePerm[ l ] ];
						}
						// Le point courant reste le même
						autocorrLocaleCourante(pt1, 0) = valeurIntermediaire*deviations[pt1]*facteurEchelleLocal;
						
						
						
						// p-valeur
						/*if ( ( (autocorrLocale(pt1, i)>=0) && (autocorrLocaleCourante(pt1, 0)>=autocorrLocale(pt1, i))) 
						 ||   ( (autocorrLocale(pt1, i)<0) && (autocorrLocaleCourante(pt1, 0)<=autocorrLocale(pt1, i)))    )
						 {
						 pValeurLocale(pt1, i)=pValeurLocale(pt1, i)+1;
						 }*/
						
						// Comparaison entre simulation et valeur réelle
						if (abs( autocorrLocaleCourante(pt1, 0) - autocorrLocale(pt1,i)) < limiteIndicesIdentiques)
						{
							++nbSimEgales;
						}
						else if (autocorrLocaleCourante(pt1, 0) > autocorrLocale(pt1,i))
						{
							++nbSimPlusGrandes;
						}
						
						// La p-valeur pour l'AC globale est calculée à part, tous les points sont permutés
					}
					
					// p-valeur
					// Discussions avec S. Rey et lexique ASU (pseudo p-valeurs)
					
					nbSimPlusPetites=AS_nbPermutations - nbSimPlusGrandes - nbSimEgales;
					
					if ( nbSimPlusPetites >= nbSimPlusGrandes) // a >= c
					{
						if ( nbSimPlusPetites >= moitieNbPerms )
						{
							pValeurLocale(pt1, i) = (nbSimPlusGrandes + nbSimEgales + 1.0)/(AS_nbPermutations+1);
						}
						else	// cas où les I' == I incluent la médiane de la distribution
						{
							pValeurLocale(pt1, i) = 0.5;
						}
					}
					else	// a<c
					{
						if ( nbSimPlusGrandes >= moitieNbPerms )
						{
							pValeurLocale(pt1, i) = (nbSimPlusPetites + nbSimEgales + 1.0)/(AS_nbPermutations+1);
						}
						else // cas où les I' == I incluent la médiane de la distribution
						{
							pValeurLocale(pt1, i) = 0.5;
						}
					}
				}	// Fin calcul p-valeurs locales
			}
			if (AS_autocorrGlobale)
			{
				nbSimPlusGrandes=0;
				nbSimEgales=0;
				nbSimPlusPetites=0;
				
				// La somme des carrés des déviations est la même pour chaque permutation
				// Idem pour le facteur d'échelle
				
				listePointsValidesPerm.resize(pointsCourants.taille);
				for (int j(0); j<pointsCourants.taille; ++j)
				{
					listePointsValidesPerm[j]=j;
				}
				
				for (int j(0); j<AS_nbPermutations; ++j)
				{
					//listePointsValidesPerm=pointsCourants.pointsValides;
					random_shuffle(listePointsValidesPerm.begin(), listePointsValidesPerm.end());
					listePointsGlobauxPerm=pointsCourants.indices; // Indices locaux des points valides (originaux)
					
					// On veut les nouveaux indices globaux des points permutés
					for (int k(0); k<pointsCourants.nbPoints; ++k)
					{
						if (listePointsGlobauxPerm[k]!=-1)
						{
							// de droite à gauche: liste [N] des indices locaux -> permutations -> indices globaux correspondants
							listePointsGlobauxPerm[k]=pointsCourants.pointsValides[ listePointsValidesPerm[listePointsGlobauxPerm[k]] ];
						}
					}
					//	toolbox::affiche(listePointsGlobauxPerm);
					// On prend la position (et la pondération) du point d'origine et on utilise la valeur du point permuté
					for (int  k(0); k<pointsCourants.taille; ++k)
					{
						pt1=pointsCourants.pointsValides[k];
						nbVoisins=pointsCourants.poids[pt1].size();
						valeurIntermediaire=0;
						for (int l(0); l<nbVoisins; ++l)
						{
							valeurIntermediaire+=pointsCourants.poids[pt1][l].second * deviations[ listePointsGlobauxPerm[ pointsCourants.poids[pt1][l].first ]];
						}
						
						autocorrLocaleCourante(pt1, 0) = valeurIntermediaire*deviations[ listePointsGlobauxPerm[pt1] ]*facteurEchelleLocal;
					}
					
					
					
					//autocorrGlobaleCourante=facteurEchelle*(sum(autocorrLocaleCourante(_,0))-sommeCarresDeviations);
					autocorrGlobaleCourante=facteurEchelleGlobal*(sum(autocorrLocaleCourante(_,0)));
					
					
					// Historique
					historiqueAutocorrGlobale(j, i)=autocorrGlobaleCourante;
					
					// p-valeur
					/*					if ( ((autocorrGlobale(0, i) >= 0) && (autocorrGlobaleCourante >= autocorrGlobale(0, i)) ) || ((autocorrGlobale(0, i) < 0) && (autocorrGlobaleCourante <= autocorrGlobale(0, i)) ))
					 //if ( abs(autocorrGlobaleCourante) >= abs(autocorrGlobale(0, i) )) 
					 {
					 pValeurGlobale(0, i)=pValeurGlobale(0, i)+1;
					 }*/
					// Comparaison entre simulation et valeur réelle
					if (abs( autocorrGlobaleCourante - autocorrGlobale(0,i)) < limiteIndicesIdentiques)
					{
						++nbSimEgales;
					}
					else if (autocorrGlobaleCourante > autocorrGlobale(0, i))
					{
						++nbSimPlusGrandes;
					}
					
				}
				
				nbSimPlusPetites=AS_nbPermutations - nbSimPlusGrandes - nbSimEgales;
				
				if ( nbSimPlusPetites >= nbSimPlusGrandes) // a >= c
				{
					if ( nbSimPlusPetites >= moitieNbPerms )
					{
						pValeurGlobale(0, i) = (nbSimPlusGrandes + nbSimEgales + 1.0)/(AS_nbPermutations+1);
					}
					else	// cas où les I' == I incluent la médiane de la distribution
					{
						pValeurGlobale(0, i) = 0.5;
					}
				}
				else	// a<c
				{
					if ( nbSimPlusGrandes >= moitieNbPerms )
					{
						pValeurLocale(0, i) = (nbSimPlusPetites + nbSimEgales + 1.0)/(AS_nbPermutations+1);
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
		cout << "Calcul autocorrélation : " << t2-t1 << "\n";
		
		
		// Ecriture des résultats
		
		//cout << autocorrLocale(0,0,10,2) << "\n"<< pValeurLocale(0,0,10,2) << "\n";
		
		sortieAS.precision(toolbox::precisionLecture);
		t1=time(NULL);
		// Ecriture de l'autocorrélation
		sortieAS.open((nomFichierResultats.first+"-AS-Env"+nomFichierResultats.second).c_str());
		if (sortieAS.fail())
		{
			throw Erreur("MSG_errOpenFileACEnv", "Error while opening file for autocorrelation of environnemental variables.");
		}
		else
		{
			// Headers
			if (existeColID)
			{
				sortieAS << "ID ";
			}
			for (int j(0); j<nbEnvActives; ++j)
			{
				sortieAS << specDataEnv[varEnvActives.at(j)].name << " ";
			}
			sortieAS << delimLignes;
			
			// AC Globale
			if (AS_autocorrGlobale)
			{
				if (existeColID)
				{
					sortieAS << "Global_AC ";
				}
				for (int j(0); j<nbEnvActives; ++j)
				{
					sortieAS << autocorrGlobale(0, j) << " ";
				}
				sortieAS << delimLignes;
			}
			
			
			// AC Locales
			if (AS_autocorrLocale)
			{
				
				for (int i(0); i<nbPoints; ++i)
				{
					if (existeColID)
					{
						sortieAS << dataSupEnv(i, specDataEnv[colIDEnv].localIndex) << " ";
					}
					for (int j(0); j<nbEnvActives; ++j)
					{
						sortieAS << autocorrLocale(i, j) << " ";
					}
					sortieAS << delimLignes;
					
				}
			}
			sortieAS.close();
		}
		
		
		// Ecriture des p-valeurs
		sortieAS.open((nomFichierResultats.first+"-AS-Env-pVal"+nomFichierResultats.second).c_str());
		if (sortieAS.fail())
		{
			throw Erreur("MSG_errOpenFileACSigEnv", "Error while opening file for autocorrelation significance of environnementales variables.");
		}
		else
		{
			// Headers
			if (existeColID)
			{
				sortieAS << "ID ";
			}
			for (int j(0); j<nbEnvActives; ++j)
			{
				sortieAS << specDataEnv[varEnvActives.at(j)].name << " ";
			}
			sortieAS << delimLignes;
			
			// AC Globale
			if (AS_autocorrGlobale)
			{
				if (existeColID)
				{
					sortieAS << "Global_AC ";
				}
				for (int j(0); j<nbEnvActives; ++j)
				{
					sortieAS <<  pValeurGlobale(0, j) << " ";
				}
				sortieAS << delimLignes;
			}
			
			// AC Locales			
			if (AS_autocorrLocale)
			{
				
				for (int i(0); i<nbPoints; ++i)
				{
					if (existeColID)
					{
						sortieAS << dataSupEnv(i, specDataEnv[colIDEnv].localIndex) << " ";
					}
					for (int j(0); j<nbEnvActives; ++j)
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
			sortieAS.open((nomFichierResultats.first+"-AS-Env-Sim"+nomFichierResultats.second).c_str());
			if (sortieAS.fail())
			{
				throw Erreur("MSG_errOpenFileACHistEnv", "Error while opening history file for autocorrelation of environnemental variables.");
			}
			else
			{
				// Headers
				for (int j(0); j<nbEnvActives; ++j)
				{
					sortieAS << specDataEnv[varEnvActives.at(j)].name << " ";
				}
				sortieAS << delimLignes;
				
				
				for (int i(0); i<AS_nbPermutations; ++i)
				{
					for (int j(0); j<nbEnvActives; ++j)
					{
						sortieAS << historiqueAutocorrGlobale(i, j) << " ";
					}
					sortieAS << delimLignes;
				}
				sortieAS.flush();
				sortieAS.close();
			}
			
		}
		t2=time(NULL);
		cout << "Ecriture autocorrélation : " << t2-t1 << "\n";
		
		
	}
	
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
			// There is no general mask for genetic data
			pointsCourants.masque=pointsAC.masque;
			for (set< int >::iterator iter(missingValuesMarq[i].begin()); iter!=missingValuesMarq[i].end(); ++iter)
			{
				//cout << *iter << endl;
				pointsCourants.masque[*iter]=false;
			}
			pointsCourants.taille=toolbox:: sommeNumerique(pointsCourants.masque);
			if (pointsCourants.taille==0)
			{
				continue;
			}
			else if (pointsCourants.taille==pointsAC.taille)	// Cas où tous les points géographiquement valides ont une valeur env
			{
				pointsCourants=pointsAC;
			}
			else
			{
				pointsCourants.miseAJour();	// La liste des points valides, la pondération, etc, sont réinitialisées
				
				// Calcul de la pondératon
				if (choixPonderation!=pondPlusProchesVoisins)
				{
					double somme(0);
					for (int u(0); u<pointsCourants.taille; ++u)
					{
						pt1=pointsCourants.pointsValides[u];
						somme=0;
						nbVoisins=pointsAC.poids[pt1].size();
						for (int v(0); v<nbVoisins; ++v)
						{
							// On copie le voisin s'il est lui-même valide
							if(pointsCourants.masque(pointsAC.poids[pt1][v].first,0))
							{
								pointsCourants.poids[pt1].push_back(pointsAC.poids[pt1][v]);
								//cout << "*"<< pointsAC.poids[pt1][v].first << " " << pointsAC.poids[pt1][v].second << endl;
								somme += (pointsAC.poids[pt1][v].second);
							}
						}
						nbVoisins=pointsCourants.poids[pt1].size();
						if (somme != 0)
						{
							for (int v(0); v<nbVoisins; ++v)
							{
								pointsCourants.poids[pt1][v].second /= somme;
								
							}
						}
					}
				}
				else	// Cas plus proches voisins
				{
					// Cas où tous les points valides sont voisins
					// On se base sur les points courants valides -> pas besoin de revérifier leur validité
					if (pointsCourants.taille<=(nbPlusProchesVoisins+1))
					{
						double poids(1./pointsCourants.taille);
						for (int u(0); u<pointsCourants.taille; ++u)
						{						
							pt1=pointsCourants.pointsValides[u];
							for (int v(u+1); v<pointsCourants.taille; ++v)
							{
								pt2=pointsCourants.pointsValides[v];
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
						
						//cout << "! " << distances(9,0,9,384) << "\n";
						for (int u(0); u<pointsCourants.taille; ++u)
						{
							pt1 = pointsCourants.pointsValides[u];
							nbVoisins=0;
							voisinCourant=voisinage[pt1].begin();
							while (nbVoisins<nbPlusProchesVoisins && voisinCourant!=voisinage[pt1].end())
							{
								if (pointsCourants.masque(voisinCourant->first,0))
								{
									pointsCourants.poids[pt1].push_back(make_pair(voisinCourant->first, 1));
									++nbVoisins;
								}
								++voisinCourant;
							}
							
							if (voisinCourant!=voisinage[pt1].end())
							{
								voisinSuivant=voisinCourant+1;
								while (voisinSuivant!=voisinage[pt1].end() && ((voisinSuivant)->second == voisinCourant->second))
								{
									if(pointsCourants.masque(voisinSuivant->first,0))
									{
										pointsCourants.poids[pt1].push_back(make_pair(voisinSuivant->first, 1));
										++nbVoisins;									
									}
									++voisinCourant;
									++voisinSuivant;
								}
								
							}
							
							double poids(1./nbVoisins);
							for (voisinCourant=pointsCourants.poids[pt1].begin(); voisinCourant!=pointsCourants.poids[pt1].end(); ++voisinCourant)
							{
								voisinCourant->second=poids;						
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
			moyenne=0;
			sommeCarresDeviations=0;
			for (int k(0); k<nbPoints; ++k)
			{
				if (pointsCourants.masque(k,0))
				{
					deviations(k, 0) = dataMarq(k, i);
					moyenne+=dataMarq(k,i);
					sommeCarresDeviations+=dataMarq(k, i)*dataMarq(k, i);
				}
				else 
				{
					deviations(k,0)=0;
				}
				
			}
			
			moyenne/=pointsCourants.taille;
			//				sommeCarresDeviations= sommeCarresDeviations/pointsCourants.taille-moyenne*moyenne;	// ! Variance
			sommeCarresDeviations= sommeCarresDeviations-pointsCourants.taille*moyenne*moyenne;
			
			for(int u(0); u<pointsCourants.taille; ++u)
			{
				deviations(pointsCourants.pointsValides[u])-=moyenne;
			}
			
			
			facteurEchelleLocal=(pointsCourants.taille-1)/sommeCarresDeviations;
			facteurEchelleGlobal=1./(pointsCourants.taille-1);
			
			// On calcule l'autocorrélation manuellement car la matrice des poids est creuse (sparse matrix)
			//autocorrTemp=0;
			double valeurIntermediaire(0);
			for (int  j(0); j<pointsCourants.taille; ++j)
			{
				pt1=pointsCourants.pointsValides[j];
				nbVoisins=pointsCourants.poids[pt1].size();
				valeurIntermediaire=0;
				for (int k(0); k<nbVoisins; ++k)
				{
					valeurIntermediaire+=pointsCourants.poids[pt1][k].second * deviations[pointsCourants.poids[pt1][k].first];
				}
				autocorrLocale(pt1, i) = valeurIntermediaire*deviations[pt1]*facteurEchelleLocal;
			}
			
			// Ecritude des valeurs dans le DBF
			if (AS_shapefile)
			{
				for (int j(0); j<nbPoints; ++j)
				{
					if(pointsCourants.masque(j,0))
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
				//cout << "\n" << autocorrLocale(_,i);
				//cout << tailleDonnees << " " << sum(ponderationCourante) << " " << sum(autocorrLocale(_,i)) << " " << sum(deviations%deviations) << "\n";
				
				autocorrGlobale(0,i)=facteurEchelleGlobal*(sum(autocorrLocale(_,i)));				
				cout << specDataMarq[marqActifs[i]].name << " " << moyenne << " " << autocorrGlobale(0,i)  << " " << sommeCarresDeviations<< "\n";
			}
			
			
			// Permutations!
			
			if (AS_autocorrLocale)
			{
				// Chaque fois qu'on prend un point, le nombre de points restants (=parmi lesquels on peut tirer un élément) diminue
				// Prop = facteur d'échelle pour le tirage d'un entier entre nbPointsValides, nbPointsValides-1, ..., 2,1,0
				// Subtilité : Le points courant reste fixe, il ne sera jamais choisi comme voisin -> nb voisins possibles = taille-1 !!!!
				vector<double> proportion(pointsCourants.taille-1);
				int choix(0);
				for (int j(0); j<(pointsCourants.taille-1); ++j)
				{
					// Comme 0<=rand<=RAND_MAX, on divise par RM+1 pour éviter les débordements de tableau
					//cout << "!" <<  pointsCourants.taille-1-j << endl;
					proportion[j]=(double)(pointsCourants.taille-1-j)/((double)RAND_MAX+1);
				}
				
				
				// Itération sur les points -> tous les points n'ont pas le même nombre de voisins
				for (int  k(0); k<pointsCourants.taille; ++k)
				{
					nbSimPlusGrandes=0;
					nbSimEgales=0;
					nbSimPlusPetites=0;
					
					pt1 = pointsCourants.pointsValides[k]; // Numéro global du point considéré
					nbVoisins = pointsCourants.poids[pt1].size();
					voisinagePerm.resize(nbVoisins);	// Redimensionnement du voisinage temporaire
					
					for (int j(0); j<AS_nbPermutations; ++j)
					{
						
						// Les pondérations sont fixes, on réordonne les déviations
						listePointsValidesPerm=pointsCourants.pointsValides;
						// L'indice local du point courant est k -> on le permute avec le dernier point pour ne jamais le sélectionner
						swap(listePointsValidesPerm[k], listePointsValidesPerm[pointsCourants.taille-1]);
						
						for (int l(0); l<nbVoisins; ++l)
						{
							choix=floor(rand()*proportion[l]);
							voisinagePerm[l]=listePointsValidesPerm[choix];
							swap(listePointsValidesPerm[choix], listePointsValidesPerm[pointsCourants.taille-2-l]);		// La dernière place est déjà occupée par le point
						}
						
						random_shuffle(voisinagePerm.begin(), voisinagePerm.end());
						// voisinagePerm contient les numéros globaux des voisins temporaires
						
						// On prend la position (et la pondération) du point d'origine et on utilise la valeur du point permuté
						
						//nbVoisins=pointsCourants.poids[pt1].size();
						valeurIntermediaire=0;
						for (int l(0); l<nbVoisins; ++l)
						{
							valeurIntermediaire+=pointsCourants.poids[pt1][l].second * deviations[ voisinagePerm[ l ] ];
						}
						// Le point courant reste le même
						autocorrLocaleCourante(pt1, 0) = valeurIntermediaire*deviations[pt1]*facteurEchelleLocal;
						
						
						
						// p-valeur
						
						//if ( ((autocorrTempCourante(k, 0)*autocorrTemp(k, 0))>0) && (abs(autocorrTempCourante(k, 0)) >= abs(autocorrTemp(k, 0))))
						/*if ( ( (autocorrLocale(pt1, i)>=0) && (autocorrLocaleCourante(pt1, 0)>=autocorrLocale(pt1, i))) 
						 ||   ( (autocorrLocale(pt1, i)<0) && (autocorrLocaleCourante(pt1, 0)<=autocorrLocale(pt1, i)))    )
						 //	if ( abs(autocorrLocaleCourante(pt1, 0))>=abs(autocorrLocale(pt1, i))   )
						 {
						 pValeurLocale(pt1, i)=pValeurLocale(pt1, i)+1;
						 }*/
						
						// Comparaison entre simulation et valeur réelle
						if (abs( autocorrLocaleCourante(pt1, 0) - autocorrLocale(pt1,i)) < limiteIndicesIdentiques)
						{
							++nbSimEgales;
						}
						else if (autocorrLocaleCourante(pt1, 0) > autocorrLocale(pt1,i))
						{
							++nbSimPlusGrandes;
						}					
						
						// La p-valeur pour l'AC globale est calculée à part, tous les points sont permutés
						
					}
					// p-valeur
					// Discussions avec S. Rey et lexique ASU (pseudo p-valeurs)
					
					nbSimPlusPetites=AS_nbPermutations - nbSimPlusGrandes - nbSimEgales;
					
					if ( nbSimPlusPetites >= nbSimPlusGrandes) // a >= c
					{
						if ( nbSimPlusPetites >= moitieNbPerms )
						{
							pValeurLocale(pt1, i) = (nbSimPlusGrandes + nbSimEgales + 1.0)/(AS_nbPermutations+1);
						}
						else	// cas où les I' == I incluent la médiane de la distribution
						{
							pValeurLocale(pt1, i) = 0.5;
						}
					}
					else	// a<c
					{
						if ( nbSimPlusGrandes >= moitieNbPerms )
						{
							pValeurLocale(pt1, i) = (nbSimPlusPetites + nbSimEgales + 1.0)/(AS_nbPermutations+1);
						}
						else // cas où les I' == I incluent la médiane de la distribution
						{
							pValeurLocale(pt1, i) = 0.5;
						}
					}
					
				}	// Fin calcul p-valeurs locales
			}
			if (AS_autocorrGlobale)
			{
				nbSimPlusGrandes=0;
				nbSimEgales=0;
				nbSimPlusPetites=0;
				
				// La somme des carrés des déviations est la même pour chaque permutation
				// Idem pour le facteur d'échelle
				
				listePointsValidesPerm.resize(pointsCourants.taille);
				for (int j(0); j<pointsCourants.taille; ++j)
				{
					listePointsValidesPerm[j]=j;
				}
				
				for (int j(0); j<AS_nbPermutations; ++j)
				{
					//listePointsValidesPerm=pointsCourants.pointsValides;
					random_shuffle(listePointsValidesPerm.begin(), listePointsValidesPerm.end()); //indices locaux permutés
					listePointsGlobauxPerm=pointsCourants.indices; // Indices locaux des points valides (originaux)
					
					// On veut les nouveaux indices globaux des points permutés
					for (int k(0); k<pointsCourants.nbPoints; ++k)
					{
						if (listePointsGlobauxPerm[k]!=-1)
						{
							// de droite à gauche: liste [N] des indices locaux -> permutations -> indices globaux correspondants
							listePointsGlobauxPerm[k]=pointsCourants.pointsValides[ listePointsValidesPerm[listePointsGlobauxPerm[k]] ];
						}
						//	cout << "*" << k << " " << listePointsGlobauxPerm[k] << endl;
					}
					//	toolbox::affiche(listePointsGlobauxPerm);
					// On prend la position (et la pondération) du point d'origine et on utilise la valeur du point permuté
					for (int  k(0); k<pointsCourants.taille; ++k)
					{
						//	cout << k << endl;
						pt1=pointsCourants.pointsValides[k];
						nbVoisins=pointsCourants.poids[pt1].size();
						valeurIntermediaire=0;
						for (int l(0); l<nbVoisins; ++l)
						{
							//		cout << pointsCourants.poids[pt1][l].first << endl;
							valeurIntermediaire+=pointsCourants.poids[pt1][l].second * deviations[ listePointsGlobauxPerm[ pointsCourants.poids[pt1][l].first ]];
						}
						
						autocorrLocaleCourante(pt1, 0) = valeurIntermediaire*deviations[ listePointsGlobauxPerm[pt1] ]*facteurEchelleLocal;
					}
					
					
					
					//autocorrGlobaleCourante=facteurEchelle*(sum(autocorrLocaleCourante(_,0))-sommeCarresDeviations);
					autocorrGlobaleCourante=facteurEchelleGlobal*(sum(autocorrLocaleCourante(_,0)));
					
					
					// Historique
					historiqueAutocorrGlobale(j, i)=autocorrGlobaleCourante;
					
					// p-valeur
					/*if ( ((autocorrGlobale(0, i) >= 0) && (autocorrGlobaleCourante >= autocorrGlobale(0, i)) ) || ((autocorrGlobale(0, i) < 0) && (autocorrGlobaleCourante <= autocorrGlobale(0, i)) ))
					 //if ( abs(autocorrGlobaleCourante) >= abs(autocorrGlobale(0, i) )) 
					 {
					 pValeurGlobale(0, i)=pValeurGlobale(0, i)+1;
					 }*/
					
					// Comparaison entre simulation et valeur réelle
					if (abs( autocorrGlobaleCourante - autocorrGlobale(0,i)) < limiteIndicesIdentiques)
					{
						++nbSimEgales;
					}
					else if (autocorrGlobaleCourante > autocorrGlobale(0, i))
					{
						++nbSimPlusGrandes;
					}
					
				}
				
				nbSimPlusPetites=AS_nbPermutations - nbSimPlusGrandes - nbSimEgales;
				
				if ( nbSimPlusPetites >= nbSimPlusGrandes) // a >= c
				{
					if ( nbSimPlusPetites >= moitieNbPerms )
					{
						pValeurGlobale(0, i) = (nbSimPlusGrandes + nbSimEgales + 1.0)/(AS_nbPermutations+1);
					}
					else	// cas où les I' == I incluent la médiane de la distribution
					{
						pValeurGlobale(0, i) = 0.5;
					}
				}
				else	// a<c
				{
					if ( nbSimPlusGrandes >= moitieNbPerms )
					{
						pValeurGlobale(0, i) = (nbSimPlusPetites + nbSimEgales + 1.0)/(AS_nbPermutations+1);
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
		cout << "Calcul autocorrélation : " << t2-t1 << "\n";
		
		
		// Ecriture des résultats
		
		//cout << autocorrLocale(0,0,10,2) << "\n"<< pValeurLocale(0,0,10,2) << "\n";
		
		sortieAS.precision(toolbox::precisionLecture);
		t1=time(NULL);
		// Ecriture de l'autocorrélation
		sortieAS.open((nomFichierResultats.first+"-AS-Mark"+nomFichierResultats.second).c_str());
		if (sortieAS.fail())
		{
			throw Erreur("MSG_errOpenFileACEnv", "Error while opening file for autocorrelation of genetic markers.");
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
			if (AS_autocorrLocale)
			{
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
			}
			
			sortieAS.close();
		}
		
		
		// Ecriture des p-valeurs
		sortieAS.open((nomFichierResultats.first+"-AS-Mark-pVal"+nomFichierResultats.second).c_str());
		if (sortieAS.fail())
		{
			throw Erreur("MSG_errOpenFileACSigEnv", "Error while opening file for autocorrelation significance of genetic markers.");
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
			if (AS_autocorrLocale)
			{
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
			}
			sortieAS.flush();
			sortieAS.close();

		}
		
		// Ecriture de l'historique des permutations (autocorr globale)
		if (AS_autocorrGlobale)
		{
			sortieAS.open((nomFichierResultats.first+"-AS-Mark-Sim"+nomFichierResultats.second).c_str());
			if (sortieAS.fail())
			{
				throw Erreur("MSG_errOpenFileACHistEnv", "Error while opening history file for autocorrelation of genetic markers.");
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
		
		t2=time(NULL);
		cout << "Ecriture autocorrélation : " << t2-t1 << "\n";
		
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



int RegressionLogistique::creeModelesGlobaux()
{
	cout << "Highest dimension : " << dimensionMax <<"\n";
	cout << "List of possible errors : " << "\n"
	<< "1: exponential divergence" << "\n"
	<< "2: J_info is singular" << "\n"
	<< "3: divergence of beta" << "\n"
	<< "4: maximum number of iterations" << "\n"
	<< "5: monomorphic marker" << "\n"
	<< "6: non-significant parents models" << "\n";
	
	
	// Initialisation du conteneur de résultats et des flots de sortie
	resultats.clear();
	resultats.resize(dimensionMax+1);
	vector<string> nomsFichiers(dimensionMax+1);
	for (int i(0); i<=dimensionMax; ++i)
	{
		ostringstream oss;
		oss << i;
		nomsFichiers[i]=(nomFichierResultats.first)+"-Out-"+oss.str()+(nomFichierResultats.second);
	}
	
	sortie.initialise(nomsFichiers);
	
	sortie.ouverture();
	sortie.precision(toolbox::precisionLecture);
	
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
	
	sortie.ecriture(0, names[0][0]);
	sortie.ecriture(0, names[1], true);
	
	for (int i(1); i<=dimensionMax; ++i)
	{
		sortie.ecriture(i, names[0][0]);
		for (int j(1); j<=i; ++j)
		{
			sortie.ecriture(i, names[0][1]+toolbox::conversion(j));
		}
		sortie.ecriture(i, names[2]);
		for (int j(0); j<i; ++j)
		{
			sortie.ecriture(i, names[0][2]+toolbox::conversion(j));
		}
		sortie.ecriture(i, names[0][2]+toolbox::conversion(i), true);
		//sortie.ecriture(i, "", true);
	}
	
	
	// Variables locales
	// int colMarq(0);
	reel tailleY(0), sommeY(0); //, tailleX(0), beta_zero(0), val_loglikelihood_zero(0);
	// pseudosRcarresCourants(nbPseudosRcarres), statsCourantes(nbStats), betaCourant(0); */
	vector<reel> loglikelihood(dimensionMax+1, 0.0);//, resultatsCourants(tailleEtiquetteInvar, 0.0); // Il y a le modèle sans paramètres!
	
	// Iteration sur les paramètres environnementaux
	vector< set< int >::iterator > cavalier(dimensionMax);
	//vector<bool> varDiscrete(dimensionMax, false);
	int niveau(0);
	bool fini(false), avance(false);
	//vector<int> varDiscretes(0), varContinues(0);
	set<int> varContinues;
	
	pair< etiquetteModele, vector<reel> > resultatCourant;
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
		pointsTot=pointsGeo;
		reel sommePond(0);
		int nbVoisins(0);
		for (int i(0); i<nbPoints; ++i)
		{
			if (pointsTot.masque(i,0))
			{
				// Si un point est valide, tous ses voisins sont considérés, vu qu'on ne compte que le masque géo
				nbVoisins=pointsTot.poids[i].size();
				for (int j(0); j<nbVoisins; ++j)
				{
					sommePond += pointsTot.poids[i][j].second;	
				}
				sommePond += 1.; // Pour le point i
				for (int j(0); j<nbVoisins; ++j)
				{
					pointsTot.poids[i][j].second /= sommePond;	
				}
				pointsTot.poids[i].push_back(make_pair(i, 1./sommePond));
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
	
	// Paramètres pour la FDR : 0.01, 0.02, ..., 0.95
	// Les p-valeurs sont dans l'ordre décroissant
	int nbPvalStorey(95);
	for (int i(nbPvalStorey); i>0; --i) 
	{
		storey.pval.push_back(0.01*i);
		storey.seuilScore.push_back(toolbox::invCDF_ChiSquare(1.-0.01*i, 1, sqrt(epsilon < reel > ())));
		cout << 0.01*i << " " << toolbox::invCDF_ChiSquare(1.-0.01*i, 1, sqrt(epsilon < reel > ())) << endl;
	}
	storey.compteurG.resize(dimensionMax+1, vector<int> (nbPvalStorey,0));
	storey.compteurWald.resize(dimensionMax+1, vector<int> (nbPvalStorey,0));

	//	clock_t t1, t2;
	
	// On ne prend en compte que les marqueurs actifs
	for (int i(0); i<nbMarqActifs; ++i)
	{
		if(sauvegardeTempsReel)
		{
			resultats.clear();
			resultats.resize(dimensionMax+1);
		}
		if (i%1000==0)
		{
			cout << i << "\n";
		}
		//colMarq = nbEnv+i;
		resultatCourant.first.first=i;
		resultatCourant.first.second.clear();
		
		// Gestion des valeurs manquantes -> On ne prend que les points dont toutes les valeurs sont connues
		// Remplissage du masque
		masqueY=true;
		// Les valeurs manquantes sont un ensemble!
		for (set< int >::iterator iter(missingValuesMarq[i].begin()); iter!=missingValuesMarq[i].end(); ++iter)
		{
			//cout << colMarq << " "<< missingValues[colMarq][k] << "\n";
			masqueY[*iter]=false;
		}
		
		sommeY = toolbox::sommeNumerique(dataMarq(_, i));
		tailleY = toolbox::sommeNumerique(masqueY);
		if(sommeY == 0 || tailleY==0 || sommeY==tailleY)	// si tous les marqueurs sont nuls ou manquants ou si tous les marqueurs non-nuls sont égaux à 1
		{
			// On veut afficher le numéro global du marqueur
			cout << "Y[" << marqActifs[i]+numPremierMarq << "] is constant ! " << dataMarq(0,i) << "\n";
			// Paramètres du modèle
			resultatCourant.second[1]=(sommeY>0? 1 : 0);
			resultatCourant.second[0]=0;
			resultatCourant.second[2]=0;
			resultatCourant.second[3]=5;
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
		resultatCourant.second[1]=(sommeY/tailleY);
		// beta_zero 
		resultatCourant.second[2]=log((double)sommeY / (tailleY - sommeY)); // beta_zero = ln( Y / (N-Y)) 
		// loglikelihood
		resultatCourant.second[0]=sommeY*resultatCourant.second[2] - tailleY*log(tailleY / (tailleY - sommeY));
		// Pas d'erreur détectée
		resultatCourant.second[3]=0;
		
		resultats[0].insert(resultatCourant);
		
		if (sauvegardeTempsReel)
		{
			ecritResultat(0, resultatCourant);
		}
		
		// Copie de la loglikelihood dans le tableau
		loglikelihood[0]=resultatCourant.second[1];
		
		if (nbEnvActives==0 || dimensionMax==0)
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
			pointsMarq.masque=pointsTot.masque%masqueY;
			pointsMarq.taille=toolbox::sommeNumerique(pointsMarq.masque);
			if (pointsMarq.taille==pointsTot.taille)	// Si on n'a pas de nouvelles valeurs manquantes (par rapport à masqueGeo)
			{
				pointsMarq=pointsTot;
			}
			else 
			{
				pointsMarq.miseAJour();
				
				reel sommePond(0);
				int nbVoisins(0);
				if (choixPonderation!=pondPlusProchesVoisins)
				{
					for (int i(0); i<nbPoints; ++i)
					{
						if (pointsMarq.masque(i,0))
						{
							nbVoisins=pointsTot.poids[i].size();
							for (int j(0); j<nbVoisins; ++j)
							{
								if (pointsMarq.masque(j,0))
								{
									sommePond += pointsTot.poids[i][j].second;	
									pointsMarq.poids[i].push_back(pointsTot.poids[i][j]);
								}	
							}
							sommePond += 1.; // Pour le point i
							nbVoisins=pointsMarq.poids[i].size();
							
							for (int j(0); j<nbVoisins; ++j)
							{
								pointsMarq.poids[i][j].second /= sommePond;	
							}
							pointsMarq.poids[i].push_back(make_pair(i, 1./sommePond));							
						}
					}
				}
				else	// Cas plus proches voisins, on reprend la liste du voisinage
				{
					
					// Cas où tous les points valides sont voisins
					if (pointsMarq.taille<=(nbPlusProchesVoisins+1))
					{
						double poids(1./pointsMarq.taille);
						for (int i(0); i<nbPoints; ++i)
						{
							if (pointsMarq.masque(i,0))
							{
								
								pointsMarq.poids[i].push_back(make_pair(i, poids));
								
								for (int j(i+1); j<nbPoints; ++j)
								{
									if (pointsMarq.masque(j,0))
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
						
						reel plusPetiteDiffDist(pow(10.,-4)); // En dessous d' 1cm ça compte pas! (Les distances sont au carré)
						for (int i(0); i<nbPoints; ++i)
						{
							if (pointsMarq.masque(i,0))
							{
								nbVoisinsCourants=0;
								pointsMarq.poids[i].push_back(make_pair(i, 1));
								voisinCourant=voisinage[i].begin();
								while ((nbVoisinsCourants<nbPlusProchesVoisins) && (voisinCourant!=voisinage[i].end()))
								{
									if (pointsMarq.masque(voisinCourant->first,0))
									{
										pointsMarq.poids[i].push_back(make_pair(voisinCourant->first, 1));
										++voisinCourant;
										++nbVoisinsCourants;	
									}
								}
								// On cherche si des points sont à la même distance que le dernier
								voisinSuivant=voisinCourant+1;
								while((voisinSuivant!=voisinage[i].end()) && abs((voisinSuivant)->second - voisinCourant->second) < plusPetiteDiffDist) 
								{
									if (pointsMarq.masque(voisinSuivant->first,0))
									{
										pointsMarq.poids[i].push_back(make_pair(voisinSuivant->first, 1));
										
										++voisinCourant;
										++voisinSuivant;
										++nbVoisinsCourants;
									}
								}
								sommePond=nbVoisinsCourants+1; // +1 pour le point lui-même
								double poids(1./sommePond);
								for (int j(0); j<sommePond; ++j)
								{
									pointsMarq.poids[i][j].second=poids;						
								}
							}
						}
					}
					
					
				}
				
			}
		}
		
		for (int variableCourante(0); variableCourante<nbEnvActives; ++variableCourante)
		{
			varContinues.clear();
			varContinues.insert(variableCourante);
			construitModele(i, varContinues);
		}
		
		// Calcul des modèles multivariés
		set<int>::iterator derniereVar;//, varCourante;
		int varCourante;
		
		// Ici la dim est celle des parents (le cas dim=1 est traité avant)
		for (int dim(1); dim<dimensionMax; ++dim)
		{
			// On peut déjà redimensionner les matrices pour les calculs
			// On connaît la taille de l'échantillon pour la regression -> construction des matrices
			// Matrice des paramètres
			nbParam=dim+2;
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
			
			
			
			// Parcours des modèles de dimension dim-1
			// La fonction lower_bound permet de trouver directement le premier modèle avec le bon marqueur
			//			for (groupeResultats::iterator generationPrecedente(resultats[dim].begin());  (generationPrecedente!=resultats[dim].end()); ++generationPrecedente)
			for (groupeResultats::iterator generationPrecedente(resultats[dim].lower_bound(resultatCourant.first));  (generationPrecedente!=resultats[dim].end()); ++generationPrecedente)
			{
				// Si le résultat considéré concerne le marqueur i:
				if (generationPrecedente->first.first==i)
				{
					
					derniereVar=generationPrecedente->first.second.end();
					--derniereVar;
					//					for (varCourante=++varActives.find(*derniereVar); varCourante!=varActives.end(); ++varCourante)
					for (varCourante=*derniereVar+1; varCourante<nbEnvActives; ++varCourante)
					{
						varContinues = generationPrecedente->first.second;
						varContinues.insert(varCourante);
						
						//t1 = clock();
						construitModele(i, varContinues);
						//t2 = clock();
						//cout << t2-t1 << " " ;
					}
				}
				
			}
			
			
		}
		//	cout << endl;
	}
	
	
	if (!sauvegardeTempsReel)
	{
		// Si on fait la sauvegarde à la fin, on prend le temps de trier les résultats
		trieEtEcritResultats();
	}
	sortie.fermeture();
	
	// Storey
	ofstream sortieStorey("res-Storey.txt");
	sortieStorey << "P-valeurs" << delimMots;
	for (int i(0); i<nbPvalStorey; ++i)
	{
		sortieStorey<< storey.pval[i] << delimMots;
	}
	sortieStorey<< endl;

	sortieStorey << "Scores" << delimMots;
	for (int i(0); i<nbPvalStorey; ++i)
	{
		sortieStorey<< storey.seuilScore[i] << delimMots;
	}
	sortieStorey<< endl;
	
	for (int i(1); i<(dimensionMax+1); ++i)
	{
		sortieStorey << "G" << delimMots;
		for (int j(0); j<nbPvalStorey; ++j)
		{
			sortieStorey<< storey.compteurG[i][j] << delimMots;
		}
		sortieStorey<< endl;

		sortieStorey << "Wald" << delimMots;
		for (int j(0); j<nbPvalStorey; ++j)
		{
			sortieStorey<< storey.compteurWald[i][j] << delimMots;
		}
		sortieStorey<< endl;
		
		
	}
	
	
	sortieStorey.close();
	
	return 0;
}



void RegressionLogistique::construitModele(int numMarq,  const set<int> & varContinues)//, const reel loglike_zero, reel& loglike_courante)
{
	//cout << "!";
	//affiche(make_pair(numMarq, varContinues));
	//	int nbVarDisc(varDiscretes.size()), nbVarCont(varContinues.size());
	int nbVarCont(varContinues.size());
	int dim(nbVarCont);
	
	if (dim==0)
	{
		return;
	}
	
	resModele resultat;
	resultat.first.first=numMarq;
	resultat.first.second=varContinues;
	
	masque=masqueY;
	for (set<int>::iterator i(varContinues.begin()); i!=varContinues.end(); ++i)
	{
		masque%=masqueX(_, *i);
	}
	taille=toolbox::sommeNumerique(masque);
	double  somme(toolbox::sommeNumerique(dataMarq(_, numMarq)%masque));
	
	if (somme==taille || somme==0)
	{
		// Y est constant!
		resultat.second.resize(nbStatsSansPseudos, 0); // loglike, Gscore et Wald -> Model non-significatif
		resultat.second[validiteModele]=5;
		if (somme==0)
		{
			resultat.second.push_back(0); // prob
		}
		else 
		{
			resultat.second.push_back(1);
		}
		
		if (selModeles==all)
		{
			resultats[dim].insert(resultat);
			// Cas où on sauvegarde les résultats au fur et à mesure
			if (sauvegardeTempsReel)
			{
				ecritResultat(dim, resultat);
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
		nbParam=dim+1;
		resultat.second.resize(nbStats+nbParam);
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
		
		X(_, 0) =1; // Initialisation à 1 -> constante ok
		if(taille < nbPoints)
		{
			X(taille, 0, nbPoints-1, nbParam-1)=0;
			Y(taille, 0, nbPoints-1, 0)=0;
		}
		// Copie des valeurs dans la 2e colonne de X et dans Y
		int position(0);
		for (int k(0); k<nbPoints; ++k)
		{
			if (masque(k,0) == true)
			{
				set<int>::iterator iter(varContinues.begin());
				for (int l(0); l<nbVarCont; ++l)
				{
					X(position, l+1) = dataEnv(k, *iter);
					++iter;
				}
				Y(position, 0) = dataMarq(k, numMarq);
				++position;
			}
			
		}
		
		bool modeleRetenu(false);
		if(sum(Y)==taille || sum(Y) ==0)
		{
			// Y est constant!
			resultat.second.resize(nbStatsSansPseudos, 0); // loglike, Gscore et Wald -> Model non-significatif
			resultat.second[validiteModele]=5;
			resultat.second.push_back(Y(0,0)); // prob
			resultat.second.push_back(1);
			modeleRetenu=false;
		}
		else 
		{
			//cout << X.cols() << " " << X(0,0,10,nbParam-1) << Y(0,0,10,0) << "\n";
			
			/*if (nbVarCont==2)
			 {
			 cout << "Coucou!" <<"\n";
			 }*/
			
			
			// Initialisation de beta_hat
			beta_hat = 0;
			
			
			int typeErreur(0);
			typeErreur=calculeRegression(resultat.second[valloglikelihood], resultat.second[Efron]);
			
			
			if (typeErreur>0)
			{
				resultat.second[validiteModele]=typeErreur;
				cout << typeErreur << " | " << numMarq+numPremierMarq << " : ";
				for (set<int>::iterator l(varContinues.begin()); l!=varContinues.end(); ++l)
				{
					cout << *l << " " ;
				}
				cout << "\n";
				
				
			}
			
			
			
			
			resultat.second[Efron] = 1. - (resultat.second[Efron]/sum((Y - somme/taille)%(Y - somme/taille) ));
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
			modeleRetenu = calculeStats(resultat, nbParam-1);
			
			// Copie de la valeur de beta
			for (int i(0); i<nbParam; ++i)
			{
				resultat.second[nbStats+i]=beta_hat(i, 0);
			}
			
			if (AS_GWR)
			{
				calculeGWR(numMarq, varContinues, resultat);
			}
			
		}
		
		// On traite la mise en mémoire et la sauvegarde différemment
		if ( (selModeles==all || modeleRetenu) && sauvegardeTempsReel)
		{
			ecritResultat(dim, resultat);
		}
		
		// Il faut garder le modèle même s'il n'est pas signif dans le cas signif, pour les comparaison ultérieures
		if ((dim<dimensionMax) || ( (selModeles==all || modeleRetenu) && !sauvegardeTempsReel) )
		{
			resultats[dim].insert(resultat);
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
	reel loglike(0);
	
	// Iteration
	while(continueCalcul && !singularMatrix && !divergentCalculation && (nbIterations < limiteIter))
	{
		// Calcul pi
		nouv_Xb = X * beta_hat; // Test avant l'exp
		
		if (max(nouv_Xb)>limiteExp)
		{
			continueCalcul=false;
			composantEfron = sum((intermScores%intermScores)(0,0,taille-1,0));
			loglikeCourante = sum((Y%(Xb) - log(1.+exp_Xb))(0,0,taille-1,0));
			typeErreur=1;
			
		}
		else
		{
			++nbIterations;
			
			// Calcul pi
			Xb=nouv_Xb;
			exp_Xb = exp(Xb);
			
			//cout << nbIterations << "\n" << beta_hat << "\n" << Xb << "\n" << exp_Xb << "\n";
			
			pi_hat = exp_Xb / (1 + exp_Xb);
			
			/*if (nbIterations==26)
			 {
			 cout << exp_Xb << "\n" << pi_hat << "\n";
			 }*/
			
			//cout << pi_hat<< "\n";
			// Calcul ni * pi * (1 - pi)
			interm = pi_hat % (1 - pi_hat);
			
			// Calcul des scores U
			intermScores = (Y - pi_hat) ;
			for (int k(0); k<nbParam; ++k)
			{
				scores(k, 0) = sum(intermScores % X(_, k));
			}
			
			// Calcul de J, qui est symétrique
			for (int k(0); k<nbParam; ++k)
			{
				J_info(k, k) = sum(interm % X(_, k) % X(_, k));
				for (int l(k+1); l<nbParam; ++l)
				{
					J_info(k, l) = sum(interm % X(_, k) % X(_, l));
					J_info(l, k) = J_info(k, l);
				}
			}
			
			//cout << "# " << nbIterations << " " << min(nouv_Xb) << " " << max(nouv_Xb) << " " << exp(max(nouv_Xb)) << " " << max(exp_Xb)<<  "\n" << scores << "\n" <<J_info<< "\n";
			
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
				catch(scythe_exception& error)
				{
					//cerr << error.message() << "\n";
					singularMatrix = true;
					continueCalcul=false;
					typeErreur=2;
				}
				
				/*			for (int m(0); m<nbParam; ++m)
				 {
				 if (inv_J_info(m, m) <= 0)
				 cerr << i << " " << j << " " << inv_J_info << "\n";
				 }
				 */
				
				if(!singularMatrix)
				{
					// Mise à jour de beta_hat
					nouv_beta_hat = beta_hat + inv_J_info * scores;
					
					//cout << inv_J_info << det(J_info) << "\n" << nouv_beta_hat << "\n" << "\n";
					
					for (int l(0); l<nbParam; ++l)
					{
						if (abs(nouv_beta_hat(l, 0)) > limiteNaN)
						{
							continueCalcul = false;
							divergentCalculation=true;
							typeErreur=3;
							break;
						}
					}
					
					if (continueCalcul)
					{
						// Test de convergence
						diff_beta_hat = nouv_beta_hat - beta_hat;
						continueCalcul = false;
						for (int l(0); l<nbParam; ++l)
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
				//cout << nbIterations << "\n" << beta_hat << "\n";
			}
			// Si le calcul est terminé, on calcule l'indice d'Effron
			if (!continueCalcul) {
				//cout << nbIterations << "\n" << beta_hat << "\n";
				
				//double ratio(sum(Y)*1.0 / taille);
				//cout << ratio << "\n";
				// indiceEfron = 1 - (sum(intermScores%intermScores))/(sum((Y - ratio)%(Y - ratio)));
				composantEfron = sum((intermScores%intermScores)(0,0,taille-1,0));
				//loglikelihood = sum(Y%log(pi_hat) + (1-Y)%log(1-pi_hat));
				loglikeCourante = sum((Y%(Xb) - log(1.+exp_Xb))(0,0,taille-1,0));
			}
		}
	}
	if (nbIterations==limiteIter)
	{
		composantEfron = sum((intermScores%intermScores)(0,0,taille-1,0));
		loglikeCourante = sum((Y%(Xb) - log(1.+exp_Xb))(0,0,taille-1,0));
		typeErreur=4;
	}
	return typeErreur;
}

bool RegressionLogistique::calculeStats(resModele& resultat, int nbParamEstimes)
{
	bool modeleRetenu(true);
	
	// Il faut trouver les meilleurs modèles "parents"
	/*for (set< int >:: iterator iter(resultat.first.second.begin()); iter!=resultat.first.second.end(); ++iter)
	 {
	 cout << *iter << " " ;
	 cout << specDataEnv[varEnvActives.at(*iter)].name<< " ";
	 
	 }
	 cout << "!"<<"\n";*/
	//cout << "ç" << resultat.first.second.size()-1 << "\n";
	//affiche(resultat);
	int dimParents(resultat.first.second.size()-1); // Taille de l'étiquette - 1
	
	// On garde les infos des modèles plus simples que dim max
	//bool modeleNonMax(resultat.first.second.size() < dimensionMax);
	
	groupeResultats::iterator modeleCourant, bestLoglike;
	
	// Calcul des pseudos R carrés
	reel loglikeCourante(0), loglikeZero(0);
	
	etiquetteModele etiquetteCourante;
	
	// Il faut trouver un modèle valide pour la comparaison
	bool parentValide(false);
	
	if (dimParents==0)	// Modèle univarié -> un seul parent
	{
		parentValide=true;
		etiquetteCourante=resultat.first;
		etiquetteCourante.second.clear();
		modeleCourant=resultats[dimParents].find(etiquetteCourante);
		bestLoglike=modeleCourant;
	}
	else // Recherche d'un parent valide et sélection du parent avec la meilleure loglike
	{
		for (set< int >::iterator variableCourante(resultat.first.second.begin()); variableCourante!=resultat.first.second.end(); ++variableCourante)
		{
			// Initialisation avec les valeurs du premier modèle
			etiquetteCourante=resultat.first;
			etiquetteCourante.second.erase(*variableCourante);	// On corrige l'étiquette pour le modèle à considérer
			modeleCourant=resultats[dimParents].find(etiquetteCourante);	// Parent courant
			
			// On teste si le parent existe et s'il n'est pas dans un état d'erreur
			if (modeleCourant!=resultats[dimParents].end() && ( (modeleCourant->second[validiteModele])==0  || (modeleCourant->second[validiteModele])==6) )
			{
				if (parentValide) //  Test si un parent valide a déjà été trouvé
				{
					if ((modeleCourant->second[valloglikelihood]) > (bestLoglike->second[valloglikelihood]))
					{
						bestLoglike=modeleCourant;
					}
				}
				else
				{
					// Ce modèle est le premier modèle valide trouvé
					bestLoglike=modeleCourant;
					parentValide=true;
				}
			}
		}
	}
	
	// On a trouvé un premier modèle pour comparer
	if (parentValide==true)
	{		
		loglikeZero=bestLoglike->second[valloglikelihood];
		
		// Calcul des scores
		resultat.second[Gscore] = 2.0*(resultat.second[valloglikelihood]-bestLoglike->second[valloglikelihood]);
		
		// Mise à jour du compteur pour la FDR
		++storey.compteurG[resultat.first.second.size()][ ( upper_bound(storey.seuilScore.begin(), storey.seuilScore.end(),  resultat.second[Gscore])-storey.seuilScore.begin() ) ];
		
		// Si on sauve tous les modèles, on cherche le plus petit score de Wald
		int tailleModele(dimParents+1);
		
		// Au cas où on choisit les modèles significatifs, on commence par tester le score G
		if (selModeles!=all && (resultat.second[Gscore]<seuilScore[tailleModele]))
		{
			// rejet car G trop petit
			modeleRetenu=false;
			resultat.second[validiteModele]=7;
		}
//		else	// STOREY!
//		{
			// On fait un test de Wald par variable (pas la constante)
			// Simplification : on calcule le score de Wald final (= le plus petit score de Wald, un par variable)
			// On teste la significativité du score à la fin.
			
			// Initialisation du score de Wald
			reel WaldCourant(beta_hat(1, 0)*beta_hat(1,0)/inv_J_info(1, 1));
			resultat.second[WaldScore]=WaldCourant;
			
			//affiche(resultat.first);
			//cout << "	" << WaldCourant;
			for (int paramCourant(2); paramCourant<=tailleModele; ++paramCourant)
			{
				WaldCourant=beta_hat(paramCourant, 0)*beta_hat(paramCourant, 0)/inv_J_info(paramCourant, paramCourant);
				//cout << "	" << WaldCourant;
				if (WaldCourant < resultat.second[WaldScore])
				{
					resultat.second[WaldScore] = WaldCourant;
				}
			}
			//cout << endl;
		
		// Mise à jour du compteur pour la FDR
		++storey.compteurWald[resultat.first.second.size()][ ( upper_bound(storey.seuilScore.begin(), storey.seuilScore.end(),  resultat.second[WaldScore])-storey.seuilScore.begin() ) ];

			// Test du score de Wald
			if (selModeles!=all && ( resultat.second[WaldScore] < seuilScore[tailleModele] ))
			{
				modeleRetenu=false;
				resultat.second[validiteModele]=7;
				
			}
			//affiche(resultat);
			
//		}
		
	}
	else //if (selModeles!=best)	// STOREY !
		// Aucun modèle parent n'est valide -> on compare avec le modèle constant 
		// Le modèle constant est de toute façon valide
		//-> Attention au changement de seuil pour la p-valeur
	{
		// Aucun parent valide -> erreur type 6
		resultat.second[validiteModele]=6;
		
		etiquetteCourante=resultat.first;
		etiquetteCourante.second.clear();
		modeleCourant=resultats[0].find(etiquetteCourante);
		
		loglikeZero=modeleCourant->second[valloglikelihood];
		
		// Calcul des scores
		resultat.second[Gscore] = 2.0*(resultat.second[valloglikelihood]-loglikeZero);

		// Mise à jour du compteur pour la FDR
		++storey.compteurG[resultat.first.second.size()][ ( upper_bound(storey.seuilScore.begin(), storey.seuilScore.end(),  resultat.second[Gscore])-storey.seuilScore.begin() ) ];

		
		// Test de Wald si le modèle passe le test G ou si on sauve tous les modèles
		if (selModeles==signif && (resultat.second[Gscore]<seuilScoreMultivarie[dimParents+1]))
		{
			modeleRetenu=false;
			resultat.second[validiteModele]=7;
			
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
			int tailleModele(dimParents+1);
			
			// Initialisation du score de Wald
			reel WaldCourant(beta_hat(1, 0)*beta_hat(1,0)/inv_J_info(1, 1));
			resultat.second[WaldScore]=WaldCourant;
			
			//affiche(resultat.first);
			//cout << "	" << WaldCourant;
			
			for (int paramCourant(2); paramCourant<=tailleModele; ++paramCourant)
			{
				WaldCourant=beta_hat(paramCourant, 0)*beta_hat(paramCourant, 0)/inv_J_info(paramCourant, paramCourant);
				//cout << "	" << WaldCourant;
				
				if (WaldCourant < resultat.second[WaldScore])
				{
					resultat.second[WaldScore] = WaldCourant;
				}
			}
			//cout << endl;
			
		// Mise à jour du compteur pour la FDR
		++storey.compteurWald[resultat.first.second.size()][ ( upper_bound(storey.seuilScore.begin(), storey.seuilScore.end(),  resultat.second[WaldScore])-storey.seuilScore.begin() ) ];

			
			
			// STOREY
			//			if (selModeles==signif && (resultat.second[WaldScore]<seuilScoreMultivarie[dimParents+1]))
			if (selModeles==!all && (resultat.second[WaldScore]<seuilScore[dimParents+1]))
			{
				modeleRetenu=false;
				resultat.second[validiteModele]=7;
				
			}
			
			
		//}
		
		
		
		
	}
	//STOREY
	/*else // Aucun parent n'est valide et on ne prend que les meilleurs modèles
	{
		modeleRetenu=false;
		resultat.second[validiteModele]=7;
		
	}*/
	
	// Ici pas besoin de calculer ceci pour les modèles non signif dans le cas signif
	if (modeleRetenu || selModeles==all)
	{
		
		// Calcul des pseudos R carrés
		loglikeCourante=resultat.second[valloglikelihood];
		
		//	cout << (resultat.first.first) << " " << *(resultat.first.second.begin()) << " " << loglikeCourante << " " << loglikeZero  
		//	<< " " << 1-scythe::pchisq(resultat.second[Gscore], 1) << " " <<  1-scythe::pchisq(resultat.second[WaldScore], 1) << "\n";
		// McFadden
		resultat.second[McFadden] = 1 - (loglikeCourante / loglikeZero);
		
		// McFadden adjusted ->TRICHE
		resultat.second[McFaddenAdj] = 1 - ((loglikeCourante-nbParamEstimes-1) / loglikeZero);
		
		reel exposant(2./taille);
		
		// Cox & Snell
		resultat.second[CoxSnell] = 1 - pow((exp(loglikeZero) / exp(loglikeCourante)), exposant);
		
		// Nagelkerke / Cragg & Uhler
		resultat.second[Nagelkerke] = (pow(exp(loglikeCourante), exposant) - pow(exp(loglikeZero), exposant))/(1 - pow(exp(loglikeZero), exposant));
		
		// AIC
		resultat.second[AIC] = -2*loglikeCourante + 2*nbParam;
		
		// BIC
		resultat.second[BIC] = -2*loglikeCourante + 2*nbParam*log(taille);
	}
	return modeleRetenu;
	
}

void RegressionLogistique::calculeGWR(int numMarq,  const set<int> & varContinues, resModele& resultat)
{
	cout << "%" << numMarq << endl;
	int nbVarCont(varContinues.size());
	int dim(nbVarCont);
	
	if (dim==0)
	{
		return;
	}
	nbParam=dim+1;
	
	int tailleResDepart(resultat.second.size());
	int nbStatsGWR(9+2*dim), nbStatsGWRbase(7);
	resultat.second.resize(tailleResDepart+nbStatsGWR, 0);
	
	bool recalculeLoglikeGlobale(false);	// Si des points inclus dans le modèle global n'ont pas été inclus dans le modèle local, il faut recalculer la loglike globale pour la cohérence du nombre de points
	
	Domaine pointsGWR;
	pointsGWR.masque = pointsMarq.masque;
	
	for (set<int>::iterator i(varContinues.begin()); i!=varContinues.end(); ++i)
	{
		pointsGWR.masque%=masqueX(_, *i);
	}
	
	pointsGWR.miseAJour();
	double  sommeGWR(toolbox::sommeNumerique(dataMarq(_, numMarq)%pointsGWR.masque));
	
	
	
	if (pointsGWR.taille==taille)	// Cas où les var env n'avaient pas de valeurs manquantes supplémentaires par rapport aux crd et au marqueur
	{
		pointsGWR.poids=pointsMarq.poids;
	}
	else
	{
		if (sommeGWR == pointsGWR.taille || sommeGWR==0) // Y est constant!
		{
			resultat.second[tailleResDepart+nbStatsGWR-1]=1;	// La p-valeur vaut 1
			return;
		}
		
		
		recalculeLoglikeGlobale=true;
		reel sommePond(0);
		int nbVoisins(0);
		if (choixPonderation!=pondPlusProchesVoisins)
		{
			cout << "ICI" << endl;
			for (int i(0); i<nbPoints; ++i)
			{
				if (pointsGWR.masque(i,0))
				{
					nbVoisins=pointsMarq.poids[i].size();
					for (int j(0); j<nbVoisins; ++j)
					{
						if (pointsGWR.masque(pointsMarq.poids[i][j].first,0))
						{
							sommePond += pointsMarq.poids[i][j].second;	
							pointsGWR.poids[i].push_back(pointsMarq.poids[i][j]);
						}	
					}
					sommePond += 1.; // Pour le point i
					nbVoisins=pointsGWR.poids[i].size();
					for (int j(0); j<nbVoisins; ++j)
					{
						pointsGWR.poids[i][j].second /= sommePond;	
					}
					pointsGWR.poids[i].push_back(make_pair(i, 1./sommePond));
					
				}
			}
		}
		else	// Cas plus proches voisins, on reprend la liste du voisinage
		{
			
			// Cas où tous les points valides sont voisins
			if (pointsGWR.taille<=(nbPlusProchesVoisins+1))
			{
				double poids(1./pointsGWR.taille);
				for (int i(0); i<nbPoints; ++i)
				{
					if (pointsGWR.masque(i,0))
					{
						for (int j(i); j<nbPoints; ++j)
						{
							if (pointsGWR.masque(j,0))
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
				for (int i(0); i<nbPoints; ++i)
				{
					if (pointsGWR.masque(i,0))
					{
						nbVoisinsCourants=0;
						voisinCourant=voisinage[i].begin();
						while (nbVoisinsCourants<nbPlusProchesVoisins && voisinCourant!=voisinage[i].end())
						{
							if (pointsGWR.masque(voisinCourant->first, 0))
							{
								pointsGWR.poids[i].push_back(make_pair(voisinCourant->first, 1));
								++voisinCourant;
								++nbVoisinsCourants;
							}
						}
						// On cherche si des points sont à la même distance que le dernier
						voisinSuivant=voisinCourant+1;
						while((voisinSuivant!=voisinage[i].end()) && abs((voisinSuivant)->second - voisinCourant->second) < plusPetiteDiffDist) 
						{
							if (pointsGWR.masque(voisinSuivant->first,0))
							{
								pointsGWR.poids[i].push_back(make_pair(voisinSuivant->first, 1));
								
								++voisinCourant;
								++voisinSuivant;
								++nbVoisinsCourants;
							}
						}
						pointsGWR.poids[i].push_back(make_pair(i, 1));
						
						sommePond=nbVoisinsCourants+1; // +1 pour le point lui-même
						double poids(1./sommePond);
						for (int j(0); j<sommePond; ++j)
						{
							pointsGWR.poids[i][j].second=poids;						
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
	
	for (int i(0); i<nbPoints; ++i)
	{
		if (pointsGWR.masque(i,0)==false)
		{
			continue;
		}
		
		X_l=0;
		Y_l=0;
		
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
		MatriceReels listePoids(nbVoisins,1,true,0);	// Sert à multiplier les pi * (1-pi) dans la boucle
		for (int k(0); k<nbVoisins; ++k)
		{
			voisinCourant=pointsGWR.poids[i][k].first;
			listePoids(k,0)=pointsGWR.poids[i][k].second;
			//if (masqueGWR(voisinCourant,0) == true)
			//{
			X_l(k, 0)=1;
			set<int>::iterator iter(varContinues.begin());
			for (int l(0); l<nbVarCont; ++l)
			{
				X_l(k, l+1) = dataEnv(voisinCourant, *iter);
				++iter;
			}
			Y_l(k, 0) = dataMarq(voisinCourant, numMarq);
			//			}
			
		}
		
		//cout << "!" << numMarq << " " << i << " " << nbVoisins; //<< " " << X_l << " " << Y_l << endl;
		
		if (sum(Y_l)==0 || sum(Y_l)==pointsGWR.taille)	// Y est constant
		{
			cout << "P " << i << " " << 5 << " " << sum(Y_l) << t(X_l(0,1,nbVoisins-1,1)) << endl;
			
			recalculeLoglikeGlobale=true;
			pointsValides(i,0)=false;
			continue;
		}
		
		// Initialisation de beta_hat_local, on prend l'estimation globale
		beta_hat_l = beta_hat;
		
		int limiteLignes(nbVoisins-1), limiteCols(nbParam-1);
		
		// Test de convergence
		bool continueCalcul(true), singularMatrix(false), divergentCalculation(false);
		int nbIterations(0), typeErreur(0);
		
		// Iteration
		while(continueCalcul && !singularMatrix && !divergentCalculation && (nbIterations < limiteIter))
		{
			cout << "%" <<i << " " << nbIterations<< " " << sum(nouv_Xb_l(0,0,limiteLignes,0)) << " "<< (J_info_l)[2,2]<< " "<< beta_hat_l << endl;
			// Calcul pi
			//cout << X_l(0,0,limiteLignes,limiteCols)<< beta_hat_l << X_l(0,0,limiteLignes,limiteCols) * beta_hat_l;
			nouv_Xb_l(0,0,limiteLignes,0) = (X_l(0,0,limiteLignes,limiteCols) * beta_hat_l)(0,0,limiteLignes,0); // Test avant l'exp
			
			if (max(nouv_Xb_l(0,0,limiteLignes,0))>limiteExp)
			{
				continueCalcul=false;
				//loglikeCourante = sum((Y_l%(Xb_l) - log(1.+exp_Xb_l))(0,0,nbVoisinsValides-1,0));
				typeErreur=1;
				
			}
			else
			{
				++nbIterations;
				
				// Calcul pi
				Xb_l(0,0,limiteLignes,0)=nouv_Xb_l(0,0,limiteLignes,0);
				exp_Xb_l(0,0,limiteLignes,0) = exp(Xb_l(0,0,limiteLignes,0));
				
				//cout << nbIterations << "\n" << beta_hat << "\n" << Xb << "\n" << exp_Xb << "\n";
				
				pi_hat_l(0,0,limiteLignes,0) = exp_Xb_l(0,0,limiteLignes,0) / (1 + exp_Xb_l(0,0,limiteLignes,0));
				//cout << endl << pi_hat_l(0,0,limiteLignes,0) << endl<< endl;
				/*if (nbIterations==26)
				 {
				 cout << exp_Xb << "\n" << pi_hat << "\n";
				 }*/
				
				//cout << pi_hat<< "\n";
				// Calcul ni * pi * (1 - pi)
				interm_l(0,0,limiteLignes,0) = (pi_hat_l(0,0,limiteLignes,0) % (1 - pi_hat_l(0,0,limiteLignes,0))) % listePoids(0,0,limiteLignes,0);
				
				// Calcul des scores U
				intermScores_l(0,0,limiteLignes,0) = (Y_l(0,0,limiteLignes,0) - pi_hat_l(0,0,limiteLignes,0))% listePoids(0,0,limiteLignes,0) ;
				//cout << "ç" << endl << intermScores_l(0,0,limiteLignes,0) << "ç" << X_l(0,0,limiteLignes,limiteCols) << endl;
				for (int k(0); k<nbParam; ++k)
				{
					//cout << "£"<< intermScores_l(0,0,limiteLignes,0) % X_l(0,k,limiteLignes,k) << endl;
					scores_l(k, 0) = sum(intermScores_l(0,0,limiteLignes,0) % X_l(0,k,limiteLignes,k));
					//cout<< "&" << intermScores_l(0,0,1,0) << " " << X_l(0,k,1,k) << " " << scores_l(k,0) << endl;
				}
				
				// Calcul de J, qui est symétrique
				for (int k(0); k<nbParam; ++k)
				{
					J_info_l(k, k) = sum(interm_l(0,0,limiteLignes,0) % X_l(0, k,limiteLignes,k) % X_l(0, k,limiteLignes,k));
					for (int l(k+1); l<nbParam; ++l)
					{
						J_info_l(k, l) = sum(interm_l(0,0,limiteLignes,0) % X_l(0, k,limiteLignes,k) % X_l(0, l,limiteLignes,l));
						J_info_l(l, k) = J_info_l(k, l);
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
						inv_J_info_l = invpd(J_info_l);
					}
					catch(scythe_exception& error)
					{
						//cerr << error.message() << "\n";
						singularMatrix = true;
						continueCalcul=false;
						typeErreur=2;
					}
					
					/*			for (int m(0); m<nbParam; ++m)
					 {
					 if (inv_J_info(m, m) <= 0)
					 cerr << i << " " << j << " " << inv_J_info << "\n";
					 }
					 */
					
					if(!singularMatrix)
					{
						// Mise à jour de beta_hat
						nouv_beta_hat_l = beta_hat_l + inv_J_info_l * scores_l;
						
						//cout << inv_J_info_l << det(J_info_l) << "\n" << nouv_beta_hat_l << "\n" << "\n";
						
						for (int l(0); l<nbParam; ++l)
						{
							if (abs(nouv_beta_hat_l(l, 0)) > limiteNaN)
							{
								continueCalcul = false;
								divergentCalculation=true;
								typeErreur=3;
								break;
							}
						}
						
						if (continueCalcul)
						{
							// Test de convergence
							diff_beta_hat_l = nouv_beta_hat_l - beta_hat_l;
							continueCalcul = false;
							for (int l(0); l<nbParam; ++l)
							{
								if (abs(diff_beta_hat_l(l, 0)) > convCrit * max(eps, abs(beta_hat_l(l, 0))))
								{
									continueCalcul = true;
									break;
								}
							}
							
							beta_hat_l = nouv_beta_hat_l;
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
		if (nbIterations==limiteIter)
		{
			typeErreur=4;
		}
		
		if (typeErreur>0)
		{
			cout << "PQ "<< i << " " << typeErreur << " " << t(nouv_beta_hat_l) << " " <<t(beta_hat_l) << endl<< t(listePoids(0,0,limiteLignes,0))<<  endl;
			pointsValides(i,0)=false;
			recalculeLoglikeGlobale=true;
			
		}
		else
		{
			//Copie des betas
			liste_beta_hat(_,i)=beta_hat_l;
			MatriceReels invXtWX(nbParam, nbParam, true,0), XtW(nbParam, nbVoisins);
			for (int j(0); j<nbVoisins; ++j)
			{
				XtW(_, j) = t(X_l(j,_))*interm(j,0);
			}
			try
			{
				invXtWX = invpd(XtW*X_l(0,0,limiteLignes,nbParam-1));
			}
			catch(scythe_exception& error)
			{
				pointsValides(i,0)=false;
				recalculeLoglikeGlobale=true;
				continue;
			}
			// Le point lui-même est le dernier de la liste
			//cout << "^" << i << " " << invXtWX.rows() << " " << invXtWX.cols() << " " << (Y_l(limiteLignes,0,limiteLignes,0)%(Xb_l(limiteLignes,0,limiteLignes,0)) - log(1.+exp_Xb_l(limiteLignes,0,limiteLignes,0)))<< endl;
			traceS += (X_l(limiteLignes,0,limiteLignes,limiteCols)*((invXtWX*XtW)))(0,limiteLignes);
			// Le point i est la dernière ligne
			loglikeGWR += sum(listePoids(0,0,limiteLignes,0)%(Y_l(limiteLignes,0,limiteLignes,0)%(Xb_l(limiteLignes,0,limiteLignes,0)) - log(1.+exp_Xb_l(limiteLignes,0,limiteLignes,0))));
			
		}
	}
	reel loglikeGlobale(resultat.second[valloglikelihood]);
	if (recalculeLoglikeGlobale)
	{
		loglikeGlobale=0;
		int indiceVariablesModeleGlobal(0);
		
		for (int i(0); i<nbPoints; ++i)
		{
			if (pointsValides(i,0))
			{
				//On fait le calcul avec la variable du modèle global
				loglikeGlobale+=Y(indiceVariablesModeleGlobal,0)*Xb(indiceVariablesModeleGlobal,0)-log(1+exp_Xb(indiceVariablesModeleGlobal,0));
				++indiceVariablesModeleGlobal;
			}
			else if(masque(i,0))
			{
				++indiceVariablesModeleGlobal;
			}
			
		}
	}
	int nbPointsValides(toolbox::sommeNumerique(pointsValides));
	cout << " " << nbPointsValides<< endl;
	
	for (int i(0); i<nbPoints; ++i)
	{
		cout << i<< " " << liste_beta_hat(0,i) <<" "<<liste_beta_hat(1,i) << endl;
	}
	if (nbPointsValides>0)
	{
		//On peut calculer AIC
		reel nbParamCorrige((1.*taille*nbParam)/(taille-nbParam-1)), scoreAIC(2*((-loglikeGWR+traceS)/nbPointsValides+(loglikeGlobale-nbParamCorrige)/taille));
		//reel pvalue(pchisq(scoreAIC, traceS-nbParamCorrige));
		
		MatriceReels resumeBeta(nbParam,2, true,0);
		// Calcul des moyennes et ecarts-types beta
		for (int i(0); i<nbParam;++i)
		{
			resumeBeta(_,0)+=liste_beta_hat(_,i);
			resumeBeta(_,1)+=liste_beta_hat(_,i)%liste_beta_hat(_,i);
		}
		resumeBeta(_,0) /= nbPointsValides;
		resumeBeta(_,1) = resumeBeta(_,1)/nbPointsValides - resumeBeta(_,0)%resumeBeta(_,0);
		//copie des résultats
		
		resultat.second[tailleResDepart] = nbPointsValides;
		resultat.second[tailleResDepart+1] = loglikeGlobale;
		resultat.second[tailleResDepart+2] = loglikeGWR;
		resultat.second[tailleResDepart+3] = traceS;
		resultat.second[tailleResDepart+4] = -loglikeGWR + traceS;
		resultat.second[tailleResDepart+5] = scoreAIC;
		//resultat.second[tailleResDepart+6] = pvalue;
		for (int i(0); i<nbParam; ++i)
		{
			resultat.second[tailleResDepart+nbStatsGWRbase+2*i] = resumeBeta(i,0);
			resultat.second[tailleResDepart+nbStatsGWRbase+2*i+1] = resumeBeta(i,1);
		}
	}
	else {
		resultat.second[tailleResDepart+nbStatsGWR-1]=1;
	}
	
}




void RegressionLogistique::initialisationParametres(ParameterSet& listeParam, ParameterSetIndex& indexParam) const
{
	ParameterSetData paramCourant;
	
	// INPUTFILE	
	paramCourant.name="INPUTFILE";
	paramCourant.mandatory=false;
	paramCourant.present=false;
	//paramCourant.tokenize=false;
	listeParam.push_back(paramCourant);
	
	// OUTPUTFILE
	paramCourant.name="OUTPUTFILE";
	listeParam.push_back(paramCourant);
	
	// WORDDELIM
	paramCourant.name="WORDDELIM";
	listeParam.push_back(paramCourant);
	
	// HEADERS
	paramCourant.name="HEADERS";	
	//paramCourant.tokenize=true;
	listeParam.push_back(paramCourant);
	
	// NUMVARENV
	paramCourant.name="NUMVARENV";	
	paramCourant.mandatory=true;
	listeParam.push_back(paramCourant);
	
	// NUMMARK
	paramCourant.name="NUMMARK";	
	listeParam.push_back(paramCourant);
	
	// NUMINDIV
	paramCourant.name="NUMINDIV";	
	listeParam.push_back(paramCourant);
	
	// IDINDIV
	paramCourant.name="IDINDIV";
	paramCourant.mandatory=false;	
	listeParam.push_back(paramCourant);	
	
	// COLSUPENV
	paramCourant.name="COLSUPENV";
	listeParam.push_back(paramCourant);
	
	// COLSUPMARK
	paramCourant.name="COLSUPMARK";	
	listeParam.push_back(paramCourant);
	
	// SUBSETVARENV
	paramCourant.name="SUBSETVARENV";	
	listeParam.push_back(paramCourant);

	// SUBSETMARK
	paramCourant.name="SUBSETMARK";	
	listeParam.push_back(paramCourant);
	
	// DIMMAX
	paramCourant.name="DIMMAX";	
	listeParam.push_back(paramCourant);
	
	// SPATIAL
	paramCourant.name="SPATIAL";	
	listeParam.push_back(paramCourant);
	
	// Pré-requis: autocorr, GWR et SHP requièrent le paramètre SPATIAL
	// AUTOCORR
	paramCourant.name="AUTOCORR";
	paramCourant.prereq.push_back("SPATIAL");
	listeParam.push_back(paramCourant);
	
	// GWR
	paramCourant.name="GWR";
	listeParam.push_back(paramCourant);
	
	// SHAPEFILE
	paramCourant.name="SHAPEFILE";
	listeParam.push_back(paramCourant);
	
	// DISCRETEVAR
	paramCourant.name="DISCRETEVAR";	
	paramCourant.prereq.clear();
	listeParam.push_back(paramCourant);
	
	// SAVETYPE
	paramCourant.name="SAVETYPE";	
	paramCourant.mandatory=true;
	listeParam.push_back(paramCourant);	
	
	int nbTotParam(listeParam.size());
	for (int i(0); i<nbTotParam; ++i)
	{
		indexParam.insert(make_pair(listeParam[i].name, i));
	}
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

int ComparaisonResultats::caseComparaisonResultats=0;

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
	caseComparaisonResultats=i;
}

bool ComparaisonResultats::plusPetitQue(const groupeResultats::value_type* const &  r1, const groupeResultats::value_type* const &  r2) 
{
	return ((r1->second[caseComparaisonResultats])<(r2->second[caseComparaisonResultats]));
}

bool ComparaisonResultats::plusGrandQue(const groupeResultats::value_type* const &  r1, const groupeResultats::value_type* const &  r2) 
{
	return ((r1->second[caseComparaisonResultats])>(r2->second[caseComparaisonResultats]));
}


ComparaisonResultats::ComparaisonResultats(ComparaisonResultats& c)
{}

