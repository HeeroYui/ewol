/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/object/Object.h>
#include <ewol/object/Manager.h>
#include <ewol/debug.h>
#include <ewol/context/Context.h>
#include <ewol/object/MultiCast.h>

#undef __class__
#define __class__ "Object"

size_t ewol::Object::m_valUID = 0;

void ewol::Object::autoDestroy() {
	if (m_objectHasBeenInit == false) {
		EWOL_WARNING("try to auto destroy inside a constructor");
		return;
	}
	EWOL_VERBOSE("Destroy object : [" << getId() << "] type:" << getObjectType());
	std::shared_ptr<ewol::Object> parent = m_parent.lock();
	// TODO : set a signal to do this ...
	if (parent != nullptr) {
		parent->requestDestroyFromChild(shared_from_this());
	}
	//if no parent ==> noting to do ...
}

void ewol::Object::requestDestroyFromChild(const std::shared_ptr<ewol::Object>& _child) {
	EWOL_CRITICAL("Call From Child with no effects ==> must implement : requestDestroyFromChild(...)");
}
void ewol::Object::setParent(const std::shared_ptr<ewol::Object>& _newParent) {
	// TODO : Implement change of parent ...
	m_parent = _newParent;
}

void ewol::Object::removeParent() {
	m_parent.reset();
}



ewol::Object::Object() :
  m_objectHasBeenInit(false),
  m_static(false),
  m_name(*this, "name", "", "Object name, might be a unique reference in all the program"),
  m_isResource(false) {
	// note this is nearly atomic ... (but it is enough)
	m_uniqueId = m_valUID++;
	EWOL_DEBUG("new Object : [" << m_uniqueId << "]");
}

ewol::Object::~Object() {
	EWOL_DEBUG("delete Object : [" << m_uniqueId << "] : " << getTypeDescription());
	m_externEvent.clear();
	m_availlableEventId.clear();
	m_uniqueId = -1;
}


void ewol::Object::init() {
	getObjectManager().add(shared_from_this());
	parameterDisplay();
	m_objectHasBeenInit = true;
}

void ewol::Object::init(const std::string& _name) {
	init();
	m_name = _name;
}

const char * const ewol::Object::getObjectType() {
	if (m_listType.size() == 0) {
		return "ewol::Object";
	}
	return m_listType.back();
}

void ewol::Object::addObjectType(const char* _type) {
	if (_type == nullptr) {
		EWOL_ERROR(" try to add a type with no value...");
		return;
	}
	m_listType.push_back(_type);
}
std::string ewol::Object::getTypeDescription() {
	std::string ret("ewol::Object");
	for(auto element : m_listType) {
		ret += "|";
		ret += element;
	}
	return ret;
}

bool ewol::Object::isTypeCompatible(const std::string& _type) {
	if (_type == "ewol::Object") {
		return true;
	}
	for(auto element : m_listType) {
		if (_type == element) {
			return true;
		}
	}
	return false;
}

void ewol::Object::addEventId(const char * _generateEventId) {
	for (auto &it : m_availlableEventId) {
		if (std::string(it) == _generateEventId) {
			EWOL_WARNING("Event already existed : '" << it << "' == '" << _generateEventId << "'");
			return;
		}
	}
	if (_generateEventId != nullptr) {
		m_availlableEventId.push_back(_generateEventId);
	}
}

