#include "define.h"

//QUI DEVO FARE UN MMU E LE FUNZIONI 

void MMU_writebyte(MMU* mmu , int pos , char c){
    int numero_pagina= pos/pag_size;
    int offset = pos % pag_size;    //formule del prof per capire pagina e offset 
    
	
	//controlliamo la pagina
    if (mmu->page_table[numero_pagina].valid == 0){
        printf("PAGE FAULT IN POSIZIONE: %d\n", pos);
        MMU_exception(mmu, pos); //chiamo la funzione per le eccezzioni
    }
    
    //ora possiamo scrivere
    int num_frame = mmu->page_table[numero_pagina].num_frame;
    //portiamolo in memoria fisica
    int pos_mem_fisica = (num_frame * numero_pagina) + offset ;
    mmu->memoria_fisica[pos_mem_fisica]= c;
    mmu->page_table[numero_pagina].write=1; 
}

char MMU_readByte(MMU *mmu, int pos){
    int numero_pagina= pos/pag_size;
    int offset = pos % pag_size;    //formule del prof per capire pagina e offset 
    
    //controlliamo la pagina
    if (mmu->page_table[numero_pagina].valid == 0){
        printf("PAGE FAULT IN POSIZIONE: %d\n", pos);
        MMU_exception(mmu, pos); //chiamo la funzione per le eccezzioni
    }
    
    int num_frame = mmu->page_table[numero_pagina].num_frame;
    mmu->page_table[numero_pagina].read = 1;
    int pos_mem_fisica = (num_frame * numero_pagina) + offset;
    char numero= mmu-> memoria_fisica[pos_mem_fisica];
    
    return numero; }
    
    
void MMU_exception(MMU* mmu, int pos){

}
    
    
    
    
    
    
    
    
    

