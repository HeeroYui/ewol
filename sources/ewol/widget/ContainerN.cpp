/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/Manager.h>

#undef __class__
#define __class__	"ContainerN"


ewol::widget::ContainerN::ContainerN() :
  m_lockExpand(false,false),
  m_subExpend(false,false) {
	addObjectType("ewol::widget::ContainerN");
	// nothing to do ...
}

ewol::widget::ContainerN::~ContainerN() {
	subWidgetRemoveAll();
}


bvec2 ewol::widget::ContainerN::canExpand() {
	bvec2 res = m_userExpand;
	if (false == m_lockExpand.x()) {
		if (true == m_subExpend.x()) {
			res.setX(true);
		}
	}
	if (false == m_lockExpand.y()) {
		if (true == m_subExpend.y()) {
			res.setY(true);
		}
	}
	//EWOL_DEBUG("Expend check : user=" << m_userExpand << " lock=" << m_lockExpand << " sub=" << m_subExpend << " res=" << res);
	return res;
}

void ewol::widget::ContainerN::lockExpand(const bvec2& _lockExpand) {
	if (_lockExpand != m_lockExpand) {
		m_lockExpand = _lockExpand;
		markToRedraw();
		requestUpdateSize();
	}
}


int32_t ewol::widget::ContainerN::subWidgetAdd(ewol::object::Shared<ewol::Widget> _newWidget) {
	if (nullptr == _newWidget) {
		EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Try to add An empty Widget ... ");
		return -1;
	}
	if (_newWidget!=nullptr) {
		_newWidget->setUpperWidget(this);
	}
	m_subWidget.push_back(_newWidget);
	markToRedraw();
	requestUpdateSize();
	// added at the last eelement :
	return _newWidget->getId();
}

int32_t ewol::widget::ContainerN::subWidgetAddStart(ewol::object::Shared<ewol::Widget> _newWidget) {
	if (nullptr == _newWidget) {
		EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Try to add start An empty Widget ... ");
		return -1;
	}
	if (_newWidget!=nullptr) {
		_newWidget->setUpperWidget(this);
	}
	m_subWidget.insert(m_subWidget.begin(), _newWidget);
	markToRedraw();
	requestUpdateSize();
	return _newWidget->getId();
}

void ewol::widget::ContainerN::subWidgetRemove(ewol::object::Shared<ewol::Widget> _newWidget) {
	if (nullptr == _newWidget) {
		return;
	}
	size_t errorControl = m_subWidget.size();
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (_newWidget == m_subWidget[iii]) {
			m_subWidget[iii]->removeUpperWidget();
			m_subWidget[iii]->removeObject();
			// no remove, this element is removed with the function onObjectRemove  == > it does not exist anymore ...
			if (errorControl == m_subWidget.size()) {
				EWOL_CRITICAL("[" << getId() << "] {" << getObjectType() << "} The number of element might have been reduced ...  == > it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[iii] = nullptr;
				m_subWidget.erase(m_subWidget.begin()+iii);
			}
			markToRedraw();
			requestUpdateSize();
			return;
		}
	}
}

void ewol::widget::ContainerN::subWidgetUnLink(ewol::object::Shared<ewol::Widget> _newWidget) {
	if (nullptr == _newWidget) {
		return;
	}
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (_newWidget == m_subWidget[iii]) {
			m_subWidget[iii]->removeUpperWidget();
			m_subWidget[iii] = nullptr;
			m_subWidget.erase(m_subWidget.begin()+iii);
			markToRedraw();
			requestUpdateSize();
			return;
		}
	}
}

void ewol::widget::ContainerN::subWidgetRemoveAll() {
	size_t errorControl = m_subWidget.size();
	// the size automaticly decrement with the auto call of the onObjectRemove function
	while (m_subWidget.size() > 0 ) {
		if (nullptr != m_subWidget[0]) {
			m_subWidget[0]->removeUpperWidget();
			m_subWidget[0]->removeObject();
			// no remove, this element is removed with the function onObjectRemove  == > it does not exist anymore ...
			if (errorControl == m_subWidget.size()) {
				EWOL_CRITICAL("[" << getId() << "] {" << getObjectType() << "} The number of element might have been reduced ...  == > it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[0] = nullptr;
			}
		} else {
			EWOL_WARNING("[" << getId() << "] {" << getObjectType() << "} Must not have null pointer on the subWidget list ...");
			m_subWidget.erase(m_subWidget.begin());
		}
		errorControl = m_subWidget.size();
	}
	m_subWidget.clear();
}

void ewol::widget::ContainerN::subWidgetRemoveAllDelayed() {
	// the size automaticly decrement with the auto call of the onObjectRemove function
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (nullptr != m_subWidget[iii]) {
			m_subWidget[iii]->removeUpperWidget();
			m_subWidget[iii]->removeObject();
			m_subWidget[iii] = nullptr;
		} else {
			EWOL_WARNING("[" << getId() << "] {" << getObjectType() << "} Must not have null pointer on the subWidget list ...");
		}
	}
	m_subWidget.clear();
}

ewol::object::Shared<ewol::Widget> ewol::widget::ContainerN::getWidgetNamed(const std::string& _widgetName) {
	ewol::object::Shared<ewol::Widget> tmpUpperWidget = ewol::Widget::getWidgetNamed(_widgetName);
	if (nullptr!=tmpUpperWidget) {
		return tmpUpperWidget;
	}
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (nullptr != m_subWidget[iii]) {
			ewol::object::Shared<ewol::Widget> tmpWidget = m_subWidget[iii]->getWidgetNamed(_widgetName);
			if (nullptr != tmpWidget) {
				return tmpWidget;
			}
		}
	}
	return nullptr;
}

