/* This tool reads a fasta file (either gzippped or uncompressed) and calculates the frequency of all triplets. */


#define progname "fast_triplet_count by Chris Creevey"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <zlib.h>



/* This array represents the values we give the nucliotides based on their position in the codon */
/* The code will add up the three values for a given codon. Going downwards, the array           */
/* represents 't/u', 'c', 'a', 'g', other (gap values). The values are ordered so that the codons*/
/* differing only in third position are numerically close to each other.						 */ 
int transform_values [5][3]  = {0,0,0,   
								16,4,1,
								32,8,2,
								48,12,3,
								64,64,64};


char what[5] = {'T', 'C', 'A', 'G', 'X'};

/* This array gives the actual make up of the codons in nucliotides, compared to their codon number, as they are stored in memory */
char triplet[65][3] = {
'T','T','T', 'T','T','C', 'T','T','A', 'T','T','G', 'T','C','T', 'T','C','C', 'T','C','A', 'T','C','G', 'T','A','T', 'T','A','C', 'T','A','A', 'T','A','G', 'T','G','T', 'T','G','C', 'T','G','A', 'T','G','G',
'C','T','T', 'C','T','C', 'C','T','A', 'C','T','G', 'C','C','T', 'C','C','C', 'C','C','A', 'C','C','G', 'C','A','T', 'C','A','C', 'C','A','A', 'C','A','G', 'C','G','T', 'C','G','C', 'C','G','A', 'C','G','G',
'A','T','T', 'A','T','C', 'A','T','A', 'A','T','G', 'A','C','T', 'A','C','C', 'A','C','A', 'A','C','G', 'A','A','T', 'A','A','C', 'A','A','A', 'A','A','G', 'A','G','T', 'A','G','C', 'A','G','A', 'A','G','G',
'G','T','T', 'G','T','C', 'G','T','A', 'G','T','G', 'G','C','T', 'G','C','C', 'G','C','A', 'G','C','G', 'G','A','T', 'G','A','C', 'G','A','A', 'G','A','G', 'G','G','T', 'G','G','C', 'G','G','A', 'G','G','G',
'X','X','X' };

int triplet_count[65] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0};


int main(int argc, char *argv[])
    {
    gzFile gzinfile = NULL;
    int i=0, j=0, frame1=0, frame2=0, frame3=0, nucleotide=0, Tcount=0, Ccount=0, Acount=0, Gcount=0, Ncount=0, totlen=0; ;
    char c;

    if(argc < 2)
		{
		printf(" the usage of this program is:\n\n\tfast_triplet_count fasta.file\n"); 
		exit(1);
		}
	if((gzinfile = gzopen(argv[1], "rb")) == '\0')		/* check to see if the file is there */
		{								/* Open the file */
		printf("Error: Cannot open file %s\n", argv[1]);
		exit(1);
		}

	/*read in the fasta file */
	while(!gzeof(gzinfile))
		{
		c=gzgetc(gzinfile);
		if(c == '>')
			{
			i=0; j=0;
			while(!gzeof(gzinfile) && (c=gzgetc(gzinfile)) != '\n' && c != '\r');
			}
		if(c != '\n' && c != '\r' && c != '\t' && c != ' ' && !gzeof(gzinfile))
			{
			/* read in the nucleotide base */
			switch(c)
				{
				case 'u':
				case 'U':
				case 't':
				case 'T':
					nucleotide=0;
					Tcount++;
					break;
				case 'c':
				case 'C':
					nucleotide=1;
					Ccount++;
					break;
				case 'a':
				case 'A':
					nucleotide=2;
					Acount++;
					break;
				case 'g':
				case 'G':
					nucleotide=3;
					Gcount++;
					break;
				default:
					if(!gzeof(gzinfile))
						{
						nucleotide=4;
						Ncount++;
						}
				}
			if(!gzeof(gzinfile))
				{	
				switch(i) /* increament the value for the current base given all three frames that this could be part of */
					{
					case 0:
						frame1+=transform_values[nucleotide][0];
						frame2+=transform_values[nucleotide][2];
						frame3+=transform_values[nucleotide][1];
						break;
					case 1:
						frame1+=transform_values[nucleotide][1];
						frame2+=transform_values[nucleotide][0];
						frame3+=transform_values[nucleotide][2];
						break;
					case 2:
						frame1+=transform_values[nucleotide][2];
						frame2+=transform_values[nucleotide][1];
						frame3+=transform_values[nucleotide][0];
						break;
					}
				if(frame1>64) frame1=64;
				if(frame2>64) frame2=64;
				if(frame3>64) frame3=64;

				if(i == 2 )  /* frame 1 is complete */
					{
					if(j > 0) 
						{
						triplet_count[frame1]++;
						}
					frame1=0;	
					i=0;
					}
				else
					{
					if(i == 0) /* frame 2 is complete */
						{
						if(j > 0)
							{
							triplet_count[frame2]++;
							}
						frame2=0;
						i++;
						}
					else
						{	
						if(i == 1) /* frame 3 is complete */
							{	
							if(j > 1) 
								{
								triplet_count[frame3]++;
								}
							frame3=0;
							i++;
							}
						}
					}
				}
			j++;
			totlen++;
			}
		}
	/* print header */	
	printf("File_Name\tLength\tT_count\tC_count\tA_count\tG_count\tN_count\t");
    for(i=0; i<65; i++)
		{
		printf("%c%c%c\t", triplet[i][0], triplet[i][1], triplet[i][2]);
		}
	printf("\n");

	/* print out the triplet counts */
	printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t", argv[1], totlen, Tcount, Ccount, Acount, Gcount, Ncount);
	for(i=0; i<65; i++)
		{	
		/*printf("%c%c%c\t%d\n", triplet[i][0], triplet[i][1], triplet[i][2], triplet_count[i]); */
		printf("%d\t", triplet_count[i] );
		}
	printf("\n");

	gzclose(gzinfile);
	}

