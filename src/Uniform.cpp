#include "RenderChimp.h"
#include "Uniform.h"



/*---------------------------------------------------------------------------*/

Uniform::Uniform(
		u32					default_entries_size
	)
{
	entries = (UniformEntry_t *) malloc(sizeof(UniformEntry_t) * default_entries_size);
	n_entries = 0;
	max_entries = default_entries_size;
}

/*---------------------------------------------------------------------------*/

Uniform::~Uniform()
{
	u32 n;
	u32 i;

	n = n_entries;

	for (i = 0; i < n; i++) {
		removeAt(0);
	}

	ASSERT(n_entries == 0);

	free(entries);

}

/*---------------------------------------------------------------------------*/

u32 Uniform::mapTypeFlag(
		UniformType_t		type
	)
{
	u32 flags = 0;

	switch (type) {
		default:
			REPORT_WARNING("Unrecognized type");
			break;
		case UNIFORM_INT32:
			flags |= UNIFORM_FLAG_INT;
			break;
		case UNIFORM_FLOAT32:
			flags |= UNIFORM_FLAG_FLOAT;
			break;
	}

	return flags;
}

/*---------------------------------------------------------------------------*/

UniformEntry_t *Uniform::prepEntry(
		const char			*name,
		u32					size,
		u32					flags,
		u32					n
	)
{
	u32 i;

	for (i = 0; i < n_entries; i++) {
		if (strcmp(entries[i].name, name) == 0) {
			if (entries[i].flags != flags || entries[i].n != n) {

				removeAt(i);
				i--;
				continue;

			} else {

				return &entries[i];

			}
		}
	}

	if (n_entries >= max_entries) {
		max_entries = max_entries + (max_entries >> 2); /* Grow by 25% */
		entries = (UniformEntry_t *) realloc(entries, sizeof(UniformEntry_t) * max_entries);
	}

	entries[i].name = duplicateString(name);

	if (size > 1)
		entries[i].ptr = (void *) malloc(sizeof(u32) * size);

	n_entries++;

	return &entries[i];
}

/*---------------------------------------------------------------------------*/

i32 Uniform::findEntry(
		const char			*name
	)
{
	u32 i;

	for (i = 0; i < n_entries; i++) {
		if (strcmp(entries[i].name, name) == 0) {
			return (i32) i;
		}
	}

	return -1;
}

/*---------------------------------------------------------------------------*/

