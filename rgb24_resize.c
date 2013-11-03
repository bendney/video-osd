void reSize(unsigned char *pSrcImg,unsigned char *pDstImg,int srcWidth,int srcHeight,int dstWidht,int dstHeight)
{
	int i,i1,i2;
	int j,j1;
	unsigned char* pSrc = pSrcImg;
	unsigned char* pDst = pDstImg;
	int dstWidthStep = dstWidht*3;
	int srcWidthStep = srcWidth*3;
	for(j = 0,j1 = 0;j1 < dstHeight*dstWidht*3;j++,j1+= dstWidthStep)
	{
		pDst = pDstImg+j1;
		pSrc = pSrcImg+j*srcHeight/dstHeight*srcWidthStep;
		for(i = 0,i1 = 0,i2 = 0;i<dstWidht;i++,i1+=3,i2 = i*srcWidth/dstWidht*3)
		{
			memcpy(pDst+i1,pSrc+i2,3);
		}

	}
}

//size = width*height
void rgb888_to_bgr565(unsigned char *pSrcImg,unsigned char *pDstImg,int size)
{
	int i;
	unsigned char* pSrc = pSrcImg;
	unsigned char* pDst = pDstImg;
	for(i=0;i<size;i++,pSrc+=3,pDst+=2)
	{
		*pDst = (*(pSrc+1)&0x1C)<<3|(*pSrc&0xF8)>>3;
		*(pDst+1) = *(pSrc+2)&0xF8|(*(pSrc+1)&0xE0)>>5;

	}
}
