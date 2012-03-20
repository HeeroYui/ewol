/**
 *******************************************************************************
 * @file parserSVG/Polyline.h
 * @brief basic Poliline parsing (Header)
 * @author Edouard DUPIN
 * @date 20/03/2012
 * @par Project
 * parserSVG
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

#ifndef __SVG_POLYLINE_H__
#define __SVG_POLYLINE_H__

#include <parserSVG/Base.h>
#include <etk/VectorType.h>

namespace svg
{
	class Polyline : public svg::Base
	{
		private:
			etk::VectorType<coord2D_ts> m_listPoint;    //!< list of all point of the polyline
		public:
			Polyline(void);
			~Polyline(void);
			virtual bool Parse(TiXmlNode * node);
	};
};

#endif

