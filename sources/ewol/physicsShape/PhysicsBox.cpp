/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ewol/debug.h>
#include <ewol/physicsShape/PhysicsBox.h>



bool ewol::PhysicsBox::parse(const char* _line) {
	if (true ==  ewol::PhysicsShape::parse(_line)) {
		return true;
	}
	if(0 == strncmp(_line, "half-extents : ", 15) ) {
		sscanf(&_line[15], "%f %f %f", &m_size.m_floats[0], &m_size.m_floats[1], &m_size.m_floats[2] );
		EWOL_DEBUG("                halfSize=" << m_size);
		return true;
	}
	return false;
}
