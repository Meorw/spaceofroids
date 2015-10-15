#include "RenderChimp.h"
#include "ObjParser.h"
#include "SceneParser.h"
#include "ctype.h"

/*---------------------------------------------------------------------------*/

u8 read_u8(
		FILE	*f
	)
{
	return (u8) fgetc(f);
}

/*---------------------------------------------------------------------------*/

u16 read_u16(
		FILE	*f
	)
{
	u16 r;

	r = ((u16) read_u8(f));
	r = r + (((u16) read_u8(f)) << 8);

	return r;
}

/*---------------------------------------------------------------------------*/

u32 read_u32(
		FILE	*f
	)
{
	u32 r;

	r = ((u32) read_u8(f));
	r += ((u32) read_u8(f)) << 8;
	r += ((u32) read_u8(f)) << 16;
	r += ((u32) read_u8(f)) << 24;

	return r;
}

/*---------------------------------------------------------------------------*/

f32 read_f32(
		FILE	*f
	)
{
	f32 v;
	u8 r[4];

	r[0] = read_u8(f);
	r[1] = read_u8(f);
	r[2] = read_u8(f);
	r[3] = read_u8(f);

	v = *((f32 *) r);

	return v;
}

/*---------------------------------------------------------------------------*/

void read_str(
		FILE				*f,
		char				*str
	)
{
	i32 i;

	while ((i = getc(f)) != '\0' && i != EOF) {
		*str = (char) i;
		str++;
	}

	*str = '\0';
}

/*---------------------------------------------------------------------------*/

void read_str(
		FILE				*f,
		char				*str,
		u32					len
	)
{
	u32 n = 0;
	i32 i;

	while (n < len && (i = getc(f)) != '\0' && i != EOF) {
		*str = (char) i;
		str++;
		n++;
	}
}

/*---------------------------------------------------------------------------*/

void ParseScene(
		Node				*target,
		const char			*filename
	)
{
	u32 len, i;
	const char *ext;
	char buf[1024];

	ext = strrchr(filename, '.');

	len = strlen(ext);

	if (len >= 1024)
		REPORT_WARNING("Filename %s too long", filename);

	strcpy(buf, ext + 1);

	for (i = 0; i < len; i++) {
		buf[i] = (char) tolower(buf[i]);
	}

	if (strcmp(buf, "obj") == 0) {
		ObjParse(target, filename);
	} else if (strcmp(buf, "pwn") == 0) {
		/* LOWPRIO: Make this work! */
		//PwnParse(target, filename);
	} else {
		REPORT_WARNING("Unsupported scene format (%s). Use either .obj or .pwn.", buf);
	}

}

/*---------------------------------------------------------------------------*/

