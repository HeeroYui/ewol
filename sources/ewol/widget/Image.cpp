/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Image.h>
#include <ewol/compositing/Image.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>
#include <ewol/ewol.h>


#undef __class__
#define __class__ "Image"

static ewol::Widget* create(void) {
	return new ewol::widget::Image();
}

void ewol::widget::Image::init(ewol::widget::Manager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}


const char * const ewol::widget::Image::eventPressed = "ewol-widget-image-event-pressed";

const char * const ewol::widget::Image::configRatio = "ratio";
const char * const ewol::widget::Image::configSize = "size";
const char * const ewol::widget::Image::configBorder = "border";
const char * const ewol::widget::Image::configSource = "src";

ewol::widget::Image::Image(const std::string& _file, const ewol::Dimension& _border) :
  m_imageSize(vec2(0,0)),
  m_keepRatio(true) {
	addObjectType("ewol::widget::Image");
	addEventId(eventPressed);
	registerConfig(configRatio, "bool", NULL, "Keep ratio of the image");
	registerConfig(configSize, "Dimension", NULL, "Basic display size of the image");
	registerConfig(configBorder, "Dimension", NULL, "Border of the image");
	registerConfig(configSource, "string", "Image source path");
	set(_file, _border);
}


void ewol::widget::Image::setFile(const std::string& _file) {
	EWOL_VERBOSE("Set Image : " << _file);
	if (m_fileName != _file) {
		// copy data :
		m_fileName = _file;
		// force redraw all :
		markToRedraw();
		EWOL_VERBOSE("Set sources : " << m_fileName << " size=" << m_imageSize);
		m_compositing.setSource(m_fileName, m_imageSize.getPixel());
	}
}

void ewol::widget::Image::setBorder(const ewol::Dimension& _border) {
	EWOL_VERBOSE("Set border=" << _border);
	// copy data :
	m_border = _border;
	// force redraw all :
	markToRedraw();
	// TODO : change the size with no size requested ...
	requestUpdateSize();
}

void ewol::widget::Image::setKeepRatio(bool _keep) {
	if (m_keepRatio != _keep) {
		// copy data :
		m_keepRatio = _keep;
		// force redraw all :
		markToRedraw();
		requestUpdateSize();
	}
}

void ewol::widget::Image::setImageSize(const ewol::Dimension& _size) {
	EWOL_VERBOSE("Set Image size : " << _size);
	if (_size != m_imageSize) {
		m_imageSize = _size;
		markToRedraw();
		requestUpdateSize();
		EWOL_VERBOSE("Set sources : " << m_fileName << " size=" << m_imageSize);
		m_compositing.setSource(m_fileName, m_imageSize.getPixel());
	}
}

void ewol::widget::Image::set(const std::string& _file, const ewol::Dimension& _border) {
	EWOL_VERBOSE("Set Image : " << _file << " border=" << _border);
	// copy data :
	if (m_border != _border) {
		m_border = _border;
		requestUpdateSize();
		markToRedraw();
	}
	setFile(_file);
}

void ewol::widget::Image::onDraw(void) {
	m_compositing.draw();
}

void ewol::widget::Image::onRegenerateDisplay(void) {
	if (true == needRedraw()) {
		// remove data of the previous composition :
		m_compositing.clear();
		
		// calculate the new position and size :
		vec2 imageBoder = m_border.getPixel();
		vec2 origin = imageBoder;
		imageBoder *= 2.0f;
		vec2 imageRealSize = m_minSize - imageBoder;
		vec2 imageRealSizeMax = m_size - imageBoder;
		
		if (m_userFill.x() == true) {
			imageRealSize.setX(imageRealSizeMax.x());
		} else {
			origin.setX(origin.x() + (m_size.x()-m_minSize.x())*0.5f);
		}
		if (m_userFill.y() == true) {
			imageRealSize.setY(imageRealSizeMax.y());
		} else {
			origin.setY(origin.y() + (m_size.y()-m_minSize.y())*0.5f);
		}
		if (m_keepRatio == true) {
			vec2 tmpSize = m_compositing.getRealSize();
			float ratio = tmpSize.x() / tmpSize.y();
			float ratioCurrent = imageRealSize.x() / imageRealSize.y();
			if (ratio == ratioCurrent) {
				// nothing to do ...
			} else if (ratio < ratioCurrent) {
				float oldX = imageRealSize.x();
				imageRealSize.setX(imageRealSize.y()*ratio);
				origin += vec2((oldX - imageRealSize.x()) * 0.5f, 0);
			} else {
				float oldY = imageRealSize.y();
				imageRealSize.setY(imageRealSize.x()/ratio);
				origin += vec2(0, (oldY - imageRealSize.y()) * 0.5f);
			}
		}
		
		// set the somposition properties :
		m_compositing.setPos(origin);
		m_compositing.print(imageRealSize);
		//EWOL_DEBUG("Paint Image at : " << origin << " size=" << imageRealSize << "  origin=" << origin);
	}
}

