#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <libgen.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "sha-2/sha-256.c"

unsigned char* img;
int width, height, channels;
int isDec=0,isEnc=0,providedFile=0,usingPassword=0,defaultImage=0,keyProvided=0;
unsigned char *img;
char* password = "";
char* filepath = NULL;
char* key = NULL;



char set_bit_zero(char byte,int bit_offset) {
    return byte & ~((char)1 << bit_offset);
}

char set_bit_one(char number,int bit_offset) {
    return number | ((char)1 << bit_offset);
}

void print_as_bin(char byte){
    for(int i = 1; i < 9; i++){
        if(byte&(1<<7)){
            printf("1");
        }
        else{
            printf("0");

        }
        byte = byte << 1;
    }
    printf("\n");
}

int is_in_arr(int array[], int value,int length){
    for(int i = 0; i<length;i++){
        if(array[i]==value){
            return 1;
        }
    }
    return 0;
}


// TODO: 
void encrypt(unsigned char* img,char* message,size_t imgSize, uint8_t* hash){

    int usedIndexes[strlen(message)*8+8];
    int indiciesPtr = 0;

    int length = strlen(message);
    int randomIndex = 1;


    for(int j = 0; j < length+1;j++){
        for(int i = 0; i < 8; i++){

            srand(hash[(i+j*8)%SIZE_OF_SHA_256_HASH]);
            randomIndex = (rand()*randomIndex)%imgSize;
            while(is_in_arr(usedIndexes,randomIndex,length*8)){
                randomIndex = (++randomIndex)%imgSize;
            }

            usedIndexes[indiciesPtr] = randomIndex;
            indiciesPtr++;


            if(message[j]&(1<<7)){
                img[randomIndex] = set_bit_one(img[randomIndex],0);
            }
            else{                
                img[randomIndex] = set_bit_zero(img[randomIndex],0);
            }
            message[j] = message[j] << 1;

        }
    }
}

// TODO: make usedIdexes array dynamic
void decrypt(unsigned char* img,char* output,size_t imgSize, uint8_t* hash){
    unsigned char* p = img;
    int j = 0;
    char currentByte = 1;

    int usedIndicies[imgSize];
    int indiciesPtr = 0;

    int randomIndex = 1;

    while(currentByte != '\0'){
        for(int i = 7; i >= 0; i--){

            srand(hash[((7-i)+(j*8))%SIZE_OF_SHA_256_HASH]);
            randomIndex = (rand()*randomIndex)%imgSize;
            while(is_in_arr(usedIndicies,randomIndex,indiciesPtr)){
                randomIndex = ++randomIndex%imgSize;
            }
            usedIndicies[indiciesPtr] = randomIndex;
            indiciesPtr++;

            // printf("%d,\n",randomIndex);

            if(img[randomIndex]&1){
                output[j] = set_bit_one(output[j],i);
            }
            else{
                output[j] = set_bit_zero(output[j],i);
            }
            p++;
        }
        currentByte = output[j];
        // printf("current byte: ");
        // print_as_bin(currentByte);

        j++;
    }
}


int int_to_hex(char* dest, const uint8_t* values, size_t val_len) {
    *dest = '\0'; /* in case val_len==0 */
    while(val_len--) {
        /* sprintf directly to where dest points */
        sprintf(dest, "%02X", *values);
        dest += 2;
        ++values;
    }
    return 1;
}

void hex_to_int(uint8_t* dest, char* values, size_t val_len){
    for(int i = 0; i<32;i++){
        char byte[2];
        byte[0] = values[i*2];
        byte[1] = values[i*2+1];
        dest[i] = (int)strtol(byte, NULL, 16);
    }
}

void print_usage(){
    printf("Usage: ./steg -e -f [image] [OPTIONS]... [message]\n");
    printf("or\n");
    printf("Usage: ./steg -d -f [image] [OPTIONS]...\n");
    printf("  -e                         hide the provided message into the provided image\n");
    printf("  -d                         decrypt the message from the provided image\n");
    printf("  -f [file]                  use this file to hide/decrypt \n");
    printf("  -p [password]              use this password to hide/decrypt \n");
    printf("  -k [key]                   use this key to hide/decrypt \n");
    printf("  -i [imageName]             hide message in preselected image\n");
}

