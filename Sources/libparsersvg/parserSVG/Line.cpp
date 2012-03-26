/**
 *******************************************************************************
 * @file parserSVG/Line.cpp
 * @brief basic line parsing (Sources)
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

#include <parserSVG/Debug.h>
#include <parserSVG/Line.h>
#include <agg-2.4/agg_conv_stroke.h>
#include <agg-2.4/agg_path_storage.h>

svg::Line::Line(PaintState parentPaintState) : svg::Base(parentPaintState)
{
	m_startPos.x = 0.0;
	m_startPos.y = 0.0;
	m_stopPos.x = 0.0;
	m_stopPos.y = 0.0;
}

svg::Line::~Line(void)
{
	
}

bool svg::Line::Parse(TiXmlNode * node, agg::trans_affine& parentTrans)
{
	// line must have a minimum size...
	m_paint.strokeWidth = 1;
	ParseTransform(node);
	ParsePaintAttr(node);
	
	// add the property of the parrent modifications ...
	m_transformMatrix *= parentTrans;
	
	const char * content = node->ToElement()->Attribute("x1");
	if (NULL != content) {
		m_startPos.x = ParseLength(content);
	}
	content = node->ToElement()->Attribute("y1");
	if (NULL != content) {
		m_startPos.y = ParseLength(content);
	}
	content = node->ToElement()->Attribute("x2");
	if (NULL != content) {
		m_stopPos.x = ParseLength(content);
	}
	content = node->ToElement()->Attribute("y2");
	if (NULL != content) {
		m_stopPos.y = ParseLength(content);
	}
	return true;
}

void svg::Line::Display(int32_t spacing)
{
	SVG_DEBUG(SpacingDist(spacing) << "Line " << m_startPos << " to " << m_stopPos);
}


void svg::Line::AggDraw(svg::Renderer& myRenderer, agg::trans_affine& basicTrans)
{
	agg::path_storage path;
	path.start_new_path();
	path.move_to(m_startPos.x, m_startPos.y);
	path.line_to(m_stopPos.x, m_stopPos.y);
	//path.close_polygon();
	
	agg::trans_affine mtx = m_transformMatrix;
	mtx *= basicTrans;
	
	if (m_paint.strokeWidth > 0) {
		myRenderer.m_renderArea->color(agg::rgba8(m_paint.stroke.red, m_paint.stroke.green, m_paint.stroke.blue, m_paint.stroke.alpha));
		// Drawing as an outline
		agg::conv_stroke<agg::path_storage> myPolygonStroke(path);
		myPolygonStroke.width(m_paint.strokeWidth);
		agg::conv_transform<agg::conv_stroke<agg::path_storage>, agg::trans_affine> transStroke(myPolygonStroke, mtx);
		// set the filling mode : 
		myRenderer.m_rasterizer.filling_rule(agg::fill_non_zero);
		myRenderer.m_rasterizer.add_path(transStroke);
		agg::render_scanlines(myRenderer.m_rasterizer, myRenderer.m_scanLine, *myRenderer.m_renderArea);
	}
}


