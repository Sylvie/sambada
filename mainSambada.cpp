/*************************************************************************
* Copyright (�) 2011-2014 EPFL (Ecole Polytechnique f�d�rale de Lausanne)
* Laboratory of Geographic information systems (LaSIG)
* 
* This file is part of Sambada.
*  
* Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.
* Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
* You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.
* 
* Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), St�phane Joost (stephane.joost@epfl.ch) 
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



#include "RegressionLogistique.h"
#include <ctime>

using namespace std;
using namespace scythe;

int main(int argc, char *argv[])
{		
	time_t temps_start(time(NULL));

	if (argc==1) 
	{
		toolbox::messageBienvenue(true);
	}
	else
	{
		toolbox::messageBienvenue();
	}	
	
	RegressionLogistique logitModel;
	
	//cout << numeric_limits < double >::max() << " " << log(numeric_limits < double >::max()/2) << endl;
	//cout << numeric_limits < long double >::max() << " " << log(numeric_limits < long double >::max()/2) << endl;
	
	try
	{
		logitModel.initialisation(argc, argv);
	}
	catch (const Erreur& err) 
	{
		cout << err.what() << endl;
		exit(1);
	}
	time_t temps_interm(time(NULL));
	cout << "Fin de la lecture : " << difftime(temps_interm, temps_start) << " s." << endl;
	
	//logitModel.analyseCategories();
	//logitModel.calculeCorrelations();
	logitModel.calculeAutocorrelations();
	logitModel.creeModelesGlobaux();
	
	time_t temps_fin_calculs(time(NULL));
	cout << "Fin des calculs " << endl;
	cout << "Temps écoulé : " << difftime(temps_fin_calculs, temps_interm) << " s." << endl;
	
	//logitModel.ecritResultats("ResultatsRegression.txt");
	
	time_t temps_stop(time(NULL));

	cout << "Ecriture des résultats : " << difftime(temps_stop, temps_fin_calculs) << " s." << endl;
	cout << "Temps total : " << difftime(temps_stop, temps_start) << " s." << endl;

}

