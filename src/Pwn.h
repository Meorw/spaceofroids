
/* WORK IN PROGRESS!!! */

/*
 * .pwn Format definition
 *
 * This file functions as the actual definition of the .pwn format. Any pwn-
 * writer or reader has to comply with the format specified here.
 *
 * Node hierarchy is listed in a depth-first manner.
 *
 */

#ifndef PWN_H
#define PWN_H

#define PWN_STRING_MAX_LENGTH					256
#define PWN_GEOMETRY_MAX_MESHES					1024
#define PWN_GEOMETRY_MAX_ATTRIBUTES				16
#define PWN_GEOMETRY_MAX_ATTRIB_NAME_LENGTH		32




/*
 * The .pwn format has the following blocks:
 *
 *  - Header
 *  - Resources & Nodes
 *  - Node hierarchy
 *
 */

/*---------------------------------------------------------------------------*/

/*
 * Field				Type			Comment
 *
 */

/*---------------------------------------------------------------------------*/

/* Required */

/*
 * -- Header --
 *
 * Header prefix		uint			"pwnd"
 * Major version		ushort
 * Minor version		ushort
 *
 */

/*
 * -- String pool --
 *
 * N strings			ushort
 * Strings:				[N strings]
 *   String				char[]			null terminated
 *
 */

/* Optional */

/*
 * -- Uniform set --
 *
 * Prefix				uint			"unif"
 * N entries			ushort
 * Uniform values:		[N entries]
 *   Name (String ID)	ushort
 *   Flags				uchar			Bit 0-1: 0 = uint32, 1 = float32,
 *												 2 = texture, 3 = cubemap
 *										Bit 2-3: 0 = scalar, 1 = vector,
 *												 2 = matrix
 *										Bit 4-5: 0 = dim2, 1 = dim3, 2 = dim4
 *   Texture/CubeMap:
 *     String ID		ushort
 *   uint32/float32:
 *     N values			uchar
 *     Value(s)			varies			if scalar: N values
 *										else if vector: N values * dim
 *										else if matrix: N values * dim * dim
 *
 */

/*
 * -- Vertex Array --
 *
 * Prefix				uint			"vtxa"
 * Name (string ID)		ushort
 * N attributes			ushort
 * Attributes:			[N attributes]
 *   Name (string ID)	ushort
 *   Type size			char
 *   N components		char
 * N vertices			uint
 * Attributes			char[]			read "pad4(stride * N vertices)"
 * N indices			uint			no index array if this equals 0
 * Indices				uint/			...if N indices > 65535
 *						ushort/			...else if N indices > 255
 *						uchar			...else
 *
 */

/*
 * -- Texture --
 *
 * Prefix				uint			"texr"
 * Name (string ID)		ushort
 * File (string ID)		ushort
 *
 */

/*
 * -- Shader Program --
 *
 * Prefix				uint			"shpr"
 * Name (string ID)		ushort
 * Descr. (string ID)	ushort			Typically "Phong" or "Lambert" or such
 * Uniform set index	short			-1 for no Uniform set
 *
 */

/*
 * -- Geometry --
 *
 * Prefix				uint			"geom"
 * Name (string ID)		ushort
 * V.Array (string ID)	ushort
 * S.Prog. (string ID)	ushort
 * Uniform set index	short			-1 for no Uniform set
 *
 */

/*
 * -- Group --
 *
 * Prefix				uint			"grou"
 * Name (string ID)		ushort
 *
 */

/*
 * -- Camera --
 *
 * Prefix				uint			"cmra"
 * Name (string ID)		ushort
 * V.FOV				float			radians
 * Aspect ratio			float
 * Near plane			float
 * Far plane			float
 *
 */

/*
 * -- Light --
 *
 * Prefix				uint			"lght"
 * Name (string ID)		ushort
 * Color				float[3]		RGB
 * Intensity			float
 * Falloff radius		float
 *
 */

/*
 * -- Node hierarchy --
 *
 * Repeated:
 *   Flags				ushort			Bit 0: Translation
 *										Bit 1: Rotation
 *										Bit 2: Scale
 *   Translation		float[3]		if Flags & (1 << 0)
 *   Rotation (quat.)	float[4]		if Flags & (1 << 1)
 *   Scale				float[3]		if Flags & (1 << 2)
 *   N children			ushort			At least N Node hierarchy tables follow
 *
 */

/*===========================================================================*/

#define PWN_HEADER_MAJOR_VERSION		0
#define PWN_HEADER_MINOR_VERSION		3

#define PWN_HEADER_PREFIX				"pwnd"

#define PWN_STRING_POOL_PREFIX			"strp"
#define PWN_UNIFORM_PREFIX				"unif"

#define PWN_VARRAY_PREFIX				"vtxa"
#define PWN_TEXTURE_PREFIX				"texr"
#define PWN_SHADER_PROGRAM_PREFIX		"shpr"

#define PWN_GEOMETRY_PREFIX				"geom"
#define PWN_GROUP_PREFIX				"grou"
#define PWN_CAMERA_PREFIX				"cmra"
#define PWN_LIGHT_PREFIX				"lght"

#define PWN_END_FILE					"xxxx"

#define PWN_FLAG_TRANSLATION			(1<<0)
#define PWN_FLAG_ROTATION				(1<<1)
#define PWN_FLAG_SCALE					(1<<2)

/*---------------------------------------------------------------------------*/

#endif /* PWN_H */

