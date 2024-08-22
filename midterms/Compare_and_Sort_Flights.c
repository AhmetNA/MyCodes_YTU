#include <stdio.h>
#include <stdlib.h>
#include <string.h>   
typedef struct Ucus {
    int ucusKodu;
    char kalkisyeri[50];
    char inisyeri[50];
    char saat[6];
    int hat;
    int pist;
    struct Ucus *sonraki;
} Ucus;


void ucusEkle(Ucus **bas, int ucusKodu, char *kalkisSehri, char *varisSehri, char *saat, int hat, int pist) {
    Ucus *ekleucus = (Ucus*)malloc(sizeof(Ucus));
    ekleucus->ucusKodu = ucusKodu;
    strcpy(ekleucus->kalkisyeri, kalkisSehri);
    strcpy(ekleucus->inisyeri, varisSehri);
    strcpy(ekleucus->saat, saat);
    ekleucus->hat = hat;
    ekleucus->pist = pist;
    ekleucus->sonraki = NULL;

    if(*bas == NULL || (*bas)->ucusKodu > ucusKodu){
        ekleucus->sonraki = *bas;
        *bas = ekleucus;
    } 
    else{
        Ucus *temporarry = *bas;
        while (temporarry->sonraki != NULL ) {
            if( temporarry->sonraki->ucusKodu < ucusKodu){
            temporarry = temporarry->sonraki;
            }
        }
        ekleucus->sonraki = temporarry->sonraki;
        temporarry->sonraki = ekleucus;
    }
}



void yeni_dosya_yaz(char *dosyaAdi, Ucus *bas) {
    FILE *dosya = fopen(dosyaAdi, "w");
    if (dosya == NULL) {
        printf("Dosya acilamadi\n");
        return;

    }


    Ucus *temporary = bas;
    while (temporary != NULL) {
        if (temporary->pist == (dosyaAdi[4] - '0')) { 

            fprintf(dosya, "%d %s %s %s %d %d\n", temporary->ucusKodu, temporary->kalkisyeri, temporary->inisyeri, temporary->saat, temporary->hat, temporary->pist);
        
        }
        temporary = temporary->sonraki;
    }
    fclose(dosya);
}

void listeyi_yazdir(Ucus *bas) {
    Ucus *temporary = bas;
    while (temporary != NULL) {
        printf("%d %s %s %s %d %d\n", temporary->ucusKodu, temporary->kalkisyeri, temporary->inisyeri, temporary->saat, temporary->hat, temporary->pist);
        temporary = temporary->sonraki;
    }
}

int main() {
    //ilk dosyay� okuma
    FILE *dosya = fopen("ucusBilgi.txt", "r");
    if (dosya == NULL) {
        printf("File not found\n");
        return 1;
    }        
    
    Ucus *ilk = NULL;
    int ucusKodu, hat, pist;
    char kalkisyeri[50], varisyeri[50], saat[6];
    //dosyadan okuma
    while (fscanf(dosya, "%d %s %s %s %d %d", &ucusKodu, kalkisyeri, varisyeri, saat, &hat, &pist) != EOF) {
     
        ucusEkle(&ilk, ucusKodu, kalkisyeri, varisyeri, saat, hat, pist);

    }       

    fclose(dosya); 

    //sirali listeyi yazd�r
    printf("Liste\n");          
    listeyi_yazdir(ilk);
 
    // pistlere gore dosyalara yaz
    yeni_dosya_yaz("pist1.txt", ilk);
    yeni_dosya_yaz("pist2.txt", ilk);    
    yeni_dosya_yaz("pist8.txt", ilk);

    // tum dosyalari yazdir printf ile
    printf("yeni dosyalar\n");
    printf("Pist 1:     \n");
    dosya = fopen("pist1.txt", "r");
    char c;
    while ((c = fgetc(dosya)) != EOF) {
        printf("%c", c);
    }
    fclose(dosya); 
    dosya = fopen("pist2.txt", "r");
    printf("Pist 2:\n");
    while ((c = fgetc(dosya)) != EOF) {
        printf("%c", c);
    }       
    fclose(dosya);
    dosya = fopen("pist8.txt", "r");      
    printf("Pist 8:\n");
    while ((c = fgetc(dosya)) != EOF) {
        printf("%c", c);
    }
    fclose(dosya);
    return 0;



}

