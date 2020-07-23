/*************************************************************************
* Copyright (©) 2011-2020 EPFL (Ecole Polytechnique fédérale de Lausanne)
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
*************************************************************************/

#include "LecteurParametres.hpp"
namespace sambada {

	void LecteurParametres::initialiseParametres(Parametres& parametres) const
	{
		Parametre paramCourant;

		// INPUTFILE
		paramCourant.name = "INPUTFILE";
		paramCourant.mandatory = false;
		paramCourant.present = false;
		parametres.entrees.push_back(paramCourant);

		// OUTPUTFILE
		paramCourant.name = "OUTPUTFILE";
		parametres.entrees.push_back(paramCourant);

		// WORDDELIM
		paramCourant.name = "WORDDELIM";
		parametres.entrees.push_back(paramCourant);

		// LOG
		paramCourant.name = "LOG";
		parametres.entrees.push_back(paramCourant);

		// UNCONVERGEDMODELS
		paramCourant.name = "UNCONVERGEDMODELS";
		parametres.entrees.push_back(paramCourant);

		// HEADERS
		paramCourant.name = "HEADERS";
		//paramCourant.tokenize=true;
		parametres.entrees.push_back(paramCourant);

		// NUMVARENV
		paramCourant.name = "NUMVARENV";
		paramCourant.mandatory = true;
		parametres.entrees.push_back(paramCourant);

		// NUMMARK
		paramCourant.name = "NUMMARK";
		parametres.entrees.push_back(paramCourant);

		// NUMINDIV
		paramCourant.name = "NUMINDIV";
		parametres.entrees.push_back(paramCourant);

		// IDINDIV
		paramCourant.name = "IDINDIV";
		paramCourant.mandatory = false;
		parametres.entrees.push_back(paramCourant);

		// COLSUPENV
		paramCourant.name = "COLSUPENV";
		parametres.entrees.push_back(paramCourant);

		// COLSUPMARK
		paramCourant.name = "COLSUPMARK";
		parametres.entrees.push_back(paramCourant);

		// SUBSETVARENV
		paramCourant.name = "SUBSETVARENV";
		parametres.entrees.push_back(paramCourant);

		// SUBSETMARK
		paramCourant.name = "SUBSETMARK";
		parametres.entrees.push_back(paramCourant);

		// DIMMAX
		paramCourant.name = "DIMMAX";
		parametres.entrees.push_back(paramCourant);

		// SPATIAL
		paramCourant.name = "SPATIAL";
		parametres.entrees.push_back(paramCourant);

		// Pré-requis: autocorr, GWR et SHP requièrent le paramètre SPATIAL
		// AUTOCORR
		paramCourant.name = "AUTOCORR";
		paramCourant.prereq.push_back("SPATIAL");
		parametres.entrees.push_back(paramCourant);

		// GWR
		paramCourant.name = "GWR";
		parametres.entrees.push_back(paramCourant);

		// SHAPEFILE
		paramCourant.name = "SHAPEFILE";
		parametres.entrees.push_back(paramCourant);

		// DISCRETEVAR
		paramCourant.name = "DISCRETEVAR";
		paramCourant.prereq.clear();
		parametres.entrees.push_back(paramCourant);

		// SAVETYPE
		paramCourant.name = "SAVETYPE";
		paramCourant.mandatory = true;
		parametres.entrees.push_back(paramCourant);

		// POPULATIONVAR
		paramCourant.name = "POPULATIONVAR";
		paramCourant.mandatory = false;
		parametres.entrees.push_back(paramCourant);

		// STOREY
		paramCourant.name = "STOREY";
		paramCourant.mandatory = false;
		parametres.entrees.push_back(paramCourant);

		int nbTotParam(parametres.entrees.size());
		for (int i(0); i < nbTotParam; ++i)
		{
			parametres.index.insert(make_pair(parametres.entrees[i].name, i));
		}
	}

}