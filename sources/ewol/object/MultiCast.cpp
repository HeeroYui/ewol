/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/object/MultiCast.h>
#include <ewol/context/Context.h>

#undef __class__
#define __class__ "object::MultiCast"

ewol::object::MultiCast::MultiCast() {
	EWOL_INFO("EObject message Multi-Cast");
}


ewol::object::MultiCast::~MultiCast() {
	EWOL_INFO("EObject message Multi-Cast");
	m_messageList.clear();
}


void ewol::object::MultiCast::add(const ewol::object::Shared<ewol::Object>& _object, const char* const _message) {
	if (nullptr == _object) {
		EWOL_ERROR("Add with nullptr object");
		return;
	}
	if (nullptr == _message) {
		EWOL_ERROR("Add with nullptr Message");
		return;
	}
	m_messageList.push_back(MessageList(_message, _object));
	EWOL_DEBUG("SendMulticast ADD listener :" << _object->getId() << " on \"" << _message << "\"" );
}


void ewol::object::MultiCast::rm(const ewol::object::Shared<ewol::Object>& _object) {
	if (nullptr == _object) {
		EWOL_ERROR("Rm with nullptr object");
		return;
	}
	// send the message at all registered widget ...
	for (int32_t iii=m_messageList.size()-1; iii >= 0; iii--) {
		if(m_messageList[iii].m_object == _object) {
			EWOL_DEBUG("SendMulticast RM listener :" << _object->getId());
			m_messageList[iii].m_message = nullptr;
			m_messageList[iii].m_object = nullptr;
			m_messageList.erase(m_messageList.begin()+iii);
		}
	}
}

void ewol::object::MultiCast::send(const ewol::object::Shared<ewol::Object>& _object, const char* const _message, const std::string& _data) {
	EWOL_VERBOSE("SendMulticast message \"" << _message << "\" data=\"" << _data << "\" to :");
	
	// send the message at all registered widget ...
	for (size_t iii=0; iii<m_messageList.size(); iii++) {
		if(    m_messageList[iii].m_message == _message
		    && m_messageList[iii].m_object != _object)
		{
			if (nullptr != m_messageList[iii].m_object) {
				EWOL_VERBOSE("        id = " << m_messageList[iii].m_object->getId() << " type=" << m_messageList[iii].m_object->getObjectType());
				// generate event ... (create message before ...
				ewol::object::Message tmpMsg(_object, m_messageList[iii].m_message, _data);
				m_messageList[iii].m_object->onReceiveMessage(tmpMsg);
			}
		}
	}
}

