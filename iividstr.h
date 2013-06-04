/* 
 * Copyright (c) 2012, Texas Instruments Incorporated
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

/**
 *  @file       ti/xdais/dm/iimgenc.h
 *
 *  @brief      This header defines all types, constants, and functions
 *              shared by all implementations of the image encoder
 *              algorithms.
 */
/**
 *  @defgroup   ti_xdais_dm_IIMGENC   IIMGENC - XDM Image Encoder Interface
 *
 *  This is the XDM IIMGENC Image Encoder Interface.
 *
 *  @deprecated This XDM image encoder interface has been deprecated, and
 *              superceeded by newer IIMGENCx image encoder interfaces.
 */

#ifndef ti_xdais_dm_IIMGENC_
#define ti_xdais_dm_IIMGENC_

//#include "std.h"

typedef int XDAS_Int32;
typedef unsigned char XDAS_UInt8;

/**
 *  @brief      This must be the first field of all VIDEOSTRING	
 *              instance objects.
 */
typedef struct VIDEOSTRING_Obj {
    struct VIDEOSTRING_Fxns *fxns;
} VIDEOSTRING_Obj;


/**
 *  @brief      Opaque handle to an VIDEOSTRING objects.
 */
typedef struct VIDEOSTRING_Obj  *VIDEOSTRING_Handle;


/**
 *  @brief      Defines the creation time parameters for
 *              all VIDEOSTRING instance objects.
 *
 *  @remarks    The application should set the parameters to 0 to use
 *              the algorithm's default values.
 *
 *  @extensibleStruct
 */
typedef struct VIDEOSTRING_Params {
    XDAS_Int32 size;            /**< @sizeField */
    XDAS_Int32 maxHeight;       /**< Maximum height. */
    XDAS_Int32 maxWidth;        /**< Maximum width. */
    XDAS_Int32 maxScans;        /**< Maximum number of scans. */
    XDAS_Int32 dataEndianness;  /**< Endianness of output data.
                                 *
                                 *   @sa    XDM_DataFormat
                                 */
    XDAS_Int32 forceChromaFormat;/**< Force encoding in given Chroma format.
                                 *
                                 *   @sa XDM_ChromaFormat
                                 */
} VIDEOSTRING_Params;


/**
 *  @brief      This structure defines the algorithm parameters that can be
 *              modified after creation via control() calls.
 *
 *  @remarks    It is not necessary that given implementation support all
 *              dynamic parameters to be configurable at run time.  If a
 *              particular algorithm does not support run-time updates to
 *              a parameter that the application is attempting to change
 *              at runtime, it may indicate this as an error.
 *
 *  @extensibleStruct
 *
 *  @sa         IIMGENC_Fxns::control()
 */
typedef struct VIDEOSTRING_DynamicParams {
    XDAS_Int32 size;            /**< @sizeField */
    XDAS_Int32 numAU;           /**< Number of Access unit to encode.
                                 *   Set to #XDM_DEFAULT to encode the
                                 *   entire frame.
                                 */
    XDAS_Int32 inputChromaFormat;/**< Input chroma format.
                                 *
                                 *    @sa   XDM_ChromaFormat
                                 */
    XDAS_Int32 inputHeight;     /**< Input frame height. */
    XDAS_Int32 inputWidth;      /**< Input frame width. */
    XDAS_Int32 captureWidth;    /**< Pitch.  If set to zero, imageWidth is
                                 *  used; else, this field (assuming it's
                                 *  larger than imageWidth) is used.
                                 */
    XDAS_Int32 generateHeader;  /**< Header generation mode.
                                 *
                                 *   @sa    XDM_EncMode
                                 */
    XDAS_Int32 qValue;          /**< Quality factor for encoder.
                                 *   Set to a value between 0 (lowest quality)
                                 *   and 100 (highest quality).
                                 */
} VIDEOSTRING_DynamicParams;


/**
 *  @brief      Defines the video buffer arguments for all VIDEOSTRING instance
 *              process() function.
 *
 *  @extensibleStruct
 *
 *  @sa         VIDEOSTRING_Fxns::process()
 */
typedef struct VIDEO_BufDesc {
    XDAS_Int32 size;            /* Video buffer size Field */
	XDAS_UInt8 *buffer;
	XDAS_Int32 chromaFormat;	/* Input chroma format */
	XDAS_Int32 frameHeight;		/* Input frame height */
	XDAS_Int32 frameWidth;		/* Input frame width */
} VIDEO_BufDesc;



/**
 *  @brief      Defines the input arguments for all VIDEOSTRING instance
 *              process() function.
 *
 *  @extensibleStruct
 *
 *  @sa         VIDEOSTRING_Fxns::process()
 */
typedef struct STRING_InArgs {
	XDAS_UInt8 *string;
    XDAS_Int32 size;            /* String size Field */
	XDAS_Int32 startCol;		/* String start col from X */
	XDAS_Int32 startRow;		/* String start row from Y */
} STRING_InArgs;


