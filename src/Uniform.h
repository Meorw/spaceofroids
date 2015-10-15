/*
 *
 *
 */

#ifndef RC_UNIFORM_H
#define RC_UNIFORM_H

#define UNIFORM_DEFAULT_ENTRIES_SIZE	8


		/* Types */

#define UNIFORM_FLAG_CLASS_MASK			0xFF

#define UNIFORM_FLAG_SCALAR				(1 << 0)

#define UNIFORM_FLAG_VECTOR2			(1 << 1)
#define UNIFORM_FLAG_VECTOR3			(1 << 2)
#define UNIFORM_FLAG_VECTOR4			(1 << 3)

#define UNIFORM_FLAG_MATRIX2			(1 << 4)
#define UNIFORM_FLAG_MATRIX3			(1 << 5)
#define UNIFORM_FLAG_MATRIX4			(1 << 6)

#define UNIFORM_FLAG_INT				(1 << 16)
#define UNIFORM_FLAG_FLOAT				(1 << 17)

#define UNIFORM_FLAG_TEXTURE			(1 << 18)
#define UNIFORM_FLAG_CUBEMAP			(1 << 19)


#define UNIFORM_FLAG_VECTOR				(UNIFORM_FLAG_VECTOR2 | UNIFORM_FLAG_VECTOR3 | UNIFORM_FLAG_VECTOR4)
#define UNIFORM_FLAG_MATRIX				(UNIFORM_FLAG_MATRIX2 | UNIFORM_FLAG_MATRIX3 | UNIFORM_FLAG_MATRIX4)

typedef struct {

	char			*name;

	u32				flags;
	u32				n;

	union {
		void		*ptr;
		void		*value;
		void		*handle;
		i32			loc;
	};

} UniformEntry_t;



class Uniform {

	public:

		Uniform(
				u32					entries_size = UNIFORM_DEFAULT_ENTRIES_SIZE
			);

		~Uniform();


	public:

		void setScalar(
				const char			*name,
				f32					value,
				UniformType_t		type
			);

		void setScalar(
				const char			*name,
				i32					value,
				UniformType_t		type
			);

		void setScalarArray(
				const char			*name,
				void				*list,
				u32					n_values,
				UniformType_t		type
			);


		void setVector(
				const char			*name,
				void				*vecptr,	/* Pointer to vec#$ */
				u32					dimension,	/* # = dimension */
				UniformType_t		type		/* $ = i or f */
			);

		void setVectorArray(
				const char			*name,
				void				*list,
				u32					n_values,
				u32					dimension,
				UniformType_t		type
			);

		void setMatrix(
				const char			*name,
				void				*matptr,	/* Pointer to mat#f */
				u32					dimension	/* # = dimension */
			);

		void setMatrixArray(
				const char			*name,
				void				*list,
				u32					n_values,
				u32					dimension
			);
		/* Note: Integer matrices are not supported. Floats only! */



		void setTexture(
				const char			*name,
				Texture				*tex
			);

		void setCubeMap(
				const char			*name,
				CubeMap				*cubemap
			);


		void remove(
				char				*name
			);

		UniformEntry_t *getEntries();

		u32 getEntryCount() const;

		void copyTo(
				Uniform				*uniform
			);

		void log();

	protected:

		void setTextureHandle(
				const char			*name,
				void				*tex
			);

		void setCubeMapHandle(
				const char			*name,
				void				*cubemap
			);

	private:

		void removeAt(
				u32					i
			);

		u32 mapTypeFlag(
				UniformType_t		type
			);

		UniformEntry_t *prepEntry(
				const char			*name,
				u32					size,
				u32					flags,
				u32					n
			);

		i32 findEntry(
				const char			*name
			);


	private:

		UniformEntry_t		*entries;
		u32					n_entries;
		u32					max_entries;

};

#endif /* RC_UNIFORM_H */


