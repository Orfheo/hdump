/****************************************************************************
 * hdump.c : hex dump utility
 * 
 * Author : GVT Giuseppe Vitillaro   peppe@unipg.it
 *
 * 10/21/93  GVT  First Implementation
 * 11/21/93  GVT  Added options via getopt
 ****************************************************************************/
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "ebcdic.h"

#define BYTESTR 16
#define BYTELEN  4

void help ( );

extern int optind;
extern char *optarg;

char *prog;
unsigned char buf[BYTESTR];


/****************************************************************************
 * main
 ****************************************************************************/
int main ( argc, argv )
     int argc;
     char **argv;
{
  FILE *f;
  char *fname;
  size_t nread;
  int byte;
  long pos;
  
  int c;
  char e;
  int eflag = 0;
  int dflag = 0;
  int flen = BYTELEN;
  char *flenenv;


  prog = argv[0];

  initialize_code_pages();
  
  flenenv = getenv("HDUMPSARG");

  if ( flenenv )
    flen = atoi ( flenenv );
  
  while ( ( c = getopt ( argc, argv, "dehs:" ) ) != EOF ) {
    switch ( c ) {
    case 'e': 
      eflag++;
      break;

    case 'd':
      dflag++;
      break;

    case 's':
      flen = atoi ( optarg );
      if ( ( flen < 1 ) || ( flen > 16 ) ) {
	fprintf ( stderr, "%s: -s Illegal Value %d\n", prog, flen );
	fprintf ( stderr, "%s:    value must be from 1 to 16\n", prog );
	help ( );
	exit ( 1 );
      }
      break;
      
    case 'h':
      help ( );
      exit ( 1 );
      break;

    default:
      help ( );
      exit ( 1 );
      break;
    }
  }

  if ( optind == argc ) {
    f = stdin;
  }
  else {
    fname = argv[optind];

    f = fopen ( fname, "r" );
    if ( !f ) {
      fprintf ( stderr, "%s: %s ", prog, fname );
      perror ( "" );
      exit ( 2 );
    }
  }
  
  pos = 0;
  
  while ( ( nread = fread ( buf, 1, BYTESTR, f ) ) > 0 ) {

    if ( dflag )
      printf ( "%010ld:  ", pos );
    else
      printf ( "%08lx:  ", pos );
    
    for ( byte = 0; byte < BYTESTR; byte++ ) {
      if ( byte < nread )
	printf ( "%02x", buf[byte] );
      else
	printf ( "  " );
          if ( ( byte % flen ) == ( flen - 1 ) )
      printf ( " " );
    }

    printf ( "  [" );
    
    for ( byte = 0; byte < BYTESTR; byte++ ) {
      if ( byte < nread ) {
        if ( eflag )
          e = EBCDIC_to_ASCII ( buf[byte] );
        else
          e = buf[byte];
	if ( isprint ( e ) )
	  printf ( "%c", e );
	else
	  printf ( "." );
      }
    }
    
    printf ( "]\n" );
    
    pos += nread;
  }

  if ( f != stdin )
    fclose ( f );
  
  return 0;
}


/****************************************************************************
 * help : Syntax 
 ****************************************************************************/
void help ( )
{
  fprintf ( stderr, "%s usage: hdump [-dh] [-s len] filename\n", prog );
  return;
}
