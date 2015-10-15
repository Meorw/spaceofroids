
/**

\page licensepage License agreement

 Copyright (c) 2009-2010 Magnus Andersson

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*/



/**

\mainpage RenderChimp
\verbatim
    _
   c "    -[ RenderChimp ]-
    (-

\endverbatim
Written by Magnus Andersson (magnusa@cs.lth.se)
 
With the assistance of Rasmus Barringer, Michael Doggett, Carl-Johan Gribel,
Björn Johnsson & Tomas Akenine-Möller

\ref licensepage "MIT license agreement" applies

\n

\section tocsec Table of Contents

\ref intropage

\ref getstartpage

\ref systempage

\ref samplepage

\ref faqpage

\ref bugspage

\ref todopage

\ref wishpage


\n

*/

/*
 * Introduction
 *
 */

/**

\page intropage Introduction

RenderChimp (RC) was originally written to ease the development of cross
platform (Win32/iPhone) graphics applications for educational purposes.
Initially it was little more than a scene graph, encapsulating calls to OpenGL
ES 2.0. RC has since evolved towards a more general graphics engine, and
continues to do so.

With this in mind, please read the documentation for each new function that you
use carefully. Because of the evolving nature of RC, some functions may be
untested, place holders, unoptimized or have limited functionality. This is
(most often) noted in the documentation accompanying the function declaration
in the source code. (Please report flat out bugs not mentioned in the function
description to magnusa@cs.lth.se.)

RC has previously been used in Introduction to Computer Graphics and Mobile
Graphics at Lunds Tekniska Högskola.

*/

