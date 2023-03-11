/*************************************************************************
 * Copyright (©) 2011-2023 EPFL (Ecole Polytechnique fédérale de Lausanne)
 * Laboratory of Geographic information systems (LaSIG)
 *
 * This file is part of Sambada.
 *
 * Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.
 * Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), Stéphane Joost (stephane.joost@epfl.ch)
 * Laboratory of Geographic information systems
 * EPFL ENAC IIE LASIG
 * Station 18
 * CH-1015 Lausanne
 * Web site : http://lasig.epfl.ch/sambada
 *
 * Sambada includes two libraries: Scythe Statistical Library (under GPL 3) and Shapefile C Library (under LGPL 2.1, courtesy of Frank Warmerdam).
 *
 * Scythe Statistical Library
 * Copyright (C) 2000-2002 Andrew D. Martin and Kevin M. Quinn;
 * 2002-2012 Andrew D. Martin, Kevin M. Quinn, and Daniel Pemstein.  All Rights Reserved.
 *
 * Shapefile C Library
 * Copyright (c) 1999, Frank Warmerdam
 *************************************************************************/


#include "Duree.hpp"

#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

PrecisionDuree::PrecisionDuree(int j, int h, int m, int s)
		: jours(j), heures(h), minutes(m), secondes(s)
{}


ChablonDuree::ChablonDuree(bool zJours, bool zSecondes)
		: zappeJours(zJours), zappeSecondes(zSecondes)
{}


Duree::Duree(int nbSec)
{
	secondes = nbSec % 60;
	nbSec /= 60;

	minutes = nbSec % 60;
	nbSec /= 60;

	heures = nbSec % 24;
	jours = nbSec / 24;
}

Duree::Duree(int j, int h, int m, int s)
		: jours(j), heures(h), minutes(m), secondes(s)
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
	oss << heures << "h "
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
	p.jours = max(ceil(log10((double) jours)), (double) p.jours);
	p.heures = max(ceil(log10((double) heures)), (double) p.heures);
	p.minutes = max(ceil(log10((double) minutes)), (double) p.minutes);
	p.secondes = max(ceil(log10((double) secondes)), (double) p.secondes);
}

Duree::Duree(const Duree& d)
		: jours(d.jours), heures(d.heures), minutes(d.minutes), secondes(d.secondes)
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
	return (int) ceil(log10(1.0 * jours));
}

Journal& operator<<(Journal& j, const Duree& d)
{
	d.affiche(j);
	return j;
}

DureeFormatee::DureeFormatee(const Duree& d, const PrecisionDuree& p, const ChablonDuree& c)
		: duree(d), precision(p), chablon(c)
{}

DureeFormatee::~DureeFormatee()
{}

Journal& DureeFormatee::affiche(Journal& j) const
{
	duree.affiche(j, precision, chablon);
	return j;
}

DureeFormatee::DureeFormatee(const DureeFormatee& d)
		: duree(d.duree), precision(d.precision), chablon(d.chablon)
{}

Journal& operator<<(Journal& j, const DureeFormatee& d)
{
	d.affiche(j);
	return j;
}
