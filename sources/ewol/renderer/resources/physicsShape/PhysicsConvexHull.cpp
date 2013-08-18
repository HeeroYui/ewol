/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ewol/debug.h>
#include <ewol/renderer/resources/physicsShape/PhysicsConvexHull.h>



bool ewol::PhysicsConvexHull::Parse(const char* _line)
{
	if (true== ewol::PhysicsShape::Parse(_line)) {
		return true;
	}
	if(0==strncmp(_line, "points : ", 8) ) {
		//EWOL_DEBUG("convex hull point parsing " << _line);
		char* base = (char*)(&_line[8]);
		char* tmp= strchr(base, '|');
		vec3 pos(0,0,0);
		while (tmp != NULL) {
			*tmp = '\0';
			sscanf(base, "%f %f %f", &pos.m_floats[0], &pos.m_floats[1], &pos.m_floats[2] );
			m_points.PushBack(pos);
			base = tmp+1;
			tmp= strchr(base, '|');
		}
		sscanf(base, "%f %f %f", &pos.m_floats[0], &pos.m_floats[1], &pos.m_floats[2] );
		m_points.PushBack(pos);
		/*
		for (int32_t iii=0; iii<m_points.Size(); iii++) {
			EWOL_DEBUG("    parsed " << m_points[iii]);
		}
		*/
		return true;
	}
	if(0==strncmp(_line, "scale : ", 8) ) {
		sscanf(&_line[8], "%f %f %f", &m_scale.m_floats[0], &m_scale.m_floats[1], &m_scale.m_floats[2] );
		EWOL_DEBUG("                scale=" << m_scale);
		return true;
	}
	return false;
}
