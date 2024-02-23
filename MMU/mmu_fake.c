#include "define.h"



//WRITEBYTE ----------------------------------------------------------
void MMU_writebyte(MMU* mmu , int pos , char c){
	
	if( pos <0 || pos > MEMORIA_VIRTUALE){
	    printf("POSIZIONE DI MEMORIA VIRTUALE NON VALIDA\n");  //VERIFICA VALIDITA DELLA POSIZIONE INIZIALE
	    return;
	}
	
	//CALCOIAMO NUMERO DI PAGINA E OFFSET ALL'INTERNO DELLA PAGINA 
	int numero_pagina= pos / PAGE_SIZE; 
	int offset = pos % PAGE_SIZE;
	
	//CONTROLLO VALIDITA DELLA PAGINA
	if(mmu->page_table[numero_pagina].valid == 0 ){
	printf("PAGE FAULT IN POSIZIONE %d\n", pos);
	MMU_exception(mmu,pos);  //GESTIAMO L'ECCEZIONE IN CASO DI PAGE FAULT 
	return ;}
	
	//SETTIAMO IL BIT DI SCRITTURA
	mmu->page_table[numero_pagina].write = 1;
	
	//CALCOLIAMO L'INDIRIZZO FISICO
	int posizione_memoria_fisica = (mmu->page_table[numero_pagina].num_frame * PAGE_SIZE ) + offset;
	
	//SCRIVIAMO ORA IL CARATTERE NEL POSTO GIUSTO IN MEMORIA FISICA
	mmu->memoria_fisica[posizione_memoria_fisica] = c ;
	
	
}
//------------------------------------------------------------

//READBYTE----------------------------------------------------------------
char MMU_readByte(MMU *mmu, int pos){

	if( pos <0 || pos > MEMORIA_VIRTUALE){
	    printf("POSIZIONE DI MEMORIA VIRTUALE NON VALIDA\n");  //VERIFICA VALIDITA DELLA POSIZIONE INIZIALE
	    return -1;
	}
	
	//CALCOIAMO NUMERO DI PAGINA E OFFSET ALL'INTERNO DELLA PAGINA 
	int numero_pagina= pos / PAGE_SIZE; 
	int offset = pos % PAGE_SIZE;
	
	//CONTROLLO VALIDITA DELLA PAGINA
	if(mmu->page_table[numero_pagina].valid == 0 ){
	printf("PAGE FAULT IN POSIZIONE %d\n", pos);
	MMU_exception(mmu,pos);  //GESTIAMO L'ECCEZIONE IN CASO DI PAGE FAULT 
	}
	
	//SETTIAMO IL BIT DI SCRITTURA
	mmu->page_table[numero_pagina].read = 1;
	
	//CALCOLIAMO L'INDIRIZZO FISICO
	int posizione_memoria_fisica = (mmu->page_table[numero_pagina].num_frame * PAGE_SIZE ) + offset;
	
	//RESTITUIAMO ORA IL CARATTERE NEL POSTO GIUSTO IN MEMORIA FISICA
	 return mmu->memoria_fisica[posizione_memoria_fisica] ;
    }
//-----------------------------------------------------------------------------

int find_page_to_replace(MMU* mmu){
	printf("INIZIA IL SECOND CHANCE ALGORITHM...\n");
	int repleced = -1;
	int vecchio = mmu->indice_vecchio;
	
	//CERCHIAMO UNA PAGINA UTILIZZABILE USANDO L'ALGROTIMO PRIMA CON 0|0
	while(1)
	{
		if(mmu->page_table[vecchio].read == 0 && mmu->page_table[vecchio].write == 0)
		{
			repleced = vecchio;
			break;
		}
	
	
	//RESETTIAMO I BIT DI RIFERIMENTO 
	mmu->page_table[repleced].write=0;
	mmu->page_table[repleced].read=0;
	
	//ANDIAMO AVANTI A CONTROLLARE
	vecchio = (vecchio + 1) % NUM_FRAMES;
	
	}
	
	printf("PAGINA REPLECED NEL FRAME: %d\n", repleced);
	
	//RETURN DEL FRAME IN CUI ABBIAMO EFFETTUATO LO SWAP
	return repleced; 



}





//MMU_EXCEPTION------------------------------------------------------    

void MMU_exception(MMU *mmu, int pos)
{
    int numero_pagina= pos / PAGE_SIZE;
    printf("PAGE FAULT DELLA PAGINA: %d IN POSIZIONE: %d\n\n", numero_pagina, pos);
    
    //CECHIAMO UN FRAME LIBERO IN MEMORIA FISICA
    int frame_libero_fisico = -1;
    for(int i=0;i< NUM_FRAMES;i++)
    {
        if(mmu->page_table[i].valid==0)
        {
            frame_libero_fisico = i;
            break;
        }
    
    }
    
    //SE NON CI SONO FRAME LIBERI DOBBIAMO USARE L'ALGORITMO DI SECOND
    if(frame_libero_fisico = -1)
    {
    	frame_libero_fisico = find_page_to_replace(mmu);
    }
    
    
    //AGGIORNIAMO LA NUOVA PAGINA
    mmu->page_table[numero_pagina].valid=1;
    mmu->page_table[numero_pagina].read=0;
    mmu->page_table[numero_pagina].write=0;
    mmu->page_table[numero_pagina].swapp=0;
    mmu->page_table[numero_pagina].num_frame= frame_libero_fisico; 
    
    //LEGGIAMO LA NUOVA PAGINA DAL FILE DI SWAPP
    fseek(mmu->swap_file , numero_pagina * PAGE_SIZE , SEEK_SET );
    fread(&mmu->memoria_fisica[frame_libero_fisico * PAGE_SIZE] , 1 , PAGE_SIZE , mmu->swap_file);
    
    printf(" PAGINA CARICATA NEL FRAME %d\n", frame_libero_fisico);

}

//----------------------------------------------------------------
    
    


    
    
    
    

