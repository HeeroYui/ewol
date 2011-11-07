/**
 *******************************************************************************
 * @file ewolWindows.cpp
 * @brief ewol window system (sources)
 * @author Edouard DUPIN
 * @date 20/10/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <etkTypes.h>
#include <etkString.h>
#include <ewolWidget.h>
#include <ewolWindows.h>
#include <ewolOObject.h>
#include <ewolTexture.h>
#include <ewolFont.h>
#include <ewol.h>
#include <GL/gl.h>



#undef __class__
#define __class__	"ewol::Windows"


//list of local events : 
const char * ewolEventWindowsClose    = "ewol Windows close";
const char * ewolEventWindowsMinimize = "ewol Windows minimize";
const char * ewolEventWindowsExpend   = "ewol Windows expend/unExpend";


ewol::Windows::Windows(void)
{
	SetDecorationDisable();
	if (true == m_hasDecoration) {
		ewol::OObject2DColored * myOObject = new ewol::OObject2DColored();
		myOObject->Rectangle( 0, 0, 20, 20,  1.0, 0.0, 0.0, 1.0); // Close
		myOObject->Rectangle(20, 0, 20, 20,  0.0, 1.0, 0.0, 1.0); // Reduce
		myOObject->Rectangle(40, 0, 20, 20,  0.0, 0.0, 1.0, 1.0); // Expend - Un-expend
		
		AddEventArea({ 0.0,0.0}, {20, 20}, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventWindowsClose);
		AddEventArea({20.0,0.0}, {20, 20}, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventWindowsMinimize);
		AddEventArea({40.0,0.0}, {20, 20}, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventWindowsExpend);
		
		AddOObject(myOObject, "leftBoutton");
		
		color_ts textColorFg;
		textColorFg.red = .0;
		textColorFg.green = .0;
		textColorFg.blue = .0;
		textColorFg.alpha = 1.0;
		ewol::OObject2DText * myOObjectText = new ewol::OObject2DText(62, 2, "Monospace", 17 , FONT_MODE_BOLD, textColorFg, "My Title ...");
		AddOObject(myOObjectText, "Title");
	}
}



bool ewol::Windows::CalculateSize(double availlableX, double availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	return true;
}


bool ewol::Windows::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, double x, double y)
{
	if (true == m_hasDecoration) {
		if(    x >= 60
		    && y <=20)
		{
			if(EVENT_INPUT_TYPE_MOVE == typeEvent && true == ewol::IsPressedInput(1) ) {
				ewol::StartMoveSystem();
			}
		}
		if(    x >= m_size.x - 20
		    && y >= m_size.y - 20)
		{
			if(EVENT_INPUT_TYPE_MOVE == typeEvent && true == ewol::IsPressedInput(1) ) {
				ewol::StartResizeSystem();
			}
		}
	}
	return true;
}



#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glut.h>
#if defined(EWOL_X11_MODE__XF86V)
#	include <X11/extensions/xf86vmode.h>
#elif defined(EWOL_X11_MODE__XRENDER)
#	include <X11/extensions/Xrender.h>
#endif


#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library; // handle to library
FT_Face face; // handle to face object

void ewol::Windows::SysDraw(void)
{

	//EWOL_DEBUG("Drow on (" << m_size.x << "," << m_size.y << ")");
	// set the size of the open GL system
	glViewport(0,0,m_size.x,m_size.y);
	
	// Clear the screen with transparency ...
	glClearColor(0.750, 0.750, 0.750, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0., m_size.x, 0., -m_size.y, 1., 20.);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -m_size.y, -5);
	
	//http://www.khronos.org/opengles/documentation/opengles1_0/html/glBlendFunc.html
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
	
	GenDraw();

	ewol::OObject2DColored tmpOObjects;
	tmpOObjects.Rectangle( 50, 50, 200, 300,  1.0, 0.0, 0.0, 1.0);
	tmpOObjects.Draw();

	glColor4f(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(50.0, 50.0, 0.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(250.0, 50.0, 0.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(250.0, 350.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(50.0, 350.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glDisable(GL_BLEND);
	return;
}


bool ewol::Windows::OnEventArea(const char * generateEventId, double x, double y)
{
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventWindowsClose == generateEventId) {
		EWOL_INFO("Request close of the windows");
		ewol::Stop();
		eventIsOK = true;
	} else if(ewolEventWindowsMinimize == generateEventId) {
		EWOL_INFO("Request Minimize of the windows");
		eventIsOK = true;
	} else if(ewolEventWindowsExpend == generateEventId) {
		EWOL_INFO("Request Expend of the windows");
		eventIsOK = true;
	}
	return eventIsOK;
}

