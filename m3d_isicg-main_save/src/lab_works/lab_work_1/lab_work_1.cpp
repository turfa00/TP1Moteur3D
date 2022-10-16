#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1() 
	{
		glDisableVertexArrayAttrib(vao, 0);
		
		glDeleteVertexArrays(1, &vao);

		glDeleteBuffers( 1, &vbo );
	}

	bool LabWork1::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		//Vector de vecteurs
		std::vector<Vec2f> vertices;
		vertices.push_back( Vec2f( -0.5, 0.5 ) );
		vertices.push_back( Vec2f( 0.5, 0.5 ) );
		vertices.push_back( Vec2f( 0.5, -0.5 ) );

		glCreateBuffers( 1, &vbo );
		//glCreateVertexArrays(number of vertex arrays objects to create, array)
		glCreateVertexArrays( 1, &vao );
		//glEnableVertexArrayAttrib(vao object, index)
		glEnableVertexArrayAttrib( vao, 0 );
		//glVertexArrayAttribFormat(vao object, GLattribindex, size, type, normalised, offset)
		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );

		//Lier VBO et VAO
		//glVertexArrayVertexBuffer(vao object, binding index, buffer, offset, distance between elements)
		glVertexArrayVertexBuffer( vao, 0, vbo, 0, sizeof( Vec2f ));
		//glVertexArrayAttribBinding(attribindex, bindingindex)
		glVertexArrayAttribBinding( vao, 0, 0 );

		glNamedBufferData( vbo, vertices.size() * sizeof( Vec2f ), vertices.data(), GL_STATIC_DRAW );

		//Fonction de read_file
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw1.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );

		//Creation de Shaders
		GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		//
		const GLchar * vsrc = vertexShaderStr.c_str(); 
		const GLchar * fsrc = fragmentShaderStr.c_str();
		//Association de shaders
		glShaderSource( vertexShader, 1, &vsrc , NULL );
		glCompileShader( vertexShader );

		glShaderSource( fragmentShader, 1, &fsrc, NULL );
		glCompileShader( fragmentShader );

		//Vérifier Compilation
		//GLint compiled;
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if (!compiled) 
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling vertex shader:" << log << std::endl;
			return false;
		}
		
		//Créer Programme
		programId = glCreateProgram();
		
		//Attacher les shaders
		glAttachShader( programId, vertexShader );
		glAttachShader( programId, fragmentShader );
		glLinkProgram( programId );
		GLint linked;
		glGetProgramiv( programId, GL_LINK_STATUS, &linked );
		if (!linked) {
			GLchar log[ 1024 ];
			glGetProgramInfoLog( programId, sizeof( log ), NULL, log );
			std::cerr << "Error linking program" << log << std::endl;
			return false;
		}
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render() 
	{ 
		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram(programId);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

	}

	void LabWork1::handleEvents( const SDL_Event & p_event )
	{}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
