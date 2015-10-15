#include "RenderChimp.h"
#include "BmpWriter.h"

/*---------------------------------------------------------------------------*/

void write_u8(
		FILE	*f,
		u8		v
	)
{
	fputc(v, f);
}

/*---------------------------------------------------------------------------*/

void write_u16(
		FILE	*f,
		u16		v
	)
{
	fputc((char) (v & 0xFF), f);
	fputc((char) (v >> 8), f);
}

/*---------------------------------------------------------------------------*/

void write_u32(
		FILE	*f,
		u32		v
	)
{
	fputc((char) (v & 0xFF), f);
	fputc((char) ((v >> 8) & 0xFF), f);
	fputc((char) ((v >> 16) & 0xFF), f);
	fputc((char) (v >> 24), f);
}

/*---------------------------------------------------------------------------*/


void BmpWriter::write(
		char		*filename,
		u8			*pixels,
		u32			width,
		u32			height,
		bool		flipped
	)
{
	i32 x, y;
	FILE *f;

	f = fopen(filename, "wb");

	if (!f)
		return;

	/* Header (14 bytes) */
	write_u8(f, 'B');
	write_u8(f, 'M');

	write_u32(f, 14 + 40 + width * height * 3);

	write_u16(f, 0);
	write_u16(f, 0);

	write_u32(f, 54);

	/* DIB header (V3) (40 bytes)*/

	write_u32(f, 40);
	write_u32(f, width);
	write_u32(f, height);
	write_u16(f, 1);
	write_u16(f, 24);
	write_u32(f, 0);
	write_u32(f, width * height * 3);
	write_u32(f, 1);
	write_u32(f, 1);
	write_u32(f, 0);
	write_u32(f, 0);

	/* Bitmap */

	if (flipped) {
		for (y = ((i32) height) - 1; y >= 0; y--) {
			
			for (x = 0; x < ((i32) width); x++) {

				write_u8(f, pixels[(y * width + x) * 3 + 2]);
				write_u8(f, pixels[(y * width + x) * 3 + 1]);
				write_u8(f, pixels[(y * width + x) * 3 + 0]);

			}

		}
	} else {
		for (y = 0; y < ((i32) height); y++) {
			
			for (x = 0; x < ((i32) width); x++) {

				write_u8(f, pixels[(y * width + x) * 3 + 2]);
				write_u8(f, pixels[(y * width + x) * 3 + 1]);
				write_u8(f, pixels[(y * width + x) * 3 + 0]);

			}

		}
	}

	fclose(f);
}



