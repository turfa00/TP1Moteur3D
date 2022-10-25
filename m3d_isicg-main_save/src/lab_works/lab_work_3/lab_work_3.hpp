#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

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
		bool init() override, modif_lum, modif_col;
		void animate( const float p_deltaTime ) override;
		void render() override;

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

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
