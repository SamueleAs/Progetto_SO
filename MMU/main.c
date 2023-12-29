#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "define.h"


// QUI DEVO FARE GLI ACCESSI IN MEMORIA E TRST
int main(int argc, char** argv){

//DOBBIAMO INIZIALIZZARE MMU ------------------------------------------
MMU *mmu = (MMU*)malloc(sizeof(MMU));
if(mmu == NULL){printf("ERRORE IN MMU"); return -1;}
mmu->memoria_fisica= (char*)malloc(sizeof(memoria_buffer));
if(mmu->memoria_fisica == NULL){printf("ERRORE IN MMU"); return -1;}


for(int i=0; i<NUM_PAGES;i++){
mmu->page_table[i].read=0;
mmu->page_table[i].write=0;
mmu->page_table[i].swapp=0;
mmu->page_table[i].num_frame=0;
mmu->page_table[i].valid=0;
}

mmu->swap_file = fopen("nome_file.bin", "w+b");
if(mmu->swap_file == NULL){printf("ERRORE IN MMU FILE"); return -1;}
mmu->free_mem_top =0;
//INIZIALIZZATO MMU --------------------------------------------------


//SCELTA TEST ------------------------------------------------
int test;
//CHIEDIAMO QUALE TIPO DI ACCESSO IN MEMORIA VUOLE 
printf("\n SCEGLIERE IL TIPO DI TEST DA EFFETTUARE IN MEMORIA: \n 1 Accesso Sequenziale\n 2 Accesso Sequenziale inverso\n 3 Accesso Randomico\n :");
scanf("%d", &test);
printf("\n");

if( test >= 4 || test <= 0 ){
printf("input errato scegliere solo tra 1/2/3 \n\n");
return -1;
}
//-------------------------------------------------------------------



srand(time(NULL));
char lettera= rand() % 10; //9??
char controllo;

if(test == 1){

	printf(" hai scelto:  TEST SEQUENZIALE\n\n ");
	for(int i = 0 ; i < 100 ; i++){
	    MMU_writebyte( mmu , i , lettera);
	    controllo = MMU_readbyte( mmu , i);
	    //if(lettera == controllo) return 1;
	    //else return -1;
	    return 1;
	}
	
}else if (test == 2 ){
	printf("hai scelto: TEST SEQUENZIALE INVERSO\n\n");
	return 0;
	
	
}else if (test == 3){
	printf("hai scelto: ACCESSO RANDOMICO\n\n");
	return 0;
	
	
}

  
 




}
