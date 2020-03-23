#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <gl/gl.h>
#include <dmedia/vl.h>

const int BYTES_PER_PIXEL = 3;
const int FRAME_COUNT = 1;

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
    char *dataBuffer;
    char *dataBufferEnd;
    FILE *outFile;
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

    /* Create buffer for the RGB frame to work from */
    dataBuffer = malloc(frameSizeBytes);
    if(!dataBuffer){
        fprintf(stderr, "%s: Couldn't malloc %d bytes\n", _progName, frameSizeBytes);
        exit(1);
    }
    dataBufferEnd = dataBuffer + frameSizeBytes - 1;
    
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
        for(c = 0; c < FRAME_COUNT; c++){
            char *dst = dataBuffer;

            do {
    	        sginap(1);		/* wait a tick */
	        info = vlGetNextValid(svr, buffer);
            } while (!info);
      
	    /* Get a pointer to the frame */
	    dataPtr = vlGetActiveRegion(svr, buffer, info);

            /* Skip alpha channel */
            dataPtr++;

            /* In memory copy of RGB data */
            for(; dst < dataBufferEnd; dst += BYTES_PER_PIXEL){
                memcpy(dst, dataPtr, 3);
                dataPtr += 4;
            }
    
            /* Finished with frame, unlock the buffer */
    	    vlPutFree(svr, buffer);

            /* Write data to file */
            fwrite(dataBuffer, xsize * BYTES_PER_PIXEL, ysize, outFile);
        }

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
