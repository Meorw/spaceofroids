
#ifndef RC_GROUP_H
#define RC_GROUP_H

/**
 * Defines a Group node.
 */
class Group : public Transformable {
	friend class SceneGraph;

/*----------------------------------------------------------[ Functions ]----*/

	protected:
		Group(
				const char		*nname
			) : Transformable(nname, NODE_GROUP) { }

		Group(
				const char		*nname,
				const char		*filename
			);

		~Group();

	void logInfo();

	private:

		Node *duplicateSelf();

/*------------------------------------------------------------[ Members ]----*/

};


#endif /* RC_GROUP_H */

