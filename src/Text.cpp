#include "RenderChimp.h"
#include "Text.h"

#define TEXT_BOLD			1
#define TEXT_ITALIC			2
#define TEXT_SHADOW			3

static const u8 colormap[10] = {0, 28, 57, 85, 113, 142, 170, 198, 227, 255};

/*---------------------------------------------------------------------------*/

Text::Text()
{
	RenderState *state;
	char namebuf[256];

	n_chars = 0;
	maxChars = 256;
	chars = (SChar_t *) malloc(sizeof(SChar_t) * maxChars);
	indices = (u16 *) malloc(sizeof(u16) * 6 * maxChars);

	resX = -1;
	resY = -1;


	sprintf(namebuf, "Font_%x_VA", (u32) this);
	glyphVA = SceneGraph::createVertexArray(namebuf, sizeof(AChar_t), TRIANGLES, USAGE_DYNAMIC);

	glyphVA->setAttribute("Vertex", 0, 2, ATTRIB_FLOAT32, false);
	glyphVA->setAttribute("Texcoord", 2 * 4, 2, ATTRIB_FLOAT32, false);
	glyphVA->setAttribute("Color", 4 * 4, 4, ATTRIB_UINT8, true);


	sprintf(namebuf, "Font_%x_SP", (u32) this);
	textShader = SceneGraph::createShaderProgram(namebuf, "Text", 0);

	state = textShader->getRenderState();

	state->enableBlend(SB_SRC_ALPHA, DB_ONE_MINUS_SRC_ALPHA);
	state->disableScissor();
	state->disableCulling();
	state->disableStencil();
	state->disableDepthTest();
	state->disableDepthWrite();

	fontTexture = 0;


	sprintf(namebuf, "Font_%x_GEO", (u32) this);
	mesh = SceneGraph::createGeometry(namebuf);

	mesh->setVertexArray(glyphVA, false);
	mesh->setShaderProgram(textShader, false);


	setXY(10, 100);

	setMetrics(1, 16, 200, 8);

	resetEffects();
}

/*---------------------------------------------------------------------------*/

Text::~Text()
{
	if (mesh)
		SceneGraph::deleteNode(mesh);

	if (chars)
		free(chars);

	if (indices)
		free(indices);
}

/*---------------------------------------------------------------------------*/

void Text::setFont(
		const char	*filename,
		bool		mono_spaced
	)
{
	char fnbuf[FILENAME_MAX];
	const char *fnprefix = RC_RESOURCE_PATH;

	u32 i, x, y;
	char namebuf[256];

	u32 w, h;
	u32 bpp;
	u8 *pixels;

	u32 glyph_width, glyph_height;
	u8 *c;

	if (strlen(filename) + strlen(fnprefix) >= (FILENAME_MAX - 1)) {
		REPORT_WARNING("Font filename too long (%s)", filename);
		return;
	}

	strcpy(fnbuf, fnprefix);
	strcat(fnbuf, filename);

	if (fontTexture)
		SceneGraph::dereferenceResource(fontTexture);

	Platform::getTexture(fnbuf, false, &pixels, &w, &h, &bpp);

	if ((w & (w - 1)) || (h & (h - 1))) {
		Platform::destroyTexture(pixels);
		REPORT_WARNING("Font texture dimensions must be powers of 2 but was %d x %d", w, h);
		return;
	}

	sprintf(namebuf, "Font_%x_TEX", (u32) this);

	glyph_width = w / 16;
	glyph_height = h / 16;

	c = pixels;

	advanceBase[0] = 0;

	if (mono_spaced) {
		/* Get advance and texcoords */
		u32 max = glyph_width;

		for (i = 1; i < 256; i++) {

			u32 glyph_x = i & 0xF;
			u32 glyph_y = i >> 4;

			advanceBase[i] = (u8) glyph_width;

			texcoords_fr[i].x = (f32) (glyph_width * glyph_x) / (float) w;
			texcoords_fr[i].y = (f32) (glyph_height * (glyph_y + 1)) / (float) h;

			texcoords_to[i].x = (f32) (glyph_width * glyph_x + max) / (float) w;
			texcoords_to[i].y = (f32) (glyph_height * glyph_y) / (float) h;
		}
	} else {
		/* Get advance and texcoords */
		for (i = 1; i < 256; i++) {
			u32 max = 0;

			u32 glyph_x = i & 0xF;
			u32 glyph_y = i >> 4;

			advanceBase[i] = 0;

			for (y = 0; y < glyph_height; y++) {
				c = pixels + ((glyph_y * glyph_height + y) * w + (glyph_x * glyph_width)) * bpp;

				for (x = 0; x < glyph_width; x++) {
					
					if (*c != 0)
						max = x > max ? x : max;

					c += bpp;
				}

			}

			advanceBase[i] = (u8) (max + 1);

			texcoords_fr[i].x = (f32) (glyph_width * glyph_x) / (float) w;
			texcoords_fr[i].y = (f32) (glyph_height * (glyph_y + 1)) / (float) h;

			texcoords_to[i].x = (f32) (glyph_width * glyph_x + max + 1) / (float) w;
			texcoords_to[i].y = (f32) (glyph_height * glyph_y) / (float) h;
		}
	}

	texcoords_fr[32].x = 0.0000f;
	texcoords_fr[32].y = 0.0001f;

	texcoords_to[32].x = 0.0000f;
	texcoords_to[32].y = 0.0001f;

	lineSpacing = glyph_height + 2;

	glyphHeight = glyph_height;

	fontTexture = SceneGraph::createTexture(namebuf, pixels, (TextureFormat_t) bpp, w, h, false, TEXTURE_FILTER_NEAREST, TEXTURE_WRAP_REPEAT);

	textShader->getUniform()->setTexture("fonttex", fontTexture);

	Platform::destroyTexture(pixels);
}

