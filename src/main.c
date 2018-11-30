#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "version.h"

#define UVERSION "12.2.0-alpha.0.1.0+12291991"

static int verbose_flag;

int main( int argc, char ** argv )
{
  int c;
 
  static struct option long_options[] = 
  {
    {"compare",	required_argument,	0,	'c'},
    {"equal",	required_argument,	0,	'e'},
    {"greater",	required_argument,	0,	'g'},
    {"help",	no_argument,		0,	'h'},
    {"less",	required_argument,	0,	'l'},
    {"quiet",	no_argument,		&verbose_flag,	0},
    {"version",	no_argument,		0,	'v'},
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
	int eval;
	eval = compareVersion( UVERSION, optarg );
        
	if (eval > 0)
        {
          printf("Utility is of newer version\n");
        }
        else if ( eval < 0)
        {
          printf("Utitliy is of older version\n");
        }
        else
        {
          printf("Utities are of equal version\n");
        }
        break;
      }
      // test if equal
      case 'e' :
        if (!compareVersion ( UVERSION, optarg ))
	{
	  printf( "Versions are equal\n" );
	}
	return 0;
      
      // test if greater
      case 'g' :
        if (compareVersion( UVERSION, optarg ))
	{
          printf( "Versions is greater\n" );
	}
	return 0;

      // test if less than
      case 'l' :
        printf("option: less with value `%s`\n", optarg);
        break;
      
      // dont echo results just return value
      case 'q' :
        printf("option: quiet");
        break;
      
      // display semver version info
      case 'v' :
        displayVersion();
        return 0;
      
      // error handling
      case ':' :
        break;
      
      case '?' :
        break;
      
      default:
        break;
    }
  }

  /* Instead of reporting ‘--verbose’
  and ‘--brief’ as they are encountered,
  we report the final status resulting from them. */
  if (verbose_flag)
    puts ("verbose flag is set");
  
  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
  {
    char argVersion[24] = {'\0'};
    getVersion( argv[optind], argVersion );
    printf("utilityVersion: %s\n", argVersion);
    printf ("non-option ARGV-elements: ");
    while (optind < argc)
    printf ("%s ", argv[optind++]);
    putchar ('\n');
  }

  exit (0);

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
