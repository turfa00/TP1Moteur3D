#include "imgui.h"
#include "lab_work_4.hpp"
#include "utils/read_file.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace M3D_ISICG
{
	const std::string LabWork4::_shaderFolder = "src/lab_works/lab_work_4/shaders/";
	LabWork4::~LabWork4()
	{
		glDisableVertexArrayAttrib( _cube.vao, 0 );
		glDisableVertexArrayAttrib( _cube.vao, 1 );

		glDeleteVertexArrays( 0, &_cube.vao );
		glDeleteVertexArrays( 1, &_cube.vao );

		triangleMesh.cleanGL();
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glDeleteBuffers( 0, &_cube.vbo );
	}
	void LabWork4::_init_buffers() {
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
		
		glVertexArrayAttribBinding( _cube.vao, 0, 0 );
		glVertexArrayAttribBinding( _cube.vao, 1, 1 );

		glNamedBufferData(_cube.vbo, _cube.pos_sommets.size() * sizeof( Vec3f ), _cube.pos_sommets.data(), GL_STATIC_DRAW );
		glNamedBufferData( _cube.vboc, _cube.col_sommets.size() * sizeof( Vec3f ), _cube.col_sommets.data(), GL_STATIC_DRAW );
		glNamedBufferData(_cube.ebo, _cube.ind_sommets.size() * sizeof( GLuint ), _cube.ind_sommets.data(), GL_STATIC_DRAW );
	}
	bool LabWork4::init()
	{
		std::cout << "Initializing lab work 4..." << std::endl;
		
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		//Cube
		//LabWork4::_createCube();
		//_init_buffers();		

		//Models
		triangleMeshModel.load( "Bunny", "data/models/bunny/bunny.obj" );
		triangleMesh = triangleMeshModel._meshes.at(0);

		triangleMesh =  TriangleMesh(triangleMesh._name, triangleMesh._vertices, triangleMesh._indices, triangleMesh._material );
		
		std::cout << "name:" << triangleMesh._name << std::endl;

		const std::string vertexShaderStr	= readFile( _shaderFolder + "mesh.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "mesh.frag" );

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
		
		uMVP = glGetUniformLocation( programId, "uMVPMatrix" );
		//uTranslationX = glGetUniformLocation( programId, "uTranslationX" );
		//luminosite = glGetUniformLocation( programId, "luminosite" );
		
		//_cube.utrans = glGetUniformLocation( programId, "uTransformationMatrix" );
		//uMVP		 = glGetUniformLocation( programId, "uMVPMatrix" );
		_camera = _initCamera();
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		std::cout << "Done!" << std::endl;
		return true;
	}
	void LabWork4::animate( const float p_deltaTime ) 
	{ 
		glProgramUniformMatrix4fv( programId, uMVP, 1, GL_FALSE, glm::value_ptr(uMVPMatrix));
		float f = p_deltaTime + 2.f;
		//_cube.uTransformationMatrix = glm::rotate( _cube.uTransformationMatrix, glm::radians( f ), glm::vec3( 0, 1, 1 ) );
		
		_updateViewMatrix();
		_updateProjectionMatrix();
		//glProgramUniform1f( programId, uTranslationX, glm::sin( _time ) );	
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
		if (modif_fov) {
			_camera.setFovy( fov );
		}
	}

	void LabWork4::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUseProgram( programId );
		glBindVertexArray( triangleMesh._vao );
		glDrawElements( GL_TRIANGLES, triangleMesh._indices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LESS );
	}

	void LabWork4::displayUI()
	{
		modif_lum = ImGui::SliderFloat( "Luminosite", &lum, 0.f, 1.0f, "" );
		modif_col = ImGui::ColorEdit3( "Background", glm::value_ptr(_bgColor) );
		fov = _camera.getFovy();
		modif_fov = ImGui::SliderFloat( "Fov", &fov, 0.f, 180.f, "" );
		
		ImGui::Begin( "Settings lab work 3" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

	void LabWork4::_updateViewMatrix() { 
		ViewM = glGetUniformLocation( programId, "uViewMatrix" );
		glProgramUniformMatrix4fv(programId, ViewM, 1, GL_FALSE, glm::value_ptr( _camera.getViewMatrix() ) );
		//uMVP = glGetUniformLocation( programId, "uMVPMatrix" );
		//uMVPMatrix = uMVPMatrix * _camera.getViewMatrix(); 
		//glProgramUniformMatrix4fv( programId, uMVP, 1, GL_FALSE, glm::value_ptr(uMVPMatrix) );
		
	}

	void LabWork4::_updateProjectionMatrix() { 
		ProjM = glGetUniformLocation( programId, "uProjectionMatrix" );
		glProgramUniformMatrix4fv( programId, ProjM, 1, GL_FALSE, glm::value_ptr( _camera.getProjectionMatrix() ) );
		//uMVP	   = glGetUniformLocation( programId, "uMVPMatrix" );
		//uMVPMatrix = uMVPMatrix * _camera.getProjectionMatrix();
		//glProgramUniformMatrix4fv( programId, uMVP, 1, GL_FALSE, glm::value_ptr( uMVPMatrix ) );

	}
	
	Camera LabWork4::_initCamera() { 
		Camera _camera;
		_camera.setPosition( Vec3f(0.f, 1.f, 3.f) );
		_camera.setScreenSize(1280, 720);
		return _camera;
	}

	void LabWork4::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}
	}

} // namespace M3D_ISICG