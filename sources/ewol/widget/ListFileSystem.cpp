/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/ListFileSystem.h>
#include <etk/tool.h>

#include <etk/os/FSNode.h>


#undef __class__
#define __class__ "ListFileSystem"

static ewol::Widget* Create(void) {
	return new ewol::widget::ListFileSystem();
}

void ewol::widget::ListFileSystem::init(ewol::widget::Manager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&Create);
}

const char * const ewol::widget::ListFileSystem::eventFileSelect     = "file-select";
const char * const ewol::widget::ListFileSystem::eventFileValidate   = "file-validate";
const char * const ewol::widget::ListFileSystem::eventFolderSelect   = "folder-select";
const char * const ewol::widget::ListFileSystem::eventFolderValidate = "folder-validate";

const char* const ewol::widget::ListFileSystem::configShowHidden = "show-hidden";
const char* const ewol::widget::ListFileSystem::configShowFile = "show-file";
const char* const ewol::widget::ListFileSystem::configShowFolder = "show-folder";
const char* const ewol::widget::ListFileSystem::configShowTemporary = "show-temporary";
const char* const ewol::widget::ListFileSystem::configPath = "path";
const char* const ewol::widget::ListFileSystem::configSelect = "select";

ewol::widget::ListFileSystem::ListFileSystem(void) :
  m_selectedLine(-1),
  m_folder ("/"),
  m_showFile(true),
  m_showFolder(true),
  m_showHidden(true),
  m_showTemporaryFile(true) {
	addObjectType("ewol::widget::ListFileSystem");
	#if defined(__TARGET_OS__Windows)
		m_folder = "c:/";
	#endif
	addEventId(eventFileSelect);
	addEventId(eventFileValidate);
	addEventId(eventFolderSelect);
	addEventId(eventFolderValidate);
	
	registerConfig(configShowHidden, "bool", NULL, "Show the hidden element (file, folder, ...)");
	registerConfig(configShowFile, "bool", NULL, "display files");
	registerConfig(configShowFolder, "bool", NULL, "display folders");
	registerConfig(configShowTemporary, "bool", NULL, "display temporary files");
	registerConfig(configPath, "string", NULL, "Path to display");
	registerConfig(configSelect, "string", NULL, "selection af a specific file");
	
	setMouseLimit(1);
};

ewol::widget::ListFileSystem::~ListFileSystem(void) {
	clearList();
};

