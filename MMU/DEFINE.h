//large virtual memory space (16 MB)
//Use a 1 MB buffer as phyisical memory
#define memoria_virtuale (128000000) //???? 
#define memoria_fisica (8000000) //?????

//THE PAGE TABLE
/*- valid
   - unswappable
   - read_bit (set by the mmu each time that page is read)
   - write_bit (set by the mmu each time a page is written)
*/
typedef struct{
 uint32_t valid:1;
 uint32_t read:1;
 uint32_t write:1;
 uint32_t swapp:1;
 int num_frame;
}page_table;


/*
typedef struct MMU {
  SegmentDescriptor* segments;
  uint32_t num_segments; // number of good segments
  PageEntry *pages;
  uint32_t num_pages;
} MMU;
*/
/*typedef struct
{
    PageTableEntry page_table[NUM_PAGES];
    char *physical_memory;
    FILE *swap_file;
    int oldest_frame_index;
    int free_frames[NUM_FRAMES]; // Freelist array
    int free_frames_top;         // Index for the top of the freelist
} MMU;*/
typedef struct{

}MMU;



// INIZIALIZZO LE FUNZIONI
/* MMU_writeByte(MMU* mmu, int pos, char c) 
   char* MMU_readByte(MMU* mmu, int pos );
    MMU_exception(MMU* mmu, int pos)
*/
MMU_writebyte(MMU* mmu, int pos, char c);
char* MMU_readbyte(MMU* mmu, int pos);
MMU_exception(MMU* mmu, int pos);


