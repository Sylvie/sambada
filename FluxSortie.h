#ifndef FLUXSORTIE_H
#define FLUXSORTIE_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class FluxSortie
{
public:
    FluxSortie();
    virtual ~FluxSortie();

    void setDelimLignes(const string& delim);
    string getDelimLignes() const;

    void setDelimMots(const string& delim);
    string getDelimMots() const;

    void setNomFichier(const string& nom);
    string getNomFichier() const;

    void setDelims(const string& delimL, const string& delimM);

    void setActiviteTerminal(bool b);
    bool getActiviteTerminal() const;

    void setActiviteFichier(bool b);
    bool getActiviteFichier() const;

    void setActivites(bool term, bool fichier);

    bool ouvertureFichier();
    void fermetureFichier();

    bool testeValiditeFichier();

    void erreurDetectee();

    template<class T>
    FluxSortie& ecrit(const T& token);

    FluxSortie& retourLigne();

    FluxSortie& nouvMot();

    template<class T>
    FluxSortie& operator<<(const T& token);

    FluxSortie& operator<<(FluxSortie& (*pf)(FluxSortie&));

    FluxSortie& operator<<(ostream& (*pf)(ostream&));


protected:
    string delimLignes; // caractère de retour ligne
    string delimMots; // caractère de séparation entre les mots
    bool terminalActif, fichierActif, fichierValide, estErreur;
    string nomFichier;
    ofstream sortie;
};

FluxSortie& rl (FluxSortie& fs);

FluxSortie& nm (FluxSortie& fs);

FluxSortie& erreur (FluxSortie& fs);

template<class T>
FluxSortie& FluxSortie::ecrit(const T& token)
{
    if (terminalActif && !estErreur)
    {
       cout << token;
    }
    else if (terminalActif && estErreur)
    {
       cout <<token << flush;
    }

    if (fichierActif && fichierValide && !estErreur)
    {
        sortie << token;
    }
    else if (fichierActif && fichierValide && estErreur)
    {
        sortie << token << flush;
    }

    estErreur=false;

    return *this;
}

template<class T>
FluxSortie& FluxSortie::operator<<(const T& token)
{
    return ecrit(token);
}


#endif // FLUXSORTIE_H
