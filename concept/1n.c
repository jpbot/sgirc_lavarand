#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER 10485760

int main(int argc, char **argv){
   FILE *f;
   char *buffer;
   int filelen;
   int nstart, nbyte;
   int newlen=0;
   int i;

   if(argc < 4){
      fprintf(stderr, "ERROR: must invoke with FILENAME <START> <SKIP>\n");
      exit(-1);
   }

   nstart = atoi(argv[2]);
   if(nstart < 1){
      fprintf(stderr, "ERROR: Start should be greater than zero.\n");
      exit(-1);
   }

   nbyte = atoi(argv[3]);
   if(nbyte < 1){
      fprintf(stderr, "ERROR: returning less than every one byte doesn't make sense.\n");
      exit(-1);
   }
   if(nbyte < 2){
      fprintf(stderr, "WARN: returning less than every 2nd byte doesn't make sense.\n");
   }

   f = fopen(argv[1], "rb");
   fseek(f, 0, SEEK_END);
   filelen = ftell(f);
   if(filelen > MAXBUFFER){
      fprintf(stderr, "WARNING: File too large (%d), truncating (%d)", filelen, MAXBUFFER);
      filelen = MAXBUFFER;
   }

   if(nstart > filelen){
      fprintf(stderr, "ERROR: Can't start reading past end of file.\n");
      exit(-1);
   }

   buffer = malloc(filelen);

   if(!buffer){
      fprintf(stderr, "ERROR: allocating buffer\n");
      return -1;
   }

   rewind(f);
   fread(buffer, 1, filelen, f);

   for(i = nstart - 1; i < filelen; i += nbyte){
      printf("%c", buffer[i]);
      newlen++;
   }

   fprintf(stderr, "ouput len %d\n", newlen);

   free(buffer);
   fclose(f);

   return 0;
}
      


