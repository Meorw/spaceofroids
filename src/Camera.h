
#ifndef RC_CAMERA_H
#define RC_CAMERA_H

/** Use this to optimize projection matrix calculations */
typedef enum {

	RC_PROJECTION_TYPE_FREE_TRANSFORM = 0,
	RC_PROJECTION_TYPE_PERSPECTIVE,
	RC_PROJECTION_TYPE_ORTHOGONAL,	

	RC_PROJECTION_TYPE_LAST

} ProjectionType_t;

/**
 * Defines a Camera node.
 */
class Camera : public Transformable {
	friend class SceneGraph;
	friend class World;

/*----------------------------------------------------------[ Functions ]----*/

	protected:

		Camera(
				const char	*nname
			);

		~Camera();

	public:

		static void registerCommands();

		/**
		 * Set orthogonal projection
		 *
		 * This sets the camera matrix to:
		 *
		 * [ 2 / (left - right)    0    0    tx ;
		 *   0    2 / (top - bottom)    0    ty ;
		 *   0    0    2 / (nfar - nnear)    tz ;
		 *   0    0    0    1 ]
		 *
		 * tx = (right + left) / (right - left)
		 * ty = (top + bottom) / (top - bottom)
		 * tz = (nfar + nnear) / (nfar - nnear)
		 *
		 */
		void setOrthogonalProjection(
				f32		left,
				f32		right,
				f32		bottom,
				f32		top,
				f32		nnear,
				f32		nfar
			);

		/**
		 * Set perspective projection
		 *
		 * This sets the camera matrix to:
		 * [ f / aspect    0    0    0 ;
		 *   0    f    0    0 ;
		 *   0    0    g    h ;
		 *   0    0    -1    0 ]
		 *
		 * f = cot(fovy / 2)
		 * g = (nfar + nnear) / (nnear - nfar)
		 * h = (2 * nfar * nnear) / (nnear - nfar)
		 *
		 * @param fovy
		 *   Field of view in radians
		 *
		 * @param aspect
		 *   y / x
		 *
		 */
		void setPerspectiveProjection(
				f32		fovy,
				f32		aspect,
				f32		nnear,
				f32		nfar
			);

		/**
		 * Set projection matrix
		 *
		 * This makes projection inverse computation much slower than for
		 * perspective or orthogonal matrices, since they are very easy to
		 * invert.
		 *
		 */
		void setProjection(
				mat4f	&matrix
			);

		/**
		 * Unproject a screen coordinate
		 *
		 * @param screen_coord
		 *   Values should lie within [-1, 1]. Set Z to -1 for near plane and
		 * 1 for far plane.
		 *
		 * @return
		 *   A point in world space
		 *
		 */
		vec3f unproject(
				vec3f	screen_coord
			);

		mat4f getProjectionMatrix() const;
		mat4f getProjectionMatrixInverse();

		mat4f getViewMatrix();
		mat4f getViewMatrixInverse();

		mat4f getViewProjectionMatrix();
		mat4f getViewProjectionMatrixInverse();

		void calculateFrustum(
				vec4f		frustum[6]
			);

		/**
		 * Enable/disable light indicator for debugging purposes
		 *
		 */
		void toggleIndicator(
				bool		visible
			);

		bool getIndicator();

		void logInfo();

	private:

		/**
		 * Create copy of this node
		 *
		 * @return
		 *   A copy of this Camera node upon succes, otherwise NULL
		 */
		Node *duplicateSelf();

		void drawSelf();

/*------------------------------------------------------------[ Members ]----*/

	private:
		f32					nearPlane;
		f32					farPlane;
		mat4f				projectionMatrix;

		mat4f				projectionMatrixInverse;

		/**
		 * Knowing the layout of the projection matrix can help optimization
		 * later on.
		 */
		ProjectionType_t	projectionType;

		bool				projectionChanged;

		bool				indicatorVisible;

};


#endif /* RC_CAMERA_H */

