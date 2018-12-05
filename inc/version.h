#include <regex.h>
#ifndef VERSION_H
#define VERSION_H

#define MAJOR 			1
#define MINOR 			0
#define PATCH 			0
#define PRERELEASE 		"-alpha"
#define BUILDMETA 		""
#define SEMVER 			"2.0.0"

#define MMP_PATTERN		"([[:digit:]]+.){2}[[:digit:]]+"
#define MM_PATTERN     	"[[:digit:]]+.[[:digit:]]+"
#define PR_PATTERN     	"-[[:alnum:].-]+"
#define B_PATTERN      	"\\+[[:alnum:].-]+"
#define SEMVER_PATTERN 	"([[:digit:]]+.){2}([[:digit:]]+)+(-[[:alnum:].-]*)?"
#define GNU_PATTERN    	"([[:digit:]]+.[[:digit:]]+)([[:digit:]()]+)?(-[a-zA-Z0-9.-]*)?(\\+[a-zA-Z0-9.-]+)?$"
#define GENERIC_PATTERN "[[:digit:]]+.[[:digit:]]+(.[[:digit:]]+)*([[:digit:]()]+)?(-[[:alnum:].-]+)?"

struct version
{
	char major[3];
	char minor[4];
	char patch[4];
	char prerelease[24];
	char build[24];
};


void displayHelp();
void displayVersion();
void getVersion( const char *utilityName, char *utilityVersion );
int checkFormat( const char *string );
void splitVersionString( const char *versionString, struct version * );
int compareVersion( const char *utilityVersion, const char *compareVersion );
int match( const char *pattern, const char*string, regmatch_t *matchptr, size_t nmatch );


#endif