void ewol::widget::ContainerN::onObjectRemove(ewol::object::Shared<ewol::Object> _removeObject) {
	// First step call parrent :
	ewol::Widget::onObjectRemove(_removeObject);
	// second step find if in all the elements ...
	for (int64_t iii=m_subWidget.size()-1; iii >= 0; iii--) {
		if(m_subWidget[iii] == _removeObject) {
			EWOL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} remove sizer sub Element [" << iii << "/" << m_subWidget.size()-1 << "]  == > destroyed object");
			m_subWidget[iii] = nullptr;
			m_subWidget.erase(m_subWidget.begin()+iii);
		}
	}
}

void ewol::widget::ContainerN::systemDraw(const ewol::DrawProperty& _displayProp) {
	if (true == m_hide){
		// widget is hidden ...
		return;
	}
	// local widget draw
	ewol::Widget::systemDraw(_displayProp);
	// subwidget draw
	ewol::DrawProperty prop = _displayProp;
	prop.limit(m_origin, m_size);
	for (int64_t iii=m_subWidget.size()-1; iii >= 0; iii--) {
		if (nullptr != m_subWidget[iii]) {
			m_subWidget[iii]->systemDraw(prop);
		}
	}
}

void ewol::widget::ContainerN::calculateSize(const vec2& _availlable) {
	m_size = _availlable;
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (nullptr != m_subWidget[iii]) {
			m_subWidget[iii]->setOrigin(m_origin+m_offset);
			m_subWidget[iii]->calculateSize(m_size);
		}
	}
	markToRedraw();
}

void ewol::widget::ContainerN::calculateMinMaxSize() {
	m_subExpend.setValue(false, false);
	m_minSize.setValue(0,0);
	m_maxSize.setValue(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE);
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} set min size : " <<  m_minSize);
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (nullptr != m_subWidget[iii]) {
			m_subWidget[iii]->calculateMinMaxSize();
			bvec2 subExpendProp = m_subWidget[iii]->canExpand();
			if (true == subExpendProp.x()) {
				m_subExpend.setX(true);
			}
			if (true == subExpendProp.y()) {
				m_subExpend.setY(true);
			}
			vec2 tmpSize = m_subWidget[iii]->getCalculateMinSize();
			m_minSize.setValue( etk_max(tmpSize.x(), m_minSize.x()),
			                    etk_max(tmpSize.y(), m_minSize.y()) );
		}
	}
	//EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} Result min size : " <<  m_minSize);
}

void ewol::widget::ContainerN::onRegenerateDisplay() {
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (nullptr != m_subWidget[iii]) {
			m_subWidget[iii]->onRegenerateDisplay();
		}
	}
}

ewol::object::Shared<ewol::Widget> ewol::widget::ContainerN::getWidgetAtPos(const vec2& _pos) {
	if (true == isHide()) {
		return nullptr;
	}
	// for all element in the sizer ...
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (nullptr != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->getSize();
			vec2 tmpOrigin = m_subWidget[iii]->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::object::Shared<ewol::Widget> tmpWidget = m_subWidget[iii]->getWidgetAtPos(_pos);
				if (nullptr != tmpWidget) {
					return tmpWidget;
				}
				// stop searching
				break;
			}
		}
	}
	return nullptr;
};


bool ewol::widget::ContainerN::loadXML(exml::Element* _node) {
	if (nullptr == _node) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::loadXML(_node);
	// remove previous element :
	subWidgetRemoveAll();
	
	std::string tmpAttributeValue = _node->getAttribute("lock");
	if (tmpAttributeValue.size()!=0) {
		m_lockExpand = tmpAttributeValue;
	}
	bool invertAdding=false;
	tmpAttributeValue = _node->getAttribute("addmode");
	if(compare_no_case(tmpAttributeValue, "invert")) {
		invertAdding=true;
	}
	// parse all the elements :
	for (size_t iii=0; iii < _node->size(); iii++) {
		exml::Element* pNode = _node->getElement(iii);
		if (pNode == nullptr) {
			// trash here all that is not element
			continue;
		}
		std::string widgetName = pNode->getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			EWOL_ERROR("[" << getId() << "] {" << getObjectType() << "} (l "<<pNode->getPos()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		EWOL_DEBUG("[" << getId() << "] {" << getObjectType() << "} load new element : \"" << widgetName << "\"");
		ewol::object::Shared<ewol::Widget> subWidget = getWidgetManager().create(widgetName);
		if (subWidget == nullptr) {
			EWOL_ERROR ("[" << getId() << "] {" << getObjectType() << "} (l "<<pNode->getPos()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add sub element : 
		if (false == invertAdding) {
			subWidgetAdd(subWidget);
		} else {
			subWidgetAddStart(subWidget);
		}
		if (false == subWidget->loadXML(pNode)) {
			EWOL_ERROR ("[" << getId() << "] {" << getObjectType() << "} (l "<<pNode->getPos()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}


void ewol::widget::ContainerN::setOffset(const vec2& _newVal) {
	if (m_offset != _newVal) {
		ewol::Widget::setOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		calculateSize(m_size);
	}
}

