#include "RenderChimp.h"
#include "Node.h"

/*---------------------------------------------------------------------------*/

static char *NodeName[] = {
	"Camera",
	"Group",
	"Light",
	"World",
	"Geometry",
	""
};

/*---------------------------------------------------------------------------*/

Node::Node(
		const char		*nname,
		NodeType_t		ntype
	)
{
	broken = false;

	is_transformable = false;

	nChildren = 0;
	parent = 0;
	firstChild = 0;
	next = 0;

	isActive = true;
	isVisible = true;

	type = ntype;

	name = duplicateString(nname);
}

/*---------------------------------------------------------------------------*/

Node::~Node()
{
	/*
	u32 dltd = 0;
	Node *c = firstChild;

	while (c) {
		Node *d = c->next;
		detachChild(c);
		SceneGraph::nodeList->remove(c);
		delete c;
		dltd++;
		c = d;
	}

	free(name);
	*/

	free(name);
}

/*---------------------------------------------------------------------------*/

void Node::renderAll()
{
	if (isActive) {
		Node *c = firstChild;

		if (isVisible)
			drawSelf();

		/* Draw children */
		while (c) {
			c->renderAll();

			c = c->next;
		}
	}
}

/*---------------------------------------------------------------------------*/

#define NODE_MAX_NAME_LENGTH			256

void Node::setName(
		char			*nname
	)
{
	u32 len = strlen(nname);

	free(name);

	if (len > NODE_MAX_NAME_LENGTH) {
		name = duplicateString(&nname[len] - NODE_MAX_NAME_LENGTH + 1);
	} else {
		name = duplicateString(nname);
	}
}

/*---------------------------------------------------------------------------*/

u32 Node::getChildrenCount() const
{
	return nChildren;
}

/*---------------------------------------------------------------------------*/

void Node::attachChild(
		Node			*node
	)
{
	Node *c;

	if (node->parent) {
		REPORT_WARNING("The node %s hasn't been detached from its parent", node->getName());

		if (node->parent == this)
			return;

		node->parent->detachChild(node);
	}

	c = firstChild;
	firstChild = node;
	node->next = c;
	node->parent = this;

	nChildren++;
}

/*---------------------------------------------------------------------------*/

void Node::detachChild(
		Node			*node
	)
{
	Node *c = firstChild;
	Node *p = 0;

	if (!node) {
		REPORT_WARNING("No node to detach");
		return;
	}

	while (c) {
		if (c == node) {
			c->parent = 0;

			if (p) {
				p->next = c->next;
			} else {
				firstChild = c->next;
			}

			c->next = 0;
			nChildren--;

			c->detached();

			break;
		}

		p = c;
		c = c->next;
	}

	if (!c)
		REPORT_WARNING("The two nodes %s and %s are not connected", getName(), node->getName());

}

/*---------------------------------------------------------------------------*/

void Node::detachFromParent()
{
	if (parent)
		parent->detachChild(this);
}

/*---------------------------------------------------------------------------*/

Node *Node::getChild(
		u32			index
	)
{
	Node *node = firstChild;
	u32 i = 0;

	while (i < index && node) {
		node = node->next;
		i++;
	}

	return node;
}

/*---------------------------------------------------------------------------*/

Node *Node::getChild(
		char		*search_name,
		u32			max_depth
	)
{
	Node *node = firstChild;

	while (node) {

		if (strcmp(node->getName(), search_name) == 0)
			return node;

		node = node->next;
	}

	if (max_depth == 0)
		return 0;

	node = firstChild;

	while (node) {

		Node *local = 0;

		if ((local = node->getChild(search_name, max_depth - 1)) != 0)
			return local;

		node = node->next;
	}

	return 0;
}

/*---------------------------------------------------------------------------*/

Node *Node::getNextSibling()
{
	return next;
}

/*---------------------------------------------------------------------------*/

Node *Node::getParent()
{
	return parent;
}

/*---------------------------------------------------------------------------*/

void Node::setActive(
		bool		active
	)
{
	isActive = active;
}

/*---------------------------------------------------------------------------*/

void Node::setVisible(
		bool		visible
	)
{
	isVisible = visible;
}

/*---------------------------------------------------------------------------*/

char *Node::getName()
{
	return name;
}

/*---------------------------------------------------------------------------*/

NodeType_t Node::getNodeType() const
{
	return type;
}

/*---------------------------------------------------------------------------*/

Node *Node::duplicate(
		char		*new_name,
		bool		recursively		/* True = recursively duplicate all
									 * children as well
									 */
	)
{
	char buf[256];

	if (!new_name || !(*new_name))
		new_name = Node::getNewNodeName(buf, NodeName[(u32) type], type);

	Node *n = duplicateSelf();

	n->setName(new_name);

	if (recursively) {
		Node *c = firstChild;

		if (n) {
			while (c) {
				n->attachChild(c->duplicate(c->getName(), true));
				c = c->next;
			}
		}
	}

	return n;
}

/*---------------------------------------------------------------------------*/

Node *Node::duplicateSelf()
{
	REPORT_WARNING("Node::duplicateSelf method not overloaded");
	return 0;
}

/*---------------------------------------------------------------------------*/

bool Node::isTransformable()
{
	return type == NODE_GEOMETRY || type == NODE_CAMERA ||
		type == NODE_LIGHT || type == NODE_GROUP;
}

/*---------------------------------------------------------------------------*/

char *Node::getNewNodeName(
		char			*buf,
		char			*base,
		NodeType_t		type
	)
{
	u32 idx = SceneGraph::getNodeCount(type);

	do {
		sprintf(buf, "%s%.2d", base, idx);
	} while (SceneGraph::getNode(buf));

	return buf;
}

/*---------------------------------------------------------------------------*/

void Node::logNodeInfo()
{
	u32 i, cn;
	cn = getChildrenCount();

	Console::log("Info for %s", getName());
	switch (type) {
		case NODE_CAMERA:
			Console::log("Type: Camera");
			break;
		case NODE_GROUP:
			Console::log("Type: Group");
			break;
		case NODE_LIGHT:
			Console::log("Type: Light");
			break;
		case NODE_WORLD:
			Console::log("Type: World");
			break;
		case NODE_GEOMETRY:
			Console::log("Type: Geometry");
			break;
		default:
			Console::log("Type: Unknown");
			break;
	}
	Console::log("Parent: %s", parent ? parent->getName() : "None");
	Console::log("Children (%u):", cn);
	for (i = 0; i < cn; i++) {
		Node *child;
		child = getChild(i);
		Console::log("  %s", child->getName());
	}
	Console::log(" ");
}

/*---------------------------------------------------------------------------*/

void Node::logInfo()
{
	REPORT_WARNING("No info available");
}

/*---------------------------------------------------------------------------*/