/*---------------------------------------------------------------------------*/

void Text::setXY(
		u32			pixel_x,
		u32			pixel_y
	)
{
	startX = pixel_x;
	startY = pixel_y;
}

/*---------------------------------------------------------------------------*/

void Text::setMetrics(
		i32			glyph_spacing,
		i32			line_spacing,
		u32			max_width,
		u32			max_lines
	)
{
	lineSpacing = line_spacing;
	glyphSpacing = glyph_spacing;
	maxWidth = max_width;
	maxLines = max_lines;
}

/*---------------------------------------------------------------------------*/

void Text::output()
{
	vec2f xy;

	Uniform *uniform;

	if (n_chars == 0)
		return;

	glyphVA->setArray((f32 *) chars, n_chars * 4);
	glyphVA->setIndexArray(indices, sizeof(u16), n_chars * 6);


	xy.x = -1.0f + ((f32) startX) * onePixel.x;
	xy.y = -1.0f + ((f32) startY) * onePixel.y;

	uniform = mesh->getUniform();

	uniform->setVector("offset", xy.vec, 2, UNIFORM_FLOAT32);



	mesh->render();

	n_chars = 0;
}

/*---------------------------------------------------------------------------*/

void Text::postWord(
		char		*str,
		u32			word_len,
		i32			x,
		i32			line,
		u8			color[8]
	)
{
	vec2f fr, to;
	u32 i;

	while (word_len + n_chars >= maxChars) {
		maxChars = maxChars + (maxChars >> 2); /* Grow by 25% */
		chars = (SChar_t *) realloc(chars, sizeof(SChar_t) * maxChars);
		indices = (u16 *) realloc(indices, sizeof(u16) * 6 * maxChars);
	}

	fr.y = ((f32) (line * lineSpacing)) * onePixel.y;
	to.y = fr.y + onePixel.y * (f32) glyphHeight;

	for (i = 0; i < word_len; i++) {
		char c = str[i];
		SChar_t *sc = &chars[n_chars];
		u32 iindex = n_chars * 6;
		u16 cindex = (u16) n_chars * 4;

		fr.x = ((f32) x) * onePixel.x;
		x += advanceBase[c];

		to.x = ((f32) x) * onePixel.x;
		x += glyphSpacing;

		sc->c[0].vertex.x = fr.x;
		sc->c[0].vertex.y = fr.y;
		sc->c[0].texcoord.x = texcoords_fr[c].x;
		sc->c[0].texcoord.y = texcoords_fr[c].y;
		memcpy(sc->c[0].color, &color[0], 4);

		sc->c[1].vertex.x = to.x;
		sc->c[1].vertex.y = fr.y;
		sc->c[1].texcoord.x = texcoords_to[c].x;
		sc->c[1].texcoord.y = texcoords_fr[c].y;
		memcpy(sc->c[1].color, &color[0], 4);

		sc->c[2].vertex.x = fr.x;
		sc->c[2].vertex.y = to.y;
		sc->c[2].texcoord.x = texcoords_fr[c].x;
		sc->c[2].texcoord.y = texcoords_to[c].y;
		memcpy(sc->c[2].color, &color[4], 4);

		sc->c[3].vertex.x = to.x;
		sc->c[3].vertex.y = to.y;
		sc->c[3].texcoord.x = texcoords_to[c].x;
		sc->c[3].texcoord.y = texcoords_to[c].y;
		memcpy(sc->c[3].color, &color[4], 4);

		indices[iindex + 0] = cindex + 0;
		indices[iindex + 1] = cindex + 1;
		indices[iindex + 2] = cindex + 2;
		indices[iindex + 3] = cindex + 2;
		indices[iindex + 4] = cindex + 1;
		indices[iindex + 5] = cindex + 3;

		n_chars++;
	}
}

/*---------------------------------------------------------------------------*/

void Text::print(
		char		*str,
		u32			nx,
		u32			ny
	)
{
	u32 px, py;

	px = nx;
	py = ny;

	setXY(nx, ny);

	print(str);

	setXY(px, py);

}