void ewol::Object::generateEventId(const char * _generateEventId, const std::string& _data) {
	if (m_objectHasBeenInit == false) {
		EWOL_WARNING("try to generate an event inside a constructor");
		return;
	}
	int32_t nbObject = getObjectManager().getNumberObject();
	EWOL_VERBOSE("try send message '" << _generateEventId << "'");
	// for every element registered ...
	for (auto &it : m_externEvent) {
		// if we find the event ...
		if (it.localEventId != _generateEventId) {
			EWOL_VERBOSE("    wrong event '" << it.localEventId << "' != '" << _generateEventId << "'");
			continue;
		}
		std::shared_ptr<ewol::Object> destObject = it.destObject.lock();
		if (destObject == nullptr) {
			EWOL_VERBOSE("    nullptr dest");
			continue;
		}
		if (it.overloadData.size() <= 0){
			ewol::object::Message tmpMsg(shared_from_this(), it.destEventId, _data);
			EWOL_VERBOSE("send message " << tmpMsg);
			destObject->onReceiveMessage(tmpMsg);
		} else {
			// set the user requested data ...
			ewol::object::Message tmpMsg(shared_from_this(), it.destEventId, it.overloadData);
			EWOL_VERBOSE("send message " << tmpMsg);
			destObject->onReceiveMessage(tmpMsg);
		}
	}
	if (nbObject > getObjectManager().getNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->removObject() which is asynchronous");
	}
}

void ewol::Object::sendMultiCast(const char* const _messageId, const std::string& _data) {
	if (m_objectHasBeenInit == false) {
		EWOL_WARNING("try to generate an multicast event inside a constructor");
		return;
	}
	int32_t nbObject = getObjectManager().getNumberObject();
	getMultiCast().send(shared_from_this(), _messageId, _data);
	if (nbObject > getObjectManager().getNumberObject()) {
		EWOL_CRITICAL("It if really dangerous ro remove (delete) element inside a callback ... use ->removObject() which is asynchronous");
	}
}

void ewol::Object::registerMultiCast(const char* const _messageId) {
	if (m_objectHasBeenInit == false) {
		EWOL_WARNING("try to generate an event inside a constructor");
		return;
	}
	getMultiCast().add(shared_from_this(), _messageId);
}

void ewol::Object::registerOnEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
                                    const char * _eventId,
                                    const char * _eventIdgenerated,
                                    const std::string& _overloadData) {
	if (nullptr == _destinationObject) {
		EWOL_ERROR("Input ERROR nullptr pointer Object ...");
		return;
	}
	if (nullptr == _eventId) {
		EWOL_ERROR("Input ERROR nullptr pointer Event Id...");
		return;
	}
	if (    _eventId[0] == '*'
	     && _eventId[1] == '\0') {
		EWOL_VERBOSE("Register on all event ...");
		for(auto &it : m_availlableEventId) {
			ewol::object::EventExtGen tmpEvent;
			tmpEvent.localEventId = it;
			tmpEvent.destObject = _destinationObject;
			tmpEvent.overloadData = _overloadData;
			if (nullptr != _eventIdgenerated) {
				tmpEvent.destEventId = _eventIdgenerated;
			} else {
				tmpEvent.destEventId = it;
			}
			m_externEvent.push_back(tmpEvent);
		}
		return;
	}
	// check if event existed :
	bool findIt = false;
	for(auto &it : m_availlableEventId) {
		if (it == _eventId) {
			findIt = true;
			break;
		}
	}
	if (false == findIt) {
		EWOL_VERBOSE("Try to register with a NON direct string name");
		for(auto &it : m_availlableEventId) {
			if (0 == strncmp(it, _eventId, 1024)) {
				findIt = true;
				_eventId = it;
				EWOL_VERBOSE("find event ID : '" << _eventId << "' ==> '" << _eventIdgenerated << "'");
				break;
			}
		}
	}
	if (false == findIt) {
		EWOL_ERROR("Can not register event on this Type=" << getObjectType() << " event=\"" << _eventId << "\"  == > unknow event");
		return;
	}
	ewol::object::EventExtGen tmpEvent;
	tmpEvent.localEventId = _eventId;
	tmpEvent.destObject = _destinationObject;
	tmpEvent.overloadData = _overloadData;
	if (nullptr != _eventIdgenerated) {
		tmpEvent.destEventId = _eventIdgenerated;
	} else {
		tmpEvent.destEventId = _eventId;
	}
	m_externEvent.push_back(tmpEvent);
}

