/**
 *******************************************************************************
 * @file ewol/texture/textureManager.cpp
 * @brief ewol Texture manager system (Sources)
 * @author Edouard DUPIN
 * @date 21/08/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <etk/File.h>
#include <etk/Vector.h>
#include <ewol/texture/TextureManager.h>
#include <ewol/texture/TextureFile.h>


static etk::Vector<ewol::Texture*>  l_textureList;
static etk::Vector<ewol::Texture*>  l_textureListToUpdate;

static uint32_t l_uniqueIdTexture = 0;
static bool     l_contextHasBeenRemoved = true;

void ewol::textureManager::Init(void)
{
	// nothing to do in th�ory then, we clean the buffers :
	// NOTE : If we do domething here, then the system does not work corectly
	l_textureList.Clear();
	l_textureListToUpdate.Clear();
	l_uniqueIdTexture = 0;
	l_contextHasBeenRemoved = true;
}

void ewol::textureManager::UnInit(void)
{
	l_textureListToUpdate.Clear();
	for (int32_t iii=0; iii<l_textureList.Size(); iii++) {
		if (l_textureList[iii] != NULL) {
			l_textureList[iii]->RemoveContext();
		}
	}
}

// note : Return the UniqueID ...
uint32_t ewol::textureManager::Add(ewol::Texture *object)
{
	if (object==NULL) {
		EWOL_CRITICAL("try to add a texture with NULL pointer ...");
		return 0;
	}
	for (int32_t iii=0; iii<l_textureList.Size(); iii++) {
		if (l_textureList[iii] != NULL) {
			if (l_textureList[iii] == object) {
				EWOL_CRITICAL("try to add a texture a second time ...");
				return 0;
			}
		}
	}
	l_uniqueIdTexture ++;
	// add it in the list
	l_textureList.PushBack(object);
	// add it to be regenerated at the next update :
	l_textureListToUpdate.PushBack(object);
	// return his ID ...
	return l_uniqueIdTexture;
}

void ewol::textureManager::Rm(ewol::Texture *object)
{
	if (object==NULL) {
		EWOL_CRITICAL("try to remove a texture with NULL pointer ...");
		return;
	}
	for (int32_t iii=l_textureListToUpdate.Size()-1; iii>=0; iii--) {
		if (l_textureListToUpdate[iii] != NULL) {
			if (l_textureListToUpdate[iii] == object) {
				// we find the texture :
				l_textureListToUpdate.Erase(iii);
			}
		}
	}
	for (int32_t iii=l_textureList.Size()-1; iii>=0; iii--) {
		if (l_textureList[iii] != NULL) {
			if (l_textureList[iii] == object) {
				// we find the texture :
				l_textureList.Erase(iii);
				return;
			}
		}
	}
	EWOL_CRITICAL("Try to remove a texture that is not present in the texture pool");
}

void ewol::textureManager::Update(ewol::Texture *object)
{
	// chek if not added before
	for (int32_t iii=0; iii<l_textureListToUpdate.Size(); iii++) {
		if (l_textureListToUpdate[iii] != NULL) {
			if (l_textureListToUpdate[iii] == object) {
				// just prevent some double add ...
				return;
			}
		}
	}
	// add it ...
	l_textureListToUpdate.PushBack(object);
}

// Specific to load or update the data in the openGl context ==> system use only
void ewol::textureManager::UpdateContext(void)
{
	if (true == l_contextHasBeenRemoved) {
		// need to update all ...
		l_contextHasBeenRemoved = false;
		for (int32_t iii=0; iii<l_textureList.Size(); iii++) {
			if (l_textureList[iii] != NULL) {
				l_textureList[iii]->UpdateContext();
			}
		}
	}else {
		for (int32_t iii=0; iii<l_textureListToUpdate.Size(); iii++) {
			if (l_textureListToUpdate[iii] != NULL) {
				l_textureListToUpdate[iii]->UpdateContext();
			}
		}
	}
	// Clean the update list
	l_textureListToUpdate.Clear();
}

void ewol::textureManager::OpenGlContextHasBeenDestroyed(void)
{
	for (int32_t iii=0; iii<l_textureList.Size(); iii++) {
		if (l_textureList[iii] != NULL) {
			l_textureList[iii]->UpdateContext();
		}
	}
	// no context preent ...
	l_contextHasBeenRemoved = true;
}


static etk::Vector<ewol::TextureFile *>  l_imageList;


// this permit to keep a specific texture and prevent the multiple loading of this one ...
ewol::Texture* ewol::textureManager::ImageKeep(etk::UString fileName, Vector2D<int32_t> size)
{
	
	for (int32_t iii=l_imageList.Size()-1; iii>=0; iii--) {
		if (l_imageList[iii] != NULL) {
			if(    l_imageList[iii]->HasName(fileName)
			    && l_imageList[iii]->Get().Get().GetWidth() == size.x
			    && l_imageList[iii]->Get().Get().GetHeight() == size.y) {
				l_imageList[iii]->Increment();
				return &l_imageList[iii]->Get();
			}
		}
	}
	ewol::TextureFile* tmpResources = new ewol::TextureFile(fileName, size);
	if (NULL == tmpResources) {
		EWOL_ERROR("allocation error of a resource Image : " << fileName);
		return NULL;
	}
	l_imageList.PushBack(tmpResources);
	return &tmpResources->Get();
}

void ewol::textureManager::ImageRelease(ewol::Texture*& object)
{
	for (int32_t iii=l_imageList.Size()-1; iii>=0; iii--) {
		if (l_imageList[iii] != NULL) {
			if(&l_imageList[iii]->Get() == object) {
				if (true == l_imageList[iii]->Decrement()) {
					// delete element
					delete(l_imageList[iii]);
					// remove element from the list :
					l_imageList.Erase(iii);
				}
				// insidiously remove the pointer for the caller ...
				object = NULL;
				return;
			}
		}
	}
}

