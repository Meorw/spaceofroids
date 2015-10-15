/*
 * .pwn Parser
 *
 * Parser for the mesh format .pwn
 *
 */

#ifndef RC_PWNPARSER_H
#define RC_PWNPARSER_H

/** Parse a .pwn file and place it under the "target" node */
void PwnParse(
		Node				*target,
		const char			*filename
	);

#endif /* RC_PWNPARSER_H */
