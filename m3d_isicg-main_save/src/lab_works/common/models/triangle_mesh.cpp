#include "triangle_mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

namespace M3D_ISICG
{
	TriangleMesh::TriangleMesh( const std::string &				  p_name,
								const std::vector<Vertex> &		  p_vertices,
								const std::vector<unsigned int> & p_indices,
								const Material &				  p_material ) :
		_name( p_name ),
		_vertices( p_vertices ), _indices( p_indices ), _material( p_material )
	{
		_vertices.shrink_to_fit();
		_indices.shrink_to_fit();
		_setupGL();
	}

	void TriangleMesh::render( const GLuint p_glProgram ) const
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUseProgram( p_glProgram );
		Vec3f ambientColor = this->_material._ambient;
		Vec3f diffuseColor = this->_material._diffuse;
		//std::cout << diffuseColor.x << diffuseColor.y << diffuseColor.z << std::endl;
		GLint vertexColor = glGetUniformLocation( p_glProgram, "ourColor" );
		glm::mat4 normalMatrix;


		glUniform4f( vertexColor, diffuseColor.x, diffuseColor.y, diffuseColor.z, 1.f );
		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LESS );
	}

	void TriangleMesh::cleanGL()
	{
		glDisableVertexArrayAttrib( _vao, 0 );
		glDisableVertexArrayAttrib( _vao, 1 );
		glDisableVertexArrayAttrib( _vao, 2 );
		glDisableVertexArrayAttrib( _vao, 3 );
		glDisableVertexArrayAttrib( _vao, 4 );
		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
	}

	void TriangleMesh::_setupGL()
	{		
		glCreateVertexArrays( 1, &_vao );
		glCreateBuffers( 1, &_vbo );
		glCreateBuffers( 1, &_ebo );

		glVertexArrayElementBuffer( _vao, _ebo );

		glEnableVertexArrayAttrib( _vao, 0 );
		glEnableVertexArrayAttrib( _vao, 1 );
		glEnableVertexArrayAttrib( _vao, 2 );
		glEnableVertexArrayAttrib( _vao, 3 );
		glEnableVertexArrayAttrib( _vao, 4 );

		glVertexArrayAttribFormat( _vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _position ) );
		glVertexArrayAttribFormat( _vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _normal ) );
		glVertexArrayAttribFormat( _vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, _texCoords ) );
		glVertexArrayAttribFormat( _vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _tangent ) );
		glVertexArrayAttribFormat( _vao, 4, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _bitangent ) );

		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, sizeof( Vertex ) );			

		glVertexArrayAttribBinding( _vao, 0, 0 );
		glVertexArrayAttribBinding( _vao, 1, 1 );
		glVertexArrayAttribBinding( _vao, 2, 2 );
		glVertexArrayAttribBinding( _vao, 3, 3 );
		glVertexArrayAttribBinding( _vao, 4, 4 );

		glNamedBufferData( _vbo, _vertices.size() * sizeof( Vertex ), _vertices.data(), GL_STATIC_DRAW );
		glNamedBufferData( _ebo, _indices.size() * sizeof( unsigned int ), _indices.data(), GL_STATIC_DRAW );
		
	}
} // namespace M3D_ISICG
