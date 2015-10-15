
#ifndef RC_NODELIST_H
#define RC_NODELIST_H

/**
 * Node list handler.
 *
 */
class NodeList {
	friend class SceneGraph;

	/**
	 * Node linked list element
	 *
	 */
	struct NodeElement {

		Node			*node;
		NodeElement		*next;

	};

/*----------------------------------------------------------[ Functions ]----*/

	protected:
		NodeList();

		~NodeList();

	public:

		Node *getNode(
				const char		*name
			) const;

		u32 getNodeCount() const;

		void getNodes(
				Node			**node_list
			) const;

		void getNodes(
				const char		*name,
				u32				max_nodes,
				Node			**node_list,
				u32				*n
			) const;


		u32 getNodeCount(
				NodeType_t		type
			) const;

		void getNodes(
				Node			**node_list,
				NodeType_t		type
			) const;

		void getNodes (
				const char		*name,
				u32				max_nodes,
				Node			**node_list,
				u32				*n,
				NodeType_t		type
			) const;


		void add(
				Node			*node
			);

		void remove(
				Node			*node
			);

	protected:



/*------------------------------------------------------------[ Members ]----*/

	private:
		u32				n_nodes;

		u32				n_nodes_of_type[NODE_TYPE_LAST];

		u32				max_free;
		u32				next_free;
		NodeElement	**free_list;

		NodeElement	*head;
};

#endif /* RC_NODELIST_H */

