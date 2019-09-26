/*
 ============================================================================
 Name        : ch1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int ViolenceSearch(const char *s, const char *p) { /*parent, pattern*/
	int lenS = strlen(s);
	int lenP = strlen(p);

	int ss=0, pp=0;
	while(ss<lenS && pp<lenP) {	//two end conditions
		if(s[ss]==p[pp]) {
			++ss;
			++pp;
		} else {
			ss=ss-pp+1;
			pp=0;
		}
	}

	if(pp>=lenP) {
		return ss-pp;
	}

	return -1;
}

int main(void) {
#if 1
	//force search string
	char parent[]="bbc abcdab abcdabcdabde";
	char pattern[]="abcdabd";

	printf("ViolenceSearch()=%d\r\n", ViolenceSearch(parent, pattern));
#endif

	return EXIT_SUCCESS;
}
