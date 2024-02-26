#include "define.h"

//WRITEBYTE ----------------------------------------------------------
void MMU_writebyte(MMU* mmu , int pos , char c){
	
	//VERIFICA SE LA POSIZIONE DI PARTENZA È VALIDA
	if( pos <0 || pos > MEMORIA_VIRTUALE){
	    printf("POSIZIONE DI MEMORIA VIRTUALE NON VALIDA\n"); 
	    return;
	}
	
	//CALCOIAMO NUMERO DI PAGINA E OFFSET ALL'INTERNO DELLA PAGINA 
	int numero_pagina= pos / PAGE_SIZE; 
	int offset = pos % PAGE_SIZE;
	
	//CONTROLLO VALIDITA DELLA PAGINA
	if(mmu->page_table[numero_pagina].valid == 0 ){
	printf("PAGE FAULT IN POSIZIONE: %d  PER LA PAGINA: %d\n", pos, numero_pagina);
	//SE LA PAGINA NON È VALIDA GESTIAMO UN PAGE_FAULT	
	MMU_exception(mmu,pos);  
	}else
	{
		//SE LA PAGINA È VALIDA HIT
		  printf("HIT IN POSIZIONE: %d DELLA PAGINA: %d\n", pos , numero_pagina);
	}
	
	
	//CALCOLIAMO L'INDIRIZZO FISICO CORRISPONDENTE ALLA POSIZIONE IN MEMPRIA FISICA
	int posizione_memoria_fisica = (mmu->page_table[numero_pagina].num_frame * numero_pagina ) + offset;
	
	//SCRIVIAMO ORA IL CARATTERE NEL POSTO GIUSTO IN MEMORIA FISICA
	mmu->memoria_fisica[posizione_memoria_fisica] = c ;
	
	//SETTIAMO IL BIT DI SCRITTURA VISTO CHE ABBIAMO LETTO
	mmu->page_table[numero_pagina].write = 1;
	
	
}
//------------------------------------------------------------

//READBYTE----------------------------------------------------------------
char MMU_readByte(MMU *mmu, int pos){

	//VERIFICA SE LA POSIZIONE DI PARTENZA È VALIDA
	if( pos <0 || pos > MEMORIA_VIRTUALE){
	    printf("POSIZIONE DI MEMORIA VIRTUALE NON VALIDA\n");
	    return -1;
	}
	
	//CALCOIAMO NUMERO DI PAGINA E OFFSET ALL'INTERNO DELLA PAGINA 
	int numero_pagina= pos / PAGE_SIZE; 
	int offset = pos % PAGE_SIZE;
	
	//CONTROLLO VALIDITA DELLA PAGINA
	if(mmu->page_table[numero_pagina].valid == 0 ){
	printf("PAGE FAULT IN POSIZIONE: %d  PER LA PAGINA: %d\n", pos,numero_pagina);
	//SE LA PAGINA NON È VALIDA GESTIAMO UN PAGE_FAULT
	MMU_exception(mmu,pos);  
	}else
	{
		 printf("HIT IN POSIZIONE: %d DELLA PAGINA: %d\n", pos , numero_pagina);
        	
	}
	
	//CALCOLIAMO L'INDIRIZZO FISICO CORRISPONDETNE ALLA POSIZIONE
	int posizione_memoria_fisica = (mmu->page_table[numero_pagina].num_frame * numero_pagina ) + offset;
	
	//RESTITUIAMO ORA IL CARATTERE NEL POSTO GIUSTO IN MEMORIA FISICA
	return mmu->memoria_fisica[posizione_memoria_fisica] ;
	 
	//SETTIAMO IL BIT DI SCRITTURA
	mmu->page_table[numero_pagina].read = 1;
    }
//-----------------------------------------------------------------------------

//MMU_EXCEPTION------------------------------------------------------    