void ewol::widget::Image::calculateMinMaxSize(void) {
	vec2 imageBoder = m_border.getPixel()*2.0f;
	vec2 imageSize = m_imageSize.getPixel();
	if (imageSize!=vec2(0,0)) {
		m_minSize = imageBoder+imageSize;
		m_maxSize = m_minSize;
	} else {
		vec2 imageSizeReal = m_compositing.getRealSize();
		vec2 min1 = imageBoder+m_userMinSize.getPixel();
		m_minSize = imageBoder+imageSizeReal;
		//EWOL_DEBUG(" set max : " << m_minSize << " " << min1);
		m_minSize.setMax(min1);
		//EWOL_DEBUG("     result : " << m_minSize);
		m_maxSize = imageBoder+m_userMaxSize.getPixel();
		m_minSize.setMin(m_maxSize);
	}
	//EWOL_DEBUG("set widget min=" << m_minSize << " max=" << m_maxSize << " with real Image size=" << imageSizeReal);
	markToRedraw();
}


bool ewol::widget::Image::onEventInput(const ewol::event::Input& _event) {
	//EWOL_DEBUG("Event on BT ...");
	if (1 == _event.getId()) {
		if(ewol::key::statusSingle == _event.getStatus()) {
			generateEventId(eventPressed);
			return true;
		}
	}
	return false;
}

bool ewol::widget::Image::loadXML(exml::Element* _node) {
	if (NULL == _node) {
		return false;
	}
	ewol::Widget::loadXML(_node);
	// get internal data : 
	
	std::string tmpAttributeValue = _node->getAttribute("ratio");
	if (tmpAttributeValue.size()!=0) {
		if (compare_no_case(tmpAttributeValue, "true") == true) {
			m_keepRatio = true;
		} else if (tmpAttributeValue == "1") {
			m_keepRatio = true;
		} else {
			m_keepRatio = false;
		}
	}
	tmpAttributeValue = _node->getAttribute("size");
	if (tmpAttributeValue.size()!=0) {
		//EWOL_CRITICAL(" Parse SIZE : " << tmpAttributeValue);
		m_imageSize = tmpAttributeValue;
		//EWOL_CRITICAL("               == > " << m_imageSize);
	}
	tmpAttributeValue = _node->getAttribute("border");
	if (tmpAttributeValue.size()!=0) {
		m_border = tmpAttributeValue;
	}
	//EWOL_DEBUG("Load label:" << node->ToElement()->getText());
	if (_node->size()!=0) {
		setFile(_node->getText());
	} else {
		tmpAttributeValue = _node->getAttribute("src");
		if (tmpAttributeValue.size()!=0) {
			setFile(tmpAttributeValue);
		}
	}
	return true;
}

bool ewol::widget::Image::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::Widget::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configRatio) {
		setKeepRatio(stobool(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configSize) {
		setImageSize(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configBorder) {
		setBorder(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configSource) {
		setFile(_conf.getData());
		return true;
	}
	return false;
}

bool ewol::widget::Image::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::Widget::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configRatio) {
		if (true == getKeepRatio()) {
			_result = "true";
		} else {
			_result = "false";
		}
		return true;
	}
	if (_config == configSize) {
		_result = getImageSize();
		return true;
	}
	if (_config == configBorder) {
		_result = getBorder();
		return true;
	}
	if (_config == configSource) {
		_result = getFile();
		return true;
	}
	return false;
}

