#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node {
int v1;
int v2;
struct Node* next;
} Node;

void printList(Node* n){
    while(n != NULL){
        printf("v1: %d v2: %d\n", n->v1, n->v2);
        n = n->next;
    }
}; 

void insert(struct Node** head_ref, int v1, int v2){
    // listenin sonuna ekle
    Node* new_node = (Node*)malloc(sizeof(Node));
    Node* last = *head_ref;
    new_node->v1 = v1; 
    new_node->v2 = v2;
    new_node->next = NULL;
    if(*head_ref == NULL){
        *head_ref = new_node;
        return ;
    }
    while(last->next != NULL){
        last = last->next;
    }
    last->next = new_node;
    return;
} // Verilen degerlere sahip Node'u linkli
//listenin sonuna ekler. 10p
Node* intersection(Node* start1, Node* start2 , int (*compare_by_v1)(Node*, Node*), int (*compare_by_v2)(Node*, Node*) , void (*insert)(Node**, int, int)){
    // v1 ve v2 değerlerine göre sıralı iki linkli listenin elemanlarının
    // verilen fonksiyon şartına göre keşimini bulur (yeni bir linkli liste olarak döndürür). 10p
    Node* start3 = NULL;
    while(start1 != NULL){
        while(start2 != NULL){
            if(compare_by_v1(start1, start2) == 1){
                insert(&start3, start1->v1, start1->v2);
            }
            start2 = start2->next;
        }
        start1 = start1->next;
    }
    Node *start4 = NULL;
    while(start1 != NULL){
        while(start2 != NULL){
            if(compare_by_v2(start1, start2) == 1){
                insert(&start4, start1->v1, start1->v2);
            }
            start2 = start2->next;
        }
        start1 = start1->next;
    }
    return start3 , start4;
};
 //v1 ve v2
//değerine göre sıralı iki linkli listenin elemanlarının
// verilen fonksiyon şartına göre birleşimini bulur (v1 değeri aynı ise v2 değerini her zaman 
//1.bağlı listeden alır, v2 değeri aynı ise v1 değerini her zaman 1. Bağlı listeden alır), v1 veya v2
//değerleri üzerinde keşim varsa yeni bağlı listeye eleman olarak ekler (yeni bir linkli liste olarak
//döndürür). 
int compare_by_v1(Node* Node1, Node* Node2 ){
    if(Node1->v1 == Node2->v1){
        return 1;
    }
    return 0;
}; //iki düğümün v1 değerleri aynı ise 1 değilse 0 döndürür. 

int compare_by_v2(Node* Node1, Node* Node2){ //iki düğümün v2 değerleri aynı ise 1 değilse 0 döndürür. 
    if(Node1->v2 == Node2->v2){
        return 1;
    }
    return 0;
}


int main(){
    
    Node* start1 = NULL;
    Node* start2 = NULL;
    // v1 e gore karsılastırma
    Node* start3 = NULL;
    // v2 ye gore karsılastırma
    Node* start4 = NULL;
    //linked list 1 e ekleme
    insert(&start1, 1, 10);
    insert(&start1, 2, 20);
    insert(&start1, 3, 5);
    // linked list 2 ye ekleme      
    insert(&start2, 2, 5);
    insert(&start2, 1, 25);
    insert(&start2, 4, 20);
    intersection(start1, start2 , compare_by_v1, compare_by_v2, insert);
    printList(start1);
    printList(start2);
    printList(start3);
    printList(start4);
    return 0;
}