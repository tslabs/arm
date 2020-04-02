/*
    mot2bin converts a Motorola hex file to binary.
    Copyright (C) 1999  Jacques Pelletier

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

20040617 Alf Lacis: Added pad byte (may not always want FF).
         Added initialisation to Checksum to remove GNU
         compiler warning about possible uninitialised usage
         Added 2x'break;' to remove GNU compiler warning about label at
         end of compound statement
         Added PROGRAM & VERSION strings.

20071005 PG: Improvements on options parsing
20091212 JP: Corrected crash on 0 byte length data records
20100402 JP: ADDRESS_MASK is now calculated from MEMORY_SIZE
*/

#define PROGRAM "mot2bin"
#define VERSION "1.0.8"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/* size in bytes */
#define MEMORY_SIZE 4096*1024
#define ADDRESS_MASK (MEMORY_SIZE-1)

#ifdef USE_FILE_BUFFERS
	#define BUFFSZ 4096
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/* FIXME how to get it from the system/OS? */
#define MAX_FILE_NAME_SIZE 81

#ifdef DOS
	#define MAX_EXTENSION_SIZE 4
#else
	#define MAX_EXTENSION_SIZE 16
#endif

#define MAX_LINE_SIZE 256

#ifdef __linux__
	  /* We don't accept an option beginning with a '/' because it could be a file name. */
  #define _COND_(x)	 (*(x) == '-')
#else
  #define _COND_(x)	 ((*(x) == '-') || (*(x) == '/'))
#endif

typedef char filetype[MAX_FILE_NAME_SIZE];
typedef int boolean;
typedef unsigned char byte;
typedef unsigned short word;

filetype    Filename;           /* string for opening files */
char        Extension[MAX_EXTENSION_SIZE];       /* filename extension for output files */

FILE        *Filin,             /* input files */
            *Filout;            /* output files */

#ifdef USE_FILE_BUFFERS
char		*FilinBuf,          /* text buffer for file input */
            *FiloutBuf;         /* text buffer for file output */
#endif

int PadByte = 0xFF;

void usage(void);
void *NoFailMalloc (size_t size);
void NoFailOpenInputFile (char *Flnm);
void NoFailOpenOutputFile (char *Flnm);
void PutExtension(char *Flnm, char *Extension);

void usage(void)
{
    fprintf (stderr,
             "\n"
             "usage: "PROGRAM" [OPTIONS] filename\n"
             "Options:\n"
             "  -s [address]  Starting address in hex\n"
             "  -e [ext]      Output filename extension\n"
             "  -c            Enable checksum verification\n"
             "  -p [value]    Pad-byte value in hex (default==%x)\n\n"
             "  -k [0|1|2]    Select checksum type\n"
             "                       0 = 8-bit,\n"
			 "                       1 = 16-bit little endian,\n"
			 "                       2 = 16-bit big endian\n"
             "  -r [start] [end]     Range to compute checksum over (default is min and max addresses)\n"
             "  -f [address] [value] Address and value of checksum to force\n\n", PadByte);
   exit(1);
} /* procedure USAGE */

void *NoFailMalloc (size_t size)
{
void *result;

	if ((result = malloc (size)) == NULL)
	{
		fprintf (stderr,"Can't allocate memory.\n");
		exit(1);
	}
	return (result);
}

/* Open the input file, with error checking */
void NoFailOpenInputFile (char *Flnm)
{
    while ((Filin = fopen(Flnm,"r")) == NULL)
    {
        fprintf (stderr,"Input file %s cannot be opened. Enter new filename: ",Flnm);
        fgets (Flnm,MAX_FILE_NAME_SIZE,stdin);
        if (Flnm[0] == '\0') exit(1);
    }

#ifdef USE_FILE_BUFFERS
    FilinBuf = (char *) NoFailMalloc (BUFFSZ);
    setvbuf(Filin, FilinBuf, _IOFBF, BUFFSZ);
#endif
} /* procedure OPENFILIN */

