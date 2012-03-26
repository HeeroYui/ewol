/**
 *******************************************************************************
 * @file parserSVG/parserSVG.h
 * @brief parserSVG : basic header of the SVG parser (Header)
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

#ifndef __SVG_PARSER_H__
#define __SVG_PARSER_H__

#include <etk/File.h>
#include <etk/VectorType.h>
#include <parserSVG/Base.h>

namespace svg
{
	class Parser : public svg::Base
	{
		private:
			etk::File                    m_fileName;
			bool                         m_loadOK;
			etk::UString                 m_version;
			etk::VectorType<svg::Base *> m_subElementList;
	
		public:
			Parser(etk::File fileName);
			~Parser(void);
			bool IsLoadOk(void) { return m_loadOK; };
			void DisplayDebug(void);
			void GenerateTestFile(void);
			virtual void AggDraw(svg::Renderer& myRenderer, agg::trans_affine& basicTrans);
	};
};

#endif

