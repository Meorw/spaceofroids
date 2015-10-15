#include "RenderChimp.h"
#include "Resource.h"
#include "ResourceList.h"
#include "Command.h"
#include "ShaderProgram.h"

#define SHADER_PROGRAM_ERROR_BUFFER_LENGTH			16384

/*---------------------------------------------------------------------------*/

ShaderProgram::ShaderProgram(
		const char			*nidentifier,

		const char			*filename_base,
		char				*prefix

	) : Resource(nidentifier, NO_PURGE, RESOURCE_SHADER_PROGRAM)
{
	handle = 0;

	if (prefix && *prefix) {
		shader_prefix = duplicateString(prefix);
	} else {
		shader_prefix = 0;
	}

	shader_description = duplicateString(filename_base);

	if (!Renderer::loadShaderProgram(this, shader_description, shader_prefix)) {

		Renderer::unloadShaderProgram(this);

		if (!Renderer::loadShaderProgram(this, "Default", 0))
			broken = true;

	}

}

/*---------------------------------------------------------------------------*/

ShaderProgram::~ShaderProgram()
{
	Renderer::unloadShaderProgram(this);

	if (shader_prefix)
		free(shader_prefix);

	if (shader_description)
		free(shader_description);

}

/*---------------------------------------------------------------------------*/

void ShaderProgram::recompile()
{
	Renderer::unloadShaderProgram(this);

	if (!Renderer::loadShaderProgram(this, shader_description, shader_prefix)) {
		REPORT("Reverting to default shader program. Failed to compile \"%s\" for \"%s\"", shader_description, identifier);
		Renderer::loadShaderProgram(this, "Default", 0);
	} else {
		REPORT("Shader program \"%s\" successfully recompiled for \"%s\"", shader_description, identifier);
	}
}

/*---------------------------------------------------------------------------*/

void *ShaderProgram::getHandle()
{
	return handle;
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setHandle(
		void				*nhandle
	)
{
	handle = nhandle;
}

/*---------------------------------------------------------------------------*/

RenderState *ShaderProgram::getRenderState()
{
	return &renderState;
}

/*---------------------------------------------------------------------------*/

Uniform *ShaderProgram::getUniform()
{
	return &uniform;
}

/*---------------------------------------------------------------------------*/

bool ShaderProgram::hasUniform(
		const char			*name
	)
{
	return Renderer::getShaderHasUniform(this, name);
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setScalar(
		const char			*name,
		f32					value,
		UniformType_t		type
	)
{
	uniform.setScalar(name, value, type);
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setScalar(
		const char			*name,
		i32					value,
		UniformType_t		type
	)
{
	uniform.setScalar(name, value, type);
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setScalarArray(
		const char			*name,
		void				*list,
		u32					n_values,
		UniformType_t		type
	)
{
	uniform.setScalarArray(name, list, n_values, type);
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setVector(
		const char			*name,
		void				*vecptr,	/* Pointer to vec#$ */
		u32					dimension,	/* # = dimension */
		UniformType_t		type		/* $ = i or f */
	)
{
	uniform.setVector(name, vecptr, dimension, type);
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setVectorArray(
		const char			*name,
		void				*list,
		u32					n_values,
		u32					dimension,
		UniformType_t		type
	)
{
	uniform.setVectorArray(name, list, n_values, dimension, type);
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setMatrix(
		const char			*name,
		void				*matptr,	/* Pointer to mat#f */
		u32					dimension	/* # = dimension */
	)
{
	uniform.setMatrix(name, matptr, dimension);
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setMatrixArray(
		const char			*name,
		void				*list,
		u32					n_values,
		u32					dimension
	)
{
	uniform.setMatrixArray(name, list, n_values, dimension);
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setTexture(
		const char			*name,
		Texture				*tex
	)
{
	uniform.setTexture(name, tex);
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::setCubeMap(
		const char			*name,
		CubeMap				*cubemap
	)
{
	uniform.setCubeMap(name, cubemap);
}

/*---------------------------------------------------------------------------*/

#ifdef DEBUG

void ShaderProgram::print(
		FILE		*f
	)
{
//	char *c = shader[0]->getIdentityStringCopy();
//	char *d = shader[1]->getIdentityStringCopy();

#ifdef RC_LOG_TO_FILE
	#define prnt0(a)		fprintf(f, a)
	#define prnt1(a, b)		fprintf(f, a, b)
#else /* RC_LOG_TO_FILE */
	#define prnt0(a)		printf(a)
	#define prnt1(a, b)		printf(a, b)
#endif /* RC_LOG_TO_FILE */

//	prnt1("    [ V.SHADER ][ %s ]\n", c);
//	prnt1("    [ F.SHADER ][ %s ]\n", d);
	//prnt1("    [ GL ID    ][ %u ]\n", id);

//	free(c);
//	free(d);
}

#endif /* DEBUG */

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_shader_recompile = {
	"/shader_recompile"
};

bool CMD_shader_recompile(
		u32		n_args,
		char	**args
	)
{
	u32 n, i;
	ShaderProgram **shader_list;

	n = SceneGraph::getResourceCount(RESOURCE_SHADER_PROGRAM);

	shader_list = (ShaderProgram **) malloc(sizeof(ShaderProgram *) * n);

	SceneGraph::getResourceList((Resource **) shader_list, RESOURCE_SHADER_PROGRAM);

	for (i = 0; i < n; i++)
		shader_list[i]->recompile();

	free(shader_list);

	return true;
}

/*---------------------------------------------------------------------------*/

void ShaderProgram::registerCommands()
{
	Command::add("shader_recompile", CMDHELP_shader_recompile, CMD_shader_recompile);
}

/*---------------------------------------------------------------------------*/
