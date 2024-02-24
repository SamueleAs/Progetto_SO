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
	}else
	{
		  printf("HIT IN POSIZIONE: %d DELLA PAGINA: %d\n", pos , numero_pagina);
	}
	
	//SETTIAMO IL BIT DI SCRITTURA
	mmu->page_table[numero_pagina].write = 1;
	
	//CALCOLIAMO L'INDIRIZZO FISICO
	int posizione_memoria_fisica = (mmu->page_table[numero_pagina].num_frame * numero_pagina ) + offset;
	
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
	}else
	{
		 printf("HIT IN POSIZIONE: %d DELLA PAGINA: %d\n", pos , numero_pagina);
        	
	}
	
	//SETTIAMO IL BIT DI SCRITTURA
	mmu->page_table[numero_pagina].read = 1;
	
	//CALCOLIAMO L'INDIRIZZO FISICO
	int posizione_memoria_fisica = (mmu->page_table[numero_pagina].num_frame * numero_pagina ) + offset;
	
	//RESTITUIAMO ORA IL CARATTERE NEL POSTO GIUSTO IN MEMORIA FISICA
	 return mmu->memoria_fisica[posizione_memoria_fisica] ;
    }
//-----------------------------------------------------------------------------

//MMU_EXCEPTION------------------------------------------------------    

void MMU_exception(MMU *mmu, int pos)
{//DOBBIAMO GESTIRE UN PAGE_FAULT--> PRIMA TROVIAMO LA PAGINA E CONTROLLIAMO SE NON È CARICATA -> ALTRIMENTI SWAP IN MEMORIA FISICA
	
	//TROVIAMO IL NUMERO DELLA PAGINA
	int numero_pagina = pos / PAGE_SIZE;
	printf("PAGE_FAULT DELLA PAGINA:  %d IN POSIZIONE: %d\n", numero_pagina, pos);
	
	//ORA CONTROLLIAMO SE È GIA NELLA MEMORIA FISICA ATTRAVERSO I BIT DI VALIDITA 
	if(mmu->page_table[numero_pagina].valid==1)
	{
		printf("LA PAGINA È GIA NELLA MEMORIA FISICA\n");
		return;
	}else
	{
		printf("LA PAGINA NON È CARICATA IN MEMORIA FISICA\n");
		//ORA DOBBIAMO CONTROLLARE SE C'E ABBASTANZA SPAZIO PER CARICARLA O EFFETTUARE L'ALGORITMO PER IL REPLACE
		
		if(mmu->free_frames_top > 0)
		{
			int frames = NUM_FRAMES - mmu->free_frames_top;//DECREMENTIAMO IL NUMERO DI FRAMES LIBERI 
			mmu->free_frames_top = mmu->free_frames_top--; //DECREMENTIAMO IL NUMERO DI FRAMES LIBERI 
			
			mmu->page_table[numero_pagina].valid=1;
			mmu->page_table[numero_pagina].read=0;
			mmu->page_table[numero_pagina].write=0;
			mmu->page_table[numero_pagina].swapp=0;
			mmu->page_table[numero_pagina].num_frame= frames;
			
			fseek(mmu->swap_file, numero_pagina * PAGE_SIZE, SEEK_SET);
			fread(&mmu->memoria_fisica[frames * PAGE_SIZE],1,PAGE_SIZE, mmu->swap_file);
			printf("PAGINA CARICATA NEL FRAME: %d\n", frames);
		}else
		{//DOBBIAMO EFFETTURARE IL REPLACE
			int pagina_da_rimpiazzare = mmu->indice_vecchio; //PARTIAMO DA QUI A CERCARE
			int trovata = 0;
			while(trovata == 0)
			{
				if( (mmu->page_table[pagina_da_rimpiazzare].read==1 && mmu->page_table[pagina_da_rimpiazzare].write==1) 
				   || (mmu->page_table[pagina_da_rimpiazzare].read==1 && mmu->page_table[pagina_da_rimpiazzare].write==0)
				   || (mmu->page_table[pagina_da_rimpiazzare].read==0 && mmu->page_table[pagina_da_rimpiazzare].write==1))
				{
					trovata = 1;
				}else
				{
					//METTIAMO I BIT A ZERO E ANDIAMO AVANTI
					mmu->page_table[pagina_da_rimpiazzare].read=0;
					mmu->page_table[pagina_da_rimpiazzare].write=0;
					pagina_da_rimpiazzare = (pagina_da_rimpiazzare + 1) % NUM_FRAMES;
					
				}
			
			} //FINE DEL WHILE
			int frame_fisico = mmu->page_table[pagina_da_rimpiazzare].num_frame;
			
			//AGGIORNIAMO LA TABELLA
			mmu->page_table[numero_pagina].valid=1;
			mmu->page_table[numero_pagina].swapp=0;
			mmu->page_table[numero_pagina].read=0;
			mmu->page_table[numero_pagina].write=0;
			mmu->page_table[numero_pagina].num_frame = frame_fisico; 
			
			fseek(mmu->swap_file, numero_pagina * PAGE_SIZE ,SEEK_SET);
			fread(&mmu->memoria_fisica[frame_fisico * PAGE_SIZE],1,PAGE_SIZE, mmu->swap_file);
			printf("PAGINA CARICATA NEL FRAME:  %d,(SOSTITUENDO LA PAGINA %d)\n",frame_fisico,pagina_da_rimpiazzare);
			
			//INCREMENIAMO L'INDICE PER IL PROSSIMO PAGE_FAULT
			mmu->indice_vecchio = (pagina_da_rimpiazzare + 1) % NUM_FRAMES;
			
			
			
		
		
		
		}
	}



}

//----------------------------------------------------------------
    
    


    
    
    
    

