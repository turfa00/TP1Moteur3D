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
		//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glClear( GL_DEPTH_BUFFER_BIT );
		glUseProgram( p_glProgram );
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LESS );
		//glDepthMask( GL_FALSE );
		Vec3f ambientColor = this->_material._ambient;
		Vec3f diffuseColor = this->_material._diffuse;
		Vec3f specularColor = this->_material._specular;
		//Uniform values
		GLfloat aColor = glGetUniformLocation( p_glProgram, "diffuseColor" );
		GLfloat	  dColor	   = glGetUniformLocation( p_glProgram, "ambientColor" );
		GLfloat sColor	  = glGetUniformLocation( p_glProgram, "specularColor" );
		GLfloat	  shininess = glGetUniformLocation( p_glProgram, "shininessVal" );

		glUniform3f( aColor, diffuseColor.x, diffuseColor.y, diffuseColor.z );
		glUniform3f( dColor, ambientColor.x, ambientColor.y, ambientColor.z );
		glUniform3f( sColor, specularColor.x, specularColor.y, specularColor.z );
		glUniform1f( shininess, this->_material._shininess );

		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
		glUseProgram( 0 );
		
		
		//glDepthFunc( GL_LEQUAL );
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
		glVertexArrayAttribBinding( _vao, 1, 0 );
		glVertexArrayAttribBinding( _vao, 2, 0 );
		glVertexArrayAttribBinding( _vao, 3, 0 );
		glVertexArrayAttribBinding( _vao, 4, 0 );

		glNamedBufferData( _vbo, _vertices.size() * sizeof( Vertex ), _vertices.data(), GL_STATIC_DRAW );
		glNamedBufferData( _ebo, _indices.size() * sizeof( unsigned int ), _indices.data(), GL_STATIC_DRAW );
		
	}
} // namespace M3D_ISICG