char* create_file_path(char* filepath){
    char* outputname = basename(filepath);
    char* outputFolder = "./output/";
    char* outputPath = (char*)calloc(1,sizeof(char)*7+sizeof(outputname)+1);
    strcat(outputPath,outputFolder);
    strcat(outputPath,outputname);
    return outputPath;
}

void set_hashKey(uint8_t* hashKey){
    if(keyProvided){
        hex_to_int(hashKey,key,64);
    }
    else if(usingPassword){
        // if using password, set the key to be the hash of the password
        calc_sha_256(hashKey, password, strlen(password));
    }
    else{
        if(isDec){
            print_usage();
            exit(1);
        }
        // if a key isn't provided and password isn't provided, then generate a random key
        time_t t;
        srand((unsigned) time(&t));
        int num = rand();
        int lengthNum = (int)((ceil(log10(num))+1)*sizeof(char));
        char str[lengthNum];
        sprintf(str, "%d", rand());
        calc_sha_256(hashKey, str,lengthNum);
    }
}

int main(int argc, char** argv) {
    int opt;




    while((opt = getopt(argc, argv, ":edf:p:i:k:")) != -1) 
	{ 
		switch(opt) 
		{ 
			case 'e':
                isEnc = 1;
                // printf("option: %c\n", opt); 
                break;
			case 'd': 
                isDec = 1;
                // printf("option: %c\n", opt); 
				break; 
            case 'f':
                providedFile=1;
                filepath = malloc(sizeof(optarg));
                filepath = optarg;
                break;
            case 'p':
                usingPassword=1;
                password = optarg;
                // printf("option: %c\n", opt);
                // printf("arg: %s\n", optarg); 
                break;
            case 'i':
                defaultImage=1;
                // printf("option: %c\n", opt);
                // printf("arg: %s\n", optarg); 
                break;
            case 'k':
                keyProvided=1;
                key = optarg;
                // printf("option: %c\n", opt);
                // printf("arg: %s\n", optarg); 
                break;
            case ':':
                printf("option %c needs a value\n",optopt); 
                break;
			case '?': 
				printf("unknown option: %c\n", optopt); 
				break; 
		} 
	}

    if(!providedFile){
        printf("please provide file\n");
        print_usage();
        exit(1);
    }
    if(key != NULL){
        if(strlen(key)!=SIZE_OF_SHA_256_HASH*2){
            printf("Invalid Key\n");
            exit(1);
        }
    }
  

    // printf("file: %s\n", filepath);
    img = stbi_load(filepath, &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    } 

    size_t imgSize = width * height * channels;

  

    uint8_t hashKey[SIZE_OF_SHA_256_HASH];
    
    

    set_hashKey(hashKey);

    
    

    if(isEnc){
        // printf("extra arguments: %s\n", argv[optind]);

        if(argv[optind]==NULL){
            printf("please provide message to hide\n");
            print_usage();
            exit(1);
        }
        char* input = malloc(sizeof(argv[optind]));
        input = argv[optind];


        encrypt(img,input,imgSize,hashKey);

        char* outputPath= create_file_path(filepath);
  
        // printf("output name: %s\n",outputPath);


        stbi_write_png(outputPath, width, height, channels, img, width*channels);
        // free(outputPath);
        // free(img);
        if(!keyProvided){
            char* hashAsHex = (char*)malloc(64);
            int_to_hex(hashAsHex,hashKey,sizeof(hashKey));
            printf("your key is: %s\n",hashAsHex);
        }
        return 0;
        // print_as_hex(key,SIZE_OF_SHA_256_HASH);
    }

    if(isDec){

        // for(int i = 0;i<32;i++){
        //     printf("%d",hash[i]);
        // }
        char* output = malloc(sizeof(char)*imgSize/2);
        decrypt(img,output,imgSize,hashKey);
        printf("output: %s\n",output);
    }


    
    // stbi_image_free(img);    
 }
//01101000 01100101 01101100 01101100 01101111


// Things to add: 
// - Randomise where bits are stored (implement use of key)
// - finish coding feature that allows file to be passed in
// - allow use of password with hashing to create key
// - create pre-defined set of images people can use
// - allow multi letter options (--password)