/**

\page systempage System Overview

To understand the role of RC we begin with a very high level and simplified
diagram;

\verbatim
                 /--> [ RenderChimp ]
                /            |
[ Application ]              |
                \            v
                 \--> [ RC::Renderer ] --> [ Low-level graphics ]
                                            (OpenGL)
                                            (OpenGL ES)
                                            (Software ray tracer)
                                            (...)

\endverbatim

The first thing to notice is that the Application has no direct contact with
the underlying, low-level graphics API. All the rendering calls and state
changes goes through a Renderer which is responsible for handling and relaying
rendering tasks. Different Renderers can use wildly different rendering
techniques. Probably the most commonly used are the different OpenGL renderers,
but to this date there is also a Cuda ray tracer and a software rasterizer. New
renderers can be added, and the only requirement is that they comply with the
set of functions specified in Renderer.h. It cannot be expected that all
platforms will be able to run all different types of renderers. Invalid
combinations should result in compile time errors.

It is important that the Application never directly calls the low-level
graphics API since this can cause undefined behavior and crashes.

(Important note: As of yet, the Renderer is not static. You must call
Renderer:: to access its members and functions. This will be fixed in the
future.)

Next we take closer looks at the different sub systems of RenderChimp.

\ref sgraphsec

\ref shadesec

\todo More to come!

\page sgraphsec Scene Graph

The main purpose of RC is to encapsulate different graphics application
building blocks and to provide a scene graph to ease the handling of these.
The building blocks are separated in to two groups that inherit from
different base classes - Node and Resource.

Nodes are scene elements like light sources and geometry, and are
structured in the scene graph in a tree-like manner.

Resources hold instantiatable data such as textures, vertex arrays and shader
programs. Resources are typically used by nodes and other resources.

For example, a texture may be used by many meshes (Geometry nodes) in a scene.
Instead of duplicating the texture data over and over, the geometry nodes
reference a common texture.

The nodes and resources cannot be created by calling a constructor. Instead
these objects are spawned by using the SceneGraph::create...-functions.
Similarly, you should not call delete to destroy these objects, but use
SceneGraph::deleteNode() and SceneGraph::dereferenceResource() respectively.
The SceneGraph-class contains many useful scene graph handling functions, so be
sure to check those out.

A brief summary of the different nodes and resources follow.

\section nodessec Nodes

The node types that can be used to describe a scene are listen below. The nodes
share functionality through inheritance. Abstract nodes are denoted with < >,
and cannot be directly created.

\verbatim

         <Node>

            |

        /       \

    World     <Transformable>

                     |

          /       /     \      \

      Group  Camera     Light  Geometry

\endverbatim


Node - This is the base class. All nodes inherit from this node type. It mostly
contains functionality to handle graph structure, such as attaching and
detaching child nodes, etc, which is common to all nodes.

World - The root object of your scene graph. Calling renderAll() on this object
will begin the traversal and rendering of the whole scene graph. You can have
any number of World nodes, and it is not required to have it in your scene
graph structure at all (but then you have to set the renderer specifics
yourself before rendering).

Transformable - Base class for nodes that can be transformed (rotated,
translated and scaled). Contains many helper functions for extracting
information about position, facing, etc. Transformations applied to a parent
that is a Transformable will be inherited by the child nodes.

Group - Doesn't really do much. Use this to stack transformations.

Camera - A camera with orthogonal, perspective or free projection. If you have
a World node at the base of your scene graph, you can set the active Camera on
that for a renderAll-call. (Alternatively set Camera by calling
Renderer::setMatrices() (overridden by World's camera)).

Light - Describes a simple point light with intensity, color and position. Also
note that since this is a transformable, direction can also be extracted, for
use in spotlights.

Geometry - Describes a mesh with acompanying shader programs. Holds a
VertexArray and at least one ShaderProgram. It also has its own set of Uniform
values that override those of the shader programs attached to it.

\section resssec Resources

Resources are "floating around" a little more than nodes. They are not
organized in the scene tree in the same manner as nodes and don't have a child
parent relation. Basically, a resource is a container for a chunk of data that
is used by multiple objects. Instead of creating many copies, the resource list
system keeps track of all the resources and only allow one copy of each unique
data set.

The available resources are;

Vertex array - A vertex array is a list of per-vertex attributes such as
position, normal, color, texture coordinate, etc. These are organized in an
"array of structure" manner, which means that all of the attributes for one
vertex are grouped together in the list (as opposed to having one whole
array for each attribute type (= non-local memory access)). However, sometimes
we don't want to duplicate all of the vertex attributes for each of the
triangles in a mesh. An index array can optionally be added, that points to the
indices in the vertex array to use for each triangle.

ShaderProgram - A shader program is something that the Renderer interprets and
applies to the Geometry nodes that are sent down for rendering. For OpenGL 2.0
this would be a vertex- and a fragment program. The ShaderProgram also has a
set of values that it can relay to the Renderer on a per-geometry basis. These
are handled by the enclosed Uniform member. Geometry nodes also have Uniform
objects and can override values set per shader program. The ShaderProgram also
holds a RenderState, which controls blending, cull mode, depth tests/write,
etc.

Texture - Textures can be used for a variety of purposes. Commonly they contain
information like diffuse color, normals and specularity. Textures can be added
to Geometry and ShaderPrograms (because they hold Uniform objects), and are
thus relayed to the Renderer upon rendering a Geometry node. Textures are also
contained in RenderTargets and can be extracted and used like regular textures.

CubeMap - Similar to the Texture resouce, but has 6 images for each of the
directions. Good for skyboxes and reflective effects. Can also be relayed to
the Renderer through Geometry and ShaderPrograms.

RenderTarget - Sets up a number of textures that can be rendered to, and
optionally a depth buffer as well. To switch render target you must explicitly
call Renderer::setRenderTarget() (pass 0 as argument for screen buffer).

\page shadesec Shader System

As previously described, RC has support for a variety of renderer backends.
Each of these can have vastly different underlying systems for handling
shaders. RC tries to incorporate a fairly simple and uniform way to handle
shading functionality from the applications perspective.

\subsection shadesetupssec Setting Up a New Shader Program

A new shader program can be created using SceneGraph::createShaderProgram().
The main parameter to this function call is a simple string, the shader_name.
The shader_name will be passed to the active Renderer which is responsible for
interpreting the string and make something useful of it. Depending on the
Renderer's underlying shading system, this interpretation can vary.

Let's consider the OpenGL 2.0 renderer as an example. This Renderer will look
for the shader_name.gl20.vs and shader_name.gl20.fs (vertex- and fragment
shader source) in the res/shaders/gl20/ folder. If the files exists, it will
compile and link these two shaders in to a shader program, and then it's ready
for use.

The ShaderProgram resource that results from the create-call is applied to
Geometry nodes and used when those nodes are rendered.

\subsection shadevaluessec Relaying Information to a Shader Program

Once the shader program is properly set up, there must be some means of
populating the variables in the shader program from the application side. The
communication is done through uniform variables.

In RC two classes can carry uniform values - the ShaderProgram resource and the
Geometry node. The reason for this is that some uniforms should be applied on a
shader program-basis, and others on a mesh-basis.

Take a look at the Uniform class to see which uniform types that can be used.
(The Geometry- and ShaderProgram classes both have the same set of functions
found in Uniform, so you don't have to fetch the any Uniform-object to set
the uniform values.)

When a Geometry node is sent for rendering, it will first relay all of the
attached ShaderProgram's uniform values, and then the uniforms carried by
the Geometry node itself. This means that Geometry uniform variables override
those of the ShaderProgram.

We argued whether more nodes and resources should have the capability to carry
uniform values, but decided against it, as the complexity vs. usefulness is too
poor. You'll just have to apply your uniform values to multiple Geometry nodes.

\subsection shadepassssec Running Multiple Passes

A Geometry node can be part of multiple rendering passes involving different
shaders. For example, shadow maps require a rendering of the scene using one
ShaderProgram from a light sources point of view, and another from the camera
using a second ShaderProgram.

To quickly switch between these two shaders, you can use the render pass-
functions found in the Geometry class. In the setup, first name two render
passes ("from light" and "from camera" for instance) using the
Geometry::addRenderPass() function. Second, set the ShaderPrograms for each
object in the scene using Geometry::setShaderProgram() (the one with the
render_pass argument.) Now, for each frame, set the current render pass to the
first pass name "from light" using Geometry::setRenderPass() and render. Switch
to the other render pass, "from camera", and render again - voila!

*/



