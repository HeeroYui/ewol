/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/compositing/Shaper.h>

#undef __class__
#define __class__	"ewol::compositing::Shaper"

ewol::compositing::Shaper::Shaper(const std::string& _shaperName) :
  m_name(_shaperName),
  m_config(NULL),
  m_confIdPaddingLeft(-1),
  m_confIdPaddingRight(-1),
  m_confIdPaddingTop(-1),
  m_confIdPaddingButtom(-1),
  m_confIdChangeTime(-1),
  m_confProgramFile(-1),
  m_confColorFile(-1),
  m_confImageFile(-1),
  m_GLprogram(NULL),
  m_GLPosition(-1),
  m_GLMatrix(-1),
  m_GLPropertySize(-1),
  m_GLPropertyInsidePos(-1),
  m_GLPropertyInsideSize(-1),
  m_GLStateActivate(-1),
  m_GLStateOld(-1),
  m_GLStateNew(-1),
  m_GLStateTransition(-1),
  m_resourceTexture(NULL),
  m_nextStatusRequested(-1),
  m_propertyOrigin(0,0),
  m_propertySize(0,0),
  m_propertyInsidePosition(0,0),
  m_propertyInsideSize(0,0),
  m_stateActivate(0),
  m_stateOld(0),
  m_stateNew(0),
  m_stateTransition(1.0) {
	loadProgram();
	updateVertex();
}

ewol::compositing::Shaper::~Shaper(void) {
	unLoadProgram();
}

void ewol::compositing::Shaper::unLoadProgram(void) {
	ewol::resource::Program::release(m_GLprogram);
	ewol::resource::TextureFile::release(m_resourceTexture);
	ewol::resource::ConfigFile::release(m_config);
	ewol::resource::ColorFile::release(m_colorProperty);
	m_confIdPaddingLeft = -1;
	m_confIdPaddingRight = -1;
	m_confIdPaddingTop = -1;
	m_confIdPaddingButtom = -1;
	m_confIdChangeTime = -1;
	m_confProgramFile = -1;
	m_confImageFile = -1;
	m_listAssiciatedId.clear();
}

void ewol::compositing::Shaper::loadProgram(void) {
	if (m_name == "") {
		EWOL_DEBUG("no Shaper set for loading resources ...");
		return;
	}
	m_config = ewol::resource::ConfigFile::keep(m_name);
	if (NULL != m_config) {
		m_confIdPaddingLeft   = m_config->request("padding-left");
		m_confIdPaddingRight  = m_config->request("padding-right");
		m_confIdPaddingTop    = m_config->request("padding-top");
		m_confIdPaddingButtom = m_config->request("padding-buttom");
		m_confIdChangeTime    = m_config->request("ChangeTime");
		m_confProgramFile     = m_config->request("program");
		m_confImageFile       = m_config->request("image");
		m_confColorFile       = m_config->request("color");
	}
	std::string basicShaderFile = m_config->getString(m_confProgramFile);
	if (basicShaderFile != "") {
		std::string tmpFilename(basicShaderFile);
		if (tmpFilename.find(':') == std::string::npos) {
			// get the relative position of the current file ...
			etk::FSNode file(m_name);
			tmpFilename = file.getRelativeFolder() + basicShaderFile;
			EWOL_DEBUG("Shaper try load shader : '" << tmpFilename << "' with base : '" << basicShaderFile << "'");
		} else {
			EWOL_DEBUG("Shaper try load shader : '" << tmpFilename << "'");
		}
		// get the shader resource :
		m_GLPosition = 0;
		m_GLprogram = ewol::resource::Program::keep(tmpFilename);
		if (m_GLprogram != NULL) {
			m_GLPosition        = m_GLprogram->getAttribute("EW_coord2d");
			m_GLMatrix          = m_GLprogram->getUniform("EW_MatrixTransformation");
			// Widget property  == > for the Vertex shader
			m_GLPropertySize       = m_GLprogram->getUniform("EW_widgetProperty.size");
			m_GLPropertyOrigin     = m_GLprogram->getUniform("EW_widgetProperty.origin");
			m_GLPropertyInsidePos  = m_GLprogram->getUniform("EW_widgetProperty.insidePos");
			m_GLPropertyInsideSize = m_GLprogram->getUniform("EW_widgetProperty.insideSize");
			// status property  == > for the fragment shader
			m_GLStateActivate      = m_GLprogram->getUniform("EW_status.activate");
			m_GLStateOld           = m_GLprogram->getUniform("EW_status.stateOld");
			m_GLStateNew           = m_GLprogram->getUniform("EW_status.stateNew");
			m_GLStateTransition    = m_GLprogram->getUniform("EW_status.transition");
			// for the texture ID : 
			m_GLtexID              = m_GLprogram->getUniform("EW_texID");
		}
		std::string basicImageFile = m_config->getString(m_confImageFile);
		if (basicImageFile != "") {
			std::string tmpFilename(basicImageFile);
			if (tmpFilename.find(':') == std::string::npos) {
				// get the relative position of the current file ...
				etk::FSNode file(m_name);
				tmpFilename = file.getRelativeFolder() + basicImageFile;
				EWOL_DEBUG("Shaper try load shaper image : '" << tmpFilename << "' with base : '" << basicImageFile << "'");
			} else {
				EWOL_DEBUG("Shaper try load shaper image : '" << tmpFilename << "'");
			}
			ivec2 size(64,64);
			m_resourceTexture = ewol::resource::TextureFile::keep(tmpFilename, size);
		}
	}
	std::string basicColorFile = m_config->getString(m_confColorFile);
	if (basicColorFile != "") {
		std::string tmpFilename(basicColorFile);
		if (tmpFilename.find(':') == std::string::npos) {
			// get the relative position of the current file ...
			etk::FSNode file(m_name);
			tmpFilename = file.getRelativeFolder() + basicColorFile;
			EWOL_DEBUG("Shaper try load colorFile : '" << tmpFilename << "' with base : '" << basicColorFile << "'");
		} else {
			EWOL_DEBUG("Shaper try load colorFile : '" << tmpFilename << "'");
		}
		m_colorProperty = ewol::resource::ColorFile::keep(tmpFilename);
		if (    m_GLprogram != NULL
		     && m_colorProperty != NULL) {
			std::vector<std::string> listColor = m_colorProperty->getColors();
			for (auto tmpColor : listColor) {
				int32_t glId = m_GLprogram->getUniform(tmpColor);
				int32_t colorID = m_colorProperty->request(tmpColor);
				m_listAssiciatedId.push_back(ivec2(glId, colorID));
			}
		}
	}
}

