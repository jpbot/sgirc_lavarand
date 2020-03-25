/*  TODO: fix awful marriage of shs1 from md5_sha project, likely making a bad library
          with shs1dual....right now there are stubs of MultiMain and MultiTest just to
          build with shs1drvr. Possible solution is pulling variables needed from the
          drvr and reimplementing the multiData that was copied from shs1dual to this
          file.
         
    TODO: command line handling, 1-shot default, n-shot and continuous options.
    
    STATUS: still not performing as quick as I'd like - maybe eliminating some of the
            memory copies will help.
            
            3.72 seconds to run with FRAME_COUNT=10 means ~3000 bits/sec
            
    UPDATE: only 2.26 seconds, 3.72 seconds was with file writing. ~4950 bits/sec
 */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dmedia/vl.h>
#include "sha1/shs1.h"

const int BYTES_PER_ARGB_PIXEL = 4;
const int BYTES_PER_PIXEL = 3;
const int FRAME_COUNT = 10;

char *_progName;

void multiTest(void){ return; }

void
multiMain(int argc, char **argv, BYTE *pre_str, UINT pre_len,
    char *data_str, int nospace, UINT cnt) { return; }
    
    
/* Report errors */
void
error_exit(void)
{
    vlPerror(_progName);
    exit(1);
}


/*
 * multiOutput - output the multiple digests
 *
 * given:
 *	str		print string after digest, NULL => none
 *	quot		1 => surround str with a double quotes
 *	nospace		1 => don't space seperate multi digests
 *	cnt		number of digests
 *	digs		array of digests, cnt elements long
 */
static void
multiOutput(char *str, int quot, int nospace, UINT cnt, SHS1_INFO *digs)
{
    UINT i;

    /*
     * firewall
     */
    if (cnt <= 0) {
	return;
    }

    /*
     * finalize the sets
     */
    for (i=0; i < cnt; ++i) {
	shs1Final(digs+i);
    }

    /*
     * print the digests
     */
    shs1Print(c_flag, 0, digs);
    for (i=1; i < cnt-1; ++i) {
	if (nospace == 0) {
	    putchar(' ');
	    shs1Print(c_flag, 0, digs+i);
	} else {
	    shs1Print(0, 0, digs+i);
	}
    }
    if (i < cnt) {
	if (nospace == 0) {
	    putchar(' ');
	    shs1Print(c_flag, i_flag, digs+cnt-1);
	} else {
	    shs1Print(0, i_flag, digs+cnt-1);
	}
    }
	if (str && !q_flag) {
	if (quot) {
	    printf(" \"%s\"\n", str);
	} else {
	    printf(" %s\n", str);
	}
    } else {
	putchar('\n');
    }

}


/*
 * multiData - divide data into multiple sections
 *
 * given:
 *	pre_str		string prefix or NULL
 *	pre_len		length of pre_str
 *	inString	string to digest
 *	in_len		length of inString
 *	cnt		number of digests
 *	digs		array of digests, cnt elements long
 */
static void
multiData(BYTE *pre_str, int pre_len, BYTE *inString,
    int in_len, UINT cnt, SHS1_INFO *digs)
{
    BYTE **bufs;		/* byte arrays for digests */
    int *buflen;		/* bytes stored in bufs[i] */
    int len;			/* total length of pre_str and inString */
    UINT indx;			/* byte stream index */
    BYTE **n;			/* n[i] is next byte to use in bufs[i] */
    BYTE *p;
    int i;

    /*
     * determine lengths
     */
    len = (pre_str == NULL) ? 0 : pre_len;
    len += (inString == NULL) ? 0 : in_len;
    /* no strings, quick return */
    if (len == 0 || cnt <= 0) {
        return;
    }

    /*
     * malloc string arrays
     */
    bufs = (BYTE **)malloc(sizeof(BYTE *)*cnt);
    if (bufs == NULL) {
	fprintf(stderr, "%s: bad malloc #1\n", program);
	exit(51);
    }
    buflen = (int *)malloc(sizeof(UINT)*cnt);
    if (buflen == NULL) {
	fprintf(stderr, "%s: bad malloc #2\n", program);
	exit(52);
    }
    n = (BYTE **)malloc(sizeof(BYTE *)*cnt);
    if (n == NULL) {
	fprintf(stderr, "%s: bad malloc #3\n", program);
	exit(53);
    }
    for (i=0; i < cnt; ++i) {
	bufs[i] = (BYTE *)malloc(1+(len/cnt));
	if (bufs[i] == NULL) {
	    fprintf(stderr, "%s: bad malloc #4\n", program);
	    exit(54);
	}
	buflen[i] = 0;
	n[i] = bufs[i];
    }

    /*
     * divide the pre-string
     */
    indx = 0;
    if (pre_str != NULL) {
	for (p=pre_str, i=0; i < pre_len; ++i, ++p, indx++) {
	    *(n[(indx = ((indx >= cnt) ? 0 : indx))]++) = *p;
	    ++buflen[indx];
	}
    }

    /*
     * divide the string
     */
    if (inString != NULL) {
	for (p=inString, i=0; i < in_len; ++indx, ++i, ++p) {
	    *(n[(indx = ((indx >= cnt) ? 0 : indx))]++) = *p;
	    ++buflen[indx];
	}
    }

    /*
     * update arrays
     */
    for (i=0; i < cnt; ++i) {
	shs1Update(digs+i, bufs[i], buflen[i]);
    }

    /*
     * cleanup
     */
    free(buflen);
    free(n);
    for (i=0; i < cnt; ++i) {
	free(bufs[i]);
    }
    free(bufs);
}

/*
 * MultiSha1 - main driver of shs1 dual routines
 *
 * given:
 *	pre_str		pre-process this data first
 *	pre_len		length of pre_str
 *	data_str	data is this string, not a file
 *	nospace		1 => don't space seperate multi digests
 *	cnt		number of digests to perform
 */
void
multiSha1(BYTE *pre_str, int pre_len,
    char *data_str, int data_len, int nospace, UINT cnt)
{
    SHS1_INFO *digs;		/* multiple digest */
    unsigned int i;

    /*
     * firewall
     */
    if (cnt <= 0) {
		return;
    }

    /*
     * initialize multiple digests
     */
    digs = (SHS1_INFO *)malloc(sizeof(SHS1_INFO)*cnt);
    if (digs == NULL) {
	fprintf(stderr, "%s: bad malloc #1\n", program);
	exit(60);
    }
    for (i=0; i < cnt; ++i) {
		shs1Init(digs+i);
    }

    /*
     * digest a string
     */

	multiData(pre_str, pre_len, (BYTE*)data_str, data_len,
		cnt, digs);
	multiOutput(NULL, 0, nospace, cnt, digs);
}
//            multiSha1(NULL, 0, dataBuffer, frameSizeBytes, 0, 7);



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
            memcpy(dst, dataPtr, BYTES_PER_PIXEL);
            dataPtr += BYTES_PER_ARGB_PIXEL;
        }
    
        /* Finished with frame, unlock the buffer */
        vlPutFree(svr, buffer);
    
        multiSha1(NULL, 0, dataBuffer, frameSizeBytes, 1, 7);    
    }


    /* End the data transfer */
    vlEndTransfer(svr, path);
     
    /* Cleanup before exiting */
    vlDeregisterBuffer(svr, path, drn, buffer);
    vlDestroyBuffer(svr, buffer);
    vlDestroyPath(svr, path);
    vlCloseVideo(svr);
}
