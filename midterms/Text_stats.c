#include<stdio.h>
int letters[26];
char str[1000];
int lettervalue;
int sum_of_letters=0;
int wordcounter = 0;
int sentence=0;
int main(){
    for (int j = 0 ; j<26 ; j++){
        letters[j] = 0;
    }
    printf("Metni giriniz: ");
    fgets(str, sizeof(str), stdin);
    int i = 0;
    while (str[i]!='\0'){
        lettervalue = str[i]-'A';
        if (lettervalue<26 && lettervalue >=0){
            letters[lettervalue]++;
        }
        else if (lettervalue>=26){
            lettervalue = str[i] - 'a';
            if (lettervalue<26 && lettervalue>=0){
                letters[lettervalue]++;
            }
        }
        i++;   
    }
    for (int a = 0 ; a<26 ; a++){
        sum_of_letters = letters[a] + sum_of_letters;
    }
    int h = 0; 
    while (str[h]!='\0'){
        if(str[h]=='.'){
            if (str[h+1]!='.'){
                sentence++;
            }
        }
        h++;
    }
    int k=0;
    while (str[k]!='\0'){
        if(str[k]==' '){
            if(str[k-1]!=' '){
            wordcounter++;
            k++;
            }
            else
            k++;
        }
        else 
        k++;
    }
    wordcounter++;
   
    printf("Cumle sayisi : %d\n" , sentence);
    printf("Kelime sayisi : %d\n" , wordcounter);
    printf("Harf sayisi : %d\n" , sum_of_letters);
    for (int k = 0 ; k<26 ; k++){
        if (letters[k]!=0){
        char letter = 'a' + k;
        printf("%c : %d" , letter , letters[k]);
        for (int a =0; a<letters[k] ; a++){
            printf("*");
            }
            printf("\n");
    }
    }
    return 0;
}