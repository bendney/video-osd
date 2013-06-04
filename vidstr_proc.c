/* 
 * Copyright (c) 2013, Bendney Yu		bendney@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/*
 *  ==================== videostr_overlay.c =======================
 *  video string overlay algorithm.
 *
 *  This file contains an implementation of the IIMGENC1 interface
 *  defined by XDM.
 */

#include <stdio.h>

#include "vidstr_proc.h"
#include "font_10x18.h"


/* TODO, need to autogenerate this */
#define VERSIONSTRING "1.00.00.00"

/* buffer definitions */
#define MININBUFS       1
#define MINOUTBUFS      1
#define MININBUFSIZE    1
#define MINOUTBUFSIZE   1


/* font data for video algorithms used */
static unsigned char *fontdata = (unsigned char *)fontdata_10x18;


#if 0
/*
 *  ======== VIDEOSTRING_TI_alloc ========
 *  Return a table of memory descriptors that describe the memory needed
 *  to construct our object.
 */
XDAS_Int32 VIDEOSTRING_TI_alloc(const IALG_Params *algParams, IALG_Fxns **pf,
		IALG_MemRec memTab[])
{
    /* Request memory for my object */
    memTab[0].size = sizeof(IMGENC1COPY_TI_Obj);
    memTab[0].alignment = 0;
    memTab[0].space = IALG_EXTERNAL;
    memTab[0].attrs = IALG_PERSIST;

    /* Return the number of records in the memTab */
    return (1);
}


/*
 *  ======== VIDEOSTRING_TI_free ========
 *  Return a table of memory pointers that should be freed.  Note
 *  that this should include *all* memory requested in the
 *  alloc operation above.
 */
XDAS_Int32 VIDEOSTRING_TI_free(IALG_Handle handle, IALG_MemRec memTab[])
{
    /*
     * Because our internal object size doesn't depend on create
     * params, we can just leverage the algAlloc() call with default
     * (NULL) create params.
     */
    return(IMGENC1COPY_TI_alloc(NULL, NULL, memTab));
}


/*
 *  ======== VIDEOSTRING_TI_initObj ========
 *  Initialize the memory allocated on our behalf (including our object).
 */
XDAS_Int32 VIDEOSTRING_TI_initObj(VIDEOSTRING_Handle h, const IALG_MemRec memTab[],
    IALG_Handle p, const IALG_Params *algParams)
{
    const IIMGENC1_Params *params = (IIMGENC1_Params *)algParams;

    /*
     * In lieu of XDM defined default params, use our codec-specific ones.
     * Note that these default values _should_ be documented in your algorithm
     * documentation so users know what to expect.
     */
    if (params == NULL) {
        params = &IMGENC1COPY_TI_PARAMS;
    }

    return (IALG_EOK);
}
#endif

/*
 *  ======== VIDEOSTRING_TI_process ========
 */
