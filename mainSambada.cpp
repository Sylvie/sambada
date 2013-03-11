#include "RegressionLogistique.h"
#include <ctime>

using namespace std;
using namespace scythe;

int main(int argc, char *argv[])
{		
	RegressionLogistique logitModel;
	
	//cout << numeric_limits < double >::max() << " " << log(numeric_limits < double >::max()/2) << endl;
	//cout << numeric_limits < long double >::max() << " " << log(numeric_limits < long double >::max()/2) << endl;
	
	time_t temps_start(time(NULL));
	try
	{
		logitModel.initialisation(argc, argv);
	}
	catch (const Erreur& err) 
	{
		cout << err.what() << endl;
		exit(1);
	}
	time_t temps_interm(time(NULL));
	cout << "Fin de la lecture : " << difftime(temps_interm, temps_start) << " s." << endl;
	
	//logitModel.analyseCategories();
	//logitModel.calculeCorrelations();
	logitModel.calculeAutocorrelations();
	logitModel.creeModelesGlobaux();
	
	time_t temps_fin_calculs(time(NULL));
	cout << "Fin des calculs " << endl;
	cout << "Temps écoulé : " << difftime(temps_fin_calculs, temps_interm) << " s." << endl;
	
	//logitModel.ecritResultats("ResultatsRegression.txt");
	
	time_t temps_stop(time(NULL));

	cout << "Ecriture des résultats : " << difftime(temps_stop, temps_fin_calculs) << " s." << endl;

}

