/**
 *******************************************************************************
 * @file parserSVG/Ellipse.h
 * @brief basic ellipse parsing (Header)
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

#ifndef __SVG_ELLIPSE_H__
#define __SVG_ELLIPSE_H__

#include <parserSVG/Base.h>

namespace svg
{
	class Ellipse : public svg::Base
	{
		private:
			coord2D_ts m_c;        //!< Center property of the ellipse
			coord2D_ts m_r;        //!< Radius property of the ellipse
		public:
			Ellipse(PaintState parentPaintState);
			~Ellipse(void);
			virtual bool Parse(TiXmlNode * node, agg::trans_affine& parentTrans);
			virtual void Display(int32_t spacing);
			virtual void AggDraw(svg::Renderer& myRenderer, agg::trans_affine& basicTrans);
	};
};

#endif

