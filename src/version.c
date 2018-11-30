#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"

int match( const char *pattern, const char *string, regmatch_t *matchptr, 
	   size_t nmatch ) 
{
  regex_t re;
  char msgbuf[100];

  if ( regcomp( &re, pattern, REG_EXTENDED) )
  {
    fprintf( stderr, "Error: Could not compile regex\n");
    exit(1);
  }
  
  // return a status value if regex finds a match and store offsets in matchptr
  //  else return 0
  int status;
  status = regexec( &re, string, (size_t) nmatch, matchptr, 0);

  if (!status)                       // No match found
  {
    return 1;
  }
  else if ( status == REG_NOMATCH )  // No match found
  {
    return 0;
  }
  else                               // Regex error
  {
    regerror( status, &re, msgbuf, sizeof(msgbuf) );
    fprintf( stderr, "Erro: Regex match failed: %s\n", msgbuf);
    return -1;
  }

  regfree(&re);
}

int checkVersionFormat( const char *string )
{
  if (!(match( "([[:digit:]].){2}[[:digit:]]+(-[a-zA-Z0-9.-]*)?(\\+[a-zA-Z0-9.-]+)?$", 
        string, NULL, (size_t) 0 )))
  {
    fprintf( stderr, "Error: Version format invalid\n" );
    exit(-1);
  }
  return 1;
}

int semverNewer( const char *ver1, const char *ver2 )
{        
  return 0;
}

void splitVersionString( const char *verStr, struct version *splitVersion )
{
  checkVersionFormat( verStr );
  regmatch_t matchptr[1];
  //struct version splitVersion;

  match ( "([[:digit:]].){2}[[:digit:]]", verStr, matchptr, (size_t) 1 );
  
  int length = (matchptr[0].rm_eo) - (matchptr[0].rm_so);
  char shortVer[16] = {'\0'};

  // dont realy know why im adding 1 to length but it seems to work
  // for some reason length is a single byte short related to how regexec()
  // returns, start and end of substring in matchptr[] ARRAYS START AT 0
  strncpy( shortVer, verStr+matchptr[0].rm_so-1, length+1);
  strcpy( splitVersion->major, strtok( shortVer, "." ));
  strcpy( splitVersion->minor, strtok( NULL, "." ));
  strcpy( splitVersion->patch, strtok( NULL, "."));
  
  // extract prerelease version
  if(match("-[[:alnum:].-]+", verStr, matchptr, (size_t) 1 ))
  {
    char shortPR[24] = {'\0'};
    // +1 on pointer to string and -1 on # of chars is to remove '-' from
    // prerelease
    strncpy( shortPR, verStr+matchptr[0].rm_so+1, 
          (matchptr[0].rm_eo - matchptr[0].rm_so)-1 );
    strcpy( splitVersion->prerelease, shortPR);
  }
  //else
  //{
  //  splitVersion.prerelease = "";
  //}
  
  // extract build info
  if(match("\\+[[:alnum:].-]+", verStr, matchptr, (size_t) 1))
  {
    char shortBuild[32] = {'\0'};
    strncpy( shortBuild, verStr+matchptr[0].rm_so+1,
          (matchptr[0].rm_eo - matchptr[0].rm_so)-1 );
    strcpy( splitVersion->build, shortBuild);
  }
  //else
  //{
  //  splitVersion.build = "";
  //}

    //return splitVersion;
}

int compareVersion( const char *ver1, const char *ver2 )
{
  // Make sure versions are in correct format
  if ( ( checkVersionFormat( ver1 ) != 1 ) || 
     ( checkVersionFormat( ver2 ) != 1 ) )
    exit(127);
  // split version strings by part MAJOR MINOR PATCH [PRERELEASE[BUILD]]
	
  struct version *splitVer1 = malloc(sizeof(struct version));
  splitVersionString( ver1, splitVer1 );
  struct version *splitVer2 = malloc(sizeof(struct version));
  splitVersionString( ver2, splitVer2 );

  #ifdef DEBUG
  printf( "Version 1:\n"
          "Major: %s\n"
	  "Minor: %s\n"
	  "Patch: %s\n"
	  "Prerelease: %s\n"
	  "Build: %s\n\n",
	  splitVer1->major, splitVer1->minor, splitVer1->patch, splitVer1->prerelease, 
	  splitVer1->build );

  printf( "Version 2:\n"
          "Major: %s\n"
	  "Minor: %s\n"
	  "Patch: %s\n"
	  "Prerelease: %s\n"
	  "Build: %s\n",
	  splitVer2->major, splitVer2->minor, splitVer2->patch, splitVer2->prerelease, 
	  splitVer2->build );
  #endif

  int status = 0;

  if ( atoi(splitVer1->major) > atoi(splitVer2->major) )
  {
    status = 1;
  }
  else if ( atoi(splitVer1->major) < atoi(splitVer2->major) )
  {
    status = -1;
  }
  else if ( atoi(splitVer1->minor) > atoi(splitVer2->minor) )
  {
    status = 1;
  }
  else if ( atoi(splitVer1->minor) < atoi(splitVer2->minor) )
  {
    status = -1;
  }
  else if ( atoi(splitVer1->patch) > atoi(splitVer2->patch) )
  {
    status = 1;
  }
  else if ( atoi(splitVer1->patch) < atoi(splitVer2->patch) )
  {
    status = -1;
  }

  // TODO: compare prerelease strings maybe strncmp() ?
  // TODO: free structs on heap
  return status;
}

void getVersion ( char *utilityName, char *utilityVersion  )
{
  char buf[1024];
  size_t nread;
  const char *shortVersion = " -v";
  const char *longVersion = " --version";
  char utilityCommand[24] = {'\0'};
  
  strcpy( utilityCommand, utilityName );
  strcat( utilityCommand, longVersion );
  
  FILE *uFile = popen( utilityCommand ,"r" );
  
  if ( uFile )
  {
    while (( nread = fread( buf, 1, sizeof buf, uFile)) > 0 )
    {
    }
      //fwrite( buf, 1, nread, stdout );
    if (ferror(uFile))
    {
    }
    regmatch_t matchptr[1];
    size_t nmatch = 1;    
    if(match( "([[:digit:]]+.){2}[[:digit:]]*([[:digit:]()]+)?(-[[:alnum:].-]+)?", buf, matchptr, nmatch ))
    {
      int length = (matchptr[0].rm_eo) - (matchptr[0].rm_so);
      strncpy( utilityVersion, buf+matchptr[0].rm_so, length );
      
      #ifdef DEBUG
      printf( "Matched: %s\n", utilityVersion );
      #endif
    }
    else
    {
      printf( "Match not found\n\n");
    }
    fclose( uFile);
  }

  pclose( uFile );
}
