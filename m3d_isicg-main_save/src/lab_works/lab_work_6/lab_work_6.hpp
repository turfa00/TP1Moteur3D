#ifndef __LAB_WORK_6_HPP__
#define __LAB_WORK_6_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
#include "utils/random.hpp"
#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/mat4x4.hpp"
#include "../common/camera.hpp"
#include "../common/models/triangle_mesh_model.hpp"
#include "../common/models/base_model.hpp"
#include "../common/models/triangle_mesh.hpp"

namespace M3D_ISICG
{
	class LabWork6 : public BaseLabWork
	{
	  public:
		LabWork6() : BaseLabWork() {}
		~LabWork6();
		GLuint programId, _geometryPassProgram, _shadingPassProgram;
		GLuint				 quadVAO;
		GLint  compiled, linked, compiledGeometryProgram, linkedGeometryProgram;
		GLint				 compiled2, linked2;
		GLfloat				 _time = 0.f, luminosite = 1.f, lum = 1.f, fov;
		glm::mat4			 uMVPMatrix, viewMatrix, normalMatrix;
		glm::mat4			 modelMatrix = glm::mat4(1.f);
		Vec3f				 v;

		//Quad
		std::vector<Vec3f>	vertices;
		std::vector<Vec3f>	couleurs;
		std::vector<GLuint> vertexindices;

		GLuint vbo, vboc, vao, ebo;

		Camera				_camera;
		GLint				uMVP, model, view, normal, light;
		float				_cameraSpeed	   = 8.0f; //Was 0.1f
		float				_cameraSensitivity = 0.2f; //Was 0.1f

		TriangleMeshModel triangleMeshModel;
		//gBuffer
		GLuint gPosition, gNormal, gAmbient, gDiffuse, gSpecular, gDepth;

		// Deferred shading
		GLuint fboId;
		GLenum drawBuffers[ 6 ] = { GL_COLOR_ATTACHMENT0,
									GL_COLOR_ATTACHMENT1,
									GL_COLOR_ATTACHMENT2,
									GL_COLOR_ATTACHMENT3,
									GL_COLOR_ATTACHMENT4 
		};
		int attachmentChoice = 1;

		
		bool init() override, modif_lum, modif_col, modif_fov, modif_attachment;
		void animate( const float p_deltaTime ) override;
		void render() override;
		void _init_buffers();
		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		void _updateViewMatrix();
		void   _updateProjectionMatrix();
		void _initCamera();

		bool _initGeometryPassProgram();
		bool _geometryPass( GLuint program );

		void gBuffer();
		void renderQuad();

	  private:
		// ================ Scene data.
		// ================

		// ================ GL data.
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_6_HPP__
