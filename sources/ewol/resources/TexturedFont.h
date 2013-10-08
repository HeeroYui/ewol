/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_TEXTURED_FONT_H__
#define __EWOL_TEXTURED_FONT_H__

#include <ewol/resources/font/FontBase.h>
#include <ewol/resources/Texture.h>
#include <ewol/resources/Resource.h>

namespace ewol {
	namespace font {
		typedef enum {
			Regular=0,
			Italic,
			Bold,
			BoldItalic,
		} mode_te;
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::font::mode_te& _obj);
	
	class TexturedFont : public ewol::Texture {
		private:
			etk::UString m_fileName[4];
			int32_t m_size;
			int32_t m_height[4];
			// specific element to have the the know if the specify element is known...
			//  == > otherwise I can just generate italic ...
			//  == > Bold is a little more complicated (maybe with the bordersize)
			ewol::FontBase* m_font[4];
			ewol::font::mode_te m_modeWraping[4];     //!< This is a wrapping mode to prevent the fact that no font is define for a specific mode
		public:
			etk::Vector<GlyphProperty> m_listElement[4];
		private:
			// for the texture generation :
			ivec2 m_lastGlyphPos[4];
			int32_t m_lastRawHeigh[4];
		protected:
			TexturedFont(etk::UString _fontName);
			~TexturedFont(void);
		public:
			virtual bool hasName(const etk::UString& _fileName);
			const char* getType(void) { return "ewol::TexturedFont"; };
			/**
			 * @brief get the display height of this font
			 * @param[in] _displayMode Mode to display the currrent font
			 * @return Dimention of the font need between 2 lines
			 */
			int32_t getHeight(const ewol::font::mode_te _displayMode = ewol::font::Regular) { return m_height[_displayMode]; };
			/**
			 * @brief get the font height (user friendly)
			 * @return Dimention of the font the user requested
			 */
			int32_t getFontSize(void) { return m_size; };
			/**
			 * @brief get the ID of a unicode charcode
			 * @param[in] _charcode The unicodeValue
			 * @param[in] _displayMode Mode to display the currrent font
			 * @return The ID in the table (if it does not exist : return 0)
			 */
			int32_t getIndex(const uniChar_t& _charcode, const ewol::font::mode_te _displayMode);
			/**
			 * @brief get the pointer on the coresponding glyph
			 * @param[in] _charcode The unicodeValue
			 * @param[in] _displayMode Mode to display the currrent font
			 * @return The pointer on the glyph  == > never NULL
			 */
			ewol::GlyphProperty* getGlyphPointer(const uniChar_t& _charcode, const ewol::font::mode_te _displayMode);
			/**
			 * @brief The wrapping mode is used to prevent the non existance of a specific mode.
			 *        For exemple when a blod mode does not exist, this resend a regular mode.
			 * @param[in] _source The requested mode.
			 * @return the best mode we have in stock.
			 */
			ewol::font::mode_te getWrappingMode(ewol::font::mode_te _source) { return m_modeWraping[_source]; };
		public:
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the texture font.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static ewol::TexturedFont* keep(const etk::UString& _filename);
			/**
			 * @brief release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void release(ewol::TexturedFont*& _object);
		private:
			/**
			 * @brief add a glyph in a texture font.
			 * @param[in] _val Char value to add.
			 * @return true if the image size have change, false otherwise
			 */
			bool addGlyph(const etk::UniChar& _val);
	};
	
	
};

#endif

