/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class ContextMenu : public ewol::widget::Container {
			public:
				enum markPosition {
					markTop,
					markRight,
					markButtom,
					markLeft,
					markNone
				};
			public: // properties
				eproperty::Value<vec2> propertyArrowPos;
				eproperty::List<enum markPosition> propertyArrawBorder;
				eproperty::Value<std::string> propertyShaper; //!< Compositing theme.
			protected:
				ContextMenu();
				void init(const std::string& _shaperName="{ewol}THEME:GUI:ContextMenu.json");
			public:
				DECLARE_WIDGET_FACTORY(ContextMenu, "ContextMenu");
				virtual ~ContextMenu();
			private:
				ewol::compositing::Shaper m_shaper; //!< Compositing theme.
				ewol::compositing::Drawing m_compositing;
				etk::Color<> m_colorBackGroung;
				etk::Color<> m_colorBorder;
				float m_offset;
			public:
				void setPositionMarkAuto(const vec2& _origin, const vec2& _size);
				void setPositionMark(enum markPosition _position, const vec2& _arrowPos);
			protected: // Derived function
				virtual void onDraw();
				virtual void onPropertyChangeValue(const eproperty::Ref& _paramPointer);
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual void onChangeSize();
				virtual void calculateMinMaxSize();
				virtual std::shared_ptr<ewol::Widget> getWidgetAtPos(const vec2& _pos);
		};
	};
};

