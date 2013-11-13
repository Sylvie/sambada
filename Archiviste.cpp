#include "Archiviste.h"

using namespace std;

Archiviste::Archiviste()
:nbFichiers(0), nomsFichiers(0), fichiers(0), signeRetourLigne("\n"), delimMots(' ')
{
}

Archiviste::~Archiviste()
{
	fermeture();
/*	for (int i(0); i<nbFichiers; ++i)
	{
			if (fichiers[i]->is_open())
			{
				fichiers[i]->close();
				delete fichiers[i];
			}
	}
	enService=false;
 */
}

Archiviste::Archiviste(const Archiviste&)
{}

void Archiviste::initialise(const vector<string>& noms)
{
	nbFichiers=noms.size();
	nomsFichiers=noms;

}


void Archiviste::fermeture()
{
	if(enService)
	{
	for (int i(0); i<nbFichiers; ++i)
	{
		if (fichiers[i]->is_open())
		{
			fichiers[i]->close();
			delete fichiers[i];
		}
	}
	}
	enService=false;
}

void Archiviste::precision(int p)
{
	if (enService)
	{
	for (int i(0); i<nbFichiers; ++i)
	{
		fichiers[i]->precision(p);
	}
	}
}

int Archiviste::precision() const
{
	if (nbFichiers>0)
	{
		return fichiers[0]->precision();
	}
	else {
		return 0;
	}

}


string Archiviste::getRetourLigne() const
{
	return signeRetourLigne;
}


void Archiviste::setRetourLigne(const string& s)
{
	signeRetourLigne=s;
}

char Archiviste::getDelimMots() const
{
	return delimMots;
}


void Archiviste::setDelimMots(const char c)
{
	delimMots=c;
}



Scribe::Scribe()
:Archiviste()
{
}

Scribe::~Scribe()
{
}



bool Scribe::ouverture()
{
	int precisionRes(22);
	fichiers.resize(nbFichiers);
	for (int i(0); i<nbFichiers; ++i)
	{
		//cerr << "*" << fichiers[i] << endl;
		fichiers[i]=new fstream(nomsFichiers[i].c_str(), ios::out);
		fichiers[i]->precision(precisionRes);
		
		if (fichiers[i]->fail())
		{
			fichiers[i]->clear();
			return false;
		}
		
	}	
	enService=true;
	return true;
}

Lecteur::Lecteur()
:Archiviste()
{
}


bool Lecteur::ouverture()
{
	fichiers.resize(nbFichiers);
	for (int i(0); i<nbFichiers; ++i)
	{
		fichiers[i]=new fstream(nomsFichiers[i].c_str(), ios::in);
		
		if (fichiers[i]->fail())
		{
			cout << fichiers[i]->rdstate() << endl;
			fichiers[i]->clear();
			return false;
		}
		
	}	
	enService=true;
	return true;
}

Lecteur::~Lecteur()
{
}

bool Lecteur::finFichier(int numFichier)
{
	*fichiers[numFichier]>>ws;
	return(fichiers[numFichier]->eof());
}



