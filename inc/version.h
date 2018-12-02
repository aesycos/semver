#ifndef VERSION_H
#define VERSION_H

#define MAJOR 0
#define MINOR 1
#define PATCH 0
#define PRERELEASE "-alpha"
#define BUILDMETA ""
#define SEMVER "2.0.0"

struct version
{
	char major[16];
	char minor[16];
	char patch[16];
	char prerelease[24];
	char build[32];
};


void displayHelp();
void displayVersion();
void getVersion( const char *utilityName, char *utilityVersion );
int checkVersionFormat( const char *string );
void splitVersionString( const char *verStr, struct version * );
int compareVersion( const char *ver1, const char *ver2 );


#endif
