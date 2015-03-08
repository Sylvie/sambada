#include "Duree.h"

#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

PrecisionDuree::PrecisionDuree(int j, int h, int m, int s)
: jours(j), heures(h), minutes(m), secondes(s)
{}


ChablonDuree::ChablonDuree(bool zJours, bool zSecondes)
:zappeJours(zJours), zappeSecondes(zSecondes)
{}


Duree::Duree(int nbSec)
{
	secondes = nbSec%60;
	nbSec /= 60;
	
	minutes = nbSec%60;
	nbSec /= 60;
	
	heures = nbSec%24;
	jours = nbSec / 24;	
}

Duree::Duree(int j, int h, int m, int s)
:jours(j), heures(h), minutes(m), secondes(s)
{}

Duree::~Duree()
{}

int Duree::calculeLargeur(const PrecisionDuree& prec, const ChablonDuree& chablon)
{
	// La précision doit avoir été ajustée au préalable!
	
	int taille(0);
	
	if (!chablon.zappeJours)
	{
		taille = taille + prec.jours + 2; // size of " d" = 2
	}
	taille = taille + prec.heures + prec.minutes + 3;
	if (!chablon.zappeSecondes)
	{
		taille = taille + prec.secondes + 2;
	}
	return taille;
}

Journal& Duree::affiche(Journal& j, const ChablonDuree& chablon) const
{
	ostringstream oss;
	if (!chablon.zappeJours)
	{
		oss << jours << "d ";
	}
	oss	<< heures << "h "
		<< minutes << "m";
	if (!chablon.zappeSecondes)
	{
		oss << " " << secondes << "s ";
	}
	j << oss.str();
	return j;
}

Journal& Duree::affiche(Journal& j, const PrecisionDuree& prec, const ChablonDuree& chablon) const
{
	ostringstream oss;	
	PrecisionDuree precision(prec);
	ajustePrecision(precision);
	if (!chablon.zappeJours)
	{
		oss << setw(precision.jours) << right << jours << "d ";
	}
	oss << setw(precision.heures) << right << heures << "h "
	<< setw(precision.minutes) << right << minutes << "m";
	if (!chablon.zappeSecondes)
	{
		oss << " " << setw(precision.secondes) << right << secondes << "s";
	}
	j << oss.str();
	return j;
}

void Duree::ajustePrecision(PrecisionDuree& p) const
{
	p.jours = max(ceil(log10((double)jours)), (double)p.jours);
	p.heures = max(ceil(log10((double)heures)), (double)p.heures);
	p.minutes = max(ceil(log10((double)minutes)), (double)p.minutes);
	p.secondes = max(ceil(log10((double)secondes)), (double)p.secondes);
}

Duree::Duree(const Duree &d)
:jours(d.jours), heures(d.heures), minutes(d.minutes), secondes(d.secondes)
{}

bool Duree::plusLongueOuEgale(const Duree& d) const
{
	if (jours > d.jours)
	{
		return true;
	}
	else if (jours < d.jours)
	{
		return false;
	}
	else
	{
		if (heures > d.heures)
		{
			return true;
		}
		else if (heures < d.heures)
		{
			return false;
		}	
		else 
		{
			if (minutes > d.minutes)
			{
				return true;
			}
			else if (minutes < d.minutes)
			{
				return false;
			}	
			else 
			{
				if (secondes >= d.secondes)
				{
					return true;
				}
				else
				{
					return false;
				}	
			}
		}
	}
}

bool Duree::plusCourte(const Duree& d) const
{
	return d.plusLongueOuEgale(*this);	
}

int Duree::calculeTailleAffichageJours() const
{
	return (int)ceil(log10(1.0*jours));
}

Journal& operator<<(Journal& j, const Duree& d)
{
	d.affiche(j);
	return j;
}

DureeFormatee::DureeFormatee(const Duree& d, const PrecisionDuree& p, const ChablonDuree& c)
:duree(d), precision(p), chablon(c)
{}

DureeFormatee::~DureeFormatee()
{}

Journal& DureeFormatee::affiche(Journal& j) const
{
	duree.affiche(j, precision, chablon);
}

DureeFormatee::DureeFormatee(const DureeFormatee& d)
:duree(d.duree), precision(d.precision), chablon(d.chablon)
{}

Journal& operator<<(Journal& j, const DureeFormatee& d)
{
	d.affiche(j);
	return j;
}

