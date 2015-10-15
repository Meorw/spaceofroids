#include "RenderChimp.h"
#include "RenderState.h"


/*---------------------------------------------------------------------------*/

RenderState::RenderState()
{
	setDefault();
}

/*---------------------------------------------------------------------------*/

RenderState::~RenderState()
{

}

/*---------------------------------------------------------------------------*/

void RenderState::enableDepthTest(
		StateCompare_t	depth_test
	)
{
	flags |= STATE_DEPTH;

	depthTest = depth_test;
}

/*---------------------------------------------------------------------------*/

void RenderState::disableDepthTest()
{
	flags &= ~STATE_DEPTH;
}

/*---------------------------------------------------------------------------*/

void RenderState::enableDepthWrite()
{
	flags |= STATE_DEPTH_WRITE;
}

/*---------------------------------------------------------------------------*/

void RenderState::disableDepthWrite()
{
	flags &= ~STATE_DEPTH_WRITE;
}

/*---------------------------------------------------------------------------*/

void RenderState::enableStencil(
		StateCompare_t	stencil_test,
		i32				ref,
		u32				mask
	)
{
	flags |= STATE_STENCIL;

	stencilRef = ref;
	stencilMask = mask;
}

/*---------------------------------------------------------------------------*/

void RenderState::disableStencil()
{
	flags &= ~STATE_STENCIL;
}

/*---------------------------------------------------------------------------*/

void RenderState::enableBlend(
		StateSrcBlend_t	src_blend,
		StateDstBlend_t	dst_blend
	)
{
	flags |= STATE_BLEND;

	srcBlend = src_blend;
	dstBlend = dst_blend;
}

/*---------------------------------------------------------------------------*/

void RenderState::disableBlend()
{
	flags &= ~STATE_BLEND;
}

/*---------------------------------------------------------------------------*/

void RenderState::enableCulling(
		StateCulling_t	cull_strategy
	)
{
	flags |= STATE_CULL;
	
	cull = cull_strategy;
}

/*---------------------------------------------------------------------------*/

void RenderState::disableCulling()
{
	flags &= ~STATE_CULL;
}

/*---------------------------------------------------------------------------*/

void RenderState::enableFrustumCulling()
{
	flags |= STATE_FRUSTUM_CULL;
}

/*---------------------------------------------------------------------------*/

void RenderState::disableFrustumCulling()
{
	flags &= ~STATE_FRUSTUM_CULL;
}

/*---------------------------------------------------------------------------*/

void RenderState::enableScissor(
		i32				x,
		i32				y,
		u32				width,
		u32				height
	)
{
	flags |= STATE_SCISSOR;
	
	scissorX = x;
	scissorY = y;
	scissorW = width;
	scissorH = height;
}

/*---------------------------------------------------------------------------*/

void RenderState::disableScissor()
{
	flags &= ~STATE_SCISSOR;
}

/*---------------------------------------------------------------------------*/

void RenderState::setDefault()
{
	flags = 0;

	enableDepthTest(CMP_LESS);
	enableDepthWrite();
	disableStencil();
	enableBlend(SB_SRC_ALPHA, DB_ONE_MINUS_SRC_ALPHA);
	enableCulling(CULL_BACK);
	disableScissor();
	disableFrustumCulling();
}

/*---------------------------------------------------------------------------*/

