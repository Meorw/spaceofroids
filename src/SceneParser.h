/*
 * .obj Parser
 *
 * Parser for the mesh format .obj
 *
 */

#ifndef RC_SCENEPARSER_H
#define RC_SCENEPARSER_H

/**
 * Parse an .obj file and place it under the "target" node, which must b either
 * a Group node or a Geometry node.
 */
void ParseScene(
		Node				*target,
		const char			*filename
	);


/* Common file reading functions */
/* LOWPRIO: I should just use fread... */

u8 read_u8(
		FILE	*f
	);

u16 read_u16(
		FILE	*f
	);

u32 read_u32(
		FILE	*f
	);

f32 read_f32(
		FILE	*f
	);

void read_str(
		FILE				*f,
		char				*str
	);

void read_str(
		FILE				*f,
		char				*str,
		u32					len
	);


#endif /* RC_SCENEPARSER_H */
