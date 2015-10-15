#include "RenderChimp.h"
#include "SceneParser.h"
#include "FloatHash.h"
#include "ObjParser.h"

typedef enum {

	RC_OBJ_IDLE = 0,

	RC_OBJ_VERTEX,
	RC_OBJ_NORMAL,
	RC_OBJ_TEXCOORD,
	RC_OBJ_FACE,
	RC_OBJ_GEO,
	RC_OBJ_MAT,

	RC_OBJ_LAST,

} OBJState_t;

#define RC_DEFAULT_OBJ_LIST_SIZE			256

#define RC_MAX_OBJECTS_IN_OBJ_FILE			512

/*---------------------------------------------------------------------------*/

bool ObjParse(
		Node				*target,
		const char			*filename
	)
{
	FloatHash *hash;

	char mat_buf[RC_MAX_OBJECTS_IN_OBJ_FILE][128];
	u32 mat_len = 0;
	bool mat_space = false;

	char name_buf[RC_MAX_OBJECTS_IN_OBJ_FILE][128];
	u32 name_len = 0;
	i32 curr_name = 0;
	u32 name_offset[RC_MAX_OBJECTS_IN_OBJ_FILE];

	u32 n_vertex_list = 0, max_vertex_list = RC_DEFAULT_OBJ_LIST_SIZE;
	vec3f *vertex_list;

	u32 n_normal_list = 0, max_normal_list = RC_DEFAULT_OBJ_LIST_SIZE;
	vec3f *normal_list;

	u32 n_texcoord_list = 0, max_texcoord_list = RC_DEFAULT_OBJ_LIST_SIZE;
	vec3f *texcoord_list;

	u32 *i_array;
	f32 *v_array;
	u32 ni = 0, nv = 0, nn = 0;

	u32 maxi = RC_DEFAULT_OBJ_LIST_SIZE;
	u32 maxv = RC_DEFAULT_OBJ_LIST_SIZE;

	u32 stride = 0;

	u32 len;
    i32 k, filelen;
	char line[4096];

#ifdef RC_ENABLE_ASSERT
	bool success;
#endif /* RC_ENABLE_ASSERT */

	bool has_texcoord = false;
	bool has_normal = false;
	bool has_vertex = false;

	FILE *f = fopen(filename, "rb");

	if (!f) {
		REPORT_WARNING("Couldn't open %s scene file", filename);
		return false;
	}

	fseek(f, 0, SEEK_END);
	filelen = ftell(f);
	fseek(f, 0, SEEK_SET);

	len = 0;

	name_buf[0][0] = 0;

	vertex_list = (vec3f *) malloc(sizeof(vec3f) * max_vertex_list);
	normal_list = (vec3f *) malloc(sizeof(vec3f) * max_normal_list);
	texcoord_list = (vec3f *) malloc(sizeof(vec3f) * max_texcoord_list);

	v_array = (f32 *) malloc(sizeof(f32) * maxv);
	i_array = (u32 *) malloc(sizeof(u32) * maxi);
		
	hash = FloatHashInit(0);

	for (k = 0; k < RC_MAX_OBJECTS_IN_OBJ_FILE;k++) {
		mat_buf[k][0] = 0;
		name_buf[k][0] = 0;
	}



	for (k = 0; k < filelen; k++) {
		char c = (char) fgetc(f);

		ASSERT(curr_name < RC_MAX_OBJECTS_IN_OBJ_FILE);

		if (c == '\n' || c == '\r') {

			/* New line hit - time to parse */

			u32 i, j;
			f32 fbuffer[3];
			u32 ibuffer[6][3];

			u32 alen = 0;
			u32 ilen = 0;
			u32 flen = 0;

			char buf[4096];

			u32 blen = 0;

			u32 expected_slashes = 0;

			OBJState_t state = RC_OBJ_IDLE;

			f32 tmp_array[9];

			line[len] = '\n';
			len++;

			for (i = 0; i < len; i++) {
				char c = line[i];
				char nc = line[i + 1];

				if (state != RC_OBJ_GEO && state != RC_OBJ_MAT) {
					/* Select state for coming chars */
					if (c == '#' || c == 's') {
						break;
					} else if (c == 'v') {
						if (nc == 't') {
							state = RC_OBJ_TEXCOORD;
							has_texcoord = true;
							continue;
						} else if (nc == 'n') {
							state = RC_OBJ_NORMAL;
							has_normal = true;
							continue;
						} else {
							state = RC_OBJ_VERTEX;
							has_vertex = true;
							continue;
						}
					} else if (c == 'f') {
						state = RC_OBJ_FACE;
						continue;
					} else if (c == 'g') {
						state = RC_OBJ_GEO;
						name_len = 0;
						continue;
					} else if (c == 'u') {
						state = RC_OBJ_MAT;
						mat_len = 0;
						mat_space = false;
						continue;
					}
				}

				switch (state) {

					case RC_OBJ_VERTEX:
					case RC_OBJ_NORMAL:
					case RC_OBJ_TEXCOORD:

						if (c == ' ' || c == '\r' || c == '\n' || c == '\t') {
							if (blen > 0) {
								if (flen >= 3) {
									REPORT_WARNING("Too many attributes");
									flen = 2;
								}
								buf[blen] = 0;
#ifdef RC_ENABLE_ASSERT
								success = parseFloat(&fbuffer[flen], buf);
#else
								parseFloat(&fbuffer[flen], buf);
#endif /* RC_ENABLE_ASSERT */
								ASSERT(success);

								flen++;

								blen = 0;
							} else {
								continue;
							}
						} else if ((c >= '0' && c <= '9') || c == '.' || c == '-'){
							buf[blen] = c;
							blen++;
							continue;
						}

						break;

					case RC_OBJ_FACE:
						if (c == ' ' || c == '\r' || c == '\n' || c == '\t') {
							if (blen > 0) {
								int num;

								if (alen >= 4) {
									REPORT_WARNING("Polygons with more than 4 vertices are not supported");
									alen = 3;
								}

								buf[blen] = 0;
#ifdef RC_ENABLE_ASSERT
								success = parseInt(&num, buf);
#else
								parseInt(&num, buf);
#endif /* RC_ENABLE_ASSERT */

								for (j = ilen; j < 3; j++) {
									ibuffer[alen][j] = num;
								}
								alen++;

								if (expected_slashes == 0) {
									expected_slashes = ilen;
								} else {
									if (expected_slashes != ilen) {
										REPORT_WARNING("Error reading %s", filename);
										break;
									}
								}

								blen = 0;
								ilen = 0;
							} else {
								continue;
							}
						} else if (c >= '0' && c <= '9'){
							buf[blen] = c;
							blen++;
							continue;
						} else if (c == '/') {
							if (blen != 0) {
								int num;
								buf[blen] = 0;
#ifdef RC_ENABLE_ASSERT
								success = parseInt(&num, buf);
#else
								parseInt(&num, buf);
#endif /* RC_ENABLE_ASSERT */
								ASSERT(success);

								for (j = ilen; j < 3; j++) {
									ibuffer[alen][j] = num;
								}
								ilen++;
							} else {
								ilen++;
							}
							blen = 0;
							continue;
						}

						break;

					case RC_OBJ_GEO:

						if (c == '\r' || c == '\n' || c == '\t' || name_len >= 127) {
							if (name_len > 0) {
								name_buf[curr_name][name_len] = 0;
							} else {
//								REPORT_WARNING("No name given");
								continue;
							}
						} else {
							if (!(c == ' ' && name_len == 0)) {
								name_buf[curr_name][name_len] = c;
								name_len++;
							}
							continue;
						}

						break;

					case RC_OBJ_MAT:

						if (curr_name - 1 < 0) {
							REPORT_WARNING("Error parsing material in .obj file");
							state = RC_OBJ_IDLE;
							continue;
						}

						if (c == '\r' || c == '\n' || c == '\t' || mat_len >= 127) {
							if (mat_len > 0) {
								mat_buf[curr_name - 1][mat_len] = 0;
							} else {
//								REPORT_WARNING("No name given");
								continue;
							}
						} else {
							if (!mat_space && c == ' ') {
								mat_space = true;
							} else if (mat_space && !(c == ' ' && mat_len == 0)) {
								mat_buf[curr_name - 1][mat_len] = c;
								mat_len++;
							}
							continue;
						}

						break;

					default:
						break;
				}

			}






			switch (state) {

				case RC_OBJ_VERTEX:
				{
					vertex_list[n_vertex_list].x = fbuffer[0];
					vertex_list[n_vertex_list].y = fbuffer[1];
					vertex_list[n_vertex_list].z = fbuffer[2];
					n_vertex_list++;

					if (n_vertex_list >= max_vertex_list) {
						max_vertex_list = max_vertex_list + (max_vertex_list >> 1); /* Grow by 50% */
						vertex_list = (vec3f *) realloc(vertex_list, sizeof(vec3f) * max_vertex_list);
					}
				}
				break;

				case RC_OBJ_NORMAL:
				{
					normal_list[n_normal_list].x = fbuffer[0];
					normal_list[n_normal_list].y = fbuffer[1];
					normal_list[n_normal_list].z = fbuffer[2];

					normal_list[n_normal_list].normalize();

					n_normal_list++;

					if (n_normal_list >= max_normal_list) {
						max_normal_list = max_normal_list + (max_normal_list >> 1); /* Grow by 50% */
						normal_list = (vec3f *) realloc(normal_list, sizeof(vec3f) * max_normal_list);
					}
				}
				break;

				case RC_OBJ_TEXCOORD:
				{
					texcoord_list[n_texcoord_list].x = fbuffer[0];
					texcoord_list[n_texcoord_list].y = fbuffer[1];
					texcoord_list[n_texcoord_list].z = fbuffer[2];
					n_texcoord_list++;

					if (n_texcoord_list >= max_texcoord_list) {
						ensure_heap();
						max_texcoord_list = max_texcoord_list + (max_texcoord_list >> 1); /* Grow by 50% */
						texcoord_list = (vec3f *) realloc(texcoord_list, sizeof(vec3f) * max_texcoord_list);
					}
				}
				break;

				case RC_OBJ_FACE:
				{
					static const u32 map[2][3] = {
							{0, 1, 2},
							{0, 2, 3}
						};

					u32 k;

					for (k = 0; k < alen - 2; k++) {
						for (i = 0; i < 3; i++) {
							u32 idx;

							if (ibuffer[map[k][i]][0] > 0) {
								idx = ibuffer[map[k][i]][0] - 1;
							} else {
								idx = nn + ibuffer[map[k][i]][0];
							}

							j = 3;

							tmp_array[0] = vertex_list[idx].x;
							tmp_array[1] = vertex_list[idx].y;
							tmp_array[2] = vertex_list[idx].z;

							if (has_texcoord) {
								if (ibuffer[map[k][i]][1] > 0) {
									idx = ibuffer[map[k][i]][1] - 1;
								} else {
									idx = nn + ibuffer[map[k][i]][1];
								}
								tmp_array[j] = texcoord_list[idx].x;
								tmp_array[j+1] = texcoord_list[idx].y;
								tmp_array[j+2] = texcoord_list[idx].z;
								j += 3;
							}

							if (has_normal) {
								if (ibuffer[map[k][i]][2] > 0) {
									idx = ibuffer[map[k][i]][2] - 1;
								} else {
									idx = nn + ibuffer[map[k][i]][2];
								}
								tmp_array[j] = normal_list[idx].x;
								tmp_array[j+1] = normal_list[idx].y;
								tmp_array[j+2] = normal_list[idx].z;
								j += 3;
							}

							{
								i32 index = FloatHashFind(hash, v_array, tmp_array, j);

								stride = j;

								if (index != -1) {
									i_array[nn] = (u32) index;

								} else {
									if (nv + j >= maxv) {
										maxv = maxv + (maxv >> 1); /* Grow by 50% */
										v_array = (f32 *) realloc(v_array, sizeof(f32) * maxv);
									}
									memcpy(&v_array[nv], tmp_array, sizeof(f32) * j);
									i_array[nn] = ni;
									FloatHashAdd(hash, v_array, nv, ni, j);
									nv += j;
									ni++;
								}

								nn++;

								if (nn >= maxi) {
									maxi = maxi + (maxi >> 1); /* Grow by 50% */
									i_array = (u32 *) realloc(i_array, sizeof(u32) * maxi);
								}

							}

						}

					}
				}
				break;
				case RC_OBJ_GEO:
				{
					name_offset[curr_name] = nn;
					curr_name++;

					if (curr_name >= RC_MAX_OBJECTS_IN_OBJ_FILE)
						REPORT_WARNING("Too many objects in .obj file");

					state = RC_OBJ_IDLE;
				}
				break;

				case RC_OBJ_MAT:
					state = RC_OBJ_IDLE;
					break;

				default:
					break;

			}



			len = 0;

		} else {

			if (len >= 4096)
				REPORT_WARNING("Line length exceeded while parsing %s", filename);

			line[len] = c;
			len++;
		}
	}

	/* File reading complete */
	fclose(f);

	stride *= sizeof(f32);

	/* Set up geometry */
	if (target->getNodeType() == NODE_GEOMETRY) {
		char namebuf[1024];
		Geometry *mesh;

		u32 offset = 0;
		VertexArray *va;

		if (name_buf[0][0] == 0) {
			sprintf(namebuf, "%s_va_%x", target->getName(), *((u32 *) &target));
		} else {
			sprintf(namebuf, "%s_va_%x", name_buf[0], *((u32 *) &target));
		}

		va = SceneGraph::createVertexArray(namebuf, v_array, stride, ni, TRIANGLES, USAGE_STATIC);
		va->setIndexArray(i_array, sizeof(u32), nn);

		va->setAttribute("Vertex", offset, 3, ATTRIB_FLOAT32, false);
		offset += 3 * sizeof(f32);

		if (has_texcoord) {
			va->setAttribute("Texcoord", offset, 3, ATTRIB_FLOAT32, false);
			offset += 3 * sizeof(f32);
		}

		if (has_normal) {
			va->setAttribute("Normal", offset, 3, ATTRIB_FLOAT32, false);
			offset += 3 * sizeof(f32);
		}

		ASSERT(offset == stride);

		mesh = (Geometry *) target;
		mesh->setVertexArray(va, false);

		if (mesh->getName() == 0) {
			mesh->setName(name_buf[0]);
		}

	} else if (target->getNodeType() == NODE_GROUP) {

		u32 s;

		u32 nsi;
		u32 nsv;
		u32 nsn;

		u32 *si_array;
		f32 *sv_array;

		name_offset[curr_name] = nn;

		sv_array = (f32 *) malloc(sizeof(f32) * maxv);
		si_array = (u32 *) malloc(sizeof(u32) * maxi);

		s = 3;

		if (has_texcoord) {
			s += 3;
		}

		if (has_normal) {
			s += 3;
		}




		for (k = 0; k < curr_name; k++) {

			u32 i;

			FloatHash *fh;

			Geometry *mesh;

			u32 offset = 0;
			VertexArray *va;


			fh = FloatHashInit(0);

			nsn = 0;
			nsi = 0;
			nsv = 0;

			for (i = name_offset[k]; i < name_offset[k + 1]; i++) {

				f32 tmp_array[9];
				u32 idx;

				idx = i_array[i];

				memcpy(tmp_array, &v_array[idx * s], sizeof(f32) * s);

				{
					i32 index = FloatHashFind(fh, sv_array, tmp_array, s);

					if (index != -1) {
						si_array[nsn] = (u32) index;

					} else {
						memcpy(&sv_array[nsv], tmp_array, sizeof(f32) * s);
						si_array[nsn] = nsi;
						FloatHashAdd(fh, sv_array, nsv, nsi, s);
						nsv += s;
						nsi++;
					}

					nsn++;

				}

			}



			offset = 0;

//			REPORT("Loaded geometry \"%s\" from \"%s\"", name_buf[k], filename);

			va = SceneGraph::createVertexArray(0, sv_array, stride, nsi, TRIANGLES, USAGE_STATIC);
			va->setIndexArray(si_array, sizeof(u32), nsn);

			va->setAttribute("Vertex", offset, 3, ATTRIB_FLOAT32, false);
			offset += 3 * sizeof(f32);

			if (has_texcoord) {
				va->setAttribute("Texcoord", offset, 3, ATTRIB_FLOAT32, false);
				offset += 3 * sizeof(f32);
			}

			if (has_normal) {
				va->setAttribute("Normal", offset, 3, ATTRIB_FLOAT32, false);
				offset += 3 * sizeof(f32);
			}

			ASSERT(offset == stride);

			mesh = SceneGraph::createGeometry(name_buf[k], va, false);
			target->attachChild(mesh);

			if (mat_buf[k][0])
				mesh->auxiliary.setString("Material", mat_buf[k]);

		}

		free(sv_array);
		free(si_array);
		
		REPORT("Found %d objects in .obj-file", curr_name);

//		mesh = SceneGraph::createGeometry("Mesh", va, false);
//		target->attachChild(mesh);

	}


	if (!has_vertex)
		REPORT_WARNING("No vertex positions found in .obj file. Expect weird results");


	/* Release temp mem */

	free(vertex_list);
	free(normal_list);
	free(texcoord_list);

	free(v_array);
	free(i_array);

	FloatHashDestroy(hash);


	REPORT("Finished loading %s.\n  Triangles: %d\n  Vertices: %d\n  Indices: %d\n\n", filename, nn / 3, ni, nn);

	return true;
}

/*---------------------------------------------------------------------------*/

