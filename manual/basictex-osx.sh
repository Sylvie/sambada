#!/usr/bin/env sh

# This script is essentially based on work of the project LaTeX3
# Source: https://github.com/latex3/latex3/

# This script is used for testing using Travis
# It is intended to work on their VM set up: OS X 10.11
# A minimal current TL is installed adding only the packages that are
# required

# See if there is a cached version of TL available
export PATH=$PATH:"/usr/local/texlive/2018basic/bin/x86_64-darwin"
if ! command -v latexmk > /dev/null; then

# Obtain BasicTeX
wget http://mirror.switch.ch/ftp/mirror/tex/systems/mac/mactex/mactex-basictex-20180417.pkg -O BasicTeX.pkg

# Install a minimal system
sudo installer -pkg BasicTeX.pkg -target /

# Setting non-persistent downloads for tlmgr
# Source: https://darrengoossens.wordpress.com/2016/07/03/a-further-little-trick-with-tlmgr-trouble-downloading-and-unzipping/
tlmgr conf tlmgr persistent-downloads 0

# Setting a fixed repository
tlmgr option repository http://mirror.switch.ch/ftp/mirror/tex/systems/texlive/tlnet/

# Updating the installation
sudo tlmgr update --self --all

fi

# Additional requirements for (u)pLaTeX, done with no dependencies to
# avoid large font payloads
sudo tlmgr install --no-depends ptex uptex ptex-base uptex-base ptex-fonts \
uptex-fonts platex uplatex

# Set up graphics: nowadays split over a few places and requiring
# HO's bundle
sudo tlmgr install graphics-def

# Other contrib packages: done as a block to avoid multiple calls to tlmgr
# Dependencies other than the core l3build set up, metafont, fontspec and the
# 'graphics stack' (itself needed by fontspec) are listed below
sudo tlmgr install \
siunitx       \
adobemapping  \
biber         \
biblatex      \
caption		  \
csquotes	  \
datetime2     \
datetime2-english \
datetime2-french  \
dirtree		  \
enumitem	  \
etoolbox	  \
ifxetex		  \
latexmk       \
lm-math       \
logreq        \
mdframed	  \
multirow	  \
needspace	  \
tracklang     \
units         \
xkeyval		  \
xstring		  \
was

# Keep no backups (not required, simply makes cache bigger)
sudo tlmgr option -- autobackup 0

cat /usr/local/texlive/2018basic/texmf-dist/web2c/updmap.cfg

# Update the TL install but add nothing new
sudo tlmgr update --self --all --no-auto-install
