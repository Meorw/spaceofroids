#include "RenderChimp.h"
#include "Resource.h"
#include "ResourceList.h"
#include "CubeMap.h"

/*---------------------------------------------------------------------------*/

CubeMap::CubeMap(
		const char			*nidentifier,

		const char			*nleft,
		const char			*nright,
		const char			*nup,
		const char			*ndown,
		const char			*nfront,
		const char			*nback

	) : Resource(nidentifier, NO_PURGE, RESOURCE_CUBEMAP)
{
	char fnbuf[FILENAME_MAX];
	const char *fnprefix = RC_RESOURCE_PATH;

	u32 i;
	u32 w, h, bpp;
	u8 *pixels[6];

	if (!nleft || !(*nleft) || !nright || !(*nright) || !nup || !(*nup) ||
		!ndown || !(*ndown) || !nfront || !(*nfront) || !nback || !(*nback)) {
		REPORT_WARNING("Cube map filename missing");

		broken = true;
		return;
	}

	if (strlen(nleft) + strlen(fnprefix) >= (FILENAME_MAX - 1) ||
		strlen(nright) + strlen(fnprefix) >= (FILENAME_MAX - 1) ||
		strlen(nup) + strlen(fnprefix) >= (FILENAME_MAX - 1) ||
		strlen(ndown) + strlen(fnprefix) >= (FILENAME_MAX - 1) ||
		strlen(nfront) + strlen(fnprefix) >= (FILENAME_MAX - 1) ||
		strlen(nback) + strlen(fnprefix) >= (FILENAME_MAX - 1)) {

		REPORT_WARNING("Cube map filename too long");

		broken = true;
		return;
	}

	strcpy(fnbuf, fnprefix);
	strcat(fnbuf, nleft);
	Platform::getTexture(fnbuf, false, &pixels[0], &w, &h, &bpp);

	strcpy(fnbuf, fnprefix);
	strcat(fnbuf, nright);
	Platform::getTexture(fnbuf, false, &pixels[1], &w, &h, &bpp);

	strcpy(fnbuf, fnprefix);
	strcat(fnbuf, nup);
	Platform::getTexture(fnbuf, false, &pixels[2], &w, &h, &bpp);

	strcpy(fnbuf, fnprefix);
	strcat(fnbuf, ndown);
	Platform::getTexture(fnbuf, false, &pixels[3], &w, &h, &bpp);

	strcpy(fnbuf, fnprefix);
	strcat(fnbuf, nfront);
	Platform::getTexture(fnbuf, false, &pixels[4], &w, &h, &bpp);

	strcpy(fnbuf, fnprefix);
	strcat(fnbuf, nback);
	Platform::getTexture(fnbuf, false, &pixels[5], &w, &h, &bpp);

	Renderer::loadCubeMap(this, pixels, w, h, bpp);

	for (i = 0; i < 6; i++)
		Platform::destroyTexture(pixels[i]);

}

/*---------------------------------------------------------------------------*/

CubeMap::~CubeMap()
{
	Renderer::unloadCubeMap(this);
}

/*---------------------------------------------------------------------------*/

void CubeMap::setCubeMap(
		CubeMap				**target_map,
		CubeMap				*new_map,
		bool				instantiate_resource
	)
{
	if (*target_map)
		SceneGraph::dereferenceResource(*target_map);

	if (instantiate_resource) {
		*target_map = (CubeMap *) new_map->instantiate();
	} else {
		*target_map = new_map;
	}
}

/*---------------------------------------------------------------------------*/

void *CubeMap::getHandle()
{
	return handle;
}

/*---------------------------------------------------------------------------*/

void CubeMap::setHandle(
		void				*nhandle
	)
{
	handle = nhandle;
}

/*---------------------------------------------------------------------------*/

#ifdef DEBUG

void CubeMap::print(
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

	//prnt1("    [ TEX ID   ][ %u ]\n", texId);

}

#endif /* DEBUG */

/*---------------------------------------------------------------------------*/

