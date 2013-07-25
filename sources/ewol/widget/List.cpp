/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/List.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Text.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"List"


widget::List::List(void)
{
	m_paddingSizeX = 2;
	#ifdef __TARGET_OS__Android
		m_paddingSizeY = 10;
	#else
		m_paddingSizeY = 2;
	#endif
	m_nbVisibleRaw=0;
	SetCanHaveFocus(true);
}

widget::List::~List(void)
{
	//clean all the object
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}


void widget::List::SetRawVisible(int32_t _id)
{
	EWOL_DEBUG("Set Raw visible : " << _id);
	if (_id<0) {
		return;
	}
	if (_id == m_displayStartRaw) {
		// nothing to do ...
		return;
	}
	if (_id < m_displayStartRaw) {
		m_displayStartRaw = _id-2;
	} else {
		if (m_displayStartRaw + m_nbVisibleRaw < _id) {
			m_displayStartRaw = _id - m_nbVisibleRaw + 2;
		}
	}
	if (m_displayStartRaw>GetNuberOfRaw()) {
		m_displayStartRaw = GetNuberOfRaw()-2;
	}
	if (m_displayStartRaw<0) {
		m_displayStartRaw = 0;
	}
	EWOL_DEBUG("Set start raw : " << m_displayStartRaw);
	MarkToRedraw();
}


void widget::List::CalculateMinMaxSize(void)
{
	/*int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_label);
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = 3+minWidth;
	m_minSize.y = 3+minHeight;
	*/
	m_minSize.setValue(200, 150);
}


void widget::List::AddOObject(ewol::Compositing* _newObject, int32_t _pos)
{
	if (NULL == _newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (_pos < 0 || _pos >= m_listOObject.Size() ) {
		m_listOObject.PushBack(_newObject);
	} else {
		m_listOObject.Insert(_pos, _newObject);
	}
}


void widget::List::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}

void widget::List::OnDraw(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (NULL != m_listOObject[iii]) {
			m_listOObject[iii]->Draw();
		}
	}
	WidgetScrooled::OnDraw();
}

