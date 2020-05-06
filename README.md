# fast_triplet_count
This tool reads a fasta file (either gzippped or uncompressed) and calculates the frequency of all triplets (codons if the data provided are CDS sequences). It is designed to do this extremely quickly so that it can be run on thousands of genomes. When run an a standard desktop computer on all 44K prokaryotic genomes from Ensembl (using a bash 'for' loop to run on all the genomes), it completes in under 1hour 40 mins (approx 6 genomes per second). 

## Prerequisites:

zlib - which should come as standard on most operating systems.

## Compilation:

`` cc fast_triplet_count.c -o fast_triplet_count -lz ``


## Overview:

The tool reads a fasta file with one or more sequences and calculate all (overlapping) triplets. The input fasta file can be gzipped or uncompressed. 

The output is printed to the stardard output stream.

The output consistes of a single line with the following fields:

*Filename* -> the file provided as input.

*Length* -> total legnth in nucleotides of all contigs/chromosomes/sequences in the file

*T_count*	-> number of nucleotide bases with a T/U

*C_count*	-> number of nucleotide bases with a C

*A_count*	-> number of nucleotide bases with a A	

*G_count*	-> number of nucleotide bases with a G

*N_count*	-> number of nucleotide bases with a non standard character (i.e. not a T/U, C, A or G)

Followed by the number of (overlapping) instances of all 64 possible triplets in the following order:

*TTT	TTC	TTA	TTG	TCT	TCC	TCA	TCG	TAT	TAC	TAA	TAG	TGT	TGC	TGA	TGG	CTT	CTC	CTA	CTG	CCT	CCC	CCA	CCG	CAT	CAC	CAA	CAG	CGT	CGC	CGA	CGG	ATT	ATC	ATA	ATG	ACT	ACC	ACA	ACG	AAT	AAC	AAA	AAG	AGT	AGC	AGA	AGG	GTT	GTC	GTA	GTG	GCT	GCC	GCA	GCG	GAT	GAC	GAA	GAG	GGT	GGC	GGA	GGG*	

Finally, the the number of non-standard triplets denoted by *XXX*

## Example



Given this sequence, the following overlapping triplets are possible:

```
ATGTGAATGAAACACT
ATG
 TGT
  GTG
   TGA
    GAA
     AAT
      ATG
       TGA
        GAA
         AAA
          AAC
           ACA
            CAC
             ACT
```

The outpue of the tools with this input file would be:

File_Name|Length|T_count|C_count|A_count|G_count|N_count|TTT|TTC|TTA|TTG|TCT|TCC|TCA|TCG|TAT|TAC|TAA|TAG|TGT|TGC|TGA|TGG|CTT|CTC|CTA|CTG|CCT|CCC|CCA|CCG|CAT|CAC|CAA|CAG|CGT|CGC|CGA|CGG|ATT|ATC|ATA|ATG|ACT|ACC|ACA|ACG|AAT|AAC|AAA|AAG|AGT|AGC|AGA|AGG|GTT|GTC|GTA|GTG|GCT|GCC|GCA|GCG|GAT|GAC|GAA|GAG|GGT|GGC|GGA|GGG|XXX
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
test.fas|16|4|2|7|3|0|0|0|0|0|0|0|0|0|0|0|0|0|1|0|2|0|0|0|0|0|0|0|0|0|0|1|0|0|0|0|0|0|0|0|0|2|1|0|1|0|1|1|1|0|0|0|0|0|0|0|0|1|0|0|0|0|0|0|2|0|0|0|0|0|0




