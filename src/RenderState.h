
#ifndef RC_RENDERSTATE_H
#define RC_RENDERSTATE_H

/* States that can be enabled or disabled */
#define STATE_DEPTH					0x01
#define STATE_DEPTH_WRITE			0x02
#define STATE_STENCIL				0x04
#define STATE_BLEND					0x08
#define STATE_CULL					0x10
#define STATE_SCISSOR				0x20
#define STATE_FRUSTUM_CULL			0x40

/*---------------------------------------------------------------------------*/

/**
 * Defines an internal State.
 *
 * This sets the rendering state. The state specified in a State node will
 * hold for the whole subtree of the node, or until a new State node is comes
 * along.
 *
 * IMPORTANT NOTE: Scissor and stencil is untested.
 *
 */
class RenderState {
	friend class SceneGraph;
	
	friend class Renderer;
#ifdef RC_OPENGL
	friend class RendererGL;
#endif

	friend class ShaderProgram;

/*----------------------------------------------------------[ Functions ]----*/

	public:
		RenderState();

		~RenderState();

	public:

		/* Enable/disable depth testing */

		void enableDepthTest(
				StateCompare_t	depth_test
			);

		void disableDepthTest();


		/* Enable/disable depth write */

		void enableDepthWrite();

		void disableDepthWrite();


		/* Enable/disable stencil testing */

		void enableStencil(
				StateCompare_t	stencil_test,
				i32				ref,
				u32				mask
			);

		void disableStencil();


		/* Enable/disable blending */

		void enableBlend(
				StateSrcBlend_t	src_blend,
				StateDstBlend_t	dst_blend
			);

		void disableBlend();


		/* Enable/disable triangle culling */

		void enableCulling(
				StateCulling_t	cull_strategy
			);

		void disableCulling();


		/* Enable/disable frustum culling */

		void enableFrustumCulling();

		void disableFrustumCulling();


		/* Enable/disable scissor test */

		void enableScissor(
				i32				x,
				i32				y,
				u32				width,
				u32				height
			);

		void disableScissor();

		/** Set all states to default */
		void setDefault();


/*------------------------------------------------------------[ Members ]----*/

	private:
		i32					stencilRef;
		u32					stencilMask;
		StateCompare_t		stencilTest;

		StateCompare_t		depthTest;

		StateSrcBlend_t		srcBlend;
		StateDstBlend_t		dstBlend;

		StateCulling_t		cull;

		i32					scissorX, scissorY;
		u32					scissorW, scissorH;

		u8					flags;

};


#endif /* RC_RENDERSTATE_H */

