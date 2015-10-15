
#ifndef RC_WORLD_H
#define RC_WORLD_H

/**
 * Defines a World node.
 */
class World : public Node {
	friend class SceneGraph;

/*----------------------------------------------------------[ Functions ]----*/

	protected:
		World(
				const char		*nname
			);

		~World();

	public:

		/**
		 * A camera must be set as the active camera, so that the scene is
		 * rendered from a specified point, with the cameras projection
		 * settings.
		 */
		void setActiveCamera(
				Camera		*camera
			);

		Camera *getActiveCamera();

		void logInfo();

	private:

		Node *duplicateSelf();

		void drawSelf();

/*------------------------------------------------------------[ Members ]----*/

	private:
		Camera			*activeCamera;

};


#endif /* RC_WORLD_H */

