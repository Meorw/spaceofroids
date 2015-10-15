#include "RenderChimp.h"
#include "NodeList.h"

/*---------------------------------------------------------------------------*/

NodeList::NodeList()
{
	n_nodes = 0;
	head = 0;

	memset(n_nodes_of_type, 0, sizeof(u32) * NODE_TYPE_LAST);


//	max_free = 64;
//	free_list = (NodeElement *) malloc(sizeof(NodeElement) * max_free);
//	next_free = 0;
}

/*---------------------------------------------------------------------------*/

NodeList::~NodeList()
{
	/*
	NodeElement *iter;

	if (!head)
		return;

	iter = head;

	while (iter) {
		NodeElement *p = (NodeElement *) iter->next;

		delete iter;

		n_nodes--;

		iter = p;
	}

	head = 0;
	*/
}

/*---------------------------------------------------------------------------*/

Node *NodeList::getNode(
		const char		*name
	) const
{
	NodeElement *iter;

	if (!name || !(*name))
		return 0;

	if (!head)
		return 0;

	iter = head;

	for (;;) {
		if (strcmp(iter->node->getName(), name) == 0)
			return iter->node;

		if (!iter->next)
			break;

		iter = iter->next;
	}

	return 0;
}

/*---------------------------------------------------------------------------*/

u32 NodeList::getNodeCount() const
{
	return n_nodes;
}

/*---------------------------------------------------------------------------*/

void NodeList::getNodes(
		Node			**node_list
	) const
{
	u32 i;
	NodeElement *iter;

	if (!head)
		return;

	iter = head;

	i = 0;

	while (iter) {
		node_list[i++] = iter->node;

		iter = (NodeElement *) iter->next;
	}

}

/*---------------------------------------------------------------------------*/

void NodeList::getNodes(
		const char		*name,
		u32				max_nodes,
		Node			**node_list,
		u32				*n
	) const
{
	NodeElement *iter;

	if (!head)
		return;

	iter = head;

	*n = 0;

	while (iter) {
		if (strcmp(iter->node->getName(), name) == 0) {

			node_list[*n] = iter->node;
			(*n)++;

			if (*n >= max_nodes)
				return;
		}

		iter = (NodeElement *) iter->next;
	}

}

/*---------------------------------------------------------------------------*/

u32 NodeList::getNodeCount(
		NodeType_t		type
	) const
{
	return n_nodes_of_type[(u32) type];
}

/*---------------------------------------------------------------------------*/

void NodeList::getNodes(
		Node			**node_list,
		NodeType_t		type
	) const
{
	u32 i;
	NodeElement *iter;

	if (!head)
		return;

	iter = head;

	i = 0;

	while (iter) {
		if (iter->node->getNodeType() == type)
			node_list[i++] = iter->node;

		iter = (NodeElement *) iter->next;
	}

	ASSERT(j == n_nodes_of_type[type]);

}

/*---------------------------------------------------------------------------*/

void NodeList::getNodes(
		const char		*name,
		u32				max_nodes,
		Node			**node_list,
		u32				*n,
		NodeType_t		type
	) const
{
	NodeElement *iter;

	if (!head)
		return;

	iter = head;

	*n = 0;

	while (iter) {
		if (iter->node->getNodeType() == type &&
			strcmp(iter->node->getName(), name) == 0) {

			node_list[*n] = iter->node;
			(*n)++;

			if (*n >= max_nodes)
				return;
		}

		iter = (NodeElement *) iter->next;
	}

}

/*---------------------------------------------------------------------------*/

void NodeList::add(
		Node			*node
	)
{
//	if (next_free >= max_free) {
//		max_free = max_free + (max_free >> 2); /* Grow by 25% */
//		free_list = (NodeElement *) realloc(free_list, sizeof(NodeElement) * max_free);
//	}

	NodeElement *ne;

	if (!node) {
		REPORT_WARNING("No node given");
		return;
	}

	ne = new NodeElement;

	ne->node = node;
	ne->next = head;

	head = ne;

	n_nodes++;

	n_nodes_of_type[(u32) node->getNodeType()]++;
}

/*---------------------------------------------------------------------------*/

void NodeList::remove(
		Node			*node
	)
{
	NodeElement *iter, *p = 0;

	if (!head)
		return;

	if (!node) {
		REPORT_WARNING("No node given");
		return;
	}

	iter = head;

	for (;;) {
		if (iter->node == node) {
			if (p) {
				p->next = iter->next;
			} else {
				head = (NodeElement *) iter->next;
			}

			n_nodes_of_type[(u32) iter->node->getNodeType()]--;

			delete iter;

			n_nodes--;

			return;
		}

		if (!iter->next) {
			REPORT_WARNING("Node %s not found", node->getName());
			return;
		}

		p = iter;
		iter = (NodeElement *) iter->next;
	}

}

/*---------------------------------------------------------------------------*/


