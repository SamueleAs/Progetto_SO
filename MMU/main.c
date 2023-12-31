#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "define.h"

// QUI DEVO FARE GLI ACCESSI IN memoria_buffer2  E TRST
int main(int argc, char** argv){

//DOBBIAMO INIZIALIZZARE MMU ------------------------------------------
MMU *mmu = (MMU*)malloc(sizeof(MMU));
if(mmu == NULL){ printf("ERRORE IN CREAZIONE MMU"); return -1; }

mmu->memoria_fisica = (char*)malloc(MEMORIA_BUFFER);
if(mmu->memoria_fisica  == NULL){ printf("ERRORE IN ALLOCAZIONE MEMORIA_FISICA  MMU"); return -1; }


for(int i=0; i<NUM_PAGES;i++){
mmu->page_table[i].read=0;
mmu->page_table[i].write=0;
mmu->page_table[i].swapp= (rand() % 10);
mmu->page_table[i].valid=0;
mmu->page_table[i].num_frame=-1;
}

mmu->swap_file = fopen("nome_file.bin", "w+");
if(mmu->swap_file == NULL){printf("ERRORE IN FILE MMU APERTURA"); return -1;}

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
printf("\n SCEGLIERE IL TIPO DI TEST DA EFFETTUARE IN MEMORIA : \n 1 Accesso Sequenziale\n 2 Accesso Randomico\n 3 ACESSO SEQUANZIALE INVERSO\n");
printf("INSERISCI:  ");
scanf("%d", &test);
printf("\n");
if( test >= 4 || test <= 0 ){
printf("input errato scegliere solo tra 1/2/3 \n\n");
return -1;
}
//-------------------------------------------------------------------


srand(time(NULL));
char scrivo;
char leggo;
int posizione;

//TEST1---------------------------------------------
if(test == 1){
        printf(" hai scelto:  TEST SEQUENZIALE\n\n ");
	for(int i = 0 ; i < 100 ; i++){
	    scrivo= '0'+i;
	    MMU_writebyte( mmu , i , scrivo);
	    leggo = MMU_readByte( mmu , i);
	    if(scrivo != leggo){ printf("ERORRE IN POSIZIONE %d\n\n",i);return -1;}
	    
	         
	      				}
	printf("TEST SEQUENZIALE SUPERATO\n\n");}
//--------------------------------------------------------------	


//TEST2-----------------------------------------------------------------	
if (test == 2 ){
	printf("hai scelto: TEST RANDOMICO\n\n");
	for (int i = 0; i < 100; i++){
        scrivo = rand() % (i+1);
        posizione = rand() % 1000;
        
        MMU_writebyte(mmu, posizione, scrivo);
        leggo = MMU_readByte(mmu, posizione);
        if (scrivo != leggo){ printf("ERRORE IN POSIZIONE: %d\n", i);return -1;
        }
                                    }
	printf("TEST RADOMICO SUPERATO!!!!!\n\n");
	}
//-------------------------------------------------------------------



//TEST3-----------------------------------------------------
if(test==3){
printf("INIZIO IL TEST AL CONTRARIO\n\n");	     
for(int i=30; i>0; --i){
scrivo= rand() % i;
posizione= i;      //RINDONDANZA NELL'USO SI POSIZIONE MA PIU FACILE DA LEGG.
MMU_writebyte(mmu,posizione,scrivo);
leggo = MMU_readByte(mmu,posizione);
if(scrivo!=leggo){printf("ERRORE IN POSIZIONE: %d",i); return -1;}
			}
printf("TEST SEQUANZIALE INVERSO SUPERATO!!!\n\n");
}
//--------------------------------------------------


//FREE----------------
printf("INIZIO FREE E CLOSE\n");
fclose(mmu->swap_file);
printf("FCLOSE FATTA\n");
free(mmu->memoria_fisica);
printf("FREE MEMMORIA_FISICA FATTA\n");
free(mmu);
printf("FREE MMU FATTO\n");
//--------------------------------------
  
    
printf("FINITO ORA RETURN \n\n");
return 1;

}
