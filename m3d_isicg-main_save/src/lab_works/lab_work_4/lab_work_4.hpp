#ifndef __LAB_WORK_4_HPP__
#define __LAB_WORK_4HPP__

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
	class LabWork4 : public BaseLabWork
	{
	  public:
		LabWork4() : BaseLabWork() {}
		~LabWork4();
		GLuint programId;
		GLuint			   vbo, vboc, vao, ebo;
		GLint  compiled, linked, uTranslationX;
		GLfloat				 _time = 0.f, luminosite = 1.f, lum = 1.f, fov, uMVP;
		std::vector<Vec3f>	vertices;
		std::vector<Vec3f>	couleurs;
		std:: vector<GLuint> vertexindices;
		glm::mat4			 uMVPMatrix = glm::mat4( 1.f );

		Camera				_camera;
		GLint				 ViewM, ProjM;
		float				_cameraSpeed	   = 0.1f;
		float				_cameraSensitivity = 0.1f;

		TriangleMeshModel triangleMeshModel;
		TriangleMesh	  triangleMesh;

		struct MESH
		{
			std::vector<Vec3f> pos_sommets;
			std::vector<Vec3f> col_sommets;
			std::vector<GLuint> ind_sommets;
			glm::mat4				uTransformationMatrix = glm::mat4(1.0f);
			GLint				utrans;
			GLuint				vbo, vboc, vao, ebo;
		};

		MESH _cube;
		bool init() override, modif_lum, modif_col, modif_fov;
		void animate( const float p_deltaTime ) override;
		void render() override;
		void _init_buffers();
		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		void _updateViewMatrix();
		void   _updateProjectionMatrix();
		Camera _initCamera();

	  private:
		// ================ Scene data.
		// ================

		// ================ GL data.
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================
		void					 _createCube() { 
			_cube.pos_sommets.push_back( Vec3f(-0.5f, 0.5f, 0.5f ));//0
			_cube.pos_sommets.push_back( Vec3f(0.5f, 0.5f, 0.5f ));//1
			_cube.pos_sommets.push_back( Vec3f(0.5f, -0.5f, 0.5f ));//2
			_cube.pos_sommets.push_back( Vec3f(-0.5f, -0.5f, 0.5f ));//3
			_cube.pos_sommets.push_back( Vec3f(-0.5f, 0.5f, -0.5f ));//4
			_cube.pos_sommets.push_back( Vec3f(0.5f, 0.5f, -0.5f ));//5
			_cube.pos_sommets.push_back( Vec3f(0.5f, -0.5f, -0.5f ));//6
			_cube.pos_sommets.push_back( Vec3f(-0.5f, -0.5f, -0.5f ));//7
			for ( int i = 0; i < 8; i++ )
				_cube.col_sommets.push_back( M3D_ISICG::getRandomVec3f() );
			//Indices
			_cube.ind_sommets.push_back(0);
			_cube.ind_sommets.push_back(1);
			_cube.ind_sommets.push_back(2);

			_cube.ind_sommets.push_back(2);
			_cube.ind_sommets.push_back(3);
			_cube.ind_sommets.push_back(0);

			_cube.ind_sommets.push_back(1);
			_cube.ind_sommets.push_back(5);
			_cube.ind_sommets.push_back(6);

			_cube.ind_sommets.push_back(6);
			_cube.ind_sommets.push_back(2);
			_cube.ind_sommets.push_back(1);

			_cube.ind_sommets.push_back(0);
			_cube.ind_sommets.push_back(4);
			_cube.ind_sommets.push_back(7);

			_cube.ind_sommets.push_back(7);
			_cube.ind_sommets.push_back(3);
			_cube.ind_sommets.push_back(0);

			_cube.ind_sommets.push_back(0);
			_cube.ind_sommets.push_back(4);
			_cube.ind_sommets.push_back(5);

			_cube.ind_sommets.push_back(5);
			_cube.ind_sommets.push_back(1);
			_cube.ind_sommets.push_back(0);

			_cube.ind_sommets.push_back(3);
			_cube.ind_sommets.push_back(2);
			_cube.ind_sommets.push_back(6);

			_cube.ind_sommets.push_back(6);
			_cube.ind_sommets.push_back(7);
			_cube.ind_sommets.push_back(3);

			_cube.ind_sommets.push_back(4);
			_cube.ind_sommets.push_back(5);
			_cube.ind_sommets.push_back(6);

			_cube.ind_sommets.push_back(6);
			_cube.ind_sommets.push_back(7);
			_cube.ind_sommets.push_back(4);

			_cube.uTransformationMatrix = glm::scale( _cube.uTransformationMatrix, glm::vec3( 0.6f ) );
			//_cube.uTransformationMatrix = glm::rotate( _cube.uTransformationMatrix, glm::radians( 90.0f ), glm::vec3( 0, 1, 1 ) );
			//_cube.trans = glm::translate( _cube.trans, Vec3f(0.8f, 0.8f, 0.8f));
		}
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_3_HPP__
