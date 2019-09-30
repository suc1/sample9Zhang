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

//1. Violence Search
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

#if 0
int main(void) {

	//force search string
	char parent[]="bbc abcdab abcdabcdabde";
	char pattern[]="abcdabd";
	printf("ViolenceSearch()=%d\r\n", ViolenceSearch(parent, pattern));

	return EXIT_SUCCESS;
}
#endif

//2. KMP
//https://blog.csdn.net/v_july_v/article/details/7041827
int KMPSearch(const char *s, const char *p) { /*parent, pattern*/
	int lenS = strlen(s);
	int lenP = strlen(p);

	int ss=0, pp=0;
	while(ss<lenS && pp<lenP) {	//two end conditions
		if(pp==-1 || s[ss]==p[pp]) {
			++ss;
			++pp;
		} else {
			pp=next[pp];
		}
	}

	if(pp>=lenP) {
		return ss-pp;
	}

	return -1;
}
#if 0
int main(void) {

	//KMP search string
	char parent[]="bbc abcdab abcdabcdabde";
	char pattern[]="abcdabd";
	printf("KMP Search()=%d\r\n", KMPSearch(parent, pattern));

	return EXIT_SUCCESS;
}
#endif
