
#ifndef RC_LIGHT_H
#define RC_LIGHT_H

/**
 * Defines a point Light source node.
 */
class Light : public Transformable {
	friend class SceneGraph;
	friend class Geometry;

/*----------------------------------------------------------[ Functions ]----*/

	protected:
		Light(
				const char	*nname
			);

		Light(
				const char	*nname,
				vec3f		ncolor,
				f32			nintensity,
				f32			falloff_radius
			);

		~Light();

	public:
		static void registerCommands();

		void setColor(
				vec3f		ncolor
			);

		void setColor(
				f32			r,
				f32			g,
				f32			b
			);

		void setIntensity(
				f32			nintensity
			);

		void setFalloff(
				f32			nfalloff
			);

		vec3f getColor();

		f32 getIntensity();

		f32 getFalloff();

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

		Node *duplicateSelf();

		void drawSelf();

/*------------------------------------------------------------[ Members ]----*/

	public:
		vec3f		color;
		f32			intensity;

		Sphere		falloff;

	private:
		bool		indicatorVisible;

};


#endif /* RC_LIGHT_H */

