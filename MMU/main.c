#include <time.h>
#include "define.h"

// ALLOCARE MMU -> INIZIALIZZO -> TEST -> CHIUDO 
int main(int argc, char** argv){

//DOBBIAMO INIZIALIZZARE MMU PER I TEST ------------------------------------------
	MMU* mmu = (MMU*)malloc(sizeof(MMU));
	if(mmu == NULL) { printf("ERRORE NELLA CREAZIONE MMU\n"); return-1; }
	
	mmu->swap_file= fopen("swap_file.bin","wb+");
	if(mmu->swap_file == NULL)
	{
		printf("ERRORE NELL'APERTURA DEL FILE\n");
		return -1;
	}
	
	for(int i=0;i<NUM_PAGES;i++)
	{
		mmu->page_table[i].read=0;
		mmu->page_table[i].write=0;
		mmu->page_table[i].valid=0;
		mmu->page_table[i].swapp=0; 
		mmu->page_table[i].num_frame=-1;
	}
	
	mmu->indice_vecchio = 0;
	mmu->free_frames_top = NUM_FRAMES; //INIZIALMETNE SONO TUTTI LIBERI
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

//TEST1 LINEARE ---------------------------------------------
if(test == 1){
        printf(" hai scelto:  TEST SEQUENZIALE\n\n ");
	for(int i = 0 ; i < 50 ; i++){
	    scrivo= '0' + i;
	    MMU_writebyte( mmu , i , scrivo);
	    leggo = MMU_readByte( mmu , i);
	    if(scrivo != leggo)
	    {
	    	printf("ERORRE IN POSIZIONE %d\n\n", i); 
	    	return -1;
	    }
	
	     				}
	printf("TEST SEQUENZIALE SUPERATO\n\n");
	    }
//--------------------------------------------------------------	


//TEST2 CASUALE -----------------------------------------------------------------	
if (test == 2 ){
	printf("hai scelto: TEST RANDOMICO\n\n");
	for (int i = 0; i < 10; ++i){
        	scrivo = rand() % 256;
        	posizione = rand() % MEMORIA_VIRTUALE;
        	MMU_writebyte(mmu, posizione, scrivo);
        	leggo = MMU_readByte(mmu, posizione);
        	if (scrivo != leggo)
        	{
        	printf("ERRORE IN POSIZIONE: %d\n", i);
        	return -1;
        	}
                                     }
                          
	printf("TEST RADOMICO SUPERATO!!!!!\n\n");
	       }
//-------------------------------------------------------------------



//TEST3 CONTRARIO -----------------------------------------------------
if(test==3){
	printf("INIZIO IL TEST AL CONTRARIO\n\n");	     
	for(int i=50; i>0; --i){
		scrivo= rand() % i;
		posizione= i;      //RINDONDANZA NELL'USO SI POSIZIONE MA PIU FACILE DA LEGG.
		MMU_writebyte(mmu,posizione,scrivo);
		leggo = MMU_readByte(mmu,posizione);
		if(scrivo!=leggo)
		{
			printf("ERRORE IN POSIZIONE: %d",i); 
			return -1;
		}
			          }
	printf("TEST SEQUANZIALE INVERSO SUPERATO!!!\n\n");
            }
//--------------------------------------------------


//FREE----------------
fclose(mmu->swap_file);
free(mmu);
printf("PULIZIA MEMORIA FATTA\n");

int check = remove ("swap_file.bin");
if(check == -1) printf("ERRORE IN RIMOZIONE FILE\n");   //COSI OGNI VOLTA ELIMINIAMO IL FILE (MEGLIO PER LA MEMRIA ????)
//--------------------------------------
  
//FINE------------------------------------------
printf("FINITO \n\n");
return 1;

}
