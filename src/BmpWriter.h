/*
 *
 *
 */

#ifndef RC_BMPWRITER_H
#define RC_BMPWRITER_H

class BmpWriter {

	public:

		/** Assumes that the pixels are in RGB8 format */
		static void write(
			char		*filename,
			u8			*pixels,
			u32			width,
			u32			height,
			bool		flipped
		);

};

#endif /* RC_BMPWRITER_H */


