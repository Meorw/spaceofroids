
#ifndef RC_TEXT_H
#define RC_TEXT_H

typedef struct {

	vec2f		vertex;
	vec2f		texcoord;
	u8			color[4];

} AChar_t;

typedef struct {

	AChar_t		c[4];

} SChar_t;


/**
 * A bitmap font text renderer.
 *
 * LOWPRIO: There are MAJOR uglyness in this code. Cleanup needed
 *
 * KNOWN BUG: The color parsing code is kind of broken, but it's too boring to
 * debug it.
 *
 *
 * Special text styles can be achieved by using the escape char '~'
 * followed by a command in the input string:
 *
 *  cRGBA - Base color. RGBA are numbers in [0, 9]
 *  b - Bold (not yet implemented)
 *  i - Italic (not yet implemented)
 *  r - Reset all flags
 *
 */
class Text {

	public:
		Text();

		~Text();

		void setFont(
				const char	*filename,
				bool		mono_spaced
			);

		void setXY(
				u32			pixel_x,
				u32			pixel_y
			);

		void setMetrics(
				i32			glyph_spacing,
				i32			line_spacing,
				u32			max_width,
				u32			max_lines
			);

		u32 getGlyphHeight() const;

		i32	getVerticalSpacing() const;

		void print(
				char		*str
			);

		void print(
				char		*str,
				u32			nx,
				u32			ny
			);

		char *printLine(
				char		*str,
				u32			line
			);

		u32 printLines(
				char		*str,
				u32			from_line,
				u32			to_line
			);

		char *nextLineSplit(
				char		*str
			);

		void resetEffects();

	private:

		void output();

		void postWord(
				char		*str,
				u32			word_len,
				i32			x,
				i32			line,
				u8			color[8]
			);

		char *printLine(
				char		*str,
				u32			line,
				bool		ignore_output
			);

	public:

		i32					lineSpacing;			/* Line spacing in pixels */
		i32					glyphSpacing;
		u32					maxWidth;
		u32					maxLines;


	private:
		Geometry			*mesh;
		Texture				*fontTexture;
		VertexArray			*glyphVA;
		ShaderProgram		*textShader;

		vec2f				texcoords_fr[256];
		vec2f				texcoords_to[256];
		u8					advanceBase[256];		/* Advance of each char */

		u32					glyphHeight;
		i32					startX;
		i32					startY;

		i32					resX, resY;				/* Device resolution */
		vec2f				onePixel;

		u32					n_chars;
		u32					maxChars;
		SChar_t				*chars;
		u16					*indices;

		u8					color[8];
		u32					style;

};



#endif /* RC_TEXT_H */


