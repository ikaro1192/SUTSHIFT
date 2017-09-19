/* Copyright (c) Shoka Shimizu, 2017.*/
/* All rights reserved.              */

#include <stdlib.h>

#include "su.h"
#include "segy.h"

/*********************** self documentation **********************/
char *sdoc[] = {
" 								",
" SUTSHIFT - do shift each trace of su data in the t direction 		",
" 								",
" sutshift <stdin >stdout tfile=					",
" 								",
" Required parameters:						",
"   tfile=     file name 							",
"								",
" Optional parameter:						",
"   None                                    ",
"								",
" Note:								",
NULL};

/**************** end self doc ***********************************/

segy tr;

int limit(int val, int under, int upper){
    if (val < under) return under;
    if (val > upper) return upper;
    return val;
}

int main(int argc, char **argv) {
	cwp_String t_filename="";
	int nt;			/* number of samples on input trace	*/
    int dt;         /* sample interval; in micro-seconds */
    FILE *tfile;
    int shiftVal = 0;


	/* Initialize */
	initargs(argc, argv);
	requestdoc(1);


	/* Get information from first trace */
	if (!gettr(&tr)) err("can't get first trace");
	nt = tr.ns;
    dt = tr.dt;

    if (!getparstring("tfile", &t_filename)) err("tfile is not set");
    if ((tfile = fopen(t_filename, "r")) == NULL) {
        err("error: can't read file.");
    }

	/* Main loop over traces */
	do {

        fscanf(tfile, "%d", &shiftVal);
        fprintf(stderr,"%d\n",shiftVal);

        if (shiftVal < 0){
            for (int i = 0; i < nt; ++i){
                if ( i-shiftVal >= 0 &&  i-shiftVal < nt){
                    tr.data[i] = tr.data[i-shiftVal];
                }else{
                    tr.data[i] = 0;
                }
            } 
        }else{
            for (int i = nt-1; i >= 0; --i){
                if ( i-shiftVal >= 0 &&  i-shiftVal < nt){
                    tr.data[i] = tr.data[i-shiftVal];
                }else{
                    tr.data[i] = 0;
                }
            }
        }
        puttr(&tr);

	} while (gettr(&tr));


	return(CWP_Exit());
}
