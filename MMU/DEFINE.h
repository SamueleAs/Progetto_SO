//large virtual memory space (16 MB)
//Use a 1 MB buffer as phyisical memory
#define memoria_virtuale 128000000 //???? 
#define memoria fisica 8000000 //?????

//THE PAGE TABLE
/*- valid
   - unswappable
   - read_bit (set by the mmu each time that page is read)
   - write_bit (set by the mmu each time a page is written)
*/




// INIZIALIZZO LE FUNZIONI
/* MMU_writeByte(MMU* mmu, int pos, char c) 
   char* MMU_readByte(MMU* mmu, int pos );
    MMU_exception(MMU* mmu, int pos)
*/
MMU_writebyte(MMU* mmu, int pos, char c);
char* MMU_readbyte(MMU* mmu, int pos);
MMU_exception(MMU* mmu, int pos);


