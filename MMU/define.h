#include<stdio.h>
#include<stdlib.h>

#define MEMORIA_BUFFER 1048576
#define memoria_virtuale 16777216 
#define pag_size  4096
#define NUM_PAGES (memoria_virtuale / pag_size)
#define NUM_FRAMES (MEMORIA_BUFFER / pag_size)

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
    Page_Table page_table[NUM_PAGES]; //TABELLA DELLE PAGINE
    char *memoria_fisica ; //PUNTATORE AL BUFFER
    FILE *swap_file; // FILE
    int oldest_frame_index; // INDICE DEL FRAME VECCHIO
    int free_mem[NUM_FRAMES]; // Array di punti memoria_buffer2  liberi
    int free_frames_top;         //  LIBERA
}MMU;



// INIZIALIZZO LE FUNZIONI
/* MMU_writeByte(MMU* mmu, int pos, char c) 
   char* MMU_readByte(MMU* mmu, int pos );
    MMU_exception(MMU* mmu, int pos)
*/
char MMU_readByte(MMU* mmu, int pos);
void MMU_writebyte(MMU* mmu, int pos, char c);
void MMU_exception(MMU* mmu, int pos);


