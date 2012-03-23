/**
 *******************************************************************************
 * @file parserSVG/Group.cpp
 * @brief Basic Group parsing (Sources)
 * @author Edouard DUPIN
 * @date 21/03/2012
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

#include <parserSVG/Debug.h>
#include <parserSVG/Group.h>
#include <etk/UString.h>
#include <parserSVG/Base.h>
#include <parserSVG/Circle.h>
#include <parserSVG/Ellipse.h>
#include <parserSVG/Line.h>
#include <parserSVG/Path.h>
#include <parserSVG/Polygon.h>
#include <parserSVG/Polyline.h>
#include <parserSVG/Rectangle.h>
#include <parserSVG/Text.h>
#include <parserSVG/Group.h>

svg::Group::Group(PaintState parentPaintState) : svg::Base(parentPaintState)
{
	
}

svg::Group::~Group(void)
{
	
}

bool svg::Group::Parse(TiXmlNode * node)
{
	// parse ...
	coord2D_ts pos;
	coord2D_ts size;
	ParseTransform(node);
	ParsePosition(node, pos, size);
	ParsePaintAttr(node);
	
	// parse all sub node :
	for(TiXmlNode * child = node->FirstChild(); NULL != child; child = child->NextSibling() ) {
		svg::Base *elementParser = NULL;
		if (child->Type()==TiXmlNode::TINYXML_COMMENT) {
			// nothing to do, just proceed to next step
		} else {
			etk::UString localValue = child->Value();
			if (localValue == "g") {
				elementParser = new svg::Group(m_paint);
			} else if (localValue == "a") {
				// TODO ...
			} else if (localValue == "path") {
				elementParser = new svg::Path(m_paint);
			} else if (localValue == "rect") {
				elementParser = new svg::Rectangle(m_paint);
			} else if (localValue == "circle") {
				elementParser = new svg::Circle(m_paint);
			} else if (localValue == "ellipse") {
				elementParser = new svg::Ellipse(m_paint);
			} else if (localValue == "line") {
				elementParser = new svg::Line(m_paint);
			} else if (localValue == "polyline") {
				elementParser = new svg::Polyline(m_paint);
			} else if (localValue == "polygon") {
				elementParser = new svg::Polygon(m_paint);
			} else if (localValue == "text") {
				elementParser = new svg::Text(m_paint);
			} else {
				SVG_ERROR("(l "<<child->Row()<<") node not suported : \""<<localValue<<"\" must be [g,a,path,rect,circle,ellipse,line,polyline,polygon,text]");
			}
			if (NULL == elementParser) {
				SVG_ERROR("(l "<<child->Row()<<") error on node: \""<<localValue<<"\" allocation error or not supported ...");
			} else {
				if (false == elementParser->Parse(child)) {
					SVG_ERROR("(l "<<child->Row()<<") error on node: \""<<localValue<<"\" Sub Parsing ERROR");
					delete(elementParser);
					elementParser = NULL;
				} else {
					// add element in the system
					m_subElementList.PushBack(elementParser);
				}
			}
		}
	}
	return true;
}

void svg::Group::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Group (START) fill=" << m_paint.fill << " stroke=" << m_paint.stroke);
	for (int32_t iii=0; iii<m_subElementList.Size(); iii++) {
		if (NULL != m_subElementList[iii]) {
			m_subElementList[iii]->Display(spacing+1);
		}
	}
	SVG_DEBUG(SpacingDist(spacing) << "Group (STOP)");
}

void svg::Group::AggDraw(svg::Renderer& myRenderer, svg::PaintState &curentPaintProp)
{
	curentPaintProp = m_paint;
	for (int32_t iii=0; iii<m_subElementList.Size(); iii++) {
		if (NULL != m_subElementList[iii]) {
			m_subElementList[iii]->AggDraw(myRenderer, curentPaintProp);
		}
	}
}