/**
 *  @brief      Defines the input argument for all VIDEOSTRING instance
 *				process() function
 *
 *  @extensibleStruct
 *
 *  @sa         VIDEOSTRING_Fxns::process()
 */
typedef struct STRING_OutArgs {
	XDAS_UInt8 *string;
    XDAS_Int32 size;            /* String size Field */
	XDAS_Int32 endCol;		/* String end col from 0 pixel */
	XDAS_Int32 endRow;		/* String end row from 0 pixel */
} STRING_OutArgs;


/**
 *  @brief      Defines instance status parameters (read-only).

 *  @extensibleStruct
 *
 *  @sa         IIMGENC_Fxns::control()
 */
typedef struct VIDEOSTRING_Status {
    XDAS_Int32 size;            /**< @sizeField */
    XDAS_Int32 extendedError;   /**< @extendedErrorField */
    XDAS_Int32 totalAU;         /**< Total number of Access Units. */
} VIDEOSTRING_Status;


/**
 *  @brief      Defines the control commands for the IIMGENC module.
 *
 *  @remarks    This ID can be extended in IMOD interface for
 *              additional controls.
 *
 *  @sa         XDM_CmdId
 *
 *  @sa         IIMGENC_Fxns::control()
 */
//typedef  IALG_Cmd VIDEOSTRING_Cmd;


/**
 *  @brief      Defines all of the operations on IIMGENC objects.
 */
typedef struct VIDEOSTRING_Fxns {
   // IALG_Fxns   ialg;
	/**< XDAIS algorithm interface.
                                   *
                                   *   @sa      IALG_Fxns
                                   */

/**
 *  @brief      Basic image encoding call.
 *
 *  @param[in]  handle          Handle to an algorithm instance.
 *  @param[in]  inBufs          Input buffer descriptors.
 *  @param[in,out] outBufs      Output buffer descriptors.  The algorithm
 *                              may modify the output buffer pointers.
 *  @param[in]  inArgs          Input arguments.  This is a required
 *                              parameter.
 *  @param[out] outArgs         Ouput results.  This is a required parameter.
 *
 *  @remarks    process() is a blocking call.  When process() returns, the
 *              algorithm's processing is complete.
 *
 *  @pre        @c handle must be a valid algorithm instance handle.
 *
 *  @pre        @c inArgs must not be NULL, and must point to a valid
 *              IIMGENC_InArgs structure.
 *
 *  @pre        @c outArgs must not be NULL, and must point to a valid
 *              IIMGENC_OutArgs structure.
 *
 *  @pre        @c inBufs must not be NULL, and must point to a valid
 *              XDM_BufDesc structure.
 *
 *  @pre        @c outBufs must not be NULL, and must point to a valid
 *              XDM_BufDesc structure.
 *
 *  @pre        The buffers in @c inBufs and @c outBufs are physically
 *              contiguous and owned by the calling application.
 *
 *  @post       The algorithm <b>must not</b> modify the contents of @c inArgs.
 *
 *  @post       The buffers in @c inBufs and @c outBufs are
 *              owned by the calling application.
 *
 *  @retval     IALG_EOK        Success.
 *  @retval     IALG_EFAIL      Failure.  See IIMGENC_OutArgs#extendedError
 *                              for more detailed further error conditions.
 */
    XDAS_Int32  (*process)(VIDEOSTRING_Handle handle, VIDEO_BufDesc *inBufs,
        VIDEO_BufDesc *outBufs, STRING_InArgs *inArgs, STRING_OutArgs *outArgs);

/**
 *  @brief      Control behaviour of an algorithm.
 *
 *  @param[in]  handle          Handle to an algorithm instance.
 *  @param[in]  id              Command id.  See #XDM_CmdId.
 *  @param[in]  params          Dynamic parameters.  This is a required
 *                              parameter.
 *  @param[out] status          Output results.  This is a required parameter.
 *
 *  @pre        @c handle must be a valid algorithm instance handle.
 *
 *  @pre        @c params must not be NULL, and must point to a valid
 *              IIMGENC_DynamicParams structure.
 *
 *  @pre        @c status must not be NULL, and must point to a valid
 *              IIMGENC_Status structure.
 *
 *  @retval     IALG_EOK        Success.
 *  @retval     IALG_EFAIL      Failure.  See IIMGENC_Status#extendedError
 *                              for more detailed further error conditions.
 */
	/*
    XDAS_Int32  (*control)(VIDEO_STRING_Handle handle, VIDEOSTRING_Cmd id,
        VIDEOSTRING_DynamicParams *params, VIDEOSTRING_Status *status);
		*/

} VIDEOSTRING_Fxns;

#endif /* #ifndef ti_xdais_dm_IIMGENC_ */
/*
 *  @(#) ti.xdais.dm; 1, 0, 7,1; 6-19-2012 17:57:45; /db/wtree/library/trees/dais/dais-w06/src/ xlibrary

 */

