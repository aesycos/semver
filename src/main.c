#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <regex.h>

#include "version.h"

static int verbose_flag;

void checkVersionFormat( const char *pattern, const char *string );

int main( int argc, char ** argv ) {
	int c;
	char * UVERSION;
	char * CVERSION;

	while (1)
	{
		static struct option long_options[] = 
		{
			{"compare", 	required_argument,	0,	'c'},
			{"equal",	required_argument,	0,	'e'},
			{"greater",	required_argument,	0,	'g'},
			{"help",	no_argument,		0,	'h'},
			{"less",	required_argument,	0,	'l'},
			{"quiet",	no_argument,		&verbose_flag,	0},
			{"version",	no_argument,		0,	'v'},
			{0, 0, 0, 0}
		};

		int option_index = 0;

		c = getopt_long(argc, argv, "c:e:g:hl:qv", long_options, &option_index);

		if ( c == -1 )
			break;

		switch (c) {
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
			case 'c' :
				checkVersionFormat( "([[:digit:]].){2}[[:digit:]]", optarg);
				CVERSION = optarg;
				UVERSION = "12.2.0";
				printf("Comparing Utility version %s with version %s\n", UVERSION, CVERSION);	
				int eval = strcmp( CVERSION, UVERSION);
				if (eval > 0) {
					printf("Utility is of older version\n");
				} else if ( eval < 0) {
					printf("Utitliy is of newer version\n");
				} else {
					printf("Utities are of equal version\n");
				}

				break;
			case 'e' :
				printf("option: equal with value `%s`\n", optarg);
				break;
			case 'g' :
				printf("option: greater with value `%s`\n", optarg);
				break;
			case 'l' :
				printf("option: less with value `%s`\n", optarg);
				break;
			case 'q' :
				printf("option: quiet");
				break;
			case 'v' :
				displayVersion();
				return 0;
			case '?' :
				break;

			default:
				abort();
		}
		printf("%s\n", CVERSION);

	}
	/* Instead of reporting ‘--verbose’
    	and ‘--brief’ as they are encountered,
     	we report the final status resulting from them. */
  	if (verbose_flag)
    		puts ("verbose flag is set");

  	/* Print any remaining command line arguments (not options). */
  	if (optind < argc)
    	{
      		printf ("non-option ARGV-elements: ");
     		while (optind < argc)
        		printf ("%s ", argv[optind++]);
      		putchar ('\n');
    	}

  	exit (0);

	return 0;
}

void displayHelp() {
	printf( "version version %d.%d.%d%s\n"
		"Usage:  version [option] utility\n"
		"\n"
		"Mandatory arguments to long options are mandatory for short options too.\n"
		"	-c, --compare=VERSION	compare version of utility with VERSION\n"
		"	-e, --equal=VERSION	tests to see if utility versions is equal to VERSION\n"
		"	-g, --greater=VERSION	tests to see if utility version is greater than VERSION\n"
		"	-h, --help		display this help and exit\n"
		"	-l, --less=VERSION	tests to see if utility version is less than VERSION\n"
		"	-v, --version		display version information and exit\n"
		"\n"
		"The VERSION argument is a string in format X.Y.Z where X is MAJOR version, Y is MINOR\n"
		"version, and Z is patch version. The format follows that specified at <https://semver.org/\n"
		"\n"
		"Full documentation at: <https://www.aesycos.com/doc/version>\n"
		"or locally available via: `man version`\n",MAJOR,MINOR,PATCH,POSTFIX );
}

void displayVersion() {

}

void checkVersionFormat( const char *pattern, const char *string ) {
	regex_t re;
	char msgbuf[100];

	#ifdef DEBUG
	printf("Pattern: %s\n", pattern);
	printf("String: %s\n", string);
	#endif

	if ( regcomp( &re, pattern, REG_EXTENDED) ) {
		fprintf( stderr, "Could not compile regex\n");
		exit(1); 
	}

	int status = regexec( &re, string, (size_t) 0, NULL, 0);

	if (!status) {
		#ifdef DEBUG
		printf("Matched\n");
		#endif
		return;
	} else if ( status == REG_NOMATCH ) {
		fprintf( stderr, "Error: VERSION format is incorrect\n" );
		return;
	} else {
		regerror( status, &re, msgbuf, sizeof(msgbuf) );
		fprintf( stderr, "Regex match failed: %s\n", msgbuf);
		exit(1);
	}

	regfree(&re);

}
