#include "fluxsortie.h"
#include <iomanip>

FluxSortie::FluxSortie()
    :delimLignes("\n"), delimMots(" "), terminalActif(true), fichierActif(true), fichierOperationnel(false), estMessageErreur(false)
{
}

FluxSortie::~FluxSortie()
{
    fermetureFichier();
}

void FluxSortie::setDelimLignes(const string &delim)
{
    delimLignes=delim;
}

string FluxSortie::getDelimLignes() const
{
    return delimLignes;
}

void FluxSortie::setDelimMots(const string &delim)
{
    delimMots=delim;
}

string FluxSortie::getDelimMots() const
{
    return delimMots;
}

void FluxSortie::setNomFichier(const string &nom)
{
    nomFichier=nom;
}

string FluxSortie::getNomFichier() const
{
    return nomFichier;
}

void FluxSortie::setDelims(const string& delimL, const string& delimM)
{
    delimLignes=delimL;
    delimMots=delimM;
}

void FluxSortie::setActiviteTerminal(bool b)
{
    terminalActif=b;
}

bool FluxSortie::getActiviteTerminal() const
{
    return terminalActif;
}

void FluxSortie::setActiviteFichier(bool b)
{
    fichierActif=b;
}

bool FluxSortie::getActiviteFichier() const
{
    return fichierActif;
}

void FluxSortie::setActivites(bool term, bool fichier)
{
    terminalActif=term;
    fichierActif=fichier;
}

bool FluxSortie::ouvertureFichier()
{
    if( nomFichier.size()==0 )
    {
        return false;
    }

    sortie.open(nomFichier.c_str());
    if (sortie.fail())
    {
        return false;
    }

    fichierOperationnel=true;
    return true;
}

void FluxSortie::fermetureFichier()
{
    sortie.close();
}

bool FluxSortie::testeValiditeFichier()
{
    if (sortie.fail())
    {
        fichierOperationnel=false;
    }

    return fichierOperationnel;
}

void FluxSortie::erreurDetectee()
{
    estMessageErreur=true;
}

FluxSortie &FluxSortie::nouvMot()
{
    ecrit(delimMots);
    return *this;
}



FluxSortie& FluxSortie::retourLigne()
{
    ecrit(delimLignes);
    return *this;
}

FluxSortie& FluxSortie::operator<<(FluxSortie& (*pf)(FluxSortie&)) {
    return pf(*this);
}

FluxSortie& FluxSortie::operator<<(ostream &(*pf)(ostream &))
{
    if (terminalActif)
    {
        pf(cout);
    }

    if (fichierActif && fichierOperationnel)
    {
        pf(sortie);
    }
    return *this;
}

FluxSortie& FluxSortie::operator<<(JournalTemporaire& jt)
{
    jt.synchronise();
    while(!jt.empty())
    {
        ecrit(jt.front());
        retourLigne();
        jt.pop();
    }
    return *this;
}

FluxSortie& nm (FluxSortie& fs)
{
    fs.nouvMot();
    return fs;
}


FluxSortie& nl (FluxSortie& fs)
{
    fs.retourLigne();
    return fs;
}


FluxSortie& erreur (FluxSortie& fs)
{
    fs.erreurDetectee();
    return fs;
}