/* Open the output file, with error checking */
void NoFailOpenOutputFile (char *Flnm)
{
    while ((Filout = fopen(Flnm,"wb")) == NULL)
    {
        /* Failure to open the output file may be
        simply due to an insufficiant permission setting. */

        fprintf(stderr,"Output file %s cannot be opened. Enter new file name: ", Flnm);
        fgets(Flnm,MAX_FILE_NAME_SIZE,stdin);
        if (Flnm[0] == '\0') exit(1);
    }
#ifdef USE_FILE_BUFFERS
    FiloutBuf = (char *) NoFailMalloc (BUFFSZ);
    setvbuf(Filout, FiloutBuf, _IOFBF, BUFFSZ);
#endif

} /* procedure OPENFILOUT */

/* Adds an extension to a file name */
void PutExtension(char *Flnm, char *Extension)
{
char        *Period;        /* location of period in file name */
boolean     Samename;

    /* This assumes DOS like file names */
    /* Don't use strchr(): consider the following filename:
    	../my.dir/file.hex
    */
    if ((Period = strrchr(Flnm,'.')) != NULL)
        *(Period) = '\0';

    Samename = FALSE;
    if (strcmp(Extension, Period+1)== 0)
        Samename = TRUE;

    strcat(Flnm,".");
    strcat(Flnm, Extension);
    if (Samename)
    {
        fprintf (stderr,"Input and output filenames (%s) are the same.", Flnm);
        exit(1);
    }
}

