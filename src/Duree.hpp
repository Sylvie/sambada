/*************************************************************************
 * Copyright (©) 2011-2019 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

#ifndef DUREE_H
#define DUREE_H

#include "Journal.hpp"

struct PrecisionDuree
{
	int jours, heures, minutes, secondes;

	PrecisionDuree(int j, int h, int m, int s);
};

struct ChablonDuree
{
	bool zappeJours, zappeSecondes;

	ChablonDuree(bool zJours, bool zSecondes);
};

class Duree
{
public:
	Duree(int nbSec);

	Duree(int j, int h, int m, int s);

	Duree(const Duree& d);

	virtual ~Duree();

	static int calculeLargeur(const PrecisionDuree& prec, const ChablonDuree& chablon);

	Journal& affiche(Journal& j, const ChablonDuree& chablon = ChablonDuree(false, false)) const;

	Journal& affiche(Journal& j, const PrecisionDuree& precision, const ChablonDuree& chablon = ChablonDuree(false, false)) const;

	bool plusLongueOuEgale(const Duree& d) const;

	bool plusCourte(const Duree& d) const;

	int calculeTailleAffichageJours() const;

protected:
	int jours, heures, minutes, secondes;

	void ajustePrecision(PrecisionDuree& p) const;
};

Journal& operator<<(Journal& j, const Duree& d);

class DureeFormatee
{
public:
	DureeFormatee(const Duree& d, const PrecisionDuree& p, const ChablonDuree& c = ChablonDuree(false, false));

	virtual ~DureeFormatee();

	DureeFormatee(const DureeFormatee& d);

	Journal& affiche(Journal& j) const;

protected:
	const Duree& duree;
	const PrecisionDuree& precision;
	const ChablonDuree& chablon;

};

Journal& operator<<(Journal& j, const DureeFormatee& d);

#endif // DUREE_H