void MMU_exception(MMU *mmu, int pos)
{//DOBBIAMO GESTIRE UN PAGE_FAULT -> PRIMA TROVIAMO LA PAGINA E CONTROLLIAMO SE NON È CARICATA -> ALTRIMENTI SWAP IN MEMORIA FISICA
	
	//CALCOLIAMO IL NUMERO DELLA PAGINA
	int numero_pagina = pos / PAGE_SIZE;
	printf("PAGE_FAULT DELLA PAGINA:  %d IN POSIZIONE: %d\n", numero_pagina, pos);
	
	//ORA CONTROLLIAMO SE È GIA NELLA MEMORIA FISICA ATTRAVERSO IL BIT DI VALIDITA 
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
			int frames =  mmu->free_frames_top;  //NUMERO DI FRAME UTILIZZABILI PER LA NUOVA PAGINA (PIU CHIARO)
			printf("ABBIAMO SPAZIO PER CARICARLA, NUMERO DI FRAME ATTUALEMTE LIBERI: %d\n", frames);
			mmu->free_frames_top = mmu->free_frames_top - 1 ;  //DECREMENTIAMO IL NUMERO DI FRAMES LIBERI 
			
			//CARICHIAMO LA PAGINA IN MEMORIA
			mmu->page_table[numero_pagina].valid=1;
			mmu->page_table[numero_pagina].read=0;
			mmu->page_table[numero_pagina].write=0;
			mmu->page_table[numero_pagina].swapp=0;
			mmu->page_table[numero_pagina].num_frame= frames;
			
			//PRENDE LO SWAP_FILE E CI METTIAMO NEL PUNTO (NUM_PAG * PAG_SIZ) PARTENDO DALL'INIZIO DEL FILE
			fseek(mmu->swap_file, numero_pagina * PAGE_SIZE, SEEK_SET);
			//PRENDE SEMPRE IL PUNTATORE ALLA MEMORIA NELLA POSIZIONE SPECIFICATA, 1 ELEMENTO LETTO DI DIM PAGE-SIZE,DAL 				PUNTATORE DELLO SWAP_FILE
			fread(&mmu->memoria_fisica[frames * PAGE_SIZE],1,PAGE_SIZE, mmu->swap_file);
			printf("PAGINA CARICATA NEL FRAME: %d\n", frames);
		}else
		{       
			printf("NON ABBIAMO SPAZIO PER CARICARLA, SWAPP");
		        //DOBBIAMO EFFETTURARE IL REPLACE
			int pagina_da_rimpiazzare = mmu->indice_vecchio; //PARTIAMO DA QUI A CERCARE (INIZIA IN 0)
			int trovata = 0;
		  	
		  	//VEDE SOLO SE ESISTE UNA PAGINA RIMPIAZABILE E NE PRENDE L'INDICE
			while(trovata == 0)
			{
				//CONDIZIONI PER IL REPLACE DI UNA PAGINA 
				if( (mmu->page_table[pagina_da_rimpiazzare].read==1 && mmu->page_table[pagina_da_rimpiazzare].write==1) 
				   || (mmu->page_table[pagina_da_rimpiazzare].read==1 && mmu->page_table[pagina_da_rimpiazzare].write==0)
				   || (mmu->page_table[pagina_da_rimpiazzare].read==0 && mmu->page_table[pagina_da_rimpiazzare].write==1))
				{
					trovata = 1;
				}else
				{
					//METTIAMO I BIT A ZERO E PASSA ALLA PAGINA SUCCESSIVA
					mmu->page_table[pagina_da_rimpiazzare].read=0;
					mmu->page_table[pagina_da_rimpiazzare].write=0;
					pagina_da_rimpiazzare = (pagina_da_rimpiazzare + 1) % NUM_FRAMES;
					
				}
			
			 }//FINE DEL WHILE
		  	
		  	 //USCIAMO CON L'INDICE "PAGINA_DA_RIMPIAZZARE" UGUALE ALLA PAGINA CHE RISPETTA UNA DELLE CONDIZIONI PER LO SWAPP
		  	 //ORA QUEL FRAME SARA UTILIZZATO DALLA NUOVA PAGINA DI CUI CI HANNO DATO LA POS
			
			
			//NUMERO DEL FRAME FISICO UTILIZZATO PER MEMORIZZARE LA PAGINA VIRTUALE DOPO LA SOSTITUZIONE
			int frame_fisico = mmu->page_table[pagina_da_rimpiazzare].num_frame;
			
			
			//ORA USO QUEL FRAME DI MEMORIA PER LA NUOVA PAGINA
			
			//AGGIORNA LA PAGINA IN MEMORIA FISICA
			mmu->page_table[numero_pagina].valid=1;
			mmu->page_table[numero_pagina].swapp=0;
			mmu->page_table[numero_pagina].read=0;
			mmu->page_table[numero_pagina].write=0;
			mmu->page_table[numero_pagina].num_frame = frame_fisico; 
			
			//PRENDE LO SWAP_FILE E CI METTIAMO NEL PUNTO (NUM_PAG * PAG_SIZ) PARTENDO DALL'INIZIO DEL FILE
			fseek(mmu->swap_file, numero_pagina * PAGE_SIZE ,SEEK_SET);
			//PRENDE SEMPRE IL PUNTATORE ALLA MEMORIA NELLA POSIZIONE SPECIFICATA, 1 ELEMENTO LETTO DI DIM PAGE-SIZE,DAL 				PUNTATORE DELLO SWAP_FILE  
			fread(&mmu->memoria_fisica[frame_fisico * PAGE_SIZE],1,PAGE_SIZE, mmu->swap_file);
			printf("PAGINA CARICATA NEL FRAME:  %d,(SOSTITUENDO LA PAGINA %d)\n",frame_fisico,pagina_da_rimpiazzare);
			
			//INCREMENIAMO L'INDICE DA CUI PARTIRE PER IL PROSSIMO PAGE_FAULT
			mmu->indice_vecchio = (pagina_da_rimpiazzare + 1) % NUM_FRAMES;
			
			
			
		
		
		
		}
	}



}




    
    
    
    

