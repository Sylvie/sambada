##########################################################################
# Copyright (©) 2011-2023 EPFL (Ecole Polytechnique fédérale de Lausanne)
# Laboratory of Geographic information systems (LaSIG)
#
# This file is part of Sambada.
#
# Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.
# Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.
#
# Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), Stéphane Joost (stephane.joost@epfl.ch)
# Laboratory of Geographic information systems
# EPFL ENAC IIE LASIG
# Station 18
# CH-1015 Lausanne
# Web site : http://lasig.epfl.ch/sambada
##########################################################################

## Adding files to internal library ##

src_modeles_libmodeles_a_SOURCES += \
	src/modeles/scriptorium/lecteur/FlotEntreeFactory.hpp \
	src/modeles/scriptorium/lecteur/FlotEntreeChaineFactory.hpp \
	src/modeles/scriptorium/lecteur/FlotEntreeChaineFactory.cpp \
	src/modeles/scriptorium/lecteur/FlotEntreeFichierFactory.hpp \
	src/modeles/scriptorium/lecteur/FlotEntreeFichierFactory.cpp \
	src/modeles/scriptorium/lecteur/Lecteur.hpp \
	src/modeles/scriptorium/lecteur/Lecteur.cpp


## Cleaning ##
clean-local-src-modeles-scriptorium-lecteur:
	$(call clean_extra_generated_files, src/modeles/scriptorium/scribe/)
