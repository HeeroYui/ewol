/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_LIST_FILE_H__
#define __EWOL_LIST_FILE_H__

#include <ewol/widget/List.h>
#include <etk/os/FSNode.h>

extern const char * const ewolEventFSFileSelect;
extern const char * const ewolEventFSFileValidate;
extern const char * const ewolEventFSFolderSelect;
extern const char * const ewolEventFSFolderValidate;

namespace widget {
	class ListFileSystem : public widget::List
	{
		private:
			etk::Vector<etk::FSNode *> m_list;
			etk::UString               m_folder;
			int32_t                    m_selectedLine;
			bool                       m_showFile;
			bool                       m_showTemporaryFile;
			bool                       m_showFolder;
			bool                       m_showHidden;
		public:
			ListFileSystem(void);
			~ListFileSystem(void);
			// display API :
			virtual draw::Color GetBasicBG(void);
			uint32_t GetNuberOfColomn(void);
			bool GetTitle(int32_t colomn, etk::UString &myTitle, draw::Color &fg, draw::Color &bg);
			uint32_t GetNuberOfRaw(void);
			bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, draw::Color &fg, draw::Color &bg);
			bool OnItemEvent(int32_t IdInput, ewol::keyEvent::status_te typeEvent, int32_t colomn, int32_t raw, float x, float y);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			const char * const GetObjectType(void) { return "EwolListFileSystem"; };
		public:
			// extern API :
			void         SetFolder(etk::UString newFolder);
			etk::UString GetFolder(void);
			// select the specific file
			void         SetSelect(etk::UString data);
			etk::UString GetSelect(void);
			// regenerate the view ....
			void         RegenerateView(void);
			void         SetShowFiles(bool state);
			void         SetShowFolder(bool state);
			void         SetShowHiddenFiles(bool state);
			void         SetShowTemporaryFiles(bool state);
	};
};

#endif

