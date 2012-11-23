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
			void Init(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolList"; };
			virtual ~List(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
		// Drawing capabilities ....
		private:
			etk::Vector<ewol::Compositing*>        m_listOObject;   //!< generic element to display...
			etk::Vector<etk::Vector2D<int32_t> >   m_lineSize;
		public:
			void    AddOObject(ewol::Compositing* newObject, int32_t pos=-1);
			void    ClearOObjectList(void);
		protected:
			virtual void OnDraw(ewol::DrawProperty& displayProp);
		// list properties ...
		private:
			int32_t        m_paddingSizeX;
			int32_t        m_paddingSizeY;
			int32_t        m_displayStartRaw;           //!< Current starting diaplayed raw
			int32_t        m_displayCurrentNbLine;      //!< Number of line in the display
		public:
			virtual void   OnRegenerateDisplay(void);
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos);
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
		protected:
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
	};
	
};

#endif
