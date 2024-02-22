#include "define.h"

//QUI DEVO FARE UN MMU E LE FUNZIONI 

//WRITEBYTE ----------------------------------------------------------
void MMU_writebyte(MMU* mmu , int pos , char c){
    int numero_pagina = pos/pag_size;
    int offset = pos % pag_size;    //formule del prof per capire pagina e offset
    //printf("SCRIVIAMO %d IN POSIZIONE: %d\n", c, pos);
    
	
	//controlliamo la pagina
    if (mmu->page_table[numero_pagina].valid == 0){
        printf("PAGE FAULT IN POSIZIONE: %d\n", pos);
        MMU_exception(mmu, pos); //chiamo la funzione per le eccezzioni
    }else
    {
        printf("HIT IN POSIZIONE: %d\n", pos);
        printf("NUMERO PAGINA: %d\n", numero_pagina);
    }
    
    //ora possiamo scrivere
    int num_frame = mmu->num_frame;
    //portiamolo in memoria_fisica
    int pos_mem_fisica = (num_frame * numero_pagina) + offset ; //CALCOLO LA POSIZIONE IN MEMORIA FISICA
    mmu->memoria_fisica[pos_mem_fisica]= c; //SCRIVO IL CHAR NELLA POSIZIONE
    mmu->page_table[numero_pagina].write=1; //AGGIORNO IL BIT DELLA PAGINA
}
//------------------------------------------------------------

//READBYTE----------------------------------------------------------------
char MMU_readByte(MMU *mmu, int pos){
    int numero_pagina= pos/pag_size;
    int offset = pos % pag_size;    //formule del prof per capire pagina e offset 
    printf("LEGGO IN POSIZIONE: %d\n\n", pos);
    
    //controlliamo la pagina
    if (mmu->page_table[numero_pagina].valid==0){
        printf("PAGE FAULT IN POSIZIONE: %d\n", pos);
        MMU_exception(mmu, pos); //chiamo la funzione per le eccezzioni
    }
    
    int num_frame = mmu->num_frame;
    mmu->page_table[numero_pagina].read = 1; //AGGIORNO IL BIT DELLA PAGINA
    int pos_mem_fisica = (num_frame * numero_pagina) + offset; // CALCOLO LA POSIZIONE IN MEMORIA FISICA
    char carattere =  mmu->memoria_fisica[pos_mem_fisica]; // "PRENDO" IL CARATTERE DALLA MEMORIA
    
    return carattere; }
//-----------------------------------------------------------------------------





//MMU_EXCEPTION------------------------------------------------------    

void MMU_exception(MMU *mmu, int pos)
{
    
    int page_number = pos / pag_size;
    int empty_frame = 0;
    printf("CHIAMATO MMU_EXCEPTION IN POSIZIONE: %d , PER LA PAGINA: %d\n", pos, page_number);

    // CHERCHIAMO LO SPAZIO IN MEMORIA LIBERO
    if (mmu->num_frame/*free_frames_top*/ > 0)  //VEDO SE NE HO UNO 
    {
        empty_frame = mmu->free_mem[mmu->free_frames_top];   //SE SI PRENDO L'INDICE DEL PIU "VICINO"
        mmu->free_frames_top = mmu->free_frames_top--;            //DECREMENTO
        printf("EMPTY FRAME TROVATO : %d\n\n", empty_frame);
    }

    // SE NESSUN FRAME LIBERO USO L'ALGORITMO
    if (empty_frame == 0)                         //ALTRIMENTI SWAPPO
    {
        printf("SECOND CHANCE ALGORITHM \n");
        int found = 0;
        int iterations = 0;
        while (found == 0)
        {
            iterations++;
            int oldest_page = -1;
            for (int i = 0; i < NUM_FRAMES; ++i)
            {
                if (mmu->num_frame == mmu->oldest_frame_index)
                {
                    oldest_page = i;
                    break;
                }
            }

            int conditions[][2] = {
                {0, 0},  // R=0 W=0
                {0, 1},  // R=0 W=1
                {1, 0},  // R=1 W=0
                {1, 1}   // R=1 W=1
            };
            // Check read and write bits for each condition: 00, 10, 01, 11
            for (int i = 0; i < 2; ++i)
            {
                
                
                int read_bit = conditions[i][0];
                int write_bit = conditions[i][1];
            
                // SALTO LE PAGINE NON VALIDE
                if (mmu->page_table[oldest_page].swapp == 1||
                    mmu->page_table[oldest_page].read != read_bit ||
                    mmu->page_table[oldest_page].write != write_bit)
                {
                    mmu->oldest_frame_index = (mmu->oldest_frame_index + 1) % NUM_FRAMES;
                    continue;
                }

                // TROVO LA PAGINA DA SOSTITUIRE
                printf("SOSTITUISCO LA PAGINA: %d\n", oldest_page);
                empty_frame = mmu->oldest_frame_index;
                found = 1;
                break;
            }

            if (found) break;

            // RESET TUUTE LE PAGINE DOPO IL SECOND CHANGE ALGORITMO
            for (int i = 0; i < NUM_PAGES; i++)
            {
                mmu->page_table[i].read = 0;
                mmu->page_table[i].write = 0;
            }
        }

        printf("ITERAZIONI FATTE: %d\n\n", iterations);
    } 

    // Update page table and swap in/out as necessary
    //  If replacing an old page, write it back to the swap file
    if (empty_frame != 0) //se trovato
    {
        for (int j = 0; j < NUM_PAGES; ++j)
        {
            if (mmu->num_frame == empty_frame)
            {
                // Only write back to disk if the page has been modified
                if (mmu->page_table[j].write)
                {
                    fseek(mmu->swap_file, j * pag_size, SEEK_SET); //SPOSTIAMO IL PUNATOTRE NEL FILE
                    fwrite(&mmu->memoria_fisica[empty_frame * pag_size], 1, pag_size, mmu->swap_file);
                }
                mmu->page_table[j].valid = 0; // Mark old page as invalid
                mmu->free_mem[mmu->free_frames_top++] = empty_frame;
                break;
            }
        }
    }

    // LEGGIAMO LA NUOVA PAGINA
   fseek(mmu->swap_file, page_number * pag_size, SEEK_SET); //SPOSTIAMO IL PUNTATORE NEL FILE 
    fread(&mmu->memoria_fisica[empty_frame * pag_size], 1, pag_size, mmu->swap_file);

    // AGGIORNIAMO LA TABELLA
    mmu->page_table[page_number].valid = 1;
    mmu->num_frame = empty_frame;

    // RESET I BTI
    mmu->page_table[page_number].read = 0;
    mmu->page_table[page_number].write = 0; 
}

//----------------------------------------------------------------
    
    


    
    
    
    

