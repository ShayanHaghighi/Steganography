// Program to illustrate the getopt() 
// function in C 

#include <stdio.h> 
#include <unistd.h> 

int main(int argc, char *argv[]) 
{ 
	int opt; 
    // int optopt;
    // int optind;
	
	// put ':' in the starting of the 
	// string so that program can 
	//distinguish between '?' and ':' 
	while((opt = getopt(argc, argv, ":if:l:rx")) != -1) 
	{ 
		switch(opt) 
		{ 
			case 'i': 
			
			case 'r': 
				printf("option: %c\n", opt); 
				break; 
                
			case 'f': 
				printf("filename: %s\n", optarg); 
				break; 
			case ':': 
				printf("option %c needs a value\n",optopt); 
				break; 
			case '?': 
				printf("unknown option: %c\n", optopt); 
				break; 
		} 
	} 
	
	// optind is for the extra arguments 
	// which are not parsed 
	for(; optind < argc; optind++){	 
		printf("extra arguments: %s\n", argv[optind]); 
	} 
	
	return 0; 
} 