void widget::List::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
		// clean the object list ...
		ClearOObjectList();
		//EWOL_DEBUG("OnRegenerateDisplay(" << m_size.x << "," << m_size.y << ")");
		int32_t tmpOriginX = 0;
		int32_t tmpOriginY = 0;
		/*
		if (true==m_userFill.x) {
			tmpOriginX = 0;
		}
		if (true==m_userFill.y) {
			tmpOriginY = 0;
		}*/
		tmpOriginX += m_paddingSizeX;
		tmpOriginY += m_paddingSizeY;
		// TODO : Remove this ...
		int32_t minHeight = 25;
	
		uint32_t nbColomn = GetNuberOfColomn();
		int32_t nbRaw    = GetNuberOfRaw();
		// For the scrooling windows
		m_maxSize = ivec2(m_size.x(),
		                  (minHeight + 2*m_paddingSizeY) * nbRaw );
		
		
		etk::Vector<int32_t> listSizeColomn;
		
		ewol::Drawing * BGOObjects = new ewol::Drawing();
		etk::Color<> basicBG = GetBasicBG();
		BGOObjects->SetColor(basicBG);
		BGOObjects->SetPos(vec3(0, 0, 0) );
		BGOObjects->RectangleWidth(vec3(m_size.x(), m_size.y(), 0) );
		
		int32_t startRaw = m_originScrooled.y() / (minHeight + 2*m_paddingSizeY);
		
		if (startRaw >= nbRaw-1 ) {
			startRaw = nbRaw - 1;
		}
		if (startRaw<0) {
			startRaw = 0;
		}
		// We display only compleate lines ...
		//EWOL_DEBUG("Request drawing list : " << startRaw << "-->" << (startRaw+displayableRaw) << " in " << nbRaw << "raws ; start display : " << m_originScrooled.y << " ==> " << tmpOriginY << " line size=" << minHeight + 2*m_paddingSizeY );
		
		/*clipping_ts drawClipping;
		drawClipping.x = 0;
		drawClipping.y = 0;
		drawClipping.w = m_size.x - (2*m_paddingSizeX);
		drawClipping.h = m_size.y;
		*/
		// remove all the positions :
		m_lineSize.Clear();
		int32_t displayPositionY = m_size.y();
		int32_t displayPositionX = 0;
		ivec2 tmpRegister(startRaw, displayPositionY);
		// add the default position raw :
		m_lineSize.PushBack(tmpRegister);
		
		for(int32_t jjj=0; jjj<nbColomn && displayPositionX < m_size.x() ; jjj++) {
			int32_t sizeColom = 0;
			displayPositionY = m_size.y();
			m_nbVisibleRaw = 0;
			for(int32_t iii=startRaw; iii<nbRaw && displayPositionY >= 0; iii++) {
				m_nbVisibleRaw++;
				etk::UString myTextToWrite;
				etk::Color<> fg;
				etk::Color<> bg;
				GetElement(jjj, iii, myTextToWrite, fg, bg);
				
				ewol::Text * tmpText = new ewol::Text();
				if (NULL != tmpText) {
					// get font size : 
					int32_t tmpFontHeight = tmpText->CalculateSize(etk::UniChar('A')).y();
					displayPositionY-=(tmpFontHeight+m_paddingSizeY);
					
					BGOObjects->SetColor(bg);
					BGOObjects->SetPos(vec3(displayPositionX, displayPositionY, 0) );
					BGOObjects->RectangleWidth(vec3(m_size.x()-displayPositionX, tmpFontHeight+2*m_paddingSizeY, 0));
					
					// get the maximum size of the colomn :
					vec3 textSize = tmpText->CalculateSize(myTextToWrite);
					sizeColom = etk_max(sizeColom, textSize.x());
					
					tmpText->SetColor(fg);
					tmpText->SetPos(vec3(tmpOriginX + displayPositionX, displayPositionY, 0) );
					tmpText->Print(myTextToWrite);
					AddOObject(tmpText);
					// madding move ...
					displayPositionY -= m_paddingSizeY;
					
					// add the raw position to remember it ...
					tmpRegister.setX(tmpRegister.x()+1);
					tmpRegister.setY(displayPositionY);
					m_lineSize.PushBack(tmpRegister);
					//EWOL_DEBUG("List indexation:" << tmpRegister);
				}
			}
			displayPositionX += sizeColom;
			tmpOriginX += m_paddingSizeX*2*2;
		}
		//m_lineSize.PushBack(tmpOriginY);
		AddOObject(BGOObjects, 0);
		
		// call the herited class...
		WidgetScrooled::OnRegenerateDisplay();
	}
}

bool widget::List::OnEventInput(const ewol::EventInput& _event)
{
	vec2 relativePos = RelativePosition(_event.GetPos());
	
	if (true == WidgetScrooled::OnEventInput(_event)) {
		ewol::widgetManager::FocusKeep(this);
		// nothing to do ... done on upper widet ...
		return true;
	}
	// parse all the loged row position to find the good one...
	int32_t rawID = -1;
	for(int32_t iii=0; iii<m_lineSize.Size()-1; iii++) {
		if(    relativePos.y()<m_lineSize[iii].y()
		    && relativePos.y()>=m_lineSize[iii+1].y() ) {
			// we find the raw :
			rawID = m_lineSize[iii].x();
			break;
		}
	}
	
	//EWOL_DEBUG("List event : idInput=" << IdInput << " typeEvent=" << typeEvent << "  raw=" << rawID << " pos=" << pos << "");
	bool isUsed = OnItemEvent(_event.GetId(), _event.GetStatus(), 0, rawID, _event.GetPos().x(), _event.GetPos().y());
	if (true == isUsed) {
		// TODO : this generate bugs ... I did not understand why ..
		//ewol::widgetManager::FocusKeep(this);
	}
	return isUsed;
}



void widget::List::OnGetFocus(void)
{
	EWOL_DEBUG("Ewol::List Get Focus");
}

void widget::List::OnLostFocus(void)
{
	EWOL_DEBUG("Ewol::List Lost Focus");
}
