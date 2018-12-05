#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "version.h"

void splitVersionString( const char *versionString, struct version *splitVersion )
{
        int versionType = 0;
        char version[16] = {'\0'};
        char build[32] = {'\0'};
        char prerelease[24] = {'\0'};
        regmatch_t matchptr[1];

        versionType = checkFormat( versionString );

        if ( versionType == 1 )
                match ( MMP_PATTERN, versionString, matchptr, (size_t) 1 );
        else if ( versionType == 2 )
                match ( MM_PATTERN, versionString, matchptr, (size_t) 1 );

        // Copy matched string to struct split by major, minor, patch, prerelease, build
        strncpy( version, versionString+matchptr[0].rm_so, ( matchptr[0].rm_eo ) - ( matchptr[0].rm_so ) );
        strcpy( splitVersion->major, strtok( version, "." ) );
        strcpy( splitVersion->minor, strtok( NULL, "." ) );

        if ( versionType == 1 )
                strcpy( splitVersion->patch, strtok( NULL, "."));
        else if ( versionType == 2 )
                strcpy( splitVersion->patch, "0");
        
        // extract prerelease version
        // +1 on pointer to string and -1 on # of chars is to remove '-' from
        // prerelease
        if(match( PR_PATTERN, versionString, matchptr, (size_t) 1 ) )
                strncpy( prerelease, versionString+matchptr[0].rm_so+1, ( matchptr[0].rm_eo - matchptr[0].rm_so )-1 );
  
        // extract build info
        if( match( B_PATTERN, versionString, matchptr, (size_t) 1 ) )    
                strncpy( build, versionString+matchptr[0].rm_so+1, ( matchptr[0].rm_eo - matchptr[0].rm_so )-1 );
  
        strcpy( splitVersion->prerelease, prerelease );
        strcpy( splitVersion->build, build );
}

int compareVersion( const char *utilityVersion, const char *compareVersion )
{
        // Make sure versions are in correct format
        
        // split version strings by part MAJOR MINOR PATCH [PRERELEASE[BUILD]]
	
        struct version *splitVer1 = malloc( sizeof( struct version ) );
        splitVersionString( utilityVersion, splitVer1 );
        struct version *splitVer2 = malloc( sizeof( struct version ) );
        splitVersionString( compareVersion, splitVer2 );

        int status = 1;

        if ( atoi(splitVer1->major) > atoi(splitVer2->major) )
                status = 2;
        else if ( atoi(splitVer1->major) < atoi(splitVer2->major) )
                status = 0;
        else if ( atoi(splitVer1->minor) > atoi(splitVer2->minor) )
                status = 2;
        else if ( atoi(splitVer1->minor) < atoi(splitVer2->minor) )
                status = 0;
        else if ( atoi(splitVer1->patch) > atoi(splitVer2->patch) )
                status = 2;
        else if ( atoi(splitVer1->patch) < atoi(splitVer2->patch) )
                status = 0;

        // TODO: compare prerelease strings maybe strncmp() ?
        // TODO: free structs on heap
        free(splitVer1);
        free(splitVer2);

        return status;
}

int match( const char *pattern, const char*string, regmatch_t *matchptr, size_t nmatch )
{
        regex_t preg;
        char msgbuf[128];
        int status = 0;

        if ( ( status = regcomp( &preg, pattern, REG_EXTENDED ) ) )
        {
                regerror( status, &preg, msgbuf, sizeof(msgbuf) );
                fprintf( stderr, "Regex Error: %s\n", msgbuf );
                exit(1);
        }

        if (regexec( &preg, string, nmatch, matchptr, 0) == REG_NOMATCH)
                status = 0;
        else
                status = 1;
        
        regfree(&preg);
        return status;
}

int checkFormat( const char *string )
{
        if ( match( SEMVER_PATTERN, string, NULL, (size_t) 0 ) )
                return 1;
        if ( match( GNU_PATTERN, string, NULL, (size_t) 0 ) )
                return 2;
        fprintf( stderr, "Error: Unrecognized Version String\n" );
        exit(1);
}

void getVersion ( const char *utilityName, char *utilityVersion  )
{
        char buf[1024] = {'\0'}; 
        regmatch_t matchptr[1];
        char utilityCommand[40] = {'\0'};
        FILE *uFile;

        // concatenate utilityCommand and options to display version information
        strcpy( utilityCommand, utilityName );

        if( !strcmp( utilityName, "bzip2" ) )             // something fucky here
                strcat( utilityCommand, " --help 2>&1" );
        else
                strcat( utilityCommand, " --version" );

        uFile = popen( utilityCommand, "r");

        if (uFile)
        {
                fread( buf, 1, 1024, uFile );
                if (match( GENERIC_PATTERN, buf, matchptr, (size_t) 1))
                        strncpy( utilityVersion, buf+matchptr[0].rm_so, (matchptr[0].rm_eo) - (matchptr[0].rm_so) );
                else
                {       
                        pclose(uFile);
                        exit(1);
                }
        }

        pclose(uFile);
}