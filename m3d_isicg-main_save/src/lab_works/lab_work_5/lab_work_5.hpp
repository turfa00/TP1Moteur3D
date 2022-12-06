#ifndef __LAB_WORK_5_HPP__
#define __LAB_WORK_5_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
#include "utils/random.hpp"
#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/mat4x4.hpp"
#include "../common/camera.hpp"7
#include "../common/models/triangle_mesh_model.hpp"
#include "../common/models/base_model.hpp"
#include "../common/models/triangle_mesh.hpp"

namespace M3D_ISICG
{
	class LabWork5 : public BaseLabWork
	{
	  public:
		LabWork5() : BaseLabWork() {}
		~LabWork5();
		GLuint programId;
		GLint  compiled, linked;
		GLfloat				 _time = 0.f, luminosite = 1.f, lum = 1.f, fov;
		std::vector<Vec3f>	vertices;
		std::vector<Vec3f>	couleurs;
		std:: vector<GLuint> vertexindices;	
		glm::mat4			 uMVPMatrix, viewMatrix, normalMatrix;
		glm::mat4			 modelMatrix = glm::mat4(1.f);
		Vec3f				 v;

		Camera				_camera;
		GLint				uMVP, model, view, normal, light;
		float				_cameraSpeed	   = 1.6f; //Was 0.1f
		float				_cameraSensitivity = 0.2f; //Was 0.1f

		TriangleMeshModel triangleMeshModel;
		//TriangleMesh	  triangleMesh;

		
		bool init() override, modif_lum, modif_col, modif_fov;
		void animate( const float p_deltaTime ) override;
		void render() override;
		void _init_buffers();
		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		void _updateViewMatrix();
		void   _updateProjectionMatrix();
		void _initCamera();

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

#endif // __LAB_WORK_5_HPP__
