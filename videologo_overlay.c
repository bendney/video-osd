#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "videologo_overlay.h"

int osdLogo_create(LOGO_Object *osd)
{
	int logo_size;
	FILE *logo_file = NULL;

	/*  UYVY format recode pixel using two bytes */
	logo_size = osd->width * osd->height * 2;

	/* malloc a space for logo file read buffer */
	osd->buffer = (unsigned char *)malloc(logo_size);
	if (NULL == osd->buffer) {
		printf("malloc logo_size failed!\n");
		return -1;
	}

	logo_file = fopen(osd->logopath, "rb");
	if (NULL == logo_file) {
		printf("osd logo file detect failed!\n");
		return -1;
	}

	fread(osd->buffer, sizeof(unsigned char), logo_size, logo_file);

	if (logo_file != NULL) {
		fclose(logo_file);
	}

	return 0;
}

int osdLogo_delete(LOGO_Object *osd)
{

	if (NULL != osd->buffer) {
		free(osd->buffer);
	}
	osd->buffer = NULL;

	return 0;
}


/* the logo must be UYVY format, the same with video format */
int osdLogo_process(LOGO_Object *osd, unsigned char *frame_buffer)
{
	int i;
	int j;
	int dst_pitch;
	int src_pitch;
	unsigned char *dstaddr = NULL;
	unsigned char *srcaddr = NULL;

	src_pitch = osd->width << 1;
	dst_pitch = 640 << 1;

	srcaddr = osd->buffer;
	dstaddr = frame_buffer + (osd->show_y * 640 + osd->show_x) * 2;	

	if (!srcaddr || !dstaddr)
		return -1;

	if (osd->flag == 1) {
		for (j = 0; j < osd->height; j++) {
			for (i = 0; i < src_pitch; i++) {
				//*dstaddr = (*dstaddr + *srcaddr) >> 1;
				*dstaddr = 255;
				dstaddr++;
				srcaddr++;
			}

			dstaddr += dst_pitch - src_pitch;
		}
	}

	if (osd->flag == 0) {
		for (j = 0; j < osd->height; j++) {
			for (i = 0; i < src_pitch; i++) {
				if (*srcaddr != 16 && *srcaddr != 128) {
					*dstaddr = *srcaddr;
				}
				dstaddr++;
				srcaddr++;
			}

			dstaddr += dst_pitch - src_pitch;
		}
	}

	return 0;
}

