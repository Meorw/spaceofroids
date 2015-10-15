/* TODO: lookAt function doesn't account for scaling, and is generally untested
 */


#ifndef RC_TRANSFORMABLE_H
#define RC_TRANSFORMABLE_H

/**
 * Defines a Transformable node.
 * Inherits from Node.
 *
 * A transformation M is composed of rotation, translation and scale. We define
 * M as:
 *
 *    M = T * R * S
 *
 * Transformations are hierachical. This means that every transformable child
 * node will inherit the transformations of its parent. Let's say for instance,
 * that there are two nodes A and B. A is the parent of B and both are
 * transformable. The local transformation of A is denoted Ma and for B its Ma.
 * The matrix that brings nodes A and B to world space is denoted Wa and Wa.
 * The resulting transformations for the nodes will then be:
 *
 *    Ma = Ta * Ra * Sa
 *    Mb = Tb * Rb * Sb
 *
 *    Wa = Ma = Ta * Ra * Sa
 *    Wb = Ma * Mb = (Ta * Ra * Sa) * (Ta * Rb * Sb)
 *
 * The MB matrix will then, in turn, be recursively be applied to any children
 * of node B to construct new model->world matrices, in the same manner as in
 * the example above.
 *
 */
class Transformable : public Node {
	friend class SceneGraph;
	friend class World;
	friend class Camera;
	friend class Geometry;

/*----------------------------------------------------------[ Functions ]----*/

	protected:
		Transformable(
				const char	*nname,
				NodeType_t	ntype
			);

		~Transformable();

	public:
		/* Reset the transformation to the identity matrix */
		void reset();

			/* Relative transformations */
		
		void translate(
				f32		x,
				f32		y,
				f32		z
			);

		void translate(
				vec3f	v
			);

		void scale(
				f32		x,
				f32		y,
				f32		z
			);

		void scale(
				vec3f	v
			);

		void scale(
				f32		uniform
			);

		/* Rotate around vector (x, y, z) */

		void rotate(
				f32		angle,
				f32		x,
				f32		y,
				f32		z
			);

		void rotate(
				f32		angle,
				vec3f	v
			);

		void rotateX(
				f32		angle
			);

		void rotateY(
				f32		angle
			);

		void rotateZ(
				f32		angle
			);

		/* Absolute transformations */

		void setTranslate(
				f32		x,
				f32		y,
				f32		z
			);

		void setTranslate(
				vec3f	v
			);

		void setScale(
				f32		x,
				f32		y,
				f32		z
			);

		void setScale(
				vec3f	v
			);

		void setScale(
				f32		uniform
			);

		/* Rotate around vector (x, y, z) */

		void setRotate(
				f32		angle,
				f32		x,
				f32		y,
				f32		z
			);

		void setRotate(
				f32		angle,
				vec3f	v
			);

//		void setRotate(
//				quat	&q
//			);

		void setRotateX(
				f32		angle
			);

		void setRotateY(
				f32		angle
			);

		void setRotateZ(
				f32		angle
			);

			/* Useful matrix fetching functions */

		mat3f getRotation();
		vec3f getTranslation();
		vec3f getScale();

		mat4f getTranslationMatrix();
		mat4f getRotationMatrix();
		mat4f getScaleMatrix();

		mat4f getTranslationMatrixInverse();
		mat4f getRotationMatrixInverse();
		mat4f getScaleMatrixInverse();

		mat4f getTranslationRotationMatrix();

		mat4f getLocalMatrix();
		mat4f getLocalMatrixInverse();

		mat4f getWorldMatrix();
		mat4f getWorldMatrixInverse();

		mat3f getWorldNormalMatrix();

		vec3f getLocalFront();
		vec3f getLocalUp();
		vec3f getLocalRight();

		vec3f getWorldFront();
		vec3f getWorldUp();
		vec3f getWorldRight();

		/**
		* LookTowards
		*
		* Note 1: Function will exit if front_vec is (close to) parallel to the Y-axis; supply your own up_vec if this is the case.
		* Note 2: Not fully bug-tested, use at own risk...
		*
		* Example:
		*	Make camera look at 'my_node'. Note that world space positions are used.
		*	camera->lookAt( my_node->getWorldPosition() - camera->getWorldPosition() );
		*/
		void lookTowards(
				vec3f	front_vec,
				vec3f	up_vec
			);

		void lookTowards(
				vec3f	front_vec
			);

		/**
		 * Transform point from model space to world space.
		 * It's faster to fetch the world-transform and perform the mult on the
		 * host side if multiple points should be transformed.
		 *
		 */
		vec3f getWorldPosition(
				vec3f pos
			);

		/**
		 * Get the world position of point (0, 0, 0) in model space transformed
		 * in to world space
		 */
		vec3f getWorldPosition();

		bool isTransformable() { return true; }

		void setFreeze(
				bool	do_freeze
			);

		static void registerCommands();



	protected:

		void detached();

		void logTransformableInfo();



/*------------------------------------------------------------[ Members ]----*/

	public:

		Group	*axis;

	protected:
		mat3f	R;
		vec3f	T;
		vec3f	S;

		mat4f	worldMatrix;
		mat4f	worldMatrixInverse;

		i32		transformTime;

		i32		parentWorldTime;
		i32		worldTime;

		i32		worldInvTime;
		i32		parentWorldInvTime;

		bool	frozen;


};


#endif /* RC_TRANSFORMABLE_H */

