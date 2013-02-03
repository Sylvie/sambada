#include "Erreur.h"

using namespace std;

Erreur::Erreur(const string& n, const string& p, bool arret) throw()
:nom(n), phrase(p), fatale(arret)
{}

const char* Erreur::what() const throw()
{
	return phrase.c_str();
}

bool Erreur::estFatale() const throw()
{
	return fatale;
}

string Erreur::getName() const
{
	return nom;
}

string Erreur::getPhrase() const
{
	return phrase;
}

Erreur::~Erreur() throw()
{}

