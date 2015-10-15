

#include "RenderChimp.h"
#include <math.h>


/*
	Circle ring

	@param res_radius
	tessellation resolution (nbr of vertices) in the radial direction ( inner_radius < radius < outer_radius )

	@param res_theta
	tessellation resolution (nbr of vertices) in the angular direction ( 0 < theta < 2PI )

	@param inner_radius
	radius of the innermost border of the ring

	@param outer_radius
	radius of the outermost border of the ring

	@return
	pointer to the generated VertexArray object 

	CJG aug 2010 - created
*/
VertexArray* createCircleRing(const i32 &res_radius, const i32 &res_theta, const f32 &inner_radius, const f32 &outer_radius)
{
	/* vertex definition */
	struct Vertex
	{
		f32 x, y, z,			/* spatial coordinates */
			texx, texy, texz,	/* texture coordinates */
			nx, ny, nz,			/* normal vector */
			tx, ty, tz,			/* tangent vector */
			bx, by, bz;			/* binormal vector */
	};

	/* vertex array */
	Vertex	*va = new Vertex[res_radius*res_theta];

	f32 theta = 0.0f,						/* 'stepping'-variable for theta: will go 0 - 2PI */
		dtheta = 2.0f*fPI/(res_theta-1);	/* step size, depending on the resolution */

	f32	radius,													/* 'stepping'-variable for radius: will go inner_radius - outer_radius */
		dradius = (outer_radius-inner_radius)/(res_radius-1);	/* step size, depending on the resolution */

	/* generate vertices iteratively */
	for (int i = 0; i < res_theta; i++)
	{
		f32	cos_theta = cosf(theta),
			sin_theta = sinf(theta);
		radius = inner_radius;

		for (int j = 0; j < res_radius; j++)
		{
			i32 vertex_index = res_radius*i + j;

			/* vertex */
			va[vertex_index].x = radius * cos_theta;
			va[vertex_index].y = radius * sin_theta;
			va[vertex_index].z = 0.0f;
			
			/* texture coordinates */
			va[vertex_index].texx = (f32)j/(res_radius-1);
			va[vertex_index].texy = (f32)i/(res_theta-1);
			va[vertex_index].texz = 0.0f;

			/* tangent */
			vec3f t = vec3f(cos_theta,
							sin_theta,
							0);
			t.normalize();

			/* binormal */
			vec3f b = vec3f(-sin_theta,
							cos_theta,
							0);
			b.normalize();
			
			/* normal */
			vec3f n = t.cross( b );
			
			va[vertex_index].nx = n.x;
			va[vertex_index].ny = n.y;
			va[vertex_index].nz = n.z;

			va[vertex_index].tx = t.x;
			va[vertex_index].ty = t.y;
			va[vertex_index].tz = t.z;

			va[vertex_index].bx = b.x;
			va[vertex_index].by = b.y;
			va[vertex_index].bz = b.z;
			
			radius += dradius;
		}

		theta += dtheta;
	}

	/* triangle def */
	struct Triangle
	{
		i32 a, b, c;		/* vertex indices */
	};

	/* create index array */
	Triangle *ia = new Triangle[ 2*(res_theta-1)*(res_radius-1) ];

	/* generate indices iteratively */
	for (int i = 0; i < res_theta-1; i++ )
	{
		for (int j = 0; j < res_radius-1; j++ )
		{
			int tri_index = 2 * ((res_radius-1)*i + j);

			ia[tri_index].a = res_radius*i+j;
			ia[tri_index].b = res_radius*i+j + 1;
			ia[tri_index].c = res_radius*i+j + 1+res_radius;

			ia[tri_index+1].a = res_radius*i+j;
			ia[tri_index+1].b = res_radius*i+j + 1+res_radius;
			ia[tri_index+1].c = res_radius*i+j + res_radius;	
		}
	}

	/* initialize the scene graph vertex array and set attributes (to be passed to shader) */
	VertexArray *cring_va = SceneGraph::createVertexArray(0, va, 15*sizeof(f32), res_radius*res_theta, TRIANGLES, USAGE_STATIC);
	cring_va->setAttribute("Vertex", 0, 3, ATTRIB_FLOAT32);
	cring_va->setAttribute("Texcoord", 3 * sizeof(f32), 3, ATTRIB_FLOAT32);
	cring_va->setAttribute("Normal", 6 * sizeof(f32), 3, ATTRIB_FLOAT32);
	cring_va->setAttribute("Tangent", 9 * sizeof(f32), 3, ATTRIB_FLOAT32);
	cring_va->setAttribute("Binormal", 12 * sizeof(f32), 3, ATTRIB_FLOAT32);

	/* set the index array */
	cring_va->setIndexArray(ia, sizeof(i32), 2*3*(res_radius-1)*(res_theta-1) );

	/* va and ia are copied within createVertexArray so we can safely release their memory */
	delete va;
    delete ia;

	return cring_va;
}


