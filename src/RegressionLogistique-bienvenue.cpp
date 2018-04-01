/*************************************************************************
 * Copyright (©) 2011-2018 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

#include "RegressionLogistique.h"

using namespace std;

vector<string> RegressionLogistique::getMessageBienvenue(bool versionLongue)
{
	vector<string> message;
	message.push_back("| Welcome to Sambada!");
	message.push_back("| Copyright (C) 2011-2018 EPFL (Ecole Polytechnique federale de Lausanne)");
	message.push_back("| Laboratory of Geographic information systems (LaSIG)");
	if (!versionLongue)
	{

		message.push_back("| Contact: sylvie.stucki@a3.epfl.ch, stephane.joost@epfl.ch");
		message.push_back("| Sambada is free software and comes with ABSOLUTELY NO WARRANTY.");
		message.push_back("| Please refer to the file AUTHORS for details.");
	}
	else
	{
		message.push_back("| Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.");
		message.push_back("| Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.");
		message.push_back("| You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.");
		message.push_back("|");

		message.push_back("| Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), Stephane Joost (stephane.joost@epfl.ch)");
		message.push_back("| Laboratory of Geographic information systems");
		message.push_back("| EPFL ENAC IIE LASIG");
		message.push_back("| Station 18");
		message.push_back("| CH-1015 Lausanne");
		message.push_back("| Web site : http://lasig.epfl.ch/sambada");
		message.push_back("|");

		message.push_back("| Sambada includes two libraries: Scythe Statistical Library (under GPL 3) and Shapefile C Library (under LGPL 2.1, courtesy of Frank Warmerdam).");
		message.push_back("|");

		message.push_back("| Scythe Statistical Library");
		message.push_back("| Copyright (C) 2000-2002 Andrew D. Martin and Kevin M. Quinn;");
		message.push_back("| 2002-2012 Andrew D. Martin, Kevin M. Quinn, and Daniel Pemstein.  All Rights Reserved.");
		message.push_back("|");

		message.push_back("| Shapefile C Library");
		message.push_back("| Copyright (c) 1999, Frank Warmerdam");
	}
	return message;
}


ostream& RegressionLogistique::messageBienvenue(ostream& out, bool versionLongue)
{
	vector<string> message(getMessageBienvenue(versionLongue));
	for (int i(0); i<message.size(); ++i)
	{
		out << message[i] << "\n";
	}
	out << flush;
	return out;
}
