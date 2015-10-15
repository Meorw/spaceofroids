
#ifndef RC_RESOURCELIST_H
#define RC_RESOURCELIST_H

#define NO_PURGE		0xFFFFFFFF

/**
 * Resource list handler.
 *
 * This handles all the resources "behind the scenes". All resources are stored
 * in hash tables and are reference counted so that they are not duplicated or
 * deleted ahead of time.
 *
 * When a new resource is added it will be identified according to their
 * "identifier" string, and not its actual content. This means that the exact
 * same data may be unintentionally duplicated, if this system isn't used as
 * intended.
 */
class ResourceList {

	friend class SceneGraph;

/*----------------------------------------------------------[ Functions ]----*/

	protected:
 		ResourceList();

		ResourceList(
				u32			list_length
			);

		~ResourceList();

		void empty();

	private:
		void add(
				Resource	*obj
			);

		/*
		 * Decrease reference counter, and remove resource if it reaches 0.
		 * Resources that are declared permanent will not be removed, even
		 * if their ref. counter reaches 0.
		 */

		// TODO: Resources are deleted if:
		//  * They are force purge'd
		//  * They are purged and the reference counter has hit 0
		//  * Purge level = 0 and the reference counter has hit 0
		bool dereference(
				Resource	*obj
			);

		void purge(
				u32			max_purge_level,
				bool		force_remove
			);

		Resource *getResource(
				const char	*id
			);

		void getAllResources(
				Resource	**res_list
			);

		u32 getResourceCount() const;

		void extend();

#ifdef DEBUG

		void printContents(
				char		*list_name
			);

#endif /* DEBUG */

/*------------------------------------------------------------[ Members ]----*/

	private:
		u32			n_entries;

		/* Hash table */
		u32			length;
		Resource	**list;


};

#endif /* RC_RESOURCELIST_H */

