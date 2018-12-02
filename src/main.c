#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "version.h"

#define UVERSION "12.2.0-alpha.0.1.0+12291991"

// static int verbose_flag;

static int cflag, eflag, gflag, lflag, vflag, version_flag = 0;
char utilityName[24] = {'\0'};
char utilityVersion[24] = {'\0'};

int main( int argc, char ** argv )
{
  int c;
  char cversion[24] = {'\0'};

  static struct option long_options[] = 
  {
    {"compare",	required_argument,	0,	'c'},
    {"equal",	required_argument,	0,	'e'},
    {"greater",	required_argument,	0,	'g'},
    {"help",	no_argument,		0,	'h'},
    {"less",	required_argument,	0,	'l'},
    {"verbose", no_argument, 		&vflag,  1},
    {"version",	no_argument,		&version_flag,  1},
    {0, 0, 0, 0}
  };

  int option_index = 0;

  while ( (c = getopt_long(argc, argv, "c:e:g:hl:qv", long_options,
                           &option_index)) != -1 )
  {
    switch (c)
    {
      // display help information
      case 'h' :
	displayHelp();
	return 0;
      
      case 0:
	/* If this option set a flag, do nothing else now. */
	if (long_options[option_index].flag != 0)
          break;
        printf ("option %s", long_options[option_index].name);
        if (optarg)
          printf (" with arg %s", optarg);
        printf ("\n");
        break;
      
      // compare (requires getVersion( utilityName )
      case 'c' :
      { 
	cflag = 1;
	strncpy(cversion, optarg, strlen(optarg));
	break;
      }
      // test if equal
      case 'e' :
        eflag = 1;
	strncpy(cversion, optarg, strlen(optarg));
	break;
             
      // test if greater
      case 'g' :
        gflag = 1;
	strncpy(cversion, optarg, strlen(optarg));
        break;
      // test if less than
      case 'l' :
        lflag = 1;
	strncpy(cversion, optarg, strlen(optarg));
        break;
      
      // be verbose
      case 'v' :
        vflag = 1;
        break; 
      // error handling
      case ':' :
        break;
      
      case '?' :
        break;
      
      default:
        printf("1\n");
        break;
    }
  }

  /* Instead of reporting ‘--verbose’
  and ‘--brief’ as they are encountered,
  we report the final status resulting from them. */
  if (version_flag)
  {
    displayVersion();
    return 0;
  }

  if ( argc == 1 )
  {
   fprintf( stderr, "Type `semver --help` for usage information\n");
   return -127;
  }

  if ( optind < argc )
  {
    strncpy( utilityName, argv[optind], strlen( argv[optind] ) );
    getVersion( utilityName, utilityVersion );
  }
  else
  {
    fprintf( stderr, "Error: No utility specified\n" );
    return -127;
  }
  
  if (cflag)
  {
    int eval;
     
    eval = compareVersion( utilityVersion, cversion );
   
    if (vflag)
    {
      if (eval > 1)
      {
        printf("Utility version is newer\n");
      }
      else if ( eval < 1)
      {
        printf("Utilityy version is older\n");
      }
      else
      {
        printf("Utility version is equal\n");
      }
    }

    
    return eval;
  }

  if (eflag)
  {
    if (compareVersion ( utilityVersion, cversion ) == 1)
    {
      if (vflag)
        printf( "Utility version is equal\n" );
      return 1;
    }
    return 0;
 }

  if (gflag)
  {
     if(compareVersion( utilityVersion, cversion) == 2)
    {
      if (vflag)
        printf( "Utility version is newer\n");
      return 1;
    }
    return 0;
 }

  if (lflag)
  {
     if(!compareVersion( utilityVersion, cversion))
    {
      if (vflag)
      printf( "Utility version is older\n");
      return 1;
    }
    return 0;
  }
  return 0;
}

void displayHelp()
{
  printf( "semver version %d.%d.%d%s%s\n"
    "Usage:  semver [option] utility\n"
    "\n"
    "Mandatory arguments to long options are mandatory for short options too.\n"
    "  -c, --compare=VERSION        compare version of utility with VERSION\n"
    "  -e, --equal=VERSION          tests to see if utility versions is equal to \n"
    "                                VERSION\n"
    "  -g, --greater=VERSION        tests to see if utility version is greater \n"
    "                                than VERSION\n"
    "  -h, --help                   display this help and exit\n"
    "  -l, --less=VERSION           tests to see if utility version is less than \n"
    "                                VERSION\n"
    "  -v, --version                display version information and exit\n"
    "\n"
    "The VERSION argument is a string in format X.Y.Z where X is MAJOR \n"
    "version, Y is MINOR version, and Z is patch version. The format follows \n"
    "that specified at <https://semver.org/> pre-release version is\n"
    "identified by an appended hyphen followed by only ASCII alphanumeric\n"
    "characters and hyphens. Identifiers MUST NOT include leading zeros as \n"
    "specified in %s\n"
    "\n"
    "Full documentation at: <https://www.aesycos.com/doc/version>\n"
    "or locally available via: `man version`\n",MAJOR,MINOR,PATCH,PRERELEASE,
    BUILDMETA,SEMVER );
}

void displayVersion() {
	printf("semver version %d.%d.%d%s%s\n",MAJOR,MINOR,PATCH,PRERELEASE,BUILDMETA);
}
