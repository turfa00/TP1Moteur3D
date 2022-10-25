#include "imgui.h"
#include "lab_work_3.hpp"
#include "utils/read_file.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";
	
	LabWork3::~LabWork3()
	{
		glDisableVertexArrayAttrib( _cube.vao, 0 );
		glDisableVertexArrayAttrib( _cube.vao, 1 );

		glDeleteVertexArrays( 0, &_cube.vao );
		glDeleteVertexArrays( 1, &_cube.vao );

		glDeleteBuffers( 0, &_cube.vbo );
	}
	void LabWork3::_init_buffers() {
		glCreateBuffers( 1, &_cube.vao );
		glCreateBuffers( 1, &_cube.vbo );
		glCreateBuffers( 1, &_cube.vboc );
		glCreateVertexArrays( 1, &_cube.ebo );

		glVertexArrayElementBuffer( _cube.vao, _cube.ebo );
		// glEnableVertexArrayAttrib(vao object, index)
		glEnableVertexArrayAttrib( _cube.vao, 0 );
		glEnableVertexArrayAttrib( _cube.vao, 1 );

		glVertexArrayAttribFormat( _cube.vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribFormat( _cube.vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );

		glVertexArrayVertexBuffer( _cube.vao, 0, _cube.vbo, 0, sizeof( Vec3f ) );
		glVertexArrayVertexBuffer( _cube.vao, 1, _cube.vboc, 0, sizeof( Vec3f ) );
		// glVertexArrayAttribBinding(attribindex, bindingindex)
		glVertexArrayAttribBinding( _cube.vao, 0, 0 );
		glVertexArrayAttribBinding( _cube.vao, 1, 1 );

		glNamedBufferData(_cube.vbo, _cube.pos_sommets.size() * sizeof( Vec3f ), _cube.pos_sommets.data(), GL_STATIC_DRAW );
		glNamedBufferData( _cube.vboc, _cube.col_sommets.size() * sizeof( Vec3f ), _cube.col_sommets.data(), GL_STATIC_DRAW );
		glNamedBufferData(_cube.ebo, _cube.ind_sommets.size() * sizeof( GLuint ), _cube.ind_sommets.data(), GL_STATIC_DRAW );
	}
	bool LabWork3::init()
	{
		std::cout << "Initializing lab work 3..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		LabWork3::_createCube();
		_init_buffers();
		/*// Vector de vecteurs
		vertices.push_back( Vec2f( -0.5, 0.5 ) );
		vertices.push_back( Vec2f( 0.5, 0.5 ) );
		vertices.push_back( Vec2f( 0.5, -0.5 ) );
		vertices.push_back( Vec2f( -0.5, -0.5 ) );
		//Ebo
		vertexindices.push_back( 0 );
		vertexindices.push_back( 1 );
		vertexindices.push_back( 2 );
		vertexindices.push_back( 2 );
		vertexindices.push_back( 3 );
		vertexindices.push_back( 0 );
		//Couleurs
		couleurs.push_back(Vec3f(1,0,0));
		couleurs.push_back(Vec3f(0,1,0));
		couleurs.push_back(Vec3f(0,0,1));
		couleurs.push_back(Vec3f(1,1,0));

		glCreateBuffers( 1, &ebo );
		
		glCreateBuffers( 1, &vbo );
		glCreateBuffers( 1, &vboc );

		// glCreateVertexArrays(number of vertex arrays objects to create, array)
		//std::cerr << "here" << std::endl;
		glCreateVertexArrays( 1, &vao );

		glVertexArrayElementBuffer( vao, ebo );
		// glEnableVertexArrayAttrib(vao object, index)
		glEnableVertexArrayAttrib( vao, 0 );
		glEnableVertexArrayAttrib( vao, 1 );
		// glVertexArrayAttribFormat(vao object, GLattribindex, size, type, normalised, offset)
		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribFormat( vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );

		// Lier VBO et VAO
		// glVertexArrayVertexBuffer(vao object, binding index, buffer, offset, distance between elements)
		glVertexArrayVertexBuffer( vao, 0, vbo, 0, sizeof( Vec2f ) );
		glVertexArrayVertexBuffer( vao, 1, vboc, 0, sizeof( Vec3f ) );
		// glVertexArrayAttribBinding(attribindex, bindingindex)
		glVertexArrayAttribBinding( vao, 0, 0 );
		glVertexArrayAttribBinding( vao, 1, 1 );

		glNamedBufferData( vbo, vertices.size() * sizeof( Vec2f ), vertices.data(), GL_STATIC_DRAW );
		glNamedBufferData( vboc, vertices.size() * sizeof( Vec3f ), couleurs.data(), GL_STATIC_DRAW );
		glNamedBufferData( ebo, vertexindices.size() * sizeof( GLuint ), vertexindices.data(), GL_STATIC_DRAW );*/
		// Fonction de read_file
		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw3.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw3.frag" );

		// Creation de Shaders
		GLuint vertexShader	  = glCreateShader( GL_VERTEX_SHADER );
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		const GLchar * vsrc = vertexShaderStr.c_str();
		const GLchar * fsrc = fragmentShaderStr.c_str();
		// Association de shaders
		glShaderSource( vertexShader, 1, &vsrc, NULL );
		glCompileShader( vertexShader );

		glShaderSource( fragmentShader, 1, &fsrc, NULL );
		glCompileShader( fragmentShader );

		// Vérifier Compilation
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling vertex shader:" << log << std::endl;
			return false;
		}

		// Créer Programme
		programId = glCreateProgram();
		// Attacher les shaders
		glAttachShader( programId, vertexShader );
		glAttachShader( programId, fragmentShader );
		glLinkProgram( programId );

		
		glGetProgramiv( programId, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( programId, sizeof( log ), NULL, log );
			std::cerr << "Error linking program" << log << std::endl;
			return false;
		}
		//Variables Uniforme
		//uTranslationX = glGetUniformLocation( programId, "uTranslationX" );

		//luminosite = glGetUniformLocation( programId, "luminosite" );
		
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork3::animate( const float p_deltaTime ) 
	{ 
		glProgramUniform1f( programId, uTranslationX, glm::sin( _time ) );
		_time += p_deltaTime;
		/*if ( modif_lum )
		{
			glProgramUniform1f( programId, luminosite, lum );
			luminosite = lum;
		}*/ 
		if ( modif_col )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}
	}

	void LabWork3::render()
	{
		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram( programId );
		glBindVertexArray( _cube.vao );
		//glDrawElements(mode, count, type, *offset_start_index)
		//glDrawArrays(GL_TRIANGLES, 0, 12*3);
		glDrawElements( GL_TRIANGLES, _cube.ind_sommets.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
		
	}

	void LabWork3::handleEvents( const SDL_Event & p_event ) {}

	void LabWork3::displayUI()
	{
		modif_lum = ImGui::SliderFloat( "Luminosite", &lum, 0.f, 1.0f, "" );
		modif_col = ImGui::ColorEdit3( "Background", glm::value_ptr(_bgColor) );
		//std::cout << glm::value_ptr(_bgColor) << std::endl;
		ImGui::Begin( "Settings lab work 3" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