void Uniform::setScalar(
		const char			*name,
		f32					value,
		UniformType_t		type
	)
{
	UniformEntry_t *entry;
	u32 flags;

	flags = UNIFORM_FLAG_SCALAR | mapTypeFlag(type);

	entry = prepEntry(name, 1, flags, 1);
	switch (type) {
		case UNIFORM_FLOAT32: {
			f32 fv = (f32)value;
			entry->value = *((void **)&fv);
			break;
		}
		default:
		case UNIFORM_INT32: {
			i32 iv = (i32)value;
			entry->value = (void *)iv;
			break;
		}
	}

	entry->n = 1;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setScalar(
		const char			*name,
		i32					value,
		UniformType_t		type
	)
{
	UniformEntry_t *entry;
	u32 flags;

	flags = UNIFORM_FLAG_SCALAR | mapTypeFlag(type);

	entry = prepEntry(name, 1, flags, 1);
	switch (type) {
		case UNIFORM_FLOAT32: {
			f32 fv = (f32)value;
			entry->value = *((void **)&fv);
			break;
		}
		default:
		case UNIFORM_INT32: {
			i32 iv = (i32)value;
			entry->value = (void *)iv;
			break;
		}
	}

	entry->n = 1;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setScalarArray(
		const char			*name,
		void				*list,
		u32					n_values,
		UniformType_t		type
	)
{
	UniformEntry_t *entry;
	u32 flags;

	if (n_values == 1) {
		if (type == UNIFORM_FLOAT32) {
			setScalar(name, *((f32 *) list), type);
		} else {
			setScalar(name, *((i32 *) list), type);
		}

		return;
	}

	flags = UNIFORM_FLAG_SCALAR | mapTypeFlag(type);

	entry = prepEntry(name, n_values, flags, n_values);

	memcpy(entry->ptr, list, sizeof(u32) * n_values);
	entry->n = n_values;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setVector(
		const char			*name,
		void				*vecptr,
		u32					dimension,
		UniformType_t		type
	)
{
	UniformEntry_t *entry;
	u32 size;
	u32 flags;

	flags = mapTypeFlag(type);

	switch (dimension) {
		
		case 1:
			if (type == UNIFORM_FLOAT32) { 
				setScalar(name, ((f32 *) vecptr)[0], type);
			} else { /* UNIFORM_INT32 */
				setScalar(name, ((i32 *) vecptr)[0], type);
			}
			return;
		default:
			REPORT_WARNING("Vector dimension must be 2, 3 or 4, but was %d", dimension);
			dimension = 2;
		case 2:
			flags |= UNIFORM_FLAG_VECTOR2;
			break;
		case 3:
			flags |= UNIFORM_FLAG_VECTOR3;
			break;
		case 4:
			flags |= UNIFORM_FLAG_VECTOR4;
			break;
	}

	size = dimension;

	entry = prepEntry(name, size, flags, 1);

	memcpy(entry->ptr, vecptr, sizeof(u32) * size);
	entry->n = 1;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setVectorArray(
		const char			*name,
		void				*list,
		u32					n_values,
		u32					dimension,
		UniformType_t		type
	)
{
	UniformEntry_t *entry;
	u32 size;
	u32 flags;

	if (n_values == 1) {
		setVector(name, list, dimension, type);
		return;
	}

	flags = mapTypeFlag(type);

	switch (dimension) {
		
		case 1:
			setScalarArray(name, list, n_values, type);
			return;
		default:
			REPORT_WARNING("Vector dimension must be 2, 3 or 4, but was %d", dimension);
			dimension = 2;
		case 2:
			flags |= UNIFORM_FLAG_VECTOR2;
			break;
		case 3:
			flags |= UNIFORM_FLAG_VECTOR3;
			break;
		case 4:
			flags |= UNIFORM_FLAG_VECTOR4;
			break;
	}

	size = dimension * n_values;

	entry = prepEntry(name, size, flags, n_values);

	memcpy(entry->ptr, list, sizeof(u32) * size);
	entry->n = n_values;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setMatrix(
		const char			*name,
		void				*matptr,
		u32					dimension
	)
{
	UniformEntry_t *entry;
	u32 size;
	u32 flags;

	flags = UNIFORM_FLAG_FLOAT;

	switch (dimension) {
		
		default:
			REPORT_WARNING("Matrix dimension must be 2, 3 or 4, but was %d", dimension);
			dimension = 2;
		case 2:
			flags |= UNIFORM_FLAG_MATRIX2;
			break;
		case 3:
			flags |= UNIFORM_FLAG_MATRIX3;
			break;
		case 4:
			flags |= UNIFORM_FLAG_MATRIX4;
			break;
	}

	size = dimension * dimension;

	entry = prepEntry(name, size, flags, 1);

	memcpy(entry->ptr, matptr, sizeof(u32) * size);
	entry->n = 1;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setMatrixArray(
		const char			*name,
		void				*list,
		u32					n_values,
		u32					dimension
	)
{
	UniformEntry_t *entry;
	u32 size;
	u32 flags;

	if (n_values == 1) {
		setMatrix(name, list, dimension);
		return;
	}

	flags = UNIFORM_FLAG_FLOAT;

	switch (dimension) {
		
		default:
			REPORT_WARNING("Matrix dimension must be 2, 3 or 4, but was %d", dimension);
			dimension = 2;
		case 2:
			flags |= UNIFORM_FLAG_MATRIX2;
			break;
		case 3:
			flags |= UNIFORM_FLAG_MATRIX3;
			break;
		case 4:
			flags |= UNIFORM_FLAG_MATRIX4;
			break;
	}

	size = dimension * dimension * n_values;

	entry = prepEntry(name, size, flags, n_values);

	memcpy(entry->ptr, list, sizeof(u32) * size);
	entry->n = n_values;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setTexture(
		const char			*name,
		Texture				*texture
	)
{
	UniformEntry_t *entry;
	u32 flags;

	flags = UNIFORM_FLAG_TEXTURE;

	entry = prepEntry(name, 1, flags, 1);

	entry->handle = texture->getHandle();
	entry->n = 1;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setCubeMap(
		const char			*name,
		CubeMap				*cubemap
	)
{
	UniformEntry_t *entry;
	u32 flags;

	flags = UNIFORM_FLAG_CUBEMAP;

	entry = prepEntry(name, 1, flags, 1);

	entry->handle = cubemap->getHandle();
	entry->n = 1;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setTextureHandle(
		const char			*name,
		void				*texture
	)
{
	UniformEntry_t *entry;
	u32 flags;

	flags = UNIFORM_FLAG_TEXTURE;

	entry = prepEntry(name, 1, flags, 1);

	entry->handle = texture;
	entry->n = 1;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::setCubeMapHandle(
		const char			*name,
		void				*cubemap
	)
{
	UniformEntry_t *entry;
	u32 flags;

	flags = UNIFORM_FLAG_CUBEMAP;

	entry = prepEntry(name, 1, flags, 1);

	entry->handle = cubemap;
	entry->n = 1;
	entry->flags = flags;
}

/*---------------------------------------------------------------------------*/

void Uniform::removeAt(
		u32					i
	)
{
	if (entries[i].n > 1 ||
		!(entries[i].flags & UNIFORM_FLAG_TEXTURE ||
		entries[i].flags & UNIFORM_FLAG_CUBEMAP ||
		entries[i].flags & UNIFORM_FLAG_SCALAR))
	{
		free(entries[i].ptr);
	}

	free(entries[i].name);

	while (i < n_entries - 1) {
		entries[i] = entries[i + 1];
		i++;
	}

	n_entries--;
}

/*---------------------------------------------------------------------------*/

void Uniform::remove(
		char				*name
	)
{
	u32 i;

	for (i = 0; i < n_entries; i++) {

		if (strcmp(name, entries[i].name) == 0) {

			removeAt(i);

			return;
		}

	}

}

/*---------------------------------------------------------------------------*/

UniformEntry_t *Uniform::getEntries()
{
	return entries;
}

/*---------------------------------------------------------------------------*/

u32 Uniform::getEntryCount() const
{
	return n_entries;
}

/*---------------------------------------------------------------------------*/

void Uniform::copyTo(
		Uniform				*uniform
	)
{
	u32 i;

	for (i = 0; i < n_entries; i++) {
		UniformEntry_t *u = &entries[i];

		if (u->flags & UNIFORM_FLAG_TEXTURE) {
			uniform->setTextureHandle(u->name, u->handle);

		} else if (u->flags & UNIFORM_FLAG_CUBEMAP) {
			uniform->setCubeMapHandle(u->name, u->handle);

		} else if (u->flags & UNIFORM_FLAG_SCALAR) {
			if (u->n > 1) {
				UniformType_t t = (u->flags & UNIFORM_FLAG_INT) ? UNIFORM_INT32 : UNIFORM_FLOAT32;
				uniform->setScalarArray(u->name, u->ptr, u->n, t);
			} else {
				if (u->flags & UNIFORM_FLAG_INT) {
					uniform->setScalar(u->name, *((i32 *) &u->value), UNIFORM_INT32);
				} else {
					uniform->setScalar(u->name, *((f32 *) &u->value), UNIFORM_FLOAT32);
				}
			}
		} else if (u->flags & UNIFORM_FLAG_VECTOR) {
			u32 dim = u->flags & UNIFORM_FLAG_VECTOR2 ? 2 : (u->flags & UNIFORM_FLAG_VECTOR3 ? 3 : 4);
			UniformType_t t = (u->flags & UNIFORM_FLAG_INT) ? UNIFORM_INT32 : UNIFORM_FLOAT32;

			if (u->n > 1) {
				uniform->setVectorArray(u->name, u->ptr, u->n, dim, t);
			} else {
				uniform->setVector(u->name, u->ptr, dim, t);
			}

		} else if (u->flags & UNIFORM_FLAG_MATRIX) {
			u32 dim = u->flags & UNIFORM_FLAG_MATRIX2 ? 2 : (u->flags & UNIFORM_FLAG_MATRIX3 ? 3 : 4);

			if (u->n > 1) {
				uniform->setMatrixArray(u->name, u->ptr, u->n, dim);
			} else {
				uniform->setMatrix(u->name, u->ptr, dim);
			}
		}
	}

}

/*---------------------------------------------------------------------------*/

void Uniform::log()
{
	u32 i, j;

	for (i = 0; i < n_entries; i++) {
		UniformEntry_t *u = &entries[i];

		if (u->flags & UNIFORM_FLAG_TEXTURE) {
			Console::log("Texture %s = %x", u->name, *((u32 *) &u->handle));

		} else if (u->flags & UNIFORM_FLAG_CUBEMAP) {
			Console::log("CubeMap %s = %x", u->name, *((u32 *) &u->handle));

		} else if (u->flags & UNIFORM_FLAG_SCALAR) {
			if (u->n > 1) {
				UniformType_t t = (u->flags & UNIFORM_FLAG_INT) ? UNIFORM_INT32 : UNIFORM_FLOAT32;

				if (t == UNIFORM_INT32) {
					i32 *p;
					p = *((i32 **) &u->ptr);
					Console::log("int[%d] %s = ", u->n, u->name);
					for (j = 0; j < u->n; j++) {
						Console::log("  %d", *p);
						p++;
					}
				} else {
					f32 *p;
					p = *((f32 **) u->ptr);
					Console::log("float[%d] %s = ", u->n, u->name);
					for (j = 0; j < u->n; j++) {
						Console::log("  %.8f", *p);
						p++;
					}
				}
			} else {
				if (u->flags & UNIFORM_FLAG_INT) {
					Console::log("int %s = %d", u->name, *((i32 *) &u->value));
				} else {
					Console::log("float %s = %.8f", u->name, *((f32 *) &u->value));
				}
			}
		} else if (u->flags & UNIFORM_FLAG_VECTOR) {
			u32 dim = u->flags & UNIFORM_FLAG_VECTOR2 ? 2 : (u->flags & UNIFORM_FLAG_VECTOR3 ? 3 : 4);
			UniformType_t t = (u->flags & UNIFORM_FLAG_INT) ? UNIFORM_INT32 : UNIFORM_FLOAT32;

			if (u->n > 1) {
				if (t == UNIFORM_INT32) {
					i32 *p;
					p = *((i32 **) &u->ptr);
					Console::log("int%d[%d] %s = ", dim, u->n, u->name);
					for (j = 0; j < u->n; j++) {
						switch (dim) {
							default:
							case 2:
								Console::log("  [%d, %d]", p[0], p[1]);
								p += 2;
							break;
							case 3:
								Console::log("  [%d, %d, %d]", p[0], p[1], p[2]);
								p += 3;
							break;
							case 4:
								Console::log("  [%d, %d, %d, %d]", p[0], p[1], p[2], p[3]);
								p += 4;
							break;
						}
					}
				} else {
					f32 *p;
					p = *((f32 **) &u->ptr);
					Console::log("vec%d[%d] %s = ", dim, u->n, u->name);
					for (j = 0; j < u->n; j++) {
						switch (dim) {
							default:
							case 2:
								Console::log("  [%.8f, %.8f]", p[0], p[1]);
								p += 2;
							break;
							case 3:
								Console::log("  [%.8f, %.8f, %.8f]", p[0], p[1], p[2]);
								p += 3;
							break;
							case 4:
								Console::log("  [%.8f, %.8f, %.8f, %.8f]", p[0], p[1], p[2], p[3]);
								p += 4;
							break;
						}
					}
				}
			} else {
				if (t == UNIFORM_INT32) {
					i32 *p;
					p = *((i32 **) &u->ptr);
					switch (dim) {
						default:
						case 2:
							Console::log("int2 %s = [%d, %d]", u->name, p[0], p[1]);
						break;
						case 3:
							Console::log("int3 %s = [%d, %d, %d]", u->name, p[0], p[1], p[2]);
						break;
						case 4:
							Console::log("int4 %s = [%d, %d, %d, %d]", u->name, p[0], p[1], p[2], p[3]);
						break;
					}
				} else {
					f32 *p;
					p = *((f32 **) &u->ptr);
					switch (dim) {
						default:
						case 2:
							Console::log("vec2 %s = [%.8f, %.8f]", u->name, p[0], p[1]);
						break;
						case 3:
							Console::log("vec3 %s = [%.8f, %.8f, %.8f]", u->name, p[0], p[1], p[2]);
						break;
						case 4:
							Console::log("vec4 %s = [%.8f, %.8f, %.8f, %.8f]", u->name, p[0], p[1], p[2], p[3]);
						break;
					}
				}
			}

		} else if (u->flags & UNIFORM_FLAG_MATRIX) {
			u32 dim = u->flags & UNIFORM_FLAG_MATRIX2 ? 2 : (u->flags & UNIFORM_FLAG_MATRIX3 ? 3 : 4);

			if (u->n > 1) {

				f32 *p;
				p = *((f32 **) &u->ptr);
				Console::log("mat%d[%d] %s = ", dim, u->n, u->name);
				for (j = 0; j < u->n; j++) {
					switch (dim) {
						default:
						case 2:
							Console::log("  [%.8f %.8f", p[0], p[1]);
							Console::log("   %.8f %.8f]", p[2], p[3]);
							p += 4;
						break;
						case 3:
							Console::log("  [%.8f %.8f %.8f", p[0], p[1], p[2]);
							Console::log("   %.8f %.8f %.8f", p[3], p[4], p[5]);
							Console::log("   %.8f %.8f %.8f]", p[6], p[7], p[8]);
							p += 9;
						break;
						case 4:
							Console::log("  [%.8f %.8f %.8f %.8f", p[0], p[1], p[2], p[3]);
							Console::log("   %.8f %.8f %.8f %.8f", p[4], p[5], p[6], p[7]);
							Console::log("   %.8f %.8f %.8f %.8f", p[8], p[9], p[10], p[11]);
							Console::log("   %.8f %.8f %.8f %.8f]", p[12], p[13], p[14], p[15]);
							p += 16;
						break;
					}
				}
			} else {
				f32 *p;
				p = *((f32 **) &u->ptr);
				switch (dim) {
					default:
					case 2:
						Console::log("mat2 %s = [%.8f %.8f", u->name, p[0], p[1]);
						Console::log("           %.8f %.8f]", p[2], p[3]);
					break;
					case 3:
						Console::log("mat3 %s = [%.8f %.8f %.8f", u->name, p[0], p[1], p[2]);
						Console::log("           %.8f %.8f %.8f", p[3], p[4], p[5]);
						Console::log("           %.8f %.8f %.8f]", p[6], p[7], p[8]);
					break;
					case 4:
						Console::log("mat4 %s = [%.8f %.8f %.8f %.8f", u->name, p[0], p[1], p[2], p[3]);
						Console::log("           %.8f %.8f %.8f %.8f", p[4], p[5], p[6], p[7]);
						Console::log("           %.8f %.8f %.8f %.8f", p[8], p[9], p[10], p[11]);
						Console::log("           %.8f %.8f %.8f %.8f]", p[12], p[13], p[14], p[15]);
					break;
				}
			}
		}
	}
}

/*---------------------------------------------------------------------------*/
