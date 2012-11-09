/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __UNICODE_H__
#define __UNICODE_H__

#include <etk/Types.h>
#include <etk/Vector.h>
namespace unicode {
	typedef enum {
		EDN_CHARSET_UTF8,
		EDN_CHARSET_ISO_8859_1,
		EDN_CHARSET_ISO_8859_2,
		EDN_CHARSET_ISO_8859_3,
		EDN_CHARSET_ISO_8859_4,
		EDN_CHARSET_ISO_8859_5,
		EDN_CHARSET_ISO_8859_6,
		EDN_CHARSET_ISO_8859_7,
		EDN_CHARSET_ISO_8859_8,
		EDN_CHARSET_ISO_8859_9,
		EDN_CHARSET_ISO_8859_10,
		EDN_CHARSET_ISO_8859_11,
		EDN_CHARSET_ISO_8859_13,
		EDN_CHARSET_ISO_8859_14,
		EDN_CHARSET_ISO_8859_15,
	} charset_te;
	
	// transform ISO <==> Unicode
	void    convertIsoToUnicode(charset_te inputCharset, char                        input_ISO,     uniChar_t &               output_Unicode);
	void    convertUnicodeToIso(charset_te inputCharset, uniChar_t                   input_Unicode, char &                    output_ISO);
	int32_t convertIsoToUnicode(charset_te inputCharset, etk::Vector<char>&          input_ISO,     etk::Vector<uniChar_t>&   output_Unicode);
	int32_t convertIsoToUnicode(charset_te inputCharset, etk::Vector<int8_t>&        input_ISO,     etk::Vector<uniChar_t>&   output_Unicode);
	int32_t convertUnicodeToIso(charset_te inputCharset, etk::Vector<uniChar_t>&     input_Unicode, etk::Vector<char>&        output_ISO);
	int32_t convertUnicodeToIso(charset_te inputCharset, etk::Vector<uniChar_t>&     input_Unicode, etk::Vector<int8_t>&      output_ISO);
	// Transform UTF-8 <==> Unicode
	void    convertUnicodeToUtf8(                        uniChar_t                     input_Unicode, char *                  output_UTF8);
	void    convertUtf8ToUnicode(                        char *                        input_UTF8,    uniChar_t&              output_Unicode);
	int32_t convertUnicodeToUtf8(                        const etk::Vector<uniChar_t>& input_Unicode, etk::Vector<char>&      output_UTF8);
	int32_t convertUnicodeToUtf8(                        const etk::Vector<uniChar_t>& input_Unicode, etk::Vector<int8_t>&    output_UTF8);
	int32_t convertUtf8ToUnicode(                        etk::Vector<char>&            input_UTF8,    etk::Vector<uniChar_t>& output_Unicode);
	int32_t convertUtf8ToUnicode(                        etk::Vector<int8_t>&          input_UTF8,    etk::Vector<uniChar_t>& output_Unicode);
	int32_t convertUtf8ToUnicode(                        char *                        input_UTF8,    etk::Vector<uniChar_t>& output_Unicode);
	// Transform ISO <==> UTF-8
	void    convertIsoToUtf8(  charset_te inputCharset,  char                  input_ISO,     char *                output_UTF8);
	void    convertUtf8ToIso(  charset_te inputCharset,  char *                input_UTF8,    char &                output_ISO);
	int32_t convertIsoToUtf8(  charset_te inputCharset,  etk::Vector<char>&    input_ISO,     etk::Vector<char>&    output_UTF8);
	int32_t convertUtf8ToIso(  charset_te inputCharset,  etk::Vector<char>&    input_UTF8,    etk::Vector<char>&    output_ISO);
	
	void    Utf8_SizeElement(const char * data, int32_t lenMax , uint8_t &size, bool &baseValid);
	int32_t strUtf8Len(const char *input_UTF8);
}

#endif

