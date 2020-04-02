#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>

void usage(){
	fprintf ( stderr, "Usage:\n\t"
			"bin2c <var_name> filename.bin > filename.c\n\t"
			"cat filename.bin | bin2c <var_name>  -- > filename.c\n\t"
	);
}

int main ( int argc, char* argv[] )
{
unsigned char buf[BUFSIZ];
char* ident;
int fd, i, total, rd, need_comma;

if(argc < 3 ) {
	usage();
	return(-1);
}
else if (!strcmp("--",argv[2])){
	fd = 0;
}
else{
	fd = open(argv[2],O_RDONLY);
}

ident = strdup(argv[1]);

if ( fd == -1 ){
	fprintf ( stderr, "%s: can't open %s for reading\n", argv[0], argv[1] );
	return -1;
}

printf ( "const unsigned char %s[] = {", ident );
for ( total = 0, need_comma = 0; ( rd = read ( fd, buf, BUFSIZ ) ) != 0; ){
	if ( rd == -1 ){
		fprintf ( stderr, "%s: file read error\n", argv[0] );
		return -1;
	}

	for ( i = 0; i < rd; i++ ){

		if ( need_comma ) printf ( ", " );
		else need_comma = 1;

		if ( ( total % 11 ) == 0 ) printf ( "\n\t" );
		printf ( "0x%.2x", buf[i] );
		total++;
	}
}
printf ( "\n};\n" );

close ( fd );

free ( ident );

return 0;
}
