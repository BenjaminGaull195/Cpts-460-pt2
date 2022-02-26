/*******************************************************
*                  @t.c file                          *
*******************************************************/

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define TRK 18
#define CYL 36
#define BLK 1024

#include "ext2.h"
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

int prints(char *s)
{
   while(*s) {
		putc(*s);
		s++;
  }
}

int gets(char *s)
{ 
   char c;
	while ((c=getc()) != '\r') {
	  *s = c;
	  putc(c);
		s++;	
	 }
  *s = 0;
}


u16 NSEC = 2;
char buf1[BLK], buf2[BLK];

GD    *gp;
INODE *ip;
DIR   *dp;

//int color = 0x0A;
u8 ino;

u16 getblk(u16 blk, char *buf)
{
    readfd( (2*blk)/CYL, ( (2*blk)%CYL)/TRK, ((2*blk)%CYL)%TRK, buf);

    // readfd( blk/18, ((blk)%18)/9, ( ((blk)%18)%9)<<1, buf);
}

u16 search(INODE *ip, char *name)
{
   char c, temp[64];
  //search for name in the data block of INODE; 
  //return its inumber if found
  //else error();
   while(c < buf2 + 1024) {
	  strncpy(temp, dp->name, dp->name_len);
	  temp[dp->name_len] = 0;

      if (strcmp(temp, name) == 0) {
         return (u16)((DIR *)temp)->inode;
      }

	  c += dp->rec_len;
	  dp = (DIR*)c;
   }
}

main()
{ 
   u16 i, iblk;
   char c;
   u32 *up;

   getblk(2, buf1);
   gp = (GD *)buf1;

   iblk = (u16) gp->bg_inode_table;

   getblk(iblk, buf1);
   ip = (INODE*)buf1 + 1;
   iblk = (u16)ip->i_block[0];

   getblk(iblk, buf2);
   dp = (DIR*)buf2;
   //c = buf2;

   ino = search(ip, "boot");
   getblk(ino, buf1);
   ip = (INODE *)buf1;

   ino = search(ip, "mtx");
   getblk(ino, buf1);
   ip = (INODE *)buf1;

//1. Write YOUR C code to get the INODE of /boot/mtx
   //INODE *ip --> INODE

   //if INODE has indirect blocks: get i_block[12] int buf2[  ]


//2. setes(0x1000);  // MTX loading segment = 0x1000
setes(0x1000);

//3. load 12 DIRECT blocks of INODE into memory
   for (i=0; i<12; i++){
      getblk((u16)ip->i_block[i], 0);
      putc('*');
      inces();
   }

//4. load INDIRECT blocks, if any, into memory
   if (ip->i_block[12]){
      getblk((u16)ip->i_block[12], buf2);
      up = (u32 *)buf2;      
      while(*up){
        getblk((u16)*up, 0); putc('.');
        inces();
        up++;
      }
  }
  //prints("go?"); getc();
  return 1;
}  
