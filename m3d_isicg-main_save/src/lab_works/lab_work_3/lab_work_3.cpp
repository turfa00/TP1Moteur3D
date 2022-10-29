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

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glDeleteBuffers( 0, &_cube.vbo );
	}
	void LabWork3::_init_buffers() {
		glCreateVertexArrays( 1, &_cube.vao );
		glCreateBuffers( 1, &_cube.vbo );
		glCreateBuffers( 1, &_cube.vboc );
		glCreateBuffers( 1, &_cube.ebo );

		glVertexArrayElementBuffer( _cube.vao, _cube.ebo );
		glEnableVertexArrayAttrib( _cube.vao, 0 );
		glEnableVertexArrayAttrib( _cube.vao, 1 );

		glVertexArrayAttribFormat( _cube.vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );
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
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_ALWAYS );
		// Fonction de read_file
		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw3.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw3.frag" );

		// Creation de Shaders
		GLuint vertexShader	  = glCreateShader( GL_VERTEX_SHADER );
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		const GLchar * vsrc = vertexShaderStr.c_str();
		const GLchar * fsrc = fragmentShaderStr.c_str();

		glShaderSource( vertexShader, 1, &vsrc, NULL );
		glCompileShader( vertexShader );

		glShaderSource( fragmentShader, 1, &fsrc, NULL );
		glCompileShader( fragmentShader );
		
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
		
		// Cr�er Programme
		programId = glCreateProgram();
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
		
		_cube.utrans = glGetUniformLocation( programId, "uTransformationMatrix" );
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		std::cout << "Done!" << std::endl;
		return true;
	}
	void LabWork3::animate( const float p_deltaTime ) 
	{ 
		_initCamera();
		float f = p_deltaTime + 2.f;
		_cube.uTransformationMatrix = glm::rotate( _cube.uTransformationMatrix, glm::radians( f ), glm::vec3( 0, 1, 1 ) );
		//glProgramUniform1f( programId, uTranslationX, glm::sin( _time ) );
		//_time += p_deltaTime;
		//_cube.uTransformationMatrix
		glProgramUniformMatrix4fv(programId, _cube.utrans, 1, GL_FALSE, glm::value_ptr(_cube.uTransformationMatrix));
		if ( modif_lum )
		{
			glProgramUniform1f( programId, luminosite, lum );
			luminosite = lum;
		}
		if ( modif_col )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}
		
	}

	void LabWork3::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUseProgram( programId );
		glBindVertexArray( _cube.vao );
		glDrawElements( GL_TRIANGLES, _cube.ind_sommets.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
		
	}

	void LabWork3::handleEvents( const SDL_Event & p_event ) {}

	void LabWork3::displayUI()
	{
		modif_lum = ImGui::SliderFloat( "Luminosite", &lum, 0.f, 1.0f, "" );
		modif_col = ImGui::ColorEdit3( "Background", glm::value_ptr(_bgColor) );
		ImGui::Begin( "Settings lab work 3" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

	void LabWork3::_updateViewMatrix(Camera camera) { 
		GLuint t;
		t = glGetUniformLocation( programId, "uviewMatrix" );
		glProgramUniformMatrix4fv(programId, t, 1, GL_FALSE, glm::value_ptr( camera.getViewMatrix() ) );
	}

	void LabWork3::_initCamera() { 
		Camera camera;
		camera.setPosition( Vec3f(0.f, 1.f, 3.f) );
		camera.setScreenSize(1280, 720);
	}

} // namespace M3D_ISICG
