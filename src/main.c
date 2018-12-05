#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "version.h"

int main( int argc, char ** argv )
{
  int c;
  static int cflag, vflag, version_flag = 0;
  char utilityName[24] = {'\0'};
  char utilityVersion[24] = {'\0'};
  char compVersion[24] = {'\0'};

  static struct option long_options[] = 
  {
    {"compare",	required_argument,	0,           'c'},
    {"help",	no_argument,		0,           'h'},
    {"verbose", no_argument, 		0,           'v'},
    {"version",	no_argument,		&version_flag, 1},
    {0, 0, 0, 0}
  };

  int option_index = 0;

        while ( (c = getopt_long(argc, argv, "c:hv", long_options, &option_index)) != -1 )
        {
                switch (c)
                {
                        // display help information
                        case 'h' :
                                displayHelp();
                                return 0;
      
                        case 0:
                                // If this option set a flag, do nothing else now.
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
                                strncpy( compVersion, optarg, strlen(optarg) );
                                break;
                        }
      
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
                                break;
                }
        }

        if (version_flag)
        {
                displayVersion();
                return 0;
        }

        if ( optind < argc)
        {
                strncpy( utilityName, argv[optind], strlen(argv[optind]) );
                getVersion( utilityName, utilityVersion );
        }
        else
        {
                fprintf( stderr, "Error: Missing utility argument.\n");
                return 1;
        }

        if (cflag)
        {
                int status = compareVersion( utilityVersion, compVersion );
                if ( vflag )
                {
                        if ( !status )
                                printf( "Utility version is older.\n" );
                        else if ( status == 2)
                                printf( "Utility version is newer.\n");
                        else
                                printf( "Utility is the same version.\n");

                }
                return status;
        }

        getVersion( utilityName, utilityVersion );
        printf( "%s: %s\n", utilityName, utilityVersion );
        return 0;
}

void displayHelp()
{
  printf( "semver version %d.%d.%d%s%s\n"
    "Usage:  semver [option] utility\n"
    "\n"
    "Mandatory arguments to long options are mandatory for short options too.\n"
    "  -c, --compare=VERSION        compare version of utility with VERSION\n"
    "  -h, --help                   display this help and exit\n"
    "  -v, --verbose                be verbose\n\n"
    "  --version                    display version information and exit\n"
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