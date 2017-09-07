#include "Chronometre.h"
#include <cmath>
using namespace std;

int main(int argc, char** argv)
{
	Journal j;
	j.setActivites(true, false);
	j.sortDeLaPause();
	
	Chronometre chrono;
	
	int nombreIterations(88), prochaineMesure;
	prochaineMesure = chrono.initialisation(&j, nombreIterations, 10, " | ");
	j << "Une entrée dans le journal" << nl;
	cout << floor(3)<< " " << floor(-1.5) << " "<< 0.3/0.1+1 << endl;
	cout << "* " << nombreIterations << endl; 
	
	for (int i(0); i<nombreIterations; ++i)
	{
		if (i%3==0)
		{
			sleep(1);
		}
		if (i==prochaineMesure)
		{
			//cout << i<< " " << 100.*i/nombreIterations << endl;
			prochaineMesure=chrono.mesureEtAffiche(i);
			//sleep(2);
		}
	}
	
	chrono.fin();
	
	return 0;
}