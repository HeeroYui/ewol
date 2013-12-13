/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Manager.h>
#include <ewol/widget/Joystick.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/ButtonColor.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Slider.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/ProgressBar.h>
#include <ewol/widget/Layer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Image.h>
#include <ewol/widget/Gird.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Scroll.h>
#include <ewol/widget/PopUp.h>
#include <vector>

#undef __class__
#define __class__ "ewol::widget::Manager"

ewol::widget::Manager::Manager(void) :
  m_focusWidgetDefault(NULL),
  m_focusWidgetCurrent(NULL),
  m_havePeriodic(false),
  m_haveRedraw(true),
  m_applWakeUpTime(0),
  m_lastPeriodicCallTime(0) {
	EWOL_DEBUG(" == > init Widget-Manager");
	// set the basic time properties :
	m_applWakeUpTime = ewol::getTime();
	m_lastPeriodicCallTime = ewol::getTime();
	
	ewol::widget::Button::init(*this);
	ewol::widget::ButtonColor::init(*this);
	ewol::widget::Spacer::init(*this);
	ewol::widget::Slider::init(*this);
	ewol::widget::Sizer::init(*this);
	ewol::widget::ProgressBar::init(*this);
	ewol::widget::Layer::init(*this);
	ewol::widget::Label::init(*this);
	ewol::widget::Image::init(*this);
	ewol::widget::Gird::init(*this);
	ewol::widget::Entry::init(*this);
	ewol::widget::CheckBox::init(*this);
	ewol::widget::Scroll::init(*this);
	ewol::widget::ContextMenu::init(*this);
	ewol::widget::PopUp::init(*this);
}

ewol::widget::Manager::~Manager(void) {
	EWOL_DEBUG(" == > Un-Init Widget-Manager");
	EWOL_INFO("Realease all FOCUS");
	focusSetDefault(NULL);
	focusRelease();
	
	m_listOfPeriodicWidget.clear();
	m_creatorList.clear();
}

void ewol::widget::Manager::rm(ewol::Widget* _newWidget) {
	periodicCallRm(_newWidget);
	focusRemoveIfRemove(_newWidget);
}

/* *************************************************************************
 * focus Area : 
 * *************************************************************************/

void ewol::widget::Manager::focusKeep(ewol::Widget* _newWidget) {
	if (NULL == _newWidget) {
		// nothing to do ...
		return;
	}
	if (false == _newWidget->canHaveFocus()) {
		EWOL_VERBOSE("Widget can not have focus, id=" << 9999999999.999);
		return;
	}
	if (_newWidget == m_focusWidgetCurrent) {
		// nothing to do ...
		return;
	}
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Rm focus on WidgetID=" << m_focusWidgetCurrent->getId() );
		m_focusWidgetCurrent->rmFocus();
	}
	m_focusWidgetCurrent = _newWidget;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set focus on WidgetID=" << m_focusWidgetCurrent->getId() );
		m_focusWidgetCurrent->setFocus();
	}
}

void ewol::widget::Manager::focusSetDefault(ewol::Widget * _newWidget) {
	if(    NULL != _newWidget
	    && false == _newWidget->canHaveFocus() ) {
		EWOL_VERBOSE("Widget can not have focus, id=" << _newWidget->getId() );
		return;
	}
	if (m_focusWidgetDefault == m_focusWidgetCurrent) {
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Rm focus on WidgetID=" << m_focusWidgetCurrent->getId() );
			m_focusWidgetCurrent->rmFocus();
		}
		m_focusWidgetCurrent = _newWidget;
		if (NULL != m_focusWidgetCurrent) {
			EWOL_DEBUG("Set focus on WidgetID=" << m_focusWidgetCurrent->getId() );
			m_focusWidgetCurrent->setFocus();
		}
	}
	m_focusWidgetDefault = _newWidget;
}

void ewol::widget::Manager::focusRelease(void) {
	if (m_focusWidgetDefault == m_focusWidgetCurrent) {
		// nothink to do ...
		return;
	}
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Rm focus on WidgetID=" << m_focusWidgetCurrent->getId() );
		m_focusWidgetCurrent->rmFocus();
	}
	m_focusWidgetCurrent = m_focusWidgetDefault;
	if (NULL != m_focusWidgetCurrent) {
		EWOL_DEBUG("Set focus on WidgetID=" << m_focusWidgetCurrent->getId() );
		m_focusWidgetCurrent->setFocus();
	}
}


ewol::Widget * ewol::widget::Manager::focusGet(void) {
	return m_focusWidgetCurrent;
}

void ewol::widget::Manager::focusRemoveIfRemove(ewol::Widget* _newWidget) {
	if (m_focusWidgetCurrent == _newWidget) {
		EWOL_WARNING("Release focus when remove widget");
		focusRelease();
	}
	if (m_focusWidgetDefault == _newWidget) {
		EWOL_WARNING("Release default focus when remove widget");
		focusSetDefault(NULL);
	}
}

