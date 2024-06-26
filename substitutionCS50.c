#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main (int argc , string argv[])

{
string key = argv[1];
if ((argc)!=2){
    printf("Usage: ./substitution key\n");
    return 1;
    }
    if((strlen(key)!=26)){
        printf("Key must contain 26 characters.\n");
        return 1;
        }
for (int i = 0 ; i<strlen(key) ; i++){
    if (!isalpha(key[i])){
    printf("Key must only contain alphabetic characters.\n");
    return 1;
    }
}
    for (int i = 0 ; i<strlen(key) ; i++){
        for (int j = i+1; j<strlen(key) ; j++)
            if (toupper(key[i]) == toupper(key[j]))
            printf("Key must not contain repeated characters.\n");
            }
    for (int i = 0 ; i<strlen(key) ; i++){
        if (islower(key[i]))
            key[i]= key[i] - 32;
        }
    string plaintext = get_string("Plaintext : \n");

    for (int i = 0 ; i<strlen(plaintext) ; i++){
            if (isupper(plaintext[i]))
            {
                int letter = plaintext[i] - 65;
                printf("%c" , key[letter]);
            }
            else if (islower(plaintext[i]))
            {
                int letter = plaintext[i] - 97;
                printf("%c" , key[letter]+32);
            }
            else
            printf("%c" , plaintext[i]);
            }
            printf("\n");

}