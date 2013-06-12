/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_LIST_H__
#define __EWOL_LIST_H__

#include <etk/types.h>
#include <draw/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/WidgetScrolled.h>
#include <ewol/compositing/Compositing.h>

namespace widget {
	class List :public widget::WidgetScrooled
	{
		public:
			List(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "ewol::List"; };
			virtual ~List(void);
			virtual void CalculateMinMaxSize(void);
			void SetLabel(etk::UString newLabel);
		// Drawing capabilities ....
		private:
			etk::Vector<ewol::Compositing*> m_listOObject;   //!< generic element to display...
			etk::Vector<ivec2 > m_lineSize;
		public:
			void AddOObject(ewol::Compositing* newObject, int32_t pos=-1);
			void ClearOObjectList(void);
		// list properties ...
		private:
			int32_t m_paddingSizeX;
			int32_t m_paddingSizeY;
			int32_t m_displayStartRaw; //!< Current starting diaplayed raw
			int32_t m_displayCurrentNbLine; //!< Number of line in the display
			int32_t m_nbVisibleRaw; // set the number of visible raw (calculate don display)
		protected:
			// function call to display the list :
			virtual draw::Color GetBasicBG(void) {
				draw::Color bg(0xFFFFFFFF);
				return bg;
			}
			virtual uint32_t GetNuberOfColomn(void) {
				return 1;
			};
			virtual bool GetTitle(int32_t colomn, etk::UString &myTitle, draw::Color &fg, draw::Color &bg) {
				myTitle = "";
				return false;
			};
			virtual uint32_t GetNuberOfRaw(void) {
				return 0;
			};
			virtual bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, draw::Color &fg, draw::Color &bg) {
				myTextToWrite = "";
				bg = 0xFFFFFFFF;
				fg = 0x000000FF;
				if (raw % 2) {
					bg = 0xFFFFFFFF;
				} else {
					bg = 0x7F7F7FFF;
				}
				return false;
			};
			virtual bool OnItemEvent(int32_t IdInput, ewol::keyEvent::status_te typeEvent,  int32_t colomn, int32_t raw, float x, float y) {
				return false;
			}
			/**
			 * @brief Set a raw visible in the main display
			 * @param[in] _id Id of the raw that might be visible.
			 */
			void SetRawVisible(int32_t _id);
		protected: // Derived function
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
			virtual void OnDraw(void);
		public: // Derived function
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
	};
	
};

#endif