void ewol::widget::Manager::periodicCallAdd(ewol::Widget* _pWidget) {
	for (size_t iii=0; iii < m_listOfPeriodicWidget.size(); iii++) {
		if (m_listOfPeriodicWidget[iii] == _pWidget) {
			return;
		}
	}
	for (size_t iii=0; iii < m_listOfPeriodicWidget.size(); iii++) {
		if (NULL == m_listOfPeriodicWidget[iii]) {
			m_listOfPeriodicWidget[iii] = _pWidget;
			return;
		}
	}
	m_listOfPeriodicWidget.push_back(_pWidget);
	m_havePeriodic = true;
}

void ewol::widget::Manager::periodicCallRm(ewol::Widget * _pWidget) {
	int32_t nbElement = 0;
	for (int32_t iii=m_listOfPeriodicWidget.size()-1; iii >= 0 ; iii--) {
		if (m_listOfPeriodicWidget[iii] == _pWidget) {
			m_listOfPeriodicWidget[iii] = NULL;
		} else {
			nbElement++;
		}
	}
	if (0 == nbElement) {
		m_havePeriodic = false;
	}
}

void ewol::widget::Manager::periodicCallResume(int64_t _localTime) {
	m_lastPeriodicCallTime = _localTime;
}

void ewol::widget::Manager::periodicCall(int64_t _localTime) {
	int64_t previousTime = m_lastPeriodicCallTime;
	m_lastPeriodicCallTime = _localTime;
	if (m_listOfPeriodicWidget.size() <= 0) {
		return;
	}
	float deltaTime = (float)(_localTime - previousTime)/1000000.0;
	
	ewol::event::Time myTime(_localTime, m_applWakeUpTime, deltaTime, deltaTime);
	
	EWOL_VERBOSE("periodic : " << _localTime);
	for (int32_t iii=m_listOfPeriodicWidget.size()-1; iii >= 0 ; iii--) {
		if (NULL != m_listOfPeriodicWidget[iii]) {
			int64_t deltaTimeCallUser = m_listOfPeriodicWidget[iii]->systemGetCallDeltaTime();
			if (deltaTimeCallUser <= 0) {
				myTime.setDeltaCall(deltaTime);
				EWOL_VERBOSE("[" << iii << "] periodic : " << myTime);
				m_listOfPeriodicWidget[iii]->systemSetLastCallTime(_localTime);
				m_listOfPeriodicWidget[iii]->periodicCall(myTime);
			} else {
				int64_t lastCallTime = m_listOfPeriodicWidget[iii]->systemGetLastCallTime();
				if (lastCallTime == 0) {
					lastCallTime = _localTime;
				}
				float deltaLocalTime = (float)(_localTime-lastCallTime)/1000000.0;;
				if (deltaLocalTime >=  lastCallTime) {
					myTime.setDeltaCall(deltaLocalTime);
					EWOL_VERBOSE("[" << iii << "] periodic : " << myTime);
					m_listOfPeriodicWidget[iii]->systemSetLastCallTime(_localTime);
					m_listOfPeriodicWidget[iii]->periodicCall(myTime);
				}
			}
		}
	}
}

bool ewol::widget::Manager::periodicCallHave(void) {
	return m_havePeriodic;
}

void ewol::widget::Manager::markDrawingIsNeeded(void) {
	m_haveRedraw = true;
}

bool ewol::widget::Manager::isDrawingNeeded(void) {
	bool tmp = m_haveRedraw;
	m_haveRedraw = false;
	return tmp;
}

// element that generate the list of elements
void ewol::widget::Manager::addWidgetCreator(const std::string& _name,
                                           ewol::widget::Manager::creator_tf _pointer) {
	if (NULL == _pointer) {
		return;
	}
	//Keep name in lower case :
	std::string nameLower = to_lower(_name);
	if (true == m_creatorList.exist(nameLower)) {
		EWOL_WARNING("Replace Creator of a specify widget : " << nameLower);
		m_creatorList[nameLower] = _pointer;
		return;
	}
	EWOL_INFO("Add Creator of a specify widget : " << nameLower);
	m_creatorList.add(nameLower, _pointer);
}

ewol::Widget* ewol::widget::Manager::create(const std::string& _name) {
	std::string nameLower = to_lower(_name);
	if (true == m_creatorList.exist(nameLower)) {
		ewol::widget::Manager::creator_tf pointerFunction = m_creatorList[nameLower];
		if (NULL != pointerFunction) {
			return pointerFunction();
		}
	}
	EWOL_WARNING("try to create an UnExistant widget : " << nameLower);
	return NULL;
}

bool ewol::widget::Manager::exist(const std::string& _name) {
	std::string nameLower = to_lower(_name);
	return m_creatorList.exist(nameLower);
}

std::string ewol::widget::Manager::list(void) {
	std::string tmpVal;
	for (int32_t iii=0; iii<m_creatorList.size() ; iii++) {
		tmpVal += m_creatorList.getKey(iii);
		tmpVal += ",";
	}
	return tmpVal;
}


