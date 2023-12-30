#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "define.h"


// QUI DEVO FARE GLI ACCESSI IN MEMORIA E TRST
int main(int argc, char** argv){

//DOBBIAMO INIZIALIZZARE MMU ------------------------------------------
MMU *mmu = (MMU*)malloc(sizeof(MMU));
if(mmu == NULL){ printf("ERRORE IN CREAZIONE MMU"); return -1; }
mmu->memoria_buffer= (char*)malloc(sizeof(memoria_buffer));
if(mmu->memoria_buffer == NULL){ printf("ERRORE IN memoria_buffer MMU"); return -1; }


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
//INIZIALIZZATO MMU --------------------------------------------------


//SCELTA TEST ------------------------------------------------
int test;
//CHIEDIAMO QUALE TIPO DI ACCESSO IN MEMORIA VUOLE 
printf("\n SCEGLIERE IL TIPO DI TEST DA EFFETTUARE IN MEMORIA: \n 1 Accesso Sequenziale\n 2 Accesso Randomico:");
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
  
  
/*MMU_writebyte(mmu, 0, 'X');
MMU_readByte(mmu, 0);
MMU_writebyte(mmu, memoria_virtuale - 1, 'X');
MMU_readByte(mmu, memoria_virtuale - 1);*/
/*fclose(mmu->swap_file);
free(mmu->memoria_buffer);
free(mmu);
  */  
    
printf("FINITO ORA RETURN");
return 0;
    




}
