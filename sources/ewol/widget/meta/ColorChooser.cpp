/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/meta/ColorChooser.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Manager.h>
//#include <vector>
#include <vector>

extern "C" {
	// file browsing ...
	#include <dirent.h>
}

#undef __class__
#define __class__	"ColorChooser"


const char * const ewol::widget::ColorChooser::eventChange = "change";


static const char * const eventColorBarHasChange          = "event-color-bar-has-change";
static const char * const eventColorSpecificHasChange     = "event-color-specific-has-change";


ewol::widget::ColorChooser::ColorChooser() :
  ewol::widget::Sizer(ewol::widget::Sizer::modeVert) {
	addObjectType("ewol::widget::ColorChooser");
	addEventId(eventChange);
	m_widgetColorBar = nullptr;
	m_widgetRed = nullptr;
	m_widgetGreen = nullptr;
	m_widgetBlue = nullptr;
	m_widgetAlpha = nullptr;
	lockExpand(bvec2(true,true));
		m_widgetColorBar = new ewol::widget::ColorBar();
			m_widgetColorBar->registerOnEvent(this, "change", eventColorBarHasChange);
			m_widgetColorBar->setFill(bvec2(true,true));
			/*
			m_widgetColorBar->setWidth(200);
			m_widgetColorBar->setHeigh(200);
			*/
			subWidgetAdd(m_widgetColorBar.get());
		
		etk::Color<> sliderColor;
		sliderColor = etk::color::black;
		
		m_widgetRed = new ewol::widget::Slider();
			m_widgetRed->registerOnEvent(this, "change", eventColorSpecificHasChange);
			m_widgetRed->setExpand(bvec2(true,false));
			m_widgetRed->setFill(bvec2(true,false));
			m_widgetRed->setMin(0);
			m_widgetRed->setMax(255);
			sliderColor = 0xFF0000FF;
			m_widgetRed->setColor(sliderColor);
			subWidgetAdd(m_widgetRed.get());
		m_widgetGreen = new ewol::widget::Slider();
			m_widgetGreen->registerOnEvent(this, "change", eventColorSpecificHasChange);
			m_widgetGreen->setExpand(bvec2(true,false));
			m_widgetGreen->setFill(bvec2(true,false));
			m_widgetGreen->setMin(0);
			sliderColor = 0x00FF00FF;
			m_widgetGreen->setColor(sliderColor);
			m_widgetGreen->setMax(255);
			subWidgetAdd(m_widgetGreen.get());
		m_widgetBlue = new ewol::widget::Slider();
			m_widgetBlue->registerOnEvent(this, "change", eventColorSpecificHasChange);
			m_widgetBlue->setExpand(bvec2(true,false));
			m_widgetBlue->setFill(bvec2(true,false));
			m_widgetBlue->setMin(0);
			sliderColor = 0x0000FFFF;
			m_widgetBlue->setColor(sliderColor);
			m_widgetBlue->setMax(255);
			subWidgetAdd(m_widgetBlue.get());
		m_widgetAlpha = new ewol::widget::Slider();
			m_widgetAlpha->registerOnEvent(this, "change", eventColorSpecificHasChange);
			m_widgetAlpha->setExpand(bvec2(true,false));
			m_widgetAlpha->setFill(bvec2(true,false));
			m_widgetAlpha->setMin(0);
			m_widgetAlpha->setMax(255);
			subWidgetAdd(m_widgetAlpha.get());
	
	m_currentColor = etk::color::white;
}


ewol::widget::ColorChooser::~ColorChooser() {
	
}


void ewol::widget::ColorChooser::setColor(etk::Color<> _newColor) {
	m_currentColor = _newColor;
	if (nullptr != m_widgetRed) {
		m_widgetRed->setValue(m_currentColor.r());
	}
	if (nullptr != m_widgetGreen) {
		m_widgetGreen->setValue(m_currentColor.g());
	}
	if (nullptr != m_widgetBlue) {
		m_widgetBlue->setValue(m_currentColor.b());
	}
	if (nullptr != m_widgetAlpha) {
		m_widgetAlpha->setValue(m_currentColor.a());
	}
	if (nullptr != m_widgetColorBar) {
		m_widgetColorBar->setCurrentColor(m_currentColor);
	}
}


etk::Color<> ewol::widget::ColorChooser::getColor() {
	return m_currentColor;
}


void ewol::widget::ColorChooser::onReceiveMessage(const ewol::object::Message& _msg) {
	if (nullptr == _msg.getCaller()) {
		return;
	}
	//EWOL_INFO("Receive Extern Event ... : widgetPointer=" << CallerObject << "\"" << eventId << "\"  == > data=\"" << data << "\"" );
	if (eventColorBarHasChange == _msg.getMessage()) {
		// == > colorBar has change ...
		uint8_t tmpAlpha = m_currentColor.a();
		// the colorbar has no notion of the alpha  == > keep it ...
		if (nullptr != m_widgetColorBar) {
			m_currentColor = m_widgetColorBar->getCurrentColor();
		}
		m_currentColor.setA(tmpAlpha);
		if (nullptr != m_widgetRed) {
			m_widgetRed->setValue(m_currentColor.r());
		}
		if (nullptr != m_widgetGreen) {
			m_widgetGreen->setValue(m_currentColor.g());
		}
		if (nullptr != m_widgetBlue) {
			m_widgetBlue->setValue(m_currentColor.b());
		}
		if (nullptr != m_widgetAlpha) {
			m_widgetAlpha->setValue(m_currentColor.a());
		}
		generateEventId(eventChange, m_currentColor.getString());
	} else if (eventColorSpecificHasChange == _msg.getMessage()) {
		// Slider has changes his color  == > get the one change ...
		if (_msg.getCaller() == m_widgetRed) {
			m_currentColor.setR(m_widgetRed->getValue());
		}
		if (_msg.getCaller() == m_widgetGreen) {
			m_currentColor.setG(m_widgetGreen->getValue());
		}
		if (_msg.getCaller() == m_widgetBlue) {
			m_currentColor.setB(m_widgetBlue->getValue());
		}
		if (_msg.getCaller() == m_widgetAlpha) {
			m_currentColor.setA(m_widgetAlpha->getValue());
		}
		if (nullptr != m_widgetColorBar) {
			m_widgetColorBar->setCurrentColor(m_currentColor);
		}
		generateEventId(eventChange, m_currentColor.getString());
	}
};


void ewol::widget::ColorChooser::onObjectRemove(ewol::object::Shared<ewol::Object> _removeObject) {
	// First step call parrent : 
	ewol::widget::Sizer::onObjectRemove(_removeObject);
	// second step find if in all the elements ...
	if(_removeObject == m_widgetRed) {
		m_widgetRed.reset();
	}
	if(_removeObject == m_widgetGreen) {
		m_widgetGreen.reset();
	}
	if(_removeObject == m_widgetBlue) {
		m_widgetBlue.reset();
	}
	if(_removeObject == m_widgetAlpha) {
		m_widgetAlpha.reset();
	}
	if(_removeObject == m_widgetColorBar) {
		m_widgetColorBar.reset();
	}
}

