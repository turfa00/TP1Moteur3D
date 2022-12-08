#include "lab_work_manager.hpp"
#include "imgui.h"
#include "lab_work_1/lab_work_1.hpp"
#include "lab_work_2/lab_work_2.hpp"
#include "lab_work_3/lab_work_3.hpp"
#include "lab_work_4/lab_work_4.hpp"
#include "lab_work_5/lab_work_5.hpp"
#include "lab_work_6/lab_work_6.hpp"
#include <iostream>

namespace M3D_ISICG
{
	LabWorkManager::LabWorkManager()
	{
		_current = new LabWork5();
		_type	 = TYPE::LAB_WORK_1;
		_type	 = TYPE::LAB_WORK_2;
		_type	 = TYPE::LAB_WORK_3;
		_type	 = TYPE::LAB_WORK_4;
		_type	 = TYPE::LAB_WORK_5;
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
		// Lab Work 4
		if ( ImGui ::MenuItem( " Lab work 4" ) )
		{
			if ( _type != TYPE ::LAB_WORK_4 ) // Change only if needed .
			{
				// Keep window size .
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  // Delete old lab work .
				_current = new LabWork4();	  // Create new lab work .
				_type	 = TYPE ::LAB_WORK_4; // Update type .
				_current->resize( w, h );	  // Update window size .
				_current->init();			  // Don �t forget to call init ().
			}
		}
		// Lab Work 5
		if ( ImGui ::MenuItem( " Lab work 5" ) )
		{
			if ( _type != TYPE ::LAB_WORK_5 ) // Change only if needed .
			{
				// Keep window size .
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  // Delete old lab work .
				_current = new LabWork5();	  // Create new lab work .
				_type	 = TYPE ::LAB_WORK_5; // Update type .
				_current->resize( w, h );	  // Update window size .
				_current->init();			  // Don �t forget to call init ().
			}
		}
		// Lab Work 6
		if ( ImGui ::MenuItem( " Lab work 6" ) )
		{
			if ( _type != TYPE ::LAB_WORK_6 ) // Change only if needed .
			{
				// Keep window size .
				const int w = _current->getWindowWidth();
				const int h = _current->getWindowHeight();
				delete _current;			  // Delete old lab work .
				_current = new LabWork6();	  // Create new lab work .
				_type	 = TYPE ::LAB_WORK_6; // Update type .
				_current->resize( w, h );	  // Update window size .
				_current->init();			  // Don �t forget to call init ().
			}
		}
	}
} // namespace M3D_ISICG
