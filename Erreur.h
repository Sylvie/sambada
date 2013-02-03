#ifndef ERREUR_H
#define ERREUR_H

#include <exception>
#include <string>

using namespace std;

class Erreur: public exception
{
public:
	Erreur(const string& n="", const string& p="", bool arret=true) throw();	

	virtual const char* what() const throw();
	
	bool estFatale() const throw();
	string getName() const;
	string getPhrase() const;
    virtual ~Erreur() throw();
	
protected:
    string nom;             // Type de l'erreur.
	string phrase;
    bool fatale;            // Gravité de l'erreur.
};

#endif // ERREUR_H

