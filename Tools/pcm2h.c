/* PCM file to C Header file converter
 *
 * This program opens a pcm file and places every sample in it 
 * in an array!
 *
 * TODO: Future versions will be able to open a wav file and identify the
 * data type correctly.
 * 
 * (c) 2007/08 Joao Figueiredo
 *
 * FIX: 2010/05 Corrected input file fopen mode to binary as suggested
 *              by juan abba
 *
 * FIX: 2014/03 Corrected line too big issue detected by Manfred Fuchs
 *              Added line break at every 16th sample
 *
 */

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<libgen.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>

void printSafeName(char *name, FILE *fp)
{
	unsigned int i;
	char *bname;

	bname = basename(name);

	for(i=0; i<strlen(bname) && bname[i]!='.'; i++)
	{
		fprintf(fp, "%c", bname[i]);
	}
}

void printCapsDefineName(char *name, FILE *fp)
{
	unsigned int i;
	char *bname;

	bname = basename(name);

	for(i=0; i<strlen(bname); i++)
	{
		if(bname[i]=='.')
			fprintf(fp, "_");
		else
			fprintf(fp, "%c", toupper(bname[i]));
	}
}

int main(int argc, char *argv[])
{
	FILE *headerFile=NULL;
	FILE *pcmFile=NULL;
	struct stat statData;

	int file;
	int type;
    int colBreak = 0;

	/* test arguments */
	if((argc==2 && strcmp(argv[1], "-h")==0) || argc<3)
	{
		printf("pcm2h - writes pcm files as .h header files (v1.1)\n");
		printf("Usage: %s file1.pcm [file2.pcm] [file3.pcm] [...] output.h\n", basename(argv[0]));
		return -1;
	}

	/* open output file */
	if( (headerFile = fopen(argv[argc-1], "w")) == NULL)
	{
		int e=errno;
		printf("Error: %s: Can't open output file!\n", strerror(e));
		return -2;
	}

	/* write .h header */
	fprintf(headerFile, "#ifndef "); printCapsDefineName(argv[argc-1], headerFile);
	fprintf(headerFile, "\n#define "); printCapsDefineName(argv[argc-1], headerFile);
	fprintf(headerFile, "\n\n");

	/* process every file */
	for(file=1; file<argc-1; file++)
	{
		/* get file size */
		if( stat(argv[file], &statData) < 0)
		{
			int e=errno;
			printf("Error: %s: Can't stat file %s.\n", strerror(e), argv[file]);
			return -3;
		}

		/* ask user for data type */
		printf("Processing file %s with %u bytes.\n", argv[file],
				(unsigned int) statData.st_size);
		do
		{
			printf("Enter data type: 1->8bit, 2->16bit: ");fflush(stdout);
			scanf("%d", &type);
		}while(type!=1 && type!=2);

		/* open file */
		if( (pcmFile = fopen(argv[file], "rb")) == NULL)
		{
			int e=errno;
			printf("Error: %s: Can't open file %s.\n", strerror(e), argv[file]);
			return -4;
		}

		/* write correct pointer type and data */
		if(type==1)
		{
			unsigned int c;

			fprintf(headerFile, "const char ");
			printSafeName(argv[file], headerFile);
			fprintf(headerFile, "[%u]={", (unsigned int) statData.st_size);

			c = fgetc(pcmFile);
			fprintf(headerFile, "0x%02X", c);

			while( (c=fgetc(pcmFile)) != EOF )
			{
				fprintf(headerFile, ",%s 0x%02X", ((++colBreak % 16) == 0?"\n":""), c);
			}

			fprintf(headerFile, "};\n\n");
		}
		else
		{
			unsigned int cl;
			unsigned int ch;
			fprintf(headerFile, "const int ");
			printSafeName(argv[file], headerFile);
			fprintf(headerFile, "[%u]={", (unsigned int) statData.st_size/2);

			cl = fgetc(pcmFile);
			ch = fgetc(pcmFile);
			fprintf(headerFile, "0x%02X%02X", ch, cl);

			while( (cl=fgetc(pcmFile))!=EOF && (ch=fgetc(pcmFile)!=EOF) )
			{
				fprintf(headerFile, ",%s 0x%02X%02X", ((++colBreak % 16) == 0?"\n":""), ch, cl);
			}

			fprintf(headerFile, "};\n\n");
		}

		fclose(pcmFile);

	}

	/* write .h footer */
	fprintf(headerFile, "#endif\n");
	fclose(headerFile);

	printf("All done!\n\n");
	return 0;
}
