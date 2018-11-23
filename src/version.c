#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"

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

int semverNewer( const char *ver1, const char *ver2 ) {
        return 0;
}

