/*
 * .obj Parser
 *
 * Parser for the mesh format .obj
 *
 */

#ifndef RC_OBJPARSER_H
#define RC_OBJPARSER_H

/** Parse an .obj file and place it under the "target" node */
bool ObjParse(
		Node				*target,
		const char			*filename
	);

#endif /* RC_OBJPARSER_H */
