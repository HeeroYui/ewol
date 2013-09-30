/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_EVENT_ENTRY_H__
#define __EWOL_EVENT_ENTRY_H__

#include <etk/types.h>
#include <ewol/key.h>

namespace ewol {
	class EventEntry {
		private:
			ewol::keyEvent::keyboard_te m_type; //!< type of hardware event
			ewol::keyEvent::status_te m_status; //!< status of hardware event
			ewol::SpecialKey m_specialKey; //!< input key status (prevent change in time..)
			uniChar_t m_unicodeData; //!< Unicode data (in some case)
		public:
			EventEntry(ewol::keyEvent::keyboard_te _type,
			           ewol::keyEvent::status_te _status,
			           ewol::SpecialKey _specialKey,
			           uniChar_t _char) :
				m_type(_type),
				m_status(_status),
				m_specialKey(_specialKey),
				m_unicodeData(_char)
			{ };
			void SetType(ewol::keyEvent::keyboard_te _type) { m_type = _type; };
			inline const ewol::keyEvent::keyboard_te& GetType(void) const { return m_type; };
			void SetStatus(ewol::keyEvent::status_te _status) { m_status = _status; };
			inline const ewol::keyEvent::status_te& GetStatus(void) const { return m_status; };
			void SetSpecialKey(const ewol::SpecialKey& _specialKey) { m_specialKey = _specialKey; };
			inline const ewol::SpecialKey& GetSpecialKey(void) const { return m_specialKey; };
			void SetChar(uniChar_t _char) { m_unicodeData = _char; };
			inline const uniChar_t& GetChar(void) const { return m_unicodeData; };
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::EventEntry& _obj);
	
	class EventEntrySystem {
		public:
			EventEntrySystem(ewol::keyEvent::keyboard_te _type,
			                 ewol::keyEvent::status_te _status,
			                 ewol::SpecialKey _specialKey,
			                 uniChar_t _char) :
				m_event(_type, _status, _specialKey, _char)
			{ };
			ewol::EventEntry m_event;
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::EventEntrySystem& _obj);
};

#endif