int main (int argc, char *argv[])
{
  /* line inputted from file */
  char Line[MAX_LINE_SIZE];

  /* flag that a file was read */
  boolean Fileread;
  boolean Enable_Checksum_Error = FALSE;
  boolean Status_Checksum_Error = FALSE;

  /* cmd-line parameter # */
  char *c, *p;

  unsigned int Param;
  unsigned int i;

  /* Application specific */

  unsigned int 	Nb_Bytes;
  unsigned int 	Address, Lowest_Address, Highest_Address, Starting_Address;
  unsigned int 	Phys_Addr, Type;
  unsigned int 	temp;

  boolean Starting_Address_Setted = FALSE;

  int temp2;

  byte	Data_Str[MAX_LINE_SIZE];
  byte 	Checksum = 0;

#define CKS_8     0
#define CKS_16LE  1
#define CKS_16BE  2

  unsigned short int wCKS;
  unsigned short int w;
  unsigned int Cks_Type = CKS_8;
  unsigned int Cks_Start, Cks_End, Cks_Addr, Cks_Value;
  boolean Cks_range_set = FALSE;
  boolean Cks_Addr_set = FALSE;

  /* This will hold binary codes translated from hex file. */
  byte *Memory_Block;

  fprintf (stdout,PROGRAM" v"VERSION", Copyright (C) 1998 Jacques Pelletier\n"
		   "checksum extensions Copyright (C) 2004 Rockwell Automation\n"
		   "improved P.G. 2007\n\n");

  if (argc == 1)
	usage();

  strcpy(Extension, "bin"); /* default is for binary file extension */

  /* read file */
  Param = 1;

  for (Param = 1; Param < argc; Param++)
	{
	  c = p = argv[Param];
//#ifdef __linux__
	  /* We don't accept an option beginning with a '/' because it could be a file name. */
//	  if(*c == '-')
//#else
//        if((*c == '-') || (*c == '/'))
//#endif
		  if ( _COND_(c) ) {
            p = c + 2;
            /* Parameter may follow immediately after option */
            /* Skips after parameter to next option */
            if (*p == '\0')
			  p = argv[Param + 1];
			if ( !_COND_(p) )
				Param++;
				
			switch(tolower (*(++c)))
			  {
			  case 'e':
				/* Last parameter was -E, so this parameter is the filename extension. */
				if (strlen(p) > MAX_EXTENSION_SIZE)
				  usage();

				/* Check to see if the user put a period in the filename extension */
				if (strchr(p, '.') == NULL)
				  strcpy(Extension, p);
				else
				  strcpy(Extension, p+1);
				break;

			  case 'c':
				Enable_Checksum_Error = TRUE;
				break;
			  case 's':
				sscanf(p,"%x",&Starting_Address);
				Starting_Address_Setted = TRUE;
				break;
			  case 'p':
				sscanf(p,"%x",&PadByte);
				break;
			  case 'h':
				usage();
				break;
				/* New Checksum parameters */
			  case 'k':
				sscanf(p,"%x",&Cks_Type);
				if (Cks_Type != CKS_8 &&
					Cks_Type != CKS_16LE &&
					Cks_Type != CKS_16BE )
				  usage();
				break;
			  case 'r':
				sscanf(p,"%x",&Cks_Start);
				p = argv[ ++Param ];
				if ( _COND_(p) ) break;
				sscanf(p,"%x",&Cks_End );
				Cks_range_set = TRUE;
				break;
			  case 'f':
				sscanf(p,"%x",&Cks_Addr);
				p = argv[ ++Param ];
				if ( _COND_(p) ) break;
				sscanf( p, "%x",&Cks_Value );
				Cks_Addr_set = TRUE;
				break;
			  default:
				usage();
			  } /* switch */
		  } else
			break;
	  /* if option */

	} /* for Param */

  /* when user enters input file name */

  /* Assume last parameter is filename */
  strcpy(Filename,argv[argc -1]);

  /* Just a normal file name */
  NoFailOpenInputFile (Filename);
  PutExtension(Filename, Extension);
  NoFailOpenOutputFile(Filename);
  Fileread = TRUE;

  /* allocate a buffer */
  Memory_Block = (byte *) NoFailMalloc(MEMORY_SIZE);

  /* For EPROM or FLASH memory types, fill unused bytes with the padding byte (FF by default) */
  memset (Memory_Block,PadByte,MEMORY_SIZE);

  /* To begin, assume the lowest address is at the end of the memory.
	 subsequent addresses will lower this number. At the end of the input
	 file, this value will be the lowest address. */
  Lowest_Address = MEMORY_SIZE - 1;
  Highest_Address = 0;

  /* Now read the file & process the lines. */
  do /* repeat until EOF(Filin) */
    {
	  /* Read a line from input file. */
	  fgets(Line,MAX_LINE_SIZE,Filin);

	  /* Remove carriage return/line feed at the end of line. */
	  i = strlen(Line)-1;

	  if (Line[i] == '\n') Line[i] = '\0';

	  /* Scan starting address and nb of bytes. */
	  /* Look at the record type after the 'S' */
	  switch(Line[1])
        {
		  /* 16 bits address */
        case '1':
		  sscanf (Line,"S%1x%2x%4x%s",&Type,&Nb_Bytes,&Address,Data_Str);
		  Checksum = Nb_Bytes + (Address >> 8) + (Address & 0xFF);

		  /* Adjust Nb_Bytes for the number of data bytes */
		  Nb_Bytes = Nb_Bytes - 3;
		  break;

		  /* 24 bits address */
        case '2':
		  sscanf (Line,"S%1x%2x%6x%s",&Type,&Nb_Bytes,&Address,Data_Str);
		  Checksum = Nb_Bytes + (Address >> 16) + (Address >> 8) + (Address & 0xFF);

		  /* Adjust Nb_Bytes for the number of data bytes */
		  Nb_Bytes = Nb_Bytes - 4;
		  break;

		  /* 32 bits address */
        case '3':
		  sscanf (Line,"S%1x%2x%8x%s",&Type,&Nb_Bytes,&Address,Data_Str);
		  Checksum = Nb_Bytes + (Address >> 24) + (Address >> 16) + (Address >> 8) + (Address & 0xFF);

		  /* Adjust Nb_Bytes for the number of data bytes */
		  Nb_Bytes = Nb_Bytes - 5;
		  break;

        default:
		  break; // 20040617+ Added to remove GNU compiler warning about label at end of compound statement
        }

	  p = Data_Str;

	  /* If we're reading the last record, ignore it. */
	  switch (Type)
        {
		  /* Data record */
        case 1:
        case 2:
        case 3:
		  if (Nb_Bytes == 0)
		    {
			  fprintf(stderr,"0 byte length Data record ignored\n");
			  break;
		    }

		  Phys_Addr = Address & ADDRESS_MASK;

		  /* Check that the physical address stays in the buffer's range. */
		  if ((Phys_Addr + Nb_Bytes) <= MEMORY_SIZE)
            {
			  /* Set the lowest address as base pointer. */
			  if (Phys_Addr < Lowest_Address)
				Lowest_Address = Phys_Addr;

			  /* Same for the top address. */
			  temp = Phys_Addr + Nb_Bytes -1;

			  if (temp > Highest_Address)
				Highest_Address = temp;

			  /* Read the Data bytes. */
			  i = Nb_Bytes;
			  
			  do
                {
				  sscanf (p, "%2x",&temp2);
				  p += 2;
				  Memory_Block[Phys_Addr++] = temp2;
				  Checksum = (Checksum + temp2) & 0xFF;
                }
              while (--i != 0);

			  /* Read the Checksum value. */
			  sscanf (Data_Str, "%2x",&temp2);

			  /* Verify Checksum value. */
			  Checksum = (0xFF - Checksum) & 0xFF;

			  if ((temp2 != Checksum) && Enable_Checksum_Error)
                {
				  Status_Checksum_Error = TRUE;
                }
            }
		  else
            {
			  fprintf(stderr,"Data record skipped at %8X\n",Phys_Addr);
            }
		  break;

		  /* Ignore all other records */
        default:
		  break; // 20040617+ Added to remove GNU compiler warning about label at end of compound statement
        }
    } while (!feof (Filin));
  /*-----------------------------------------------------------------------------*/

  fprintf(stdout,"Lowest address  = %08X\n",Lowest_Address);
  fprintf(stdout,"Highest address = %08X\n",Highest_Address);
  fprintf(stdout,"Pad Byte        = %X\n",  PadByte);

  wCKS = 0;
  if( !Cks_range_set )
	{
	  Cks_Start = Lowest_Address;
	  Cks_End = Highest_Address;
	}
  switch (Cks_Type)
	{
	case CKS_8:

	  for (i=Cks_Start; i<=Cks_End; i++)
		{
		  wCKS += Memory_Block[i];
		}

	  fprintf(stdout,"8-bit Checksum = %02X\n",wCKS & 0xff);
	  if( Cks_Addr_set )
		{
		  wCKS = Cks_Value - (wCKS - Memory_Block[Cks_Addr]);
		  Memory_Block[Cks_Addr] = (byte)(wCKS & 0xff);
		  fprintf(stdout,"Addr %08X set to %02X\n",Cks_Addr, wCKS & 0xff);
		}
	  break;

	case CKS_16BE:

	  for (i=Cks_Start; i<=Cks_End; i+=2)
		{
		  w =  Memory_Block[i+1] | ((word)Memory_Block[i] << 8);
		  wCKS += w;
		}

	  fprintf(stdout,"16-bit Checksum = %04X\n",wCKS);
	  if( Cks_Addr_set )
		{
		  w = Memory_Block[Cks_Addr+1] | ((word)Memory_Block[Cks_Addr] << 8);
		  wCKS = Cks_Value - (wCKS - w);
		  Memory_Block[Cks_Addr] = (byte)(wCKS >> 8);
		  Memory_Block[Cks_Addr+1] = (byte)(wCKS & 0xff);
		  fprintf(stdout,"Addr %08X set to %04X\n",Cks_Addr, wCKS);
		}
	  break;

	case CKS_16LE:

	  for (i=Cks_Start; i<=Cks_End; i+=2)
		{
		  w =  Memory_Block[i] | ((word)Memory_Block[i+1] << 8);
		  wCKS += w;
		}

	  fprintf(stdout,"16-bit Checksum = %04X\n",wCKS);
	  if( Cks_Addr_set )
		{
		  w = Memory_Block[Cks_Addr] | ((word)Memory_Block[Cks_Addr+1] << 8);
		  wCKS = Cks_Value - (wCKS - w);
		  Memory_Block[Cks_Addr+1] = (byte)(wCKS >> 8);
		  Memory_Block[Cks_Addr] = (byte)(wCKS & 0xff);
		  fprintf(stdout,"Addr %08X set to %04X\n",Cks_Addr, wCKS);
		}

	default:
	  ;
	}

  if(Starting_Address_Setted)
    {
	  Lowest_Address = Starting_Address;
    }

  /* write binary file */
  fwrite (&Memory_Block[Lowest_Address],
		  1,
		  Highest_Address - Lowest_Address +1,
		  Filout);

  free (Memory_Block);

#ifdef USE_FILE_BUFFERS
  free (FilinBuf);
  free (FiloutBuf);
#endif

  fclose (Filin);
  fclose (Filout);

  if (Status_Checksum_Error && Enable_Checksum_Error)
    {
	  fprintf(stderr,"Checksum error detected.\n");
	  return 1;
    }

  if (!Fileread)
	usage();
  return 0;
}
