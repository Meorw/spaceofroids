#include "RenderChimp.h"
#include "Resource.h"
#include "ResourceList.h"
#include "Texture.h"

/*---------------------------------------------------------------------------*/

RenderTarget::RenderTarget(
		const char			*nidentifier,

		u32					nwidth,
		u32					nheight,

		u32					n_targets,

		TextureFilter_t		filter,
		TextureWrap_t		wrap,

		bool				include_depth_buffer,
		bool				include_stencil_buffer
	) : Resource(nidentifier, NO_PURGE, RESOURCE_RENDER_TARGET)
{
	u32 i;

	width = nwidth;
	height = nheight;
	
	format = TEXTURE_FORMAT_RGBA8;

	if (n_targets > RC_MAX_RENDER_TARGET_TEXTURES) {
		REPORT_WARNING("Too many render targets (%d). Only %d allowed", n_targets, RC_MAX_RENDER_TARGET_TEXTURES);
		broken = true;
		return;
	}

	for (i = 0; i < n_targets; i++) {
		textures[i] = SceneGraph::createTexture(0, 0, format, nwidth, nheight, false, filter, TEXTURE_WRAP_CLAMP);
		textures[i]->setPurgeLevel(0);
	}

	n_textures = n_targets;

	Renderer::loadRenderTarget(this, n_targets, include_depth_buffer, include_stencil_buffer);
}

/*---------------------------------------------------------------------------*/

RenderTarget::~RenderTarget()
{
	u32 i;

	Renderer::unloadRenderTarget(this);

	for (i = 0; i < n_textures; i++) {
		SceneGraph::dereferenceResource(textures[i]);
	}
}

/*---------------------------------------------------------------------------*/

Texture *RenderTarget::getTexture(
		u32					target_index
	)
{
	return textures[target_index];
}

/*---------------------------------------------------------------------------*/

void *RenderTarget::getHandle()
{
	return handle;
}

/*---------------------------------------------------------------------------*/

void RenderTarget::setHandle(
		void				*nhandle
	)
{
	handle = nhandle;
}

/*---------------------------------------------------------------------------*/

void RenderTarget::shareDepthBuffer(
		RenderTarget		*target
	)
{
	Renderer::shareDepthBuffer(target, this);
}

/*---------------------------------------------------------------------------*/
