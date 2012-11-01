/**
 *******************************************************************************
 * @file ewol/Texture/TextureBMP.h
 * @brief ewol Texture Bitmap abstraction (header)
 * @author Edouard DUPIN
 * @date 28/03/2012
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

#ifndef __EWOL_TEXTURE_BITMAP_H__
#define __EWOL_TEXTURE_BITMAP_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <draw/Image.h>
#include <etk/os/FSNode.h>

namespace ewol
{
	namespace imageBMP
	{
		void GenerateImage(etk::FSNode & fileName, draw::Image & ouputImage);
	};
};

#endif

