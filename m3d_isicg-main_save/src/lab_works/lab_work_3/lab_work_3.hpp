#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
#include "utils/random.hpp"

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
		std::vector<Vec2f>	vertices;
		std::vector<Vec3f>	couleurs;
		std:: vector<GLuint> vertexindices;
		struct MESH
		{
			std::vector<Vec3f> pos_sommets;
			std::vector<Vec3f> col_sommets;
			std::vector<GLuint> ind_sommets;
			Mat4f				trans;
			GLuint				vbo, vboc, vao, ebo;
		}mesh;

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
			_cube.pos_sommets.push_back( Vec3f(-0.5f, 0.5f, 0.5f ));
			_cube.pos_sommets.push_back( Vec3f(0.5f, 0.5f, 0.5f ));
			_cube.pos_sommets.push_back( Vec3f(0.5f, -0.5f, 0.5f ));
			_cube.pos_sommets.push_back( Vec3f(-0.5f, -0.5f, 0.5f ));
			_cube.pos_sommets.push_back( Vec3f(-0.5f, 0.5f, -0.5f ));
			_cube.pos_sommets.push_back( Vec3f(0.5f, 0.5f, -0.5f ));
			_cube.pos_sommets.push_back( Vec3f(0.5f, -0.5f, -0.5f ));
			_cube.pos_sommets.push_back( Vec3f(-0.5f, -0.5f, -0.5f ));
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
			_cube.ind_sommets.push_back(2);
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
		}
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
