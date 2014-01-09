/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_TEXT_DF_H__
#define __EWOL_COMPOSITING_TEXT_DF_H__

#include <etk/Color.h>

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/resource/DistanceFieldFont.h>
#include <ewol/compositing/Text.h>
#include <exml/exml.h>
#include <string>

namespace ewol {
	namespace compositing {
		class TextDF : public ewol::compositing::Text {
			protected:
				ewol::resource::DistanceFieldFont* m_fontDF; //!< Font resources
			public:
				/**
				 * @brief generic constructor
				 * @param[in] _fontName Name of the font that might be loaded
				 * @param[in] _fontSize size of the font that might be loaded
				 */
				TextDF(const std::string& _fontName="", int32_t _fontSize=-1);
				/**
				 * @brief generic destructor
				 */
				virtual ~TextDF(void);
			public:
				virtual void drawD(bool _disableDepthTest);
				/**
				 * @previous
				 */
				virtual void drawMT(const mat4& _transformationMatrix, bool _enableDepthTest);
			protected:
				float m_size;
			public:
				virtual float getHeight(void);
				virtual float getSize(void) {
					return m_size;
				}
				virtual ewol::GlyphProperty * getGlyphPointer(char32_t _charcode);
				
			public:
				virtual void setFontSize(int32_t _fontSize);
				virtual void setFontName(const std::string& _fontName);
				virtual void setFont(std::string _fontName, int32_t _fontSize);
				virtual void setFontMode(enum ewol::font::mode _mode);
		};
	};
};

#endif

