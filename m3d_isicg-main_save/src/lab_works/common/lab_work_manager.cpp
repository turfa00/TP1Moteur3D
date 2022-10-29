#include "lab_work_manager.hpp"
#include "imgui.h"
#include "lab_work_1/lab_work_1.hpp"
#include "lab_work_2/lab_work_2.hpp"
#include "lab_work_3/lab_work_3.hpp"
#include <iostream>

namespace M3D_ISICG
{
	LabWorkManager::LabWorkManager()
	{
		//_current = new LabWork1();
		//_current = new LabWork2();
		_current = new LabWork3();
		_type	 = TYPE::LAB_WORK_1;
		_type	 = TYPE::LAB_WORK_2;
		_type	 = TYPE::LAB_WORK_3;
	}

	void LabWorkManager::drawMenu()
	{
		// Here you can add other lab works to the menu.
		if ( ImGui::MenuItem( "Lab work 1" ) )
		{
			if ( _type != TYPE::LAB_WORK_1 ) // Change only if needed.
			{
				// Keep window size.
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			 // Delete old lab work.
				_current = new LabWork1();	 // Create new lab work.
				_type	 = TYPE::LAB_WORK_1; // Update type.
				_current->resize( w, h );	 // Update window size.
				_current->init();			 // Don't forget to call init().
			}
		}
		//Lab Work 2
		if ( ImGui ::MenuItem( " Lab work 2 " ) )
		{
			if ( _type != TYPE ::LAB_WORK_2 ) // Change only if needed .
			{
				// Keep window size .
				const int w = _current -> getWindowWidth();
				const int h = _current -> getWindowHeight();
				delete _current;			  // Delete old lab work .
				_current = new LabWork2();	  // Create new lab work .
				_type	 = TYPE ::LAB_WORK_2; // Update type .
				_current -> resize( w, h );  // Update window size .
				_current -> init();		  // Don �t forget to call init ().
			}
		}
		// Lab Work 3
		if ( ImGui ::MenuItem( " Lab work 3 " ) )
		{
			if ( _type != TYPE ::LAB_WORK_3 ) // Change only if needed .
			{
				// Keep window size .
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  // Delete old lab work .
				_current = new LabWork3();	  // Create new lab work .
				_type	 = TYPE ::LAB_WORK_3; // Update type .
				_current->resize( w, h );	  // Update window size .
				_current->init();			  // Don �t forget to call init ().
			}
		}
	}
} // namespace M3D_ISICG
