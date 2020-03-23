#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <gl/gl.h>
#include <dmedia/vl.h>

const int BYTES_PER_PIXEL = 4;

char *_progName;
int exit_val;

/* Report errors */
void
error_exit(void)
{
    vlPerror(_progName);
    exit(1);
}

void
main(int argc, char **argv)
{
    VLServer svr;
    VLPath path;
    VLNode src, drn;
    VLControlValue val;
    VLBuffer buffer;
    VLInfoPtr info;
    char *dataPtr;
    FILE * outFile;
    int c;
    int xsize;
    int ysize;
    int pixelCount;
    int frameSizeBytes;
    
    _progName = argv[0];

    /* Connect to the daemon */
    if (!(svr = vlOpenVideo(""))) 
	error_exit();

    /* Set up a drain node in memory */
    drn = vlGetNode(svr, VL_DRN, VL_MEM, VL_ANY);
    
    /* Set up a source node on any video source  */
    src = vlGetNode(svr, VL_SRC, VL_VIDEO, VL_ANY);

    /* Create a path using the first device that will support it */
    path = vlCreatePath(svr, VL_ANY, src, drn); 

    /* Set up the hardware for and define the usage of the path */
    if ((vlSetupPaths(svr, (VLPathList)&path, 1, VL_SHARE, VL_SHARE)) < 0)
	error_exit();

    /* Set the packing to RGB */
    val.intVal = VL_PACKING_RGB_8;
    vlSetControl(svr, path, drn, VL_PACKING, &val);
    
    /* Get the video size */
    vlGetControl(svr, path, drn, VL_SIZE, &val);
    xsize = val.xyVal.x;
    ysize = val.xyVal.y;
    pixelCount = xsize * ysize;
    frameSizeBytes = pixelCount * BYTES_PER_PIXEL;
    
    /* Create and register a buffer for 1 frame */
    buffer = vlCreateBuffer(svr, path, drn, 1);
    if (buffer == NULL)
	error_exit();	
    vlRegisterBuffer(svr, path, drn, buffer);
    
    /* Begin the data transfer */
    if (vlBeginTransfer(svr, path, 0, NULL))
	error_exit();
    
    outFile = fopen("out.seed", "w");
    if (outFile) {
        ulong *data;
        do {
    	    sginap(1);		/* wait a tick */
	    info = vlGetNextValid(svr, buffer);
        } while (!info);
      
	/* Get a pointer to the frame */
	dataPtr = vlGetActiveRegion(svr, buffer, info);

        data = (ulong *)dataPtr;
        /* data is ALPHA RED GREEN BLUE (check rgb order) 
            bitwise and with 0xFFFFFF for RGB only         */   

         for(c = 0; c < 5; c++){
             printf("%d: %lx\n", c, 0xFFFFFF & data[c]);
         }
         for(c = 1; c < frameSizeBytes; c+= BYTES_PER_PIXEL){
             fwrite(dataPtr+c, 3, 1, outFile);
         }
    
        /* Finished with frame, unlock the buffer */
    	vlPutFree(svr, buffer);

        printf("xsize: %d\n", xsize);
        printf("ysize: %d\n", ysize);
        printf(" size: %d\n", xsize * ysize);

        fclose(outFile);
    } else {
        fprintf(stderr, "can't open file: seed (%d)\n", errno );
        perror(_progName);
        exit_val = errno;
    }

    /* End the data transfer */
    vlEndTransfer(svr, path);
     
    /* Cleanup before exiting */
    vlDeregisterBuffer(svr, path, drn, buffer);
    vlDestroyBuffer(svr, buffer);
    vlDestroyPath(svr, path);
    vlCloseVideo(svr);

    exit(exit_val);
}
