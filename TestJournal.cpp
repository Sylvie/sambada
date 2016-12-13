#include "Journal.h"

using namespace std;

int main (int argc, char * argv[])
{
	
	FluxSortie fs;
	string fichierFlux("mon-flux.txt");
	fs.setNomFichier(fichierFlux);
	fs.ouvertureFichier();
	fs << "Hello World!";
	fs << nl;
	fs << "How are you today?" << nl;
	fs.fermetureFichier();
	
	ostringstream oss;
	
	oss << "Hello World!";
	cout << "X " << oss.str() << endl;
	
	ostringstream* ossp;
	ossp=new ostringstream;
	
	(*ossp) << "How are you today?";
	cout << "X " << ossp->str() << endl; 
	Journal j;
	string fichier("mon-journal.txt");
	j << "Hello World!";
	j << nl;
	j << "How are you today?" << nl;
	
	j.afficheJournalTemporaire();
	
	j.setNomFichier(fichier);
	j.sortDeLaPause();
	j.afficheJournalTemporaire();
	
# ifdef DEBUG
	cout << "Coucou" << endl;
#endif
	
	cout << "true=" << true << ", false=" << false << endl; 
	return 0;
}
