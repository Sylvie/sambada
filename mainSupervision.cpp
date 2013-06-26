#include "RegressionLogistique.h"
#include "Supervision.h"
#include <ctime>

using namespace std;
using namespace scythe;

int main(int argc, char *argv[])
{	
	Supervision regisseur;
	
	if(argc!=2 && argc!=5 && argc!=7 && argc !=8)
	{
		cerr << "Nombre d'arguments incorrects." << endl;
		return 1;
	}
	
	time_t temps_start(time(NULL));
	try
	{
		cerr << string(argv[1]) << endl;
		if (argc ==2)
		{
		regisseur.preparationsCalculs(argv[1]);
		}
		else 
		{
			regisseur.fusionResultats(argc, argv);
		}

	}
	catch (const Erreur& err) 
	{
		cout << err.what() << endl;
		exit(1);
	}
	//time_t temps_interm(time(NULL));
	//cout << "Fin de la lecture : " << difftime(temps_interm, temps_start) << " s." << endl;
	
	
	time_t temps_fin_calculs(time(NULL));
	//cout << "Fin des calculs " << endl;
	cout << "Temps écoulé : " << difftime(temps_fin_calculs, temps_start) << " s." << endl;
	
	
	time_t temps_stop(time(NULL));
	
	cout << "Ecriture des résultats : " << difftime(temps_stop, temps_fin_calculs) << " s." << endl;
	
}


