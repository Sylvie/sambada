#!/bin/bash

FILE=VERSION

if ( [ -d .git ] && ( command -v git >/dev/null 2>&1 ) )
then
	VERSION_NUM=$(git describe --tags --abbrev=0)
	BUILD_NUM=$(git describe --long --tags --dirty --always)
	OLD_VERSION_NUM=$(head -1 ${FILE})
	OLD_BUILD_NUM=$(head -2 ${FILE} | tail -1)

	#echo $VERSION_NUM
	#echo $BUILD_NUM
	#echo $OLD_VERSION_NUM
	#echo $OLD_BUILD_NUM
	
	if ( [ "$VERSION_NUM" == "$OLD_VERSION_NUM" ] && [ "$BUILD_NUM" = "$OLD_BUILD_NUM" ] )
	then
		echo "Version number is up-to-date." > /dev/stderr
	else	
		echo "Updating version number..." > /dev/stderr
		echo $VERSION_NUM > ${FILE}
		echo $BUILD_NUM >> ${FILE}
		echo "Version is "$VERSION_NUM" and build number is "$BUILD_NUM.  > /dev/stderr
	fi
	echo ${VERSION_NUM};
else
	CHECKFILE="yes"
	if ( [ -r $FILE ] )
	then
		echo "File $FILE is present." > /dev/stderr
		declare -a data
		while read -r; do
			data+=( "$REPLY" )
		done < ${FILE}
		if (( ${#data[*]} == 2 ))
		then
			echo "File $FILE has two lines as expected." > /dev/stderr
			echo "Version is "${data[0]}" and build number is "${data[1]}.  > /dev/stderr
			echo  ${data[0]}
		else
			echo "File ${FILE} has ${#data[*]} lines instead of 2." > /dev/stderr
			CHECKFILE="no"
		fi
	else
		CHECKFILE="no"
	fi
	if [[  "$CHECKFILE" == "no"  ]]
	then
		echo "Unknown version, updating file $FILE accordingly." > /dev/stderr
		VERSION_NUM="vUnknown"
		BUILD_NUM="unknown-version"
		echo $VERSION_NUM > ${FILE}
		echo $BUILD_NUM >> ${FILE}
		echo $VERSION_NUM 
	fi 
fi