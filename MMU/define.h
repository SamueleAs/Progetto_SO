#include<stdio.h>
#include<stdlib.h>

#ifndef MMU_H
#define MMU_H

#define MEMORIA_BUFFER (1*1024*1024)              //DIMENSIONE DELLA MEMORIA FISICA (1MB)
#define MEMORIA_VIRTUALE (16*1024*1024)           //DIMENSIONE DELLA MEMORIA VIRTUALE (16MB)
#define PAGE_SIZE  4096                            //DIMENSIONE DELLA PAGINA (4KB)
#define NUM_PAGES (MEMORIA_VIRTUALE / PAGE_SIZE)   //NUMERO DI PAGINE
#define NUM_FRAMES (MEMORIA_BUFFER / PAGE_SIZE)    //NUMERO DI FRAME NELLA MEMORIA FISICA 


typedef struct{
 int valid:1; //BIT DI VALIDITA
 int read:1;  // BIT SE PAGINA LETTA
 int write:1;  // BIT SE PAGINA SCRITA
 int swapp:1;  // BIT SE PAGINA SWAPPATA
 int num_frame; // NUMERO DEL FRAME ASSOCIATO ALLA PAGINA 
}PageTableEntry;


typedef struct{
    PageTableEntry page_table[NUM_PAGES];  ///TABELLA DELLE PAGINE
    char memoria_fisica[MEMORIA_BUFFER] ; //PUNTATORE ALLA MEMORIA FISICA
    FILE* swap_file;  //FILE PER LO SWAP
    int indice_vecchio; //INDICE USATO PER L'ALGORITMO
    int free_frames_top; //INDICA IL NUMERO DI FRAMES ATTUALMENTE LIBERI NELLA MEMORIA FISICA
}MMU;


// DICHIARO LE FUNZIONI
char MMU_readByte(MMU* mmu, int pos); //LEGGE E RESTITUISCE UN CHAR 
void MMU_writebyte(MMU* mmu, int pos, char c); //SCRIVE UN CHAR NON RESTITUISCE NIENTE
void MMU_exception(MMU* mmu, int pos); //CHIAMATA IN CASO DI ECCEZIONI 
int find_page_to_replace(MMU* mmu); //FUNZIONE CHE TROVA LA PAGINA DA CAMBIARE

#endif /* MMU_H */ 








