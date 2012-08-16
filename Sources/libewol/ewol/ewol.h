/**
 *******************************************************************************
 * @file ewol/ewol.h
 * @brief Main include of ewol (header)
 * @author Edouard DUPIN
 * @date 17/10/2011
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


#ifndef __EWOL_H__
#define __EWOL_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <etk/File.h>
#include <ewol/Widget.h>
#include <ewol/Windows.h>

#if 1

namespace ewol {
	void Stop(void);
	void DisplayWindows(ewol::Windows * windows);
	// only on computer
	void ChangeSize(int32_t w, int32_t h);
	void ChangePos(int32_t x, int32_t y);
	// TODO : Deprecated used FOR???
	void GetAbsPos(int32_t & x, int32_t & y);
	
	// TODO : Deprecated remove it ...
	bool IsPressedInput(int32_t inputID);
	typedef enum {
		KEYBOARD_MODE_TEXT,
		KEYBOARD_MODE_NUMBER,
		KEYBOARD_MODE_CODE,
	} keyboardMode_te;
	void KeyboardShow(ewol::keyboardMode_te mode);
	void KeyboardHide(void);
	void ForceRedrawAll(void);
	void PopUpWidgetPush(ewol::Widget * tmpWidget);
	
	int32_t      CmdLineNb(void);
	etk::UString CmdLineGet(int32_t id);
	// TODO : This might be deprecated ... 
	bool IsSetCapsLock(void);
	bool IsSetShift(void);
	bool IsSetCtrl(void);
	bool IsSetMeta(void);
	bool IsSetAlt(void);
	bool IsSetAltGr(void);
	bool IsSetVerNum(void);
	bool IsSetInsert(void);
	// basic shortcut setting (set default value, the configutration file will overloaded it automaticly
	namespace shortCut {
		void Add(bool shift, bool control, bool alt, bool meta, uniChar_t unicodeValue, const char * generateEventId, etk::UString data);
		void Add(const char * descriptiveString, const char * generateEventId, etk::UString data);
	};
	// TODO : This is dangerous and might be deprecated ... Soon
	int32_t GetCurrentWidth(void);
	int32_t GetCurrentHeight(void);
	
	void SetTitle(etk::UString title);
	void RequestUpdateSize(void);
	
	etk::UString GetVersion(void);
	
	// get current time in ms...
	int64_t GetTime(void);
};

#else

namespace ewol {
	// stop the program :
	void Stop(void);
	// display a specific windows
	void WindowsSet(ewol::Windows * windows);
	void WindowsPopUpAdd(ewol::Widget * tmpWidget);
	// only on computer
	// change the windows size
	void ChangeSize(int32_t w, int32_t h);
	// change the windows curent position
	void ChangePos(int32_t x, int32_t y);
	// force the redraw of all the widget (this was a bad case ...
	void ForceRedrawAll(void);
	// force the calculation of all the sizes
	void RequestUpdateSize(void);
	// get the cmd line for computer call
	etk::Vector<etk::UString> CmdLineGet(void);
	// get the special key properties
	enum {
		EWOL_KEY_LEFT     = 1 <<  0,
		EWOL_KEY_RIGHT    = 1 <<  1,
		EWOL_KEY_CAP_LOCK = 1 <<  2,
		EWOL_KEY_SHIFT    = 1 <<  3,
		EWOL_KEY_CTRL     = 1 <<  4,
		EWOL_KEY_META     = 1 <<  5,
		EWOL_KEY_ALT      = 1 <<  6,
		EWOL_KEY_ALT_GR   = 1 <<  7,
		EWOL_KEY_VER_NUM  = 1 <<  8,
		EWOL_KEY_INSERT   = 1 <<  9,
	};
	int32_t KeyboardGetStatus(void);
	// set the vew title
	void SetTitle(etk::UString title);
	// get EWOL version
	etk::UString GetVersion(void);
	// get current time in ms...
	int64_t GetTime(void);
};
#endif

#endif