void ewol::widget::ListFileSystem::clearList(void) {
	for (size_t iii=0; iii<m_list.size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
};

etk::Color<> ewol::widget::ListFileSystem::getBasicBG(void) {
	return etk::Color<>(0x00000010);
}


void ewol::widget::ListFileSystem::regenerateView(void) {
	clearList();
	m_selectedLine = -1;
	m_list.clear();
	m_originScrooled.setValue(0,0);
	etk::FSNode tmpFolder(m_folder);
	// get the new list : 
	m_list = tmpFolder.folderGetSubList(m_showHidden, m_showFolder, m_showFile, m_showTemporaryFile);
	// request a redraw ...
	markToRedraw();
}

std::string ewol::widget::ListFileSystem::getSelect(void) const {
	std::string tmpVal = "";
	if (m_selectedLine >= 0) {
		if (m_list[m_selectedLine] != NULL) {
			tmpVal = m_list[m_selectedLine]->getNameFile();
		}
	}
	return tmpVal;
}

// select the specific file
void ewol::widget::ListFileSystem::setSelect(const std::string& _data) {
	// remove selected line
	m_selectedLine = -1;
	// search the coresponding file :
	for (size_t iii=0; iii<m_list.size(); iii++) {
		if (NULL!=m_list[iii]) {
			if (m_list[iii]->getNameFile() == _data) {
				// we find the line :
				m_selectedLine = iii;
				break;
			}
		}
	}
	markToRedraw();
}

uint32_t ewol::widget::ListFileSystem::getNuberOfColomn(void) {
	return 1;
}

bool ewol::widget::ListFileSystem::getTitle(int32_t _colomn, std::string &_myTitle, etk::Color<>& _fg, etk::Color<>& _bg) {
	_myTitle = "title";
	return true;
}

uint32_t ewol::widget::ListFileSystem::getNuberOfRaw(void) {
	int32_t offset = 0;
	if (true == m_showFolder) {
		offset = 2;
	}
	return m_list.size() + offset;
}

bool ewol::widget::ListFileSystem::getElement(int32_t _colomn, int32_t _raw, std::string& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg) {
	int32_t offset = 0;
	if (true == m_showFolder) {
		offset = 2;
		if (_raw == 0) {
			_myTextToWrite = ".";
		} else if (_raw == 1) {
			_myTextToWrite = "..";
		}
	}
	if(    _raw-offset >= 0
	    && _raw-offset < (int32_t)m_list.size()
	    && NULL != m_list[_raw-offset]) {
		/*if (etk::FSN_FILE == m_list[raw-offset]->getNodeType()) {
			myTextToWrite = m_list[raw-offset]->getRight().getRight();
			myTextToWrite += " ";
			myTextToWrite += m_list[raw-offset]->getNameFile();
		} else */{
			_myTextToWrite = m_list[_raw-offset]->getNameFile();
		}
	}
	_fg = etk::color::black;
	if (_raw % 2) {
		_bg = 0xFFFFFF00;
	} else {
		_bg = 0xBFBFBFFF;
	}
	if (m_selectedLine == _raw) {
		_bg = 0x8F8FFFFF;
	}
	return true;
};


bool ewol::widget::ListFileSystem::onItemEvent(int32_t _IdInput,
                                               enum ewol::key::status _typeEvent,
                                               int32_t _colomn,
                                               int32_t _raw,
                                               float _x,
                                               float _y) {
	int32_t offset = 0;
	if (true == m_showFolder) {
		offset = 2;
	}
	if (_typeEvent == ewol::key::statusSingle) {
		EWOL_INFO("Event on List : IdInput=" << _IdInput << " colomn=" << _colomn << " raw=" << _raw );
		if (1 == _IdInput) {
			int32_t previousRaw = m_selectedLine;
			if (_raw > (int32_t)m_list.size()+offset ) {
				m_selectedLine = -1;
			} else {
				m_selectedLine = _raw;
			}
			if (previousRaw != m_selectedLine) {
				if(    m_showFolder == true
				    && m_selectedLine == 0) {
					// "." folder
					generateEventId(eventFolderSelect, ".");
				} else if (    m_showFolder == true
				            && m_selectedLine == 1) {
					// ".." folder
					generateEventId(eventFolderSelect, "..");
				} else if(    m_selectedLine-offset  >= 0
				           && m_selectedLine-offset < (int32_t)m_list.size()
				           && NULL != m_list[m_selectedLine-offset] ) {
					// generate event extern : 
					switch(m_list[m_selectedLine-offset]->getNodeType()) {
						case etk::FSN_FILE :
							generateEventId(eventFileSelect, m_list[m_selectedLine-offset]->getNameFile());
							break;
						case etk::FSN_FOLDER :
							generateEventId(eventFolderSelect, m_list[m_selectedLine-offset]->getNameFile());
							break;
						default:
							EWOL_ERROR("Can not generate event on an unknow type");
							break;
					}
				}
			} else {
				if(    m_showFolder == true
				    && m_selectedLine == 0) {
					// "." folder
					generateEventId(eventFolderValidate, ".");
				} else if (    m_showFolder == true
				            && m_selectedLine == 1) {
					// ".." folder
					generateEventId(eventFolderValidate, "..");
				} else if(    m_selectedLine-offset >= 0
				           && m_selectedLine-offset < (int32_t)m_list.size()
				           && NULL != m_list[m_selectedLine-offset] ) {
					switch(m_list[m_selectedLine-offset]->getNodeType())
					{
						case etk::FSN_FILE :
							generateEventId(eventFileValidate, m_list[m_selectedLine-offset]->getNameFile());
							break;
						case etk::FSN_FOLDER :
							generateEventId(eventFolderValidate, m_list[m_selectedLine-offset]->getNameFile());
							break;
						default:
							EWOL_ERROR("Can not generate event on an unknow type");
							break;
					}
				}
			}
			// need to regenerate the display of the list : 
			markToRedraw();
			return true;
		}
	}
	return false;
}


bool ewol::widget::ListFileSystem::onSetConfig(const ewol::object::Config& _conf) {
	if (true == ewol::widget::List::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configShowHidden) {
		setShowHidden(std::stob(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configShowFile) {
		setShowFiles(std::stob(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configShowFolder) {
		setShowFolder(std::stob(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configShowTemporary) {
		setShowTemporaryFiles(std::stob(_conf.getData()));
		return true;
	}
	if (_conf.getConfig() == configPath) {
		setFolder(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configSelect) {
		setSelect(_conf.getData());
		return true;
	}
	return false;
}

bool ewol::widget::ListFileSystem::onGetConfig(const char* _config, std::string& _result) const {
	if (true == ewol::widget::List::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configShowHidden) {
		_result = std::to_string(getShowHidden());
		return true;
	}
	if (_config == configShowFile) {
		_result = std::to_string(getShowFiles());
		return true;
	}
	if (_config == configShowFolder) {
		_result = std::to_string(getShowFolder());
		return true;
	}
	if (_config == configShowTemporary) {
		_result = std::to_string(getShowTemporaryFiles());
		return true;
	}
	if (_config == configPath) {
		_result = getFolder();
		return true;
	}
	if (_config == configSelect) {
		_result = getSelect();
		return true;
	}
	return false;
}