/*
	Sphere
	@param radius
	sphere radius

	@param res_theta
	tessellation resolution (nbr of vertices) in the angular direction ( 0 < theta < 2PI )

	@param res_phi
	tessellation resolution (nbr of vertices) in the angular direction ( 0 < phi < PI )


	@return
	pointer to the generated VertexArray object 

	Author(s):
*/
VertexArray* createSphere(const i32 &res_theta, const i32 &res_phi, const f32 &radius)
{
	/* Implement me! */

	/* vertex definition */
	struct Vertex
	{
		f32 x, y, z,			/* spatial coordinates */
			texx, texy, texz,	/* texture coordinates */
			nx, ny, nz,			/* normal vector */
			tx, ty, tz,			/* tangent vector */
			bx, by, bz;			/* binormal vector */
	};

	/* vertex array */
	Vertex	*va = new Vertex[res_theta*res_phi];

	f32 theta = 0.0f,						/* 'stepping'-variable for theta: will go 0 - 2PI */
		dtheta = 2.0f*fPI/(res_theta-1);	/* step size, depending on the resolution */

	f32 phi = 0.0f,						/* 'stepping'-variable for theta: will go 0 - PI */
		dphi = 1.0f*fPI/(res_phi-1);	/* step size, depending on the resolution */
	
	/* generate vertices iteratively */
	for (int i = 0; i < res_theta; i++)
	{
		f32	cos_theta = cosf(theta),
			sin_theta = sinf(theta);
		
		phi = 0;

		for (int j = 0; j < res_phi; j++)
		{
			i32 vertex_index = res_phi*i + j;

			f32	cos_phi = cosf(phi),
				sin_phi = sinf(phi);
				

			/* vertex */
			va[vertex_index].x = radius * sin_theta * sin_phi;
			va[vertex_index].y = - radius * cos_phi;
			va[vertex_index].z = radius * cos_theta * sin_phi;

			/* tangent */
			vec3f t = vec3f(radius * cos_theta,
							0,
							- radius * sin_theta);
			t.normalize();

			/* binormal */
			vec3f b = vec3f(radius * sin_theta * cos_phi,
							radius * sin_phi,
							radius * cos_theta * cos_phi);
			b.normalize();
			
			/* normal */
			vec3f n = t.cross( b );

			
			/* texture coordinates */
			va[vertex_index].texx = i / (res_theta-1.0f); //atan2(n.x, n.z) / (2 * 3.1415f) + 0.5f;
			va[vertex_index].texy = j / (res_phi-1.0f); //asin(n.y) / 3.1415f + 0.5f;
			va[vertex_index].texz = 0.0f;
			
			va[vertex_index].nx = n.x;
			va[vertex_index].ny = n.y;
			va[vertex_index].nz = n.z;

			va[vertex_index].tx = t.x;
			va[vertex_index].ty = t.y;
			va[vertex_index].tz = t.z;

			va[vertex_index].bx = b.x;
			va[vertex_index].by = b.y;
			va[vertex_index].bz = b.z;
			
			phi += dphi;
		}

		theta += dtheta;
	}

	/* triangle def */
	struct Triangle
	{
		i32 a, b, c;		/* vertex indices */
	};

	/* create index array */
	Triangle *ia = new Triangle[ 2*(res_theta-1)*(res_phi-1) ];

	/* generate indices iteratively */
	for (int i = 0; i < res_theta-1; i++ )
	{
		for (int j = 0; j < res_phi-1; j++ )
		{
			int tri_index = 2 * ((res_phi-1)*i + j);

			ia[tri_index].a = res_phi*i+j;
			ia[tri_index].c = res_phi*i+j + 1;
			ia[tri_index].b = res_phi*i+j + 1+res_phi;

			ia[tri_index+1].a = res_phi*i+j;
			ia[tri_index+1].c = res_phi*i+j + 1+res_phi;
			ia[tri_index+1].b = res_phi*i+j + res_phi;	
		}
	}

	VertexArray *sphere_va = SceneGraph::createVertexArray(0, va, 15*sizeof(f32), res_phi*res_theta, TRIANGLES, USAGE_STATIC);
	sphere_va->setAttribute("Vertex", 0, 3, ATTRIB_FLOAT32);
	sphere_va->setAttribute("Texcoord", 3 * sizeof(f32), 3, ATTRIB_FLOAT32);
	sphere_va->setAttribute("Normal", 6 * sizeof(f32), 3, ATTRIB_FLOAT32);
	sphere_va->setAttribute("Tangent", 9 * sizeof(f32), 3, ATTRIB_FLOAT32);
	sphere_va->setAttribute("Binormal", 12 * sizeof(f32), 3, ATTRIB_FLOAT32);

	/* set the index array */
	sphere_va->setIndexArray(ia, sizeof(i32), 2*3*(res_phi-1)*(res_theta-1) );

	/* va and ia are copied within createVertexArray so we can safely release their memory */
	delete va;
    delete ia;

	return sphere_va;
	
}


