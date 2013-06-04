#ifndef _OSD_TEST_H_
#define _OSD_TEST_H_

typedef struct LOGO_Object_tag {
	unsigned char *buffer;
	char *logopath;
	int flag;
	/* logo image width */
	int width;
	/* logo image height */
	int height;
	int show_x;
	int show_y;
} LOGO_Object;


//typedef struct LOGO_Object * LOGO_Handle;

int osdLogo_create(LOGO_Object *osd);

int osdLogo_delete(LOGO_Object *osd);

int osdLogo_process(LOGO_Object *osd, unsigned char *frame_buffer);

#endif /* _OSD_TEST_H_ */
