#include "define.h"

//QUI DEVO FARE UN MMU E LE FUNZIONI 

//WRITEBYTE ----------------------------------------------------------
void MMU_writebyte(MMU* mmu , int pos , char c){
    int numero_pagina= pos/pag_size;
    int offset = pos % pag_size;    //formule del prof per capire pagina e offset
    printf("SCRIVIAMO %d IN POSIZIONE: %d\n", c, pos);
    
	
	//controlliamo la pagina
    if (!mmu->page_table[numero_pagina].valid){
        printf("PAGE FAULT IN POSIZIONE: %d\n", pos);
        MMU_exception(mmu, pos); //chiamo la funzione per le eccezzioni
    }else
    {
        printf("HIT IN POSIZIONE: %d\n", pos);
        printf("NUMERO PAGINA: %d\n", numero_pagina);
    }
    
    //ora possiamo scrivere
    int num_frame = mmu->page_table[numero_pagina].num_frame;
    //portiamolo in memoria_buffer2  memoria_buffer2 
    int pos_mem_fisica = (num_frame * numero_pagina) + offset ;
    mmu->memoria_fisica [pos_mem_fisica]= c;
    mmu->page_table[numero_pagina].write=1; 
}
//------------------------------------------------------------

//WRITEBYTE----------------------------------------------------------------
char MMU_readByte(MMU *mmu, int pos){
    int numero_pagina= pos/pag_size;
    int offset = pos % pag_size;    //formule del prof per capire pagina e offset 
    printf("LEGGO IN POSIZIONE: %d\n\n", pos);
    //controlliamo la pagina
    if (!mmu->page_table[numero_pagina].valid){
        printf("PAGE FAULT IN POSIZIONE: %d\n", pos);
        MMU_exception(mmu, pos); //chiamo la funzione per le eccezzioni
    }
    
    int num_frame = mmu->page_table[numero_pagina].num_frame;
    mmu->page_table[numero_pagina].read = 1;
    int pos_mem_fisica = (num_frame * numero_pagina) + offset;
    char carattere =  mmu->memoria_fisica[pos_mem_fisica];
    
    return carattere; }
//-----------------------------------------------------------------------------





//MMU_EXCEPTION------------------------------------------------------    

void MMU_exception(MMU *mmu, int pos)
{
    printf("CHIAMATO MMU_EXCEPTION IN POSIZIONE: %d\n", pos);

    int page_number = pos / pag_size;
    int empty_frame = -1;

    // Check for empty space in physical memory
    if (mmu->free_frames_top > 0)
    {
        empty_frame = mmu->free_mem[--mmu->free_frames_top];
        printf("EMPTY FRAME TROVATO : %d\n\n", empty_frame);
    }

    // If no empty frame, use Second Chance algorithm
    if (empty_frame == -1)
    {
        printf("SECOND CHANCE ALGORITHM\n");
        int found = 0;
        int iterations = 0;
        while (!found)
        {
            iterations++;
            int oldest_page = -1;
            for (int j = 0; j < NUM_PAGES; ++j)
            {
                if (mmu->page_table[j].num_frame == mmu->oldest_frame_index)
                {
                    oldest_page = j;
                    break;
                }
            }

            int conditions[][2] = {
                {0, 0},  // condition: read_bit = 0, write_bit = 0
                {1, 0},  // condition: read_bit = 1, write_bit = 0
                {0, 1},  // condition: read_bit = 0, write_bit = 1
                {1, 1}   // condition: read_bit = 1, write_bit = 1
            };
            // Check read and write bits for each condition: 00, 10, 01, 11
            for (int i = 0; i < 4; ++i)
            {
                int read_bit = conditions[i][0];
                int write_bit = conditions[i][1];
            
                // Skip unswappable pages or pages not matching the condition
                if (mmu->page_table[oldest_page].swapp ||
                    mmu->page_table[oldest_page].read != read_bit ||
                    mmu->page_table[oldest_page].write != write_bit)
                {
                    mmu->oldest_frame_index = (mmu->oldest_frame_index + 1) % NUM_FRAMES;
                    continue;
                }

                // Replace this page
                printf("SOSTITUISCO LA PAGINA: %d\n", oldest_page);
                empty_frame = mmu->oldest_frame_index;
                found = 1;
                break;
            }

            if (found)
                break;

            // Give a second chance to all pages
            for (int j = 0; j < NUM_PAGES; ++j)
            {
                mmu->page_table[j].read = 0;
                mmu->page_table[j].write = 0;
            }
        }

        printf("ITERAZIONI FATTE: %d\n\n", iterations);
    }

    // Update page table and swap in/out as necessary
    //  If replacing an old page, write it back to the swap file
    if (empty_frame != -1)
    {
        for (int j = 0; j < NUM_PAGES; ++j)
        {
            if (mmu->page_table[j].num_frame == empty_frame)
            {
                // Only write back to disk if the page has been modified
                if (mmu->page_table[j].write)
                {
                    fseek(mmu->swap_file, j * pag_size, SEEK_SET);
                    fwrite(&mmu->memoria_fisica[empty_frame * pag_size], 1, pag_size, mmu->swap_file);
                }
                mmu->page_table[j].valid = 0; // Mark old page as invalid
                mmu->free_mem[mmu->free_frames_top++] = empty_frame;
                break;
            }
        }
    }

    // Read the new page into the frame
    fseek(mmu->swap_file, page_number * pag_size, SEEK_SET);
    fread(&mmu->memoria_fisica[empty_frame * pag_size], 1, pag_size, mmu->swap_file);

    // Update the page table
    mmu->page_table[page_number].valid = 1;
    mmu->page_table[page_number].num_frame = empty_frame;

    // Reset read and write bits
    mmu->page_table[page_number].read = 0;
    mmu->page_table[page_number].write = 0;
}

//----------------------------------------------------------------
    
    


    
    
    
    