/*
 * Getting started
 *
 */

/**

\page getstartpage Getting Started

\section gendocsec Generating Documentation
First of all, if you're viewing this text in ReadMe.h, you should download
Doxygen and run
\verbatim
doxygen $RENDERCHIMP$/doc/doxy
\endverbatim
to generate HTML documentaion.

\section setupsec Setting up

RC comes with ready made project solutions for Visual Studio (Windows) and
XCode (Mac). These projects have different build configurations, usually
targeting a specific renderer in debug or release mode. If you've downloaded a
fresh copy of RC, you should be able to select the desired configuration,
compile and run without any further setup. The default program renders an
example scene consisting of a single triangle.

RC comes with a few example programs. You can switch the example to build by
modifying the RC_ACTIVE_EXAMPLE define in Example.h and recompiling. Short
descriptions of the availble examples follows.
 - Example 00: Hello Triangle. Displays a single moving triangle.
 - Example 01: Hello Ogre. Loads a model and applies Lambert shading. Also
 receives mouse input to modify the camera.

It is encouraged that you take a look at the code of these example programs
before proceeding with your own development, as the examples are thoroughly
commented. The filenames are Example##.cpp, where ## is the example number.

For a program to be properly set up with RC a few simple criterias must be met:
 - The file "RenderChimp.h" must be included before any other include file.
 - The functions RCInit, RCUpdate and RCDestroy must be defined.

It is also a good idea to take a look at RenderChimp.h and glance through the
build flags available there. (Note that some flags may already be set in the
build configuration.)

\section dbgsec Debugging

Apart from using Visual Studio, GDB or whatever, RC has some additional means
to hunt for bugs.

First of all, calls to any of the REPORT-functions found in Result.h is logged
to the file log.txt. RC usually give ample information leading up to a crash
and this can be viewed in log.txt. It can also be a good idea to set a break
point inside the ResultReport-function found in Result.cpp to capture reported
warnings and errors.

Second, if you suspect that there are memory leaks or memory corruption a
memory debugger can be enabled by enabling the RC_MEMORY_DEBUG-define found
in RenderChimp.h. Your program will run much slower, but the allocated and
freed memory will constantly be checked for suspicios activity. It's a good
idea to enable this flag every now and then, just to ensure that everything is
OK with the memory usage.

The status of the memory (number of allocations, memory used, etc) can be
polled at any time from your application. Check Memory.h for more details.

*/


/*
 * FAQ
 *
 */

/**

\page faqpage Frequently Asked Questions

Q: My application requires feature X, but can't find it in RC. Why is that?

A: There are many features on our wish list, but we don't have the time and/or
resources to fulfill most of them. Development is mainly driven by the courses
RC is used for. Features that have not served a purpose in these courses are
unlikely to be found in RC. Hopefully, the code is straight forward enough that
unsupported, sought after features can be implemented by yourself. You have the
right to change and use this code however you want (see the \ref licensepage
"License agreement" for more information.)

\n

Q: I found a bug, what should I do?

A: Don't panic. First, make sure it's a bug. Read log.txt and see if you can
get any clues if it's on the application side or in RC. Do some debugging first
using the RC_MEMORY_DEBUG flag in RenderChimp.h. Use break points - they're
great! If you still think it's a bug in RC, e-mail me (magnusa@cs.lth.se) with
a description.
Then you're free to panic if you wish.

\n


*/


/*
 * Known Bugs
 *
 */

/**

\page bugspage Known Bugs

 - The Text class seems to contain many subtle bugs, and seems to render very
slowly on some GPUs. The code should probably be rewritten from scratch.

*/


/*===========================================================================*/