/*---------------------------------------------------------------------------*/

char *Text::printLine(
		char		*str,
		u32			line
	)
{
	return printLine(str, line, false);
}

/*---------------------------------------------------------------------------*/

char *Text::printLine(
		char		*str,
		u32			line,
		bool		ignore_output
	)
{
	char *p, *b;

	bool await_command;
	u32 ccol = 0;
	u32 gcol = 0;

	u32 totallen;

	u32 vp_w, vp_h;

	i32 word_advance = 0;
	i32 total_advance = 0;
	u32 word_len = 0;

	totallen = strlen(str);

	if (totallen == 0)
		return 0;

	Renderer::getViewportDimensions(&vp_w, &vp_h);

	if ((i32) vp_w != resX || (i32) vp_h != resY) {

		resX = vp_w;
		resY = vp_h;

		onePixel.x = 2.0f / (float) resX;
		onePixel.y = 2.0f / (float) resY;
	}

	p = str;
	b = str;

	await_command = false;

	for (;;) {
		char c = *p;

		if (!await_command) {
			switch (c) {
				case 0:
				case ' ':
				case '\t':

					word_advance += advanceBase[32] + glyphSpacing;

					if (total_advance + word_advance > (i32) maxWidth) {

						if (!ignore_output)
							output();

						return b;
					}

					if (!ignore_output)
						postWord(b, word_len, total_advance, line, color);

					b += word_len + 1;
					word_len = 0;
					total_advance += word_advance;
					word_advance = 0;

					break;

				case '\n':
				case '\r':

					if (!ignore_output)
						postWord(b, word_len, total_advance, line, color);

					b += word_len + 1; /* Also consume the new line */

					if (!ignore_output)
						output();

					return b;

					break;
				case '~':
					if (p[1] != '~') {
						await_command = true;
						break;
					}
				default:

					if (word_advance + advanceBase[c] + glyphSpacing > (i32) maxWidth) {

						if (!ignore_output)
							output();

						return b;
					}

					word_advance += advanceBase[c] + glyphSpacing;
					word_len++;

					break;

			}
		} else { /* await_command */

			if (ccol == 0) {


				switch (c) {

					case 'c':
						gcol = 0;
						ccol = 1;
						break;
					case 'g':
						gcol = 1;
						ccol = 1;
						break;
					case 'b':
						style ^= TEXT_BOLD;
						await_command = false;
						b += 2;
						break;
					case 'i':
						style ^= TEXT_ITALIC;
						await_command = false;
						b += 2;
						break;
					case 'r':
						memset(color, 0xFF, 8);
						style = 0;
						await_command = false;
						b += 2;
						break;
					default:
						break;
				}

			} else {

				if (c >= '0' && c <= '9') {

					if (gcol) {
						color[ccol - 1] = colormap[c - '0'];
					} else {
						color[ccol - 1] = color[4 + ccol - 1] = colormap[c - '0'];
					}

					ccol++;
				} else {
					memset(color, 0xFF, 8);
					ccol = 0;
					await_command = false;
				}

				if (ccol == 5) {
					ccol = 0;
					await_command = false;
					b += 6;
				}

			}

		}

		if (*p == 0)
			break;

		p++;
	}

	if (!ignore_output)
		output();

	return 0;
}

/*---------------------------------------------------------------------------*/

void Text::print(
		char		*str
	)
{
	u32 line = 0;
	char *cstr, *pstr;

	resetEffects();

	pstr = str;

	while (line < maxLines) {
		cstr = printLine(pstr, line);

		if (cstr == pstr) { /* TODO: A bug lurks here... */
			//print("...");
			return;
		}

		pstr = cstr;
		line++;
	}
}

/*---------------------------------------------------------------------------*/

u32 Text::printLines(
		char		*str,
		u32			from_line,
		u32			to_line
	)
{
	u32 line = 0;
	u32 lline = 0;
	char *cstr, *pstr;

	pstr = str;

	while (line < from_line) {
		cstr = printLine(pstr, line, true);

		if (cstr == pstr) {
			//print("...");
			return 0;
		}

		pstr = cstr;
		line++;
	}

	lline = line;
	line = 0;

	while (line < maxLines && lline < to_line) {
		cstr = printLine(pstr, line, false);

		if (cstr == pstr) {
			return line;
		}

		pstr = cstr;
		line++;
		lline++;
	}

	return line;
}

/*---------------------------------------------------------------------------*/

u32 Text::getGlyphHeight() const
{
	return glyphHeight;
}

/*---------------------------------------------------------------------------*/

i32 Text::getVerticalSpacing() const
{
	return lineSpacing;
}

/*---------------------------------------------------------------------------*/

char *Text::nextLineSplit(
		char		*str
	)
{
	return printLine(str, 0, true);
}

/*---------------------------------------------------------------------------*/

void Text::resetEffects()
{
	memset(color, 0xFF, 8);
	style = 0;
}

/*---------------------------------------------------------------------------*/

