#include<stdio.h>
#include<stdlib.h>
//large virtual memory space (16 MB)
//Use a 1 MB buffer as phyisical memory
#define memoria_virtuale  16000000 

#define memoria_buffer  8000000 

//#define memoria_fisica    8000000 
#define pag_size  4096
#define NUM_PAGES (memoria_virtuale / pag_size)
#define NUM_FRAMES (memoria_buffer / pag_size)

//THE PAGE TABLE
/*- valid
   - unswappable
   - read_bit (set by the mmu each time that page is read)
   - write_bit (set by the mmu each time a page is written)
*/
typedef struct{
 int valid:1;
 int read:1;
 int write:1;
 int swapp:1;
 int num_frame;
}Page_Table;


/*
typedef struct MMU {
  SegmentDescriptor* segments;
  uint32_t num_segments; // number of good segments
  PageEntry *pages;
  uint32_t num_pages;
} MMU;
*/

typedef struct{
    Page_Table page_table[NUM_PAGES];
    char *memoria_fisica;
    FILE *swap_file;
    int oldest_frame_index;
    int free_mem[NUM_FRAMES]; // Array di punti memoria liberi
    int free_mem_top;         // Indice della mem piu alto
}MMU;



// INIZIALIZZO LE FUNZIONI
/* MMU_writeByte(MMU* mmu, int pos, char c) 
   char* MMU_readByte(MMU* mmu, int pos );
    MMU_exception(MMU* mmu, int pos)
*/
void MMU_writebyte(MMU* mmu, int pos, char c);
char* MMU_readbyte(MMU* mmu, int pos);
void MMU_exception(MMU* mmu, int pos);