XDAS_Int32 VIDEOSTRING_TI_process(VIDEOSTRING_Handle h, VIDEO_BufDesc *inBufs,
    VIDEO_BufDesc *outBufs, STRING_InArgs *inArgs, STRING_OutArgs *outArgs)
{

	XDAS_Int32 matrixCol;
	XDAS_Int32 matrixRow;
	XDAS_Int32 pixelOffset;
	XDAS_Int32 pixelStart;

	/* font matrix size descriptions */
    XDAS_Int32 matrixSize;
	XDAS_Int32 widthMatrix;
	XDAS_Int32 heightMatrix;

	XDAS_UInt8 *charHeader;
	XDAS_Int32 indexCharaters = 0;

	widthMatrix = FONTWIDTH;
	heightMatrix = FONTHEIGHT;

	outArgs->endCol = inArgs->startCol;
	outArgs->endRow = inArgs->startRow;

	/* validate arguments - this algorithm depends on frame region */
	if (inArgs->startRow > inBufs->frameHeight - heightMatrix) {
		printf("VIDEOSTRING_TI_process: parameters invalidate!");
		return -1;
	}
	/* validate arguments - this algorithm depends on frame region */
    if (inArgs->startCol > inBufs->frameWidth - (inArgs->size * widthMatrix)) {
		printf("VIDEOSTRING_TI_process: parameters invalidate!");
		return -1;
    }
	/* Matrix size, the widthMatrix is bigger then 8 */
	matrixSize = heightMatrix * (1 + widthMatrix / 8);

	while ('\0' != inArgs->string[indexCharaters]) {
		/* Pixel start point index location */
		pixelStart = outArgs->endRow * inBufs->frameWidth + outArgs->endCol;
		/* Find out the charater's offset from font matrix */
		charHeader = fontdata + inArgs->string[indexCharaters] * matrixSize;

		for (matrixRow = 0; matrixRow < heightMatrix; matrixRow++) {
			/* Index matrix status for first 8 pixels */
			for (matrixCol = 0; matrixCol < (widthMatrix / 8) * 8; matrixCol++){
				if (charHeader[matrixRow * 2] & (0x80 >> matrixCol)) {
					/* Calculate pixels to show */
					pixelOffset = matrixRow * inBufs->frameWidth +
						matrixCol + pixelStart;
					//inBufs->buffer[pixelOffset * 2]= uComponent;
					//inBufs->buffer[pixelOffset * 2 + 1]  = yComponent;
					inBufs->buffer[pixelOffset * 2]= 128;
					inBufs->buffer[pixelOffset * 2 + 1]  = 16;
				}
			}
			/* Index matrix status for remain pixels */
			for (matrixCol = 0; matrixCol < widthMatrix % 8; matrixCol++) {
				if (charHeader[matrixRow * 2 + 1] & (0x80 >> matrixCol)) {
					/* Calculate pixels to show */
					pixelOffset = matrixRow * inBufs->frameWidth +
						(matrixCol + 8)  + pixelStart;
					inBufs->buffer[pixelOffset * 2] = 16;
					inBufs->buffer[pixelOffset * 2 + 1]  = 128;
				}
			}
		}
		indexCharaters++;
		outArgs->endCol += widthMatrix;
	}

	return 0;
}

#if 0
/*
 *  ======== VIDEOSTRING_TI_control ========
 */
/* ARGSUSED - this line tells the TI compiler not to warn about unused args. */
XDAS_Int32 VIDEOSTRING_TI_control(IIMGENC1_Handle handle, IIMGENC1_Cmd id,
    IIMGENC1_DynamicParams *dynParams, IIMGENC1_Status *status)
{
    XDAS_Int32 retVal;

    /* validate arguments - this codec only supports "base" XDM. */
    if ((dynParams->size != sizeof(*dynParams)) ||
            (status->size != sizeof(*status))) {
        return (IIMGENC1_EUNSUPPORTED);
    }

    /* initialize for the general case where we don't access the data buffer */
    XDM_CLEARACCESSMODE_READ(status->data.accessMask);
    XDM_CLEARACCESSMODE_WRITE(status->data.accessMask);

    switch (id) {
        case XDM_GETSTATUS:
        case XDM_GETBUFINFO:
            status->extendedError = 0;
            status->totalAU = 0;  /* TODO */

            status->bufInfo.minNumInBufs = MININBUFS;
            status->bufInfo.minNumOutBufs = MINOUTBUFS;
            status->bufInfo.minInBufSize[0] = MININBUFSIZE;
            status->bufInfo.minOutBufSize[0] = MINOUTBUFSIZE;

            retVal = IIMGENC1_EOK;
            break;

        case XDM_SETPARAMS:
            retVal = IIMGENC1_EOK;
            break;

        case XDM_SETDEFAULT:
            retVal = IIMGENC1_EOK;
            break;

        case XDM_RESET:
        case XDM_FLUSH:
            retVal = IIMGENC1_EOK;
            break;

        case XDM_GETVERSION:
            if ((status->data.buf != NULL) &&
                (status->data.bufSize > strlen(VERSIONSTRING))) {
                strncpy((char *)status->data.buf, VERSIONSTRING,
                        strlen(VERSIONSTRING) + 1);

                /* strncpy wrote to the data buffer */
                XDM_SETACCESSMODE_WRITE(status->data.accessMask);

                retVal = IIMGENC1_EOK;
            }
            else {
                retVal = IIMGENC1_EFAIL;
            }

            break;

        default:
            /* unsupported cmd */
            retVal = IIMGENC1_EFAIL;

            break;
    }

    return (retVal);
}
#endif

/*
 *  @(#) ti.xdais.dm.examples.imgenc1_copy; 2, 0, 0,1; 6-19-2012 17:57:58; /db/wtree/library/trees/dais/dais-w06/src/ xlibrary
 */

