/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_W_SLIDER_H__
#define __EWOL_W_SLIDER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/WidgetManager.h>


namespace widget {
	class WSlider :public widget::ContainerN
	{
		public:
			static void Init(ewol::WidgetManager& _widgetManager);
			// Event list of properties
			static const char* const eventStartSlide;
			static const char* const eventStopSlide;
			// Config list of properties
			static const char* const configMode;
			typedef enum {
				sladingTransitionVert,
				sladingTransitionHori,
				sladingTransition_count,
			} sladingMode_te;
		public:
			WSlider(void);
			virtual ~WSlider(void);
		private:
			int32_t m_windowsSources; //!< widget source viewed
			int32_t m_windowsDestination; //!< widget destinated viewed
			int32_t m_windowsRequested; //!< widget destination requested when change in modification in progress
			float m_slidingProgress; //!< ratio progression of a sliding
		public:
			/** 
			 * @brief Select a new subwidget to display
			 * @param[in] _id Id of the subwidget requested
			 */
			void SubWidgetSelectSet(int32_t _id);
			/** 
			 * @brief Select a new subwidget to display
			 * @param[in] _widgetPointer Pointer on the widget selected (must be added before)
			 */
			void SubWidgetSelectSet(ewol::Widget* _widgetPointer);
			/** 
			 * @brief Select a new subwidget to display
			 * @param[in] _widgetName Name of the subwidget name
			 */
			void SubWidgetSelectSet(const etk::UString& _widgetName);
		private:
			float m_transitionSpeed; //!< speed of the transition (default 1 ==> 1s)
		public:
			/** 
			 * @brief Set transition speed element.
			 * @param[in] _timeSecond number of second needed to do the transition.
			 */
			void SetTransitionSpeed(float _timeSecond) { m_transitionSpeed = _timeSecond; };
			/** 
			 * @brief Get transition speed element.
			 * @return number of second needed to do the transition.
			 */
			float GetTransitionSpeed(void) { return m_transitionSpeed; };
		private:
			sladingMode_te m_transitionSlide; //!< mode to slide the widgets
		public:
			/** 
			 * @brief Set a new mode of sliding element
			 * @param[in] _mode new display mode
			 */
			void SetTransitionMode(sladingMode_te _mode);
			/** 
			 * @brief Get a new mode of sliding element
			 * @return The current sliding mode
			 */
			sladingMode_te GetTransitionMode(void) { return m_transitionSlide; };
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::WSlider"; };
			virtual void CalculateSize(const vec2& _availlable);
			virtual void SystemDraw(const ewol::DrawProperty& _displayProp);
			virtual void OnRegenerateDisplay(void);
			virtual ewol::Widget* GetWidgetAtPos(const vec2& _pos);
			virtual void PeriodicCall(const ewol::EventTime& _event);
			virtual bool OnSetConfig(const ewol::EConfig& _conf);
			virtual bool OnGetConfig(const char* _config, etk::UString& _result) const;
	};
	
	etk::CCout& operator <<(etk::CCout& _os, const widget::WSlider::sladingMode_te _obj);
};

#endif
