#ifndef VERSION_H
#define VERSION_H

#define MAJOR 0
#define MINOR 1
#define PATCH 0
#define PRERELEASE "-alpha"
#define BUILDMETA ""
#define SEMVER "2.0.0"

void displayHelp();
void displayVersion();
void getMajor( const char *verString );
void getMinor( const char *verString );
void getPatch( const char *verString);
void getPrerelease( const char *verString );
void getBuildmeta( const char *verString );
void getPrecedence( const char *verString );
void getVersion( const char *utilityName );
void checkVersionFormat( const char *pattern, const char *string );
int semverNewer( const char *ver1, const char *ver2 );


#endif
