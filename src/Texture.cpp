#include "RenderChimp.h"
#include "Resource.h"
#include "ResourceList.h"
#include "Texture.h"

/*---------------------------------------------------------------------------*/

Texture::Texture(
		const char			*nidentifier,
		const char			*filename,
		bool				generate_mipmaps,
		TextureFilter_t		filter,
		TextureWrap_t		wrap

	) : Resource(nidentifier, NO_PURGE, RESOURCE_TEXTURE)
{
	char fnbuf[FILENAME_MAX];
	const char *fnprefix = RC_RESOURCE_PATH;

	u8 *pixels;
	u32 w, h, bpp;
	TextureFormat_t format;

	if (!filename) {
		REPORT_WARNING("No texture filename given");
		broken = true;
		return;
	}

	if (strlen(filename) + strlen(fnprefix) >= (FILENAME_MAX - 1)) {
		REPORT_WARNING("Texture filename too long (%s)", filename);
		broken = true;
		return;
	}

	strcpy(fnbuf, fnprefix);
	strcat(fnbuf, filename);

	handle = 0;

	Platform::getTexture(fnbuf, true, &pixels, &w, &h, &bpp);

	if (!pixels) {
		REPORT_WARNING("Did not receive pixels from platform layer for texture %s", nidentifier);
		broken = true;
		return;
	}
	
	if (bpp == 4) {
		format = TEXTURE_FORMAT_RGBA8;
	} else if (bpp == 3) {
		format = TEXTURE_FORMAT_RGB8;
	} else if (bpp == 1) {
		format = TEXTURE_FORMAT_GRAY8;
	} else {
		format = TEXTURE_FORMAT_GRAY8; /* Just to get rid of a warning... */
		REPORT("The texture doesn't have 1, 3 or 4 bytes per pixel");
		broken = true;
		return;
	}

	Renderer::loadTexture(this, pixels, w, h, format, generate_mipmaps, filter, wrap);

	Platform::destroyTexture(pixels);
}

/*---------------------------------------------------------------------------*/

Texture::Texture(
		const char			*nidentifier,
		u8					*pixels,
		TextureFormat_t		nformat,
		u32					nwidth,
		u32					nheight,
		bool				generate_mipmaps,
		TextureFilter_t		filter,
		TextureWrap_t		wrap
	) : Resource(nidentifier, NO_PURGE, RESOURCE_TEXTURE)
{
	handle = 0;

	Renderer::loadTexture(this, pixels, nwidth, nheight, nformat, generate_mipmaps, filter, wrap);
}

/*---------------------------------------------------------------------------*/

Texture::~Texture()
{
	Renderer::unloadTexture(this);
}

/*---------------------------------------------------------------------------*/

void Texture::setTexture(
		Texture				**target_tex,
		Texture				*new_tex,
		bool				instantiate_resource
	)
{
	if (*target_tex)
		SceneGraph::dereferenceResource(*target_tex);

	if (instantiate_resource) {
		*target_tex = (Texture *) new_tex->instantiate();
	} else {
		*target_tex = new_tex;
	}
}

/*---------------------------------------------------------------------------*/

void Texture::setTexture(
		Texture				**target_tex,
		const char			*nidentifier,
		const char			*filename
	)
{
	if (*target_tex)
		SceneGraph::dereferenceResource(*target_tex);

	*target_tex = SceneGraph::createTexture(nidentifier, filename,
		true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);

}

/*---------------------------------------------------------------------------*/

void *Texture::getHandle()
{
	return handle;
}

/*---------------------------------------------------------------------------*/

void Texture::setHandle(
		void				*nhandle
	)
{
	handle = nhandle;
}

/*---------------------------------------------------------------------------*/

void Texture::generateMipMaps()
{
	Renderer::generateMipmaps(this);
	hasMipmaps = true;
}

/*---------------------------------------------------------------------------*/

#ifdef DEBUG

void Texture::print(
		FILE		*f
	)
{
#ifdef RC_LOG_TO_FILE
	#define prnt0(a)		fprintf(f, a)
	#define prnt1(a, b)		fprintf(f, a, b)
#else /* RC_LOG_TO_FILE */
	#define prnt0(a)		printf(a)
	#define prnt1(a, b)		printf(a, b)
#endif /* RC_LOG_TO_FILE */

	prnt1("    [ FORMAT   ][ %u ]\n", (u32) format);
	prnt1("    [ WIDTH    ][ %u ]\n", width);
	prnt1("    [ HEIGHT   ][ %u ]\n", height);
	prnt1("    [ MIP'D    ][ %u ]\n", hasMipmaps ? 1 : 0);
//	prnt1("    [ GL ID    ][ %u ]\n", texId);
}

#endif /* DEBUG */

/*---------------------------------------------------------------------------*/