void ewol::compositing::Shaper::draw(bool _disableDepthTest) {
	if (m_config == NULL) {
		// this is a normale case ... the user can choice to have no config basic file ...
		return;
	}
	if (m_GLprogram == NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	//glScalef(m_scaling.x, m_scaling.y, 1.0);
	m_GLprogram->use();
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = ewol::openGL::getMatrix();
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->sendAttribute(m_GLPosition, 2/*x,y*/, m_coord);
	// all entry parameters :
	m_GLprogram->uniform2(m_GLPropertySize,       m_propertySize);
	m_GLprogram->uniform2(m_GLPropertyOrigin,     m_propertyOrigin);
	m_GLprogram->uniform2(m_GLPropertyInsidePos,  m_propertyInsidePosition);
	m_GLprogram->uniform2(m_GLPropertyInsideSize, m_propertyInsideSize);
	m_GLprogram->uniform1i(m_GLStateActivate,     m_stateActivate);
	m_GLprogram->uniform1i(m_GLStateOld,        m_stateOld);
	m_GLprogram->uniform1i(m_GLStateNew,        m_stateNew);
	m_GLprogram->uniform1f(m_GLStateTransition, m_stateTransition);
	for (auto element : m_listAssiciatedId) {
		m_GLprogram->uniform(element.x(), m_colorProperty->get(element.y()));
	}
	if (NULL!=m_resourceTexture) {
		// TextureID
		m_GLprogram->setTexture0(m_GLtexID, m_resourceTexture->getId());
	}
	// Request the draw of the elements : 
	ewol::openGL::drawArrays(GL_TRIANGLES, 0, 6);
	m_GLprogram->unUse();
}

void ewol::compositing::Shaper::clear(void) {
	// nothing to do ...
	m_propertySize = vec2(0,0);
	m_propertyOrigin = vec2(0,0);
	m_propertyInsidePosition = vec2(0,0);
	m_propertyInsideSize = vec2(0,0);
	memset(m_coord, 0, sizeof(m_coord));
}

bool ewol::compositing::Shaper::changeStatusIn(int32_t _newStatusId) {
	if (_newStatusId != m_stateNew) {
		m_nextStatusRequested = _newStatusId;
		return true;
	}
	if(    m_nextStatusRequested != -1
	    || m_stateNew != m_stateOld) {
		return true;
	}
	return false;
}

bool ewol::compositing::Shaper::periodicCall(const ewol::event::Time& _event) {
	//EWOL_DEBUG("call=" << _event);
	// start :
	if (m_stateTransition >= 1.0) {
		m_stateOld = m_stateNew;
		if(    m_nextStatusRequested != -1
		    && m_nextStatusRequested != m_stateOld) {
			m_stateNew = m_nextStatusRequested;
			m_nextStatusRequested = -1;
			m_stateTransition = 0.0;
			//EWOL_DEBUG("     ##### START #####  ");
		} else {
			m_nextStatusRequested = -1;
			// disable periodic call ...
			return false;
		}
	}
	if (m_stateTransition<1.0) {
		// check if no new state requested:
		if (m_nextStatusRequested != -1 && m_stateTransition<0.5) {
			// invert sources with destination
			int32_t tmppp = m_stateOld;
			m_stateOld = m_stateNew;
			m_stateNew = tmppp;
			m_stateTransition = 1.0 - m_stateTransition;
			if (m_nextStatusRequested == m_stateNew) {
				m_nextStatusRequested = -1;
			}
		}
		float timeRelativity = m_config->getNumber(m_confIdChangeTime) / 1000.0;
		m_stateTransition += _event.getDeltaCall() / timeRelativity;
		//m_stateTransition += _event.getDeltaCall();
		m_stateTransition = etk_avg(0.0f, m_stateTransition, 1.0f);
		//EWOL_DEBUG("relative=" << timeRelativity << " Transition : " << m_stateTransition);
	}
	return true;
}

void ewol::compositing::Shaper::updateVertex(void) {
	// set coord  == > must be a static VBO ...
	m_coord[0].setValue( m_propertyOrigin.x(),
	                     m_propertyOrigin.y()+m_propertySize.y());
	m_coord[1].setValue( m_propertyOrigin.x(),
	                     m_propertyOrigin.y());
	m_coord[2].setValue( m_propertyOrigin.x()+m_propertySize.x(),
	                     m_propertyOrigin.y());
	
	m_coord[3].setValue( m_propertyOrigin.x()+m_propertySize.x(),
	                     m_propertyOrigin.y());
	m_coord[4].setValue( m_propertyOrigin.x()+m_propertySize.x(),
	                     m_propertyOrigin.y()+m_propertySize.y());
	m_coord[5].setValue( m_propertyOrigin.x(),
	                     m_propertyOrigin.y()+m_propertySize.y());
}

void ewol::compositing::Shaper::setOrigin(const vec2& _newOri) {
	if (m_propertyOrigin != _newOri) {
		m_propertyOrigin = _newOri;
		updateVertex();
	}
}

void ewol::compositing::Shaper::setSize(const vec2& _newSize) {
	if (m_propertySize != _newSize) {
		m_propertySize = _newSize;
		updateVertex();
	}
}

void ewol::compositing::Shaper::setInsideSize(const vec2& _newInsideSize) {
	m_propertyInsideSize = _newInsideSize;
}

void ewol::compositing::Shaper::setInsidePos(const vec2& _newInsidePos) {
	m_propertyInsidePosition = _newInsidePos;
}

ewol::Padding ewol::compositing::Shaper::getPadding(void) {
	ewol::Padding padding(0,0,0,0);
	if (m_config != NULL) {
		padding.setValue(m_config->getNumber(m_confIdPaddingLeft),
		                 m_config->getNumber(m_confIdPaddingTop),
		                 m_config->getNumber(m_confIdPaddingRight),
		                 m_config->getNumber(m_confIdPaddingButtom));
	}
	return padding;
}

void ewol::compositing::Shaper::setSource(const std::string& _newFile) {
	clear();
	unLoadProgram();
	m_name = _newFile;
	loadProgram();
}

bool ewol::compositing::Shaper::hasSources(void) {
	return m_GLprogram!=NULL;
}


const etk::Color<float>& ewol::compositing::Shaper::getColor(int32_t _id) {
	static const etk::Color<float> errorValue(0,0,0,0);
	if (m_colorProperty == NULL) {
		return errorValue;
	}
	return m_colorProperty->get(_id);
}
