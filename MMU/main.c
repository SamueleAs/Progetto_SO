#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "define.h"

// QUI DEVO FARE GLI ACCESSI IN memoria_buffer2  E TRST
int main(int argc, char** argv){

//DOBBIAMO INIZIALIZZARE MMU ------------------------------------------
MMU *mmu = (MMU*)malloc(sizeof(MMU));
if(mmu == NULL){ printf("ERRORE IN CREAZIONE MMU"); return -1; }
mmu->memoria_fisica = (char*)malloc(sizeof( MEMORIA_BUFFER ));
if(mmu->memoria_fisica  == NULL){ printf("ERRORE IN memoria_buffer2  MMU"); return -1; }


for(int i=0; i<NUM_PAGES;i++){
mmu->page_table[i].read=0;
mmu->page_table[i].write=0;
mmu->page_table[i].swapp=(rand() % 10 == 0);
mmu->page_table[i].num_frame=-1;
mmu->page_table[i].valid=0;
}

mmu->swap_file = fopen("nome_file.bin", "wb");
if(mmu->swap_file == NULL){printf("ERRORE IN MMU FILE APERTURA"); return -1;}
mmu->free_frames_top =0;
for (int i = 0; i < NUM_FRAMES; ++i)
    {
        mmu->free_mem[i] = i;
        mmu->free_frames_top++;
    }
//INIZIALIZZATO MMU --------------------------------------------------


//SCELTA TEST ------------------------------------------------
int test;
//CHIEDIAMO QUALE TIPO DI ACCESSO IN memoria_buffer2  VUOLE 
printf("\n SCEGLIERE IL TIPO DI TEST DA EFFETTUARE IN memoria_buffer2 : \n 1 Accesso Sequenziale\n 2 Accesso Randomico:");
scanf("%d", &test);
printf("\n");

if( test >= 3 || test <= 0 ){
printf("input errato scegliere solo tra 1/2 \n\n");
return -1;
}
//-------------------------------------------------------------------


srand(time(NULL));
char lettera;
//char lettera= rand() % 256; //9+1 caratt??
char controllo;
int pos;

if(test == 1){
        printf(" hai scelto:  TEST SEQUENZIALE\n\n ");
	for(int i = 0 ; i < 257 ; i++){
	    lettera= '0'+i; //9+1 caratt??
	    MMU_writebyte( mmu , i , lettera);
	    controllo = MMU_readByte( mmu , i);
	    if(lettera != controllo){printf("ERORRE IN POSIZIONE %d\n\n",i);
	      	return -1;}
	    else{
	      	continue;
	         }
	      				}
	printf("TEST SEQUENZIALE SUPERATO");
	
}else if (test == 2 ){
	printf("hai scelto: TEST RANDOMICO\n\n");
	for (int i = 0; i < 9; i++){
        lettera = rand() % (i+1);
        pos = rand() % memoria_virtuale;
        MMU_writebyte(mmu, pos, lettera);
        controllo = MMU_readByte(mmu, pos);
        if (lettera != controllo)
        {
            printf("ERRORE IN POSIZIONE: %d\n", i);
            return -1;
        }
    }
	
	printf("TEST RADOMICO SUPERATO!!!!!\n\n");
	
}
  

fclose(mmu->swap_file);
free(mmu->memoria_fisica);
free(mmu);
    
    
printf("FINITO ORA RETURN");

    




}