void ewol::Object::unRegisterOnEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
                                     const char * _eventId) {
	if (_destinationObject == nullptr) {
		EWOL_ERROR("Input ERROR nullptr pointer Object ...");
		return;
	}
	// check if event existed :
	auto it(m_externEvent.begin());
	while(it != m_externEvent.end()) {
		std::shared_ptr<ewol::Object> obj = it->destObject.lock();
		if (obj == nullptr) {
			m_externEvent.erase(it);
			it = m_externEvent.begin();
		} else if (    obj == _destinationObject
		            && it->localEventId == _eventId) {
			m_externEvent.erase(it);
			it = m_externEvent.begin();
			EWOL_INFO("[" << getId() << "] Remove extern event : to object id=" << _destinationObject->getId() << " event=" << _eventId);
		} else {
			++it;
		}
	}
}

void ewol::Object::onObjectRemove(const std::shared_ptr<ewol::Object>& _object) {
	EWOL_VERBOSE("[" << getId() << "] onObjectRemove(" << _object->getId() << ")");
	auto it(m_externEvent.begin());
	while(it != m_externEvent.end()) {
		std::shared_ptr<ewol::Object> obj = it->destObject.lock();
		if (obj == nullptr) {
			m_externEvent.erase(it);
			it = m_externEvent.begin();
		} else if (obj == _object) {
			m_externEvent.erase(it);
			it = m_externEvent.begin();
			EWOL_INFO("[" << getId() << "] Remove extern event : to object id=" << _object->getId());
		} else {
			++it;
		}
	}
}

bool ewol::Object::loadXML(exml::Element* _node) {
	if (nullptr == _node) {
		return false;
	}
	bool errorOccured = false;
	
	for(size_t iii=0 ; iii<_node->sizeAttribute() ; iii++) {
		auto pair = _node->getAttrPair(iii);
		if (pair.first == "") {
			continue;
		}
		if (parameterSet(pair.first, pair.second) == false) {
			errorOccured = true;
		}
	}
	return errorOccured;
}

bool ewol::Object::storeXML(exml::Element* _node) const {
	if (nullptr == _node) {
		return false;
	}
	bool errorOccured = true;
	for (auto &it : parameterGetAll(true)) {
		_node->setAttribute(it.first, it.second);
	}
	return errorOccured;
}

void ewol::Object::onParameterChangeValue(const ewol::object::ParameterRef& _paramPointer) {
	if (_paramPointer == m_name) {
		EWOL_VERBOSE("[" << getId() << "] Parameter name change : " << m_name);
	}
}

bool ewol::Object::setConfigNamed(const std::string& _objectName, const std::string& _config, const std::string& _value) {
	std::shared_ptr<ewol::Object> object = getObjectManager().get(_objectName);
	if (object == nullptr) {
		return false;
	}
	return object->setConfig(_config, _value);
}

ewol::object::Manager& ewol::Object::getObjectManager() const {
	ewol::object::Manager& tmp = ewol::getContext().getEObjectManager();
	return tmp;
}

ewol::object::MultiCast& ewol::Object::getMultiCast() const {
	return ewol::getContext().getEObjectManager().multiCast();
}

ewol::Context& ewol::Object::getContext() const {
	return ewol::getContext();
}

void ewol::Object::registerOnObjectEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
                                         const std::string& _objectName,
                                         const char * _eventId,
                                         const char * _eventIdgenerated,
                                         const std::string& _overloadData) {
	std::shared_ptr<ewol::Object> tmpObject = getObjectManager().getObjectNamed(_objectName);
	if (nullptr != tmpObject) {
		EWOL_DEBUG("Find widget named : '" << _objectName << "' register event='" << _eventId << "'");
		tmpObject->registerOnEvent(_destinationObject, _eventId, _eventIdgenerated, _overloadData);
	} else {
		EWOL_WARNING("[" << getId() << "] {" << getObjectType() << "} Can not register event : \"" << _eventId << "\" the object named=\"" << _objectName << "\" does not exist");
	}
}

std::shared_ptr<ewol::Object> ewol::Object::getObjectNamed(const std::string& _objectName) const {
	return getObjectManager().getObjectNamed(_objectName);
}
