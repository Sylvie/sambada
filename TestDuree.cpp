#include "Duree.h"
#include <iomanip>

using namespace std;

int main(int argc, char** argv)
{
	Journal j;
	j.setActivites(true, false);
	j.sortDeLaPause();
	
	PrecisionDuree p(1,2,2,1);
	Duree d(4000);
	
	d.affiche(j, p);
	j <<nl;
	
	j << DureeFormatee(d,p) << nl;
	
	j << d << nl;
		
	return 0;
}	