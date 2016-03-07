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
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		class Spacer;
		using SpacerShared = ewol::SharedPtr<ewol::widget::Spacer>;
		using SpacerWeak = ewol::WeakPtr<ewol::widget::Spacer>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Spacer : public ewol::Widget {
			public: // properties:
				eproperty::Value<etk::Color<>> propertyColor; //!< Background color
			protected:
				/**
				 * @brief Main constructer
				 */
				Spacer();
			public:
				DECLARE_WIDGET_FACTORY(Spacer, "Spacer");
				/**
				 * @brief Main destructer
				 */
				virtual ~Spacer();
			private:
				ewol::compositing::Drawing m_draw; //!< Compositing drawing element
			public: // Derived function
				virtual ewol::WidgetShared getWidgetAtPos(const vec2& _pos) { return nullptr; };
				virtual void onRegenerateDisplay();
				virtual void onDraw();
			protected:
				virtual void onChangePropertyColor();
		};
	}
}

