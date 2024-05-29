#include "sha-2/sha-256.c"
#include <stdio.h>

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

int main(){
    // struct Sha_256 sha_256;
    uint8_t hash[SIZE_OF_SHA_256_HASH];
    // sha_256_init(&sha_256, hash);
    calc_sha_256(hash, "", strlen(""));
    // sha_256_close(&sha_256);

    for(int i = 0; i<32;i++){
        print_as_bin(hash[i]);
    }
}