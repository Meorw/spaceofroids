/*
 *
 *
 */

#ifndef RC_AUXDATA_H
#define RC_AUXDATA_H

#define AUX_DEFAULT_ENTRIES_SIZE			4

typedef struct {

	char			*tag;
	void			*data;
	u32				size;

} AuxData_t;

class Auxiliary {

	public:

		Auxiliary(
				u32					entries_size = AUX_DEFAULT_ENTRIES_SIZE
			);

		~Auxiliary();


	public:

		void setData(
				char				*tag,
				void				*data,
				u32					size
			);

		void setString(
				char				*tag,
				char				*str
			);

		void remove(
				char				*name
			);

		void *getData(
				char				*tag
			);

		void *getData(
				char				*tag,
				i32					*size
			);

		char *getString(
				char				*tag
			);


		AuxData_t *getEntries();

		u32 getEntryCount() const;

		void copyTo(
				Auxiliary			*aux
			);

	private:

		i32 findEntry(
				const char			*tag
			);

		void ensureStorage();

	private:

		AuxData_t			*entries;
		u32					n_entries;
		u32					max_entries;

};

#endif /* RC_AUXDATA_H */


