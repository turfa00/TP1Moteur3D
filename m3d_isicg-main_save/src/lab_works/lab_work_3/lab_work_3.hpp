#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
#include "utils/random.hpp"
#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/mat4x4.hpp"

namespace M3D_ISICG
{
	class LabWork3 : public BaseLabWork
	{
	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();
		// Identification de program
		GLuint programId;
		// Vertex Objects
		GLuint			   vbo, vboc, vao, ebo;
		GLint  compiled, linked, uTranslationX;
		GLfloat				 _time = 0.f, luminosite = 1.f, lum = 1.f;
		//glm::mat4			 uTransformationMatrix = glm::mat4( 1.f );
		std::vector<Vec3f>	vertices;
		std::vector<Vec3f>	couleurs;
		std:: vector<GLuint> vertexindices;
		//glm::mat4			 uTransformMatrix = glm::mat4(1.f);
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
		bool init() override, modif_lum, modif_col;
		void animate( const float p_deltaTime ) override;
		void render() override;
		void _init_buffers();
		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

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

			_cube.uTransformationMatrix = glm::scale( _cube.uTransformationMatrix, glm::vec3( 0.8f ) );
			//_cube.uTransformationMatrix = glm::rotate( _cube.uTransformationMatrix, glm::radians( 90.0f ), glm::vec3( 0, 1, 1 ) );
			//_cube.trans = glm::translate( _cube.trans, Vec3f(0.8f, 0.8f, 0.8f));
		}
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
