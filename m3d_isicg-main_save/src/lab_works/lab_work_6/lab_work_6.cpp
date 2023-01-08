#include "imgui.h"
#include "lab_work_6.hpp"
#include "utils/read_file.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace M3D_ISICG
{
	const std::string LabWork6::_shaderFolder = "src/lab_works/lab_work_6/shaders/";
	LabWork6::~LabWork6()
	{
		triangleMeshModel.cleanGL();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}
	bool LabWork6::init()
	{
		std::cout << "Initializing lab work 6..." << std::endl;
		
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		gBuffer();
		triangleMeshModel.load( "Sponza", "data/models/sponza/sponza.obj" );
		triangleMeshModel._transformation	= glm::scale( triangleMeshModel._transformation, glm::vec3(1.f) );
		const std::string vertexShaderStr	 = readFile( _shaderFolder + "geometry_pass.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "geometry_pass.frag" );
		const std::string fragmentShaderPassStr= readFile( _shaderFolder + "shading_pass.frag" );

		GLuint vertexShader	  = glCreateShader( GL_VERTEX_SHADER );
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
		GLuint shadingPass = glCreateShader( GL_FRAGMENT_SHADER );
		
		const GLchar * vsrc = vertexShaderStr.c_str();
		const GLchar * fsrc = fragmentShaderStr.c_str();
		const GLchar * shsrc = fragmentShaderPassStr.c_str();

		glShaderSource( vertexShader, 1, &vsrc, NULL );
		glCompileShader( vertexShader );

		glShaderSource( fragmentShader, 1, &fsrc, NULL );
		glCompileShader( fragmentShader );

		glShaderSource( shadingPass, 1, &shsrc, NULL );
		glCompileShader( shadingPass );
		
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

		_geometryPassProgram = glCreateProgram();
		glAttachShader( _geometryPassProgram, vertexShader );
		glAttachShader( _geometryPassProgram, fragmentShader );
		glLinkProgram( _geometryPassProgram );
		
		_shadingPassProgram = glCreateProgram();
		glAttachShader( _shadingPassProgram, shadingPass );
		glLinkProgram( _shadingPassProgram );

		//Geometry Pass
		glGetShaderiv( shadingPass, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( shadingPass, sizeof( log ), NULL, log );
			glDeleteShader( shadingPass );
			std::cerr << "Error compiling vertex shader:" << log << std::endl;
			return false;
		}
		glGetProgramiv( _geometryPassProgram, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _geometryPassProgram, sizeof( log ), NULL, log );
			std::cerr << "Error linking program" << log << std::endl;
			return false;
		}
		
		//Shading Pass
		glGetProgramiv( _shadingPassProgram, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _shadingPassProgram, sizeof( log ), NULL, log );
			std::cerr << "Error linking Shading program" << log << std::endl;
			return false;
		}

		_initCamera();
		
		uMVP   = glGetUniformLocation( _geometryPassProgram, "uMVPMatrix" );
		model  = glGetUniformLocation( _geometryPassProgram, "modelMatrix" );
		view   = glGetUniformLocation( _geometryPassProgram, "viewMatrix" );
		normal = glGetUniformLocation( _geometryPassProgram, "NormalMat" );
		light  = glGetUniformLocation( _geometryPassProgram, "lightPosition" );
		//v = Vec3f( 1.f, 1.f, -1.f );
		glProgramUniformMatrix4fv( _geometryPassProgram, uMVP, 1, GL_FALSE, glm::value_ptr( uMVPMatrix ) );
		glProgramUniformMatrix4fv(
			_geometryPassProgram, model, 1, GL_FALSE, glm::value_ptr( triangleMeshModel._transformation ) );
		glProgramUniform3f( _geometryPassProgram, light, 0.f, 0.f, 0.f );
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		glDeleteShader( shadingPass);

		glEnable( GL_DEPTH_TEST );
		//glDisable( GL_DEPTH_TEST );
		std::cout << "Done!" << std::endl;
		renderQuad();
		return true;
	}
	void LabWork6::animate( const float p_deltaTime ) 
	{ 
		_updateViewMatrix();
		_updateProjectionMatrix();
		glProgramUniformMatrix4fv( _geometryPassProgram, uMVP, 1, GL_FALSE, glm::value_ptr( uMVPMatrix ) );
		if (modif_fov) {
			_camera.setFovy( fov );
		}
		
		normalMatrix = glm::transpose( glm::inverse( _camera.getViewMatrix() * triangleMeshModel._transformation ) );
		glProgramUniformMatrix4fv( _geometryPassProgram, normal, 1, GL_FALSE, glm::value_ptr( normalMatrix ) );
		//glProgramUniform3f( _geometryPassProgram, light, _camera.getLightDirection().x, _camera.getLightDirection().y, _camera.getLightDirection().z );
	}

	void LabWork6::render()
	{
		glUseProgram( 0 );
		
		glEnable( GL_DEPTH_TEST );
		glUseProgram( _geometryPassProgram );
		glBindFramebuffer( GL_FRAMEBUFFER, fboId );
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		

		triangleMeshModel.render( _geometryPassProgram );

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		//glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		glNamedFramebufferReadBuffer( fboId, drawBuffers[attachmentChoice] );
		glBlitFramebuffer( fboId,
						   0,
						   _windowWidth,
						   _windowHeight,
						   0,
						   0,
						   _windowWidth,
						   _windowHeight,
						   GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
						   GL_NEAREST );

		glUseProgram( _shadingPassProgram );
		glDisable( GL_DEPTH_TEST );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glClear( GL_COLOR_BUFFER_BIT );
		glBindVertexArray( vao );
		

		glBindTextureUnit( 0, gPosition );
		glBindTextureUnit( 1, gNormal );
		glBindTextureUnit( 2, gAmbient );
		glBindTextureUnit( 3, gDiffuse );
		glBindTextureUnit( 4, gSpecular );

		glDrawElements( GL_TRIANGLES, vertexindices.size(), GL_UNSIGNED_INT, 0 );

		glBindTextureUnit( 0, 0 );
		glBindTextureUnit( 1, 0 );
		glBindTextureUnit( 2, 0 );
		glBindTextureUnit( 3, 0 );
		glBindTextureUnit( 4, 0 );
		glBindVertexArray( 0 );

	}

	void LabWork6::displayUI()
	{
		modif_col = ImGui::ColorEdit3( "Background", glm::value_ptr(_bgColor) );
		modif_attachment = ImGui::SliderInt("Change attachment", &attachmentChoice, 0, 5, "" );
		fov = _camera.getFovy();
		modif_fov = ImGui::SliderFloat( "Fov", &fov, 0.f, 180.f, "" );
		
		ImGui::Begin( "Settings lab work 6" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

	void LabWork6::_updateViewMatrix() { 
		uMVP	   = glGetUniformLocation( _geometryPassProgram, "uMVPMatrix" );
		uMVPMatrix = _camera.getProjectionMatrix() * _camera.getViewMatrix() * triangleMeshModel._transformation;
		glProgramUniformMatrix4fv( _geometryPassProgram, uMVP, 1, GL_FALSE, glm::value_ptr( uMVPMatrix ) );

		view	   = glGetUniformLocation( _geometryPassProgram, "viewMatrix" );
		viewMatrix = _camera.getViewMatrix();
		glProgramUniformMatrix4fv( _geometryPassProgram, view, 1, GL_FALSE, glm::value_ptr( viewMatrix ) );
	}

	void LabWork6::_updateProjectionMatrix() { 
		uMVP	   = glGetUniformLocation( _geometryPassProgram, "uMVPMatrix" );
		uMVPMatrix = _camera.getProjectionMatrix() * _camera.getViewMatrix() * triangleMeshModel._transformation;
		glProgramUniformMatrix4fv( _geometryPassProgram, uMVP, 1, GL_FALSE, glm::value_ptr( uMVPMatrix ) );
	}
	
	void LabWork6::_initCamera() { 
		_camera.setPosition( Vec3f(0.f, 0.f, 0.2f) );
		_camera.setScreenSize(_windowWidth, _windowHeight);
	}

	void LabWork6::handleEvents( const SDL_Event & p_event )
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

	void LabWork6::renderQuad() { 
		// Vector de vecteurs
		vertices.push_back( Vec2f( -1, -1 ) );
		vertices.push_back( Vec2f( -1, 1 ) );
		vertices.push_back( Vec2f( 1, 1 ) );
		vertices.push_back( Vec2f( 1, -1 ) );
		// Ebo
		vertexindices.push_back( 0 );
		vertexindices.push_back( 1 );
		vertexindices.push_back( 2 );
		vertexindices.push_back( 0 );
		vertexindices.push_back( 2 );
		vertexindices.push_back( 3 );

		glCreateBuffers( 1, &ebo );

		glCreateBuffers( 1, &vbo );

		glCreateVertexArrays( 1, &vao );

		glVertexArrayElementBuffer( vao, ebo );
		
		glEnableVertexArrayAttrib( vao, 0 );
		glEnableVertexArrayAttrib( vao, 1 );

		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribFormat( vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );

		glVertexArrayVertexBuffer( vao, 0, vbo, 0, sizeof( Vec2f ) );
		glVertexArrayAttribBinding( vao, 0, 0 );
		glVertexArrayAttribBinding( vao, 1, 1 );

		glNamedBufferData( vbo, vertices.size() * sizeof( Vec2f ), vertices.data(), GL_STATIC_DRAW );
		glNamedBufferData( ebo, vertexindices.size() * sizeof( GLuint ), vertexindices.data(), GL_STATIC_DRAW );
	}
	bool LabWork6::_initGeometryPassProgram()
	{
		_geometryPassProgram = glCreateProgram();

		glGetProgramiv( _geometryPassProgram, GL_LINK_STATUS, &linked2 );
		if ( !linked2 )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _geometryPassProgram, sizeof( log ), NULL, log );
			std::cerr << "Error linking program" << log << std::endl;
			return false;
		}
	}

	bool LabWork6::_geometryPass( GLuint program )
	{ 
		GLuint geometryShader = glCreateShader( GL_GEOMETRY_SHADER );
		GLuint			  vertexShader2		 = glCreateShader( GL_VERTEX_SHADER );
		const std::string geometryShaderStr	 = readFile( _shaderFolder + "geometry_pass.frag" );
		const std::string vertexShaderStr2	 = readFile( _shaderFolder + "geometry_pass.vert" );
		
		const GLchar * vsrc2 = vertexShaderStr2.c_str();

		glShaderSource( vertexShader2, 1, &vsrc2, NULL );
		glCompileShader( vertexShader2 );
		const GLchar * gsrc = geometryShaderStr.c_str();

		glShaderSource( geometryShader, 1, &gsrc, NULL );
		glCompileShader( geometryShader );
		glGetShaderiv( geometryShader, GL_COMPILE_STATUS, &compiled2 );
		if ( !compiled2 )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( geometryShader, sizeof( log ), NULL, log );
			glDeleteShader( geometryShader );
			std::cerr << "Error compiling Geometry shader:" << log << std::endl;
			return false;
		}
		glAttachShader( _geometryPassProgram, geometryShader );
		glAttachShader( _geometryPassProgram, vertexShader2 );
		glLinkProgram( _geometryPassProgram );
		glGetProgramiv( _geometryPassProgram, GL_LINK_STATUS, &linked2 );
		if ( !linked2 )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _geometryPassProgram, sizeof( log ), NULL, log );
			std::cerr << "Error linking program" << log << std::endl;
			return false;
		}
	}
	void LabWork6::gBuffer()
	{
		//Cr�er le gBuffer
		glCreateFramebuffers( 1, &fboId );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fboId );
		//Position
		glGenTextures( 1, &gPosition );
		glBindTexture( GL_TEXTURE_2D, gPosition );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, _windowWidth, _windowHeight, 0, GL_RGBA, GL_FLOAT, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0 );
		glNamedFramebufferTexture( fboId, GL_COLOR_ATTACHMENT0, gPosition, 0 );
		

		//Normal
		glGenTextures( 1, &gNormal );
		glBindTexture( GL_TEXTURE_2D, gNormal );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, _windowWidth, _windowHeight, 0, GL_RGBA, GL_FLOAT, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0 );
		glNamedFramebufferTexture( fboId, GL_COLOR_ATTACHMENT1, gNormal, 0 );

		//Ambient
		glGenTextures( 1, &gAmbient );
		glBindTexture( GL_TEXTURE_2D, gAmbient );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _windowWidth, _windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAmbient, 0 );
		glNamedFramebufferTexture( fboId, GL_COLOR_ATTACHMENT2, gAmbient, 0 );

		// Diffuse
		glGenTextures( 1, &gDiffuse );
		glBindTexture( GL_TEXTURE_2D, gDiffuse );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _windowWidth, _windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gDiffuse, 0 );
		glNamedFramebufferTexture( fboId, GL_COLOR_ATTACHMENT3, gDiffuse, 0 );

		// Specular
		glGenTextures( 1, &gSpecular );
		glBindTexture( GL_TEXTURE_2D, gSpecular );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _windowWidth, _windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gSpecular, 0 );
		glNamedFramebufferTexture( fboId, GL_COLOR_ATTACHMENT4, gSpecular, 0 );

		// Depth
		glGenTextures( 1, &gDepth );
		glBindTexture( GL_TEXTURE_2D, gDepth );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _windowWidth, _windowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0 );
		
		glNamedFramebufferTexture( fboId, GL_DEPTH_ATTACHMENT, gDepth, 0 );
		std::cout << "Here" << std::endl;

		glNamedFramebufferDrawBuffers( fboId, 5, drawBuffers );

		GLenum status = glCheckNamedFramebufferStatus( fboId, GL_FRAMEBUFFER );
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "gBuffer Incomplete" << std::endl;
		}
		else {
			std::cout << "gBuffer complete" << std::endl;
		}
		
		/* glNamedFramebufferReadBuffer( fboId, GL_COLOR_ATTACHMENT2 );
		glBlitFramebuffer( fboId, 0, _windowWidth, _windowHeight, 0, 0, _windowWidth, _windowHeight,
						   GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST );

		glNamedFramebufferReadBuffer( fboId, GL_COLOR_ATTACHMENT1 );
		glBlitFramebuffer( fboId,
						   0,
						   _windowWidth,
						   _windowHeight,
						   0,
						   0,
						   _windowWidth,
						   _windowHeight,
						   GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
						   GL_NEAREST );*/

		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		//glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

} // namespace M3D_ISICG