/*
	Torus

	Author(s):
*/
VertexArray* createTorus(const i32 &res_theta, const i32 &res_phi, const f32 &rA, const f32 &rB)
{
	/*
	

	@param res_theta
	tessellation resolution (nbr of vertices) in the angular direction ( 0 < theta < 2PI)

	@param res_phi
	tessellation resolution (nbr of vertices) in the angular direction ( 0 < phi < 2PI )

	@param rA
	sphere radius

	@param rB
	ring thickness

	@return
	pointer to the generated VertexArray object 

	Author(s):

	/* Implement me! */

	/* vertex definition */
	struct Vertex
	{
		f32 x, y, z,			/* spatial coordinates */
			texx, texy, texz,	/* texture coordinates */
			nx, ny, nz,			/* normal vector */
			tx, ty, tz,			/* tangent vector */
			bx, by, bz;			/* binormal vector */
	};

	/* vertex array */
	Vertex	*va = new Vertex[res_theta*res_phi];

	f32 theta = 0.0f,						/* 'stepping'-variable for theta: will go 0 - 2PI */
		dtheta = 2.0f*fPI/(res_theta-1);	/* step size, depending on the resolution */

	f32 phi = 0.0f,						/* 'stepping'-variable for theta: will go 0 - PI */
		dphi = 2.0f*fPI/(res_phi-1);	/* step size, depending on the resolution */
	
	/* generate vertices iteratively */
	for (int i = 0; i < res_theta; i++)
	{
		f32	cos_theta = cosf(theta),
			sin_theta = sinf(theta);

		phi = 0;

		for (int j = 0; j < res_phi; j++)
		{
			i32 vertex_index = res_phi*i + j;

			f32	cos_phi = cosf(phi),
				sin_phi = sinf(phi);

			/* vertex */
			va[vertex_index].x = (rA + rB * cos_theta) * cos_phi;
			va[vertex_index].y = (rA + rB * cos_theta) * sin_phi;
			va[vertex_index].z = - rB * sin_theta;
			
			

			/* tangent */
			vec3f t = vec3f(- rB * sin_theta * cos_phi,
							- rB * sin_theta * sin_phi,
							- rB * cos_theta);
			t.normalize();

			/* binormal */
			vec3f b = vec3f( - (rA + rB * cos_theta) * sin_phi,
							(rA + rB * cos_theta) * cos_phi,
							0);
			b.normalize();
			
			/* normal */
			vec3f n = t.cross( b );

			/* texture coordinates */
			va[vertex_index].texx = i / (res_theta-1.0f); //atan2(n.x, n.z) / (2 * 3.1415f) + 0.5f;
			va[vertex_index].texy = j / (res_phi-1.0f); //asin(n.y) / 3.1415f + 0.5f;
			va[vertex_index].texz = 0.0f;

			va[vertex_index].nx = n.x;
			va[vertex_index].ny = n.y;
			va[vertex_index].nz = n.z;

			va[vertex_index].tx = t.x;
			va[vertex_index].ty = t.y;
			va[vertex_index].tz = t.z;

			va[vertex_index].bx = b.x;
			va[vertex_index].by = b.y;
			va[vertex_index].bz = b.z;
			
			phi += dphi;
		}

		theta += dtheta;
	}

	/* triangle def */
	struct Triangle
	{
		i32 a, b, c;		/* vertex indices */
	};

	/* create index array */
	Triangle *ia = new Triangle[ 2*(res_theta-1)*(res_phi-1) ];

	/* generate indices iteratively */
		for (int i = 0; i < res_theta-1; i++ )
	{
		for (int j = 0; j < res_phi-1; j++ )
		{
			int tri_index = 2 * ((res_phi-1)*i + j);

			ia[tri_index].a = res_phi*i+j;
			ia[tri_index].c = res_phi*i+j + 1;
			ia[tri_index].b = res_phi*i+j + 1+res_phi;

			ia[tri_index+1].a = res_phi*i+j;
			ia[tri_index+1].c = res_phi*i+j + 1+res_phi;
			ia[tri_index+1].b = res_phi*i+j + res_phi;	
		}
	}
	
	VertexArray *torus_va = SceneGraph::createVertexArray(0, va, 15*sizeof(f32), res_phi*res_theta, TRIANGLES, USAGE_STATIC);
	torus_va->setAttribute("Vertex", 0, 3, ATTRIB_FLOAT32);
	torus_va->setAttribute("Texcoord", 3 * sizeof(f32), 3, ATTRIB_FLOAT32);
	torus_va->setAttribute("Normal", 6 * sizeof(f32), 3, ATTRIB_FLOAT32);
	torus_va->setAttribute("Tangent", 9 * sizeof(f32), 3, ATTRIB_FLOAT32);
	torus_va->setAttribute("Binormal", 12 * sizeof(f32), 3, ATTRIB_FLOAT32);

	/* set the index array */
	torus_va->setIndexArray(ia, sizeof(i32), 2*3*(res_phi-1)*(res_theta-1) );

	/* va and ia are copied within createVertexArray so we can safely release their memory */
	delete va;
    delete ia;

	return torus_va;

}


