
#ifndef RC_RESOURCE_H
#define RC_RESOURCE_H

/* Resource type */
typedef enum {
	RESOURCE_VERTEX_ARRAY = 0,
	RESOURCE_TEXTURE,
	RESOURCE_RENDER_TARGET,
	RESOURCE_CUBEMAP,
	RESOURCE_SHADER_PROGRAM,

	RESOURCE_TYPE_LAST

} ResourceType_t;

/*
 * Resource base class
 * 
 */
class Resource {
	friend class SceneGraph;
	friend class ResourceList;

/*----------------------------------------------------------[ Functions ]----*/

	protected:
		Resource(
				const char				*nidentifier,
				u32						purge_level,
				ResourceType_t			rtype
			);

		virtual ~Resource();

	public:

		/**
		 * Compares the hash values and identifiers of this and the incoming
		 * resource. Return true if they are equal, otherwise false.
		 */
		virtual bool isEqual(
				Resource		*res
			);

		/** Instantiate a resource. This increases the reference counter */
		virtual Resource *instantiate();

		ResourceType_t getType() const;

		void setPurgeLevel(
				u32				purge_level
			);

		void setMaxPurgeLevel(
				u32				purge_level
			);

		u32 getPurgeLevel() const;

		u32 getReferenceCount() const;

		/** Returns the identifier. DO NOT MODIFY IT! */
		const char *getIdentifier() const;

		/**
		 * Returns a copy of the identifier, which you'll have to delete your-
		 * self when you're done with it
		 */
		char *getIdentifierStringCopy();

	protected:

		static char *getNewResourceName(
				char			*buf,
				char			*base,
				ResourceType_t	type
			);

#ifdef DEBUG
		virtual void print(
				FILE	*f
			) { }
#endif /* DEBUG */

/*------------------------------------------------------------[ Members ]----*/

	private:
		u32				hash;			/* Hash value based on the
										 * "identifier".
										 */
		u32				n_refs;			/* Number of references to this
										 * resource object.
										 */
		Resource		*next;			/* Used for List hash table */

	protected:
		ResourceType_t	type;

		char			*identifier;

		u32				purgeLevel;		/* Don't delete, even if there are no
										 * references to this resource if this
										 * is not 0.
										 */

		bool			broken;

	public:

		Auxiliary		auxiliary;

};

#endif /* RC_RESOURCE_H */

