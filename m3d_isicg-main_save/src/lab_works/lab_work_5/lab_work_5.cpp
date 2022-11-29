#include "imgui.h"
#include "lab_work_5.hpp"
#include "utils/read_file.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace M3D_ISICG
{
	const std::string LabWork5::_shaderFolder = "src/lab_works/lab_work_5/shaders/";
	LabWork5::~LabWork5()
	{
		//triangleMesh.cleanGL();
		triangleMeshModel.cleanGL();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}
	bool LabWork5::init()
	{
		std::cout << "Initializing lab work 5..." << std::endl;
		
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
	
		//triangleMeshModel.load( "Bunny", "data/models/bunny/bunny.obj" );	
		//triangleMeshModel.load( "Conference", "data/models/conference/conference.obj" );	
		triangleMeshModel.load( "Bad_Bunny", "data/models/bunny_2/bunny_2.obj" );
		triangleMeshModel._transformation	= glm::scale( triangleMeshModel._transformation, glm::vec3(1.f) );
		const std::string vertexShaderStr	= readFile( _shaderFolder + "mesh_texture.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "mesh_texture.frag" );

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
		_initCamera();
		
		uMVP = glGetUniformLocation( programId, "uMVPMatrix" );
		model = glGetUniformLocation( programId, "modelMatrix" );
		view  = glGetUniformLocation( programId, "viewMatrix" );
		normal = glGetUniformLocation( programId, "NormalMat" );
		light  = glGetUniformLocation( programId, "lightDirection" );
		//v = Vec3f( 1.f, 1.f, -1.f );
		glProgramUniformMatrix4fv( programId, uMVP, 1, GL_FALSE, glm::value_ptr( uMVPMatrix ) );
		glProgramUniformMatrix4fv( programId, model, 1, GL_FALSE, glm::value_ptr( triangleMeshModel._transformation ) );
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		
		glEnable( GL_DEPTH_TEST );

		std::cout << "Done!" << std::endl;
		return true;
	}
	void LabWork5::animate( const float p_deltaTime ) 
	{ 
		_updateViewMatrix();
		_updateProjectionMatrix();
		glProgramUniformMatrix4fv( programId, uMVP, 1, GL_FALSE, glm::value_ptr( uMVPMatrix ) );
		if (modif_fov) {
			_camera.setFovy( fov );
		}
		
		normalMatrix = glm::transpose( glm::inverse( _camera.getViewMatrix() * triangleMeshModel._transformation ) );
		glProgramUniformMatrix4fv( programId, normal, 1, GL_FALSE, glm::value_ptr( normalMatrix ) );
		glProgramUniform3f( programId, light, _camera.getLightPosition().x, _camera.getLightPosition().y, _camera.getLightPosition().z );
		//std::cout << glm::to_string(_camera.getPosition()) << std::endl; 
	}

	void LabWork5::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		triangleMeshModel.render(programId);
	}

	void LabWork5::displayUI()
	{
		modif_lum = ImGui::SliderFloat( "Luminosite", &lum, 0.f, 1.0f, "" );
		modif_col = ImGui::ColorEdit3( "Background", glm::value_ptr(_bgColor) );
		fov = _camera.getFovy();
		modif_fov = ImGui::SliderFloat( "Fov", &fov, 0.f, 180.f, "" );
		
		ImGui::Begin( "Settings lab work 4" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

	void LabWork5::_updateViewMatrix() { 
		uMVP = glGetUniformLocation( programId, "uMVPMatrix" );
		uMVPMatrix = _camera.getProjectionMatrix() * _camera.getViewMatrix() * triangleMeshModel._transformation;
		glProgramUniformMatrix4fv(programId, uMVP, 1, GL_FALSE, glm::value_ptr( uMVPMatrix ) );

		view = glGetUniformLocation( programId, "viewMatrix" );
		viewMatrix = _camera.getViewMatrix();
		glProgramUniformMatrix4fv( programId, view, 1, GL_FALSE, glm::value_ptr( viewMatrix ) );
	}

	void LabWork5::_updateProjectionMatrix() { 
		uMVP = glGetUniformLocation( programId, "uMVPMatrix" );
		uMVPMatrix = _camera.getProjectionMatrix() * _camera.getViewMatrix() * triangleMeshModel._transformation;
		glProgramUniformMatrix4fv( programId, uMVP, 1, GL_FALSE, glm::value_ptr( uMVPMatrix) );
	}
	
	void LabWork5::_initCamera() { 
		_camera.setPosition( Vec3f(0.f, 0.f, 0.2f) );
		_camera.setScreenSize(_windowWidth, _windowHeight);
	}

	void LabWork5::handleEvents( const SDL_Event & p_event )
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