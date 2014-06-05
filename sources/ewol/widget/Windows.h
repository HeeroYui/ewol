/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WINDOWS_H__
#define __EWOL_WINDOWS_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <etk/Color.h>
#include <ewol/resource/ColorFile.h>
#include <list>

namespace ewol {
	namespace widget {
		/**
		 * @brief Windows basic interface
		 */
		class Windows : public ewol::Widget {
			protected:
				ewol::object::Shared<ewol::resource::ColorFile> m_colorProperty; //!< theme color property
				int32_t m_colorBg; //!< Default background color of the windows
			public:
				Windows();
				virtual ~Windows();
			// internal event at ewol system :
			public:
				void sysDraw();
				void sysOnShow() {};
				void sysOnHide() {};
				void sysOnKill() {};
			public:
				virtual void onShow() { };
				virtual void onHide() { };
				virtual bool onKill() {
					return true;
				};
				virtual void onReduce() { };
				virtual void onStateBackground() {};
				virtual void onStateForeground() {};
				virtual void onStateSuspend() {};
				virtual void onStateResume() {};
				virtual void on() { };
			private:
				bool m_hasDecoration;
			public:
				void setDecorationDisable() {
					m_hasDecoration = false;
				}
				
				void setDecorationEnable() {
					m_hasDecoration = true;
				}
			private:
				ewol::object::Owner<ewol::Widget> m_subWidget;
				std::list<ewol::object::Owner<ewol::Widget>> m_popUpWidgetList;
			public:
				void setSubWidget(ewol::object::Shared<ewol::Widget> _widget);
				void popUpWidgetPush(ewol::object::Shared<ewol::Widget> _widget);
				void popUpWidgetPop();
				size_t popUpCount() {
					return m_popUpWidgetList.size();
				}
			private:
				etk::Color<float> m_backgroundColor; //!< reset color of the Main windows
			public:
				/**
				 * @brief get the background color.
				 * @return A reference on the color
				 */
				const etk::Color<float>& getBackgroundColor() {
					return m_backgroundColor;
				};
				/**
				 * @brief set the background color.
				 * @param[IN] the new requested color.
				 */
				void setBackgroundColor(const etk::Color<float>& _color);
			protected: // Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual void onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject);
				virtual void calculateSize(const vec2& _availlable);
				virtual ewol::object::Shared<ewol::Widget> getWidgetAtPos(const vec2& _pos);
				void setTitle(const std::string& _title);
			public:
				enum popUpMessageType {
					messageTypeInfo, //!< information message pop-up
					messageTypeWarning, //!< warning message pop-up
					messageTypeError, //!< Error message pop-up
					messageTypeCritical //!< Critical message pop-up
				};
				/**
				 * @brief Create a simple pop-up message on the screen for application error.
				 * @param[in] _type Type of the error.
				 * @param[in] _message message to display (decorated text)
				 */
				virtual void createPopUpMessage(enum popUpMessageType _type, const std::string& _message);
				/**
				 * @brief Create a simple information message
				 * @param[in] _message message to display (decorated text)
				 */
				void displayInfoMessage(const std::string& _message) {
					createPopUpMessage(messageTypeInfo, _message);
				}
				/**
				 * @brief Create a simple warning message
				 * @param[in] _message message to display (decorated text)
				 */
				void displayWarningMessage(const std::string& _message) {
					createPopUpMessage(messageTypeWarning, _message);
				}
				/**
				 * @brief Create a simple error message
				 * @param[in] _message message to display (decorated text)
				 */
				void displayErrorMessage(const std::string& _message) {
					createPopUpMessage(messageTypeError, _message);
				}
				/**
				 * @brief Create a simple critical message
				 * @param[in] _message message to display (decorated text)
				 */
				void displayCriticalMessage(const std::string& _message) {
					createPopUpMessage(messageTypeCritical, _message);
				}
				
				
				virtual bool onEventHardwareInput(const ewol::key::keyboardSystem& _event, bool _down) {
					return false;
				};
		};
	};
};

#endif

