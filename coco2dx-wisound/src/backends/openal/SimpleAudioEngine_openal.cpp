/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-wisound
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifdef BACKEND_OPENAL

#include "cocos2d.h"
#include "SimpleAudioEngine_openal.h"
#include "CCOpenAL.h"
#include "CCMp3Stream.h"
#include "CCOggStream.h"
#include "CCWavStream.h"
#include "CCUtils.h"
#include "CCAudioPlayer.h"

USING_NS_CC;

namespace WiSound {
    
// shared instance
static SimpleAudioEngine* s_instance = NULL;
    
// sound id
static int s_nextSoundId = 1;
    
SimpleAudioEngine* SimpleAudioEngine::sharedEngine() {
    if(!s_instance) {
        s_instance = new SimpleAudioEngine_openal();
    }
    return s_instance;
}
    
void SimpleAudioEngine::end() {
    CC_SAFE_RELEASE(s_instance);
}
    
SimpleAudioEngine_openal::SimpleAudioEngine_openal() :
        m_backgroundVolume(1.f),
        m_effectVolume(1.f),
        m_audios(NULL),
        m_pendingRemoveAudios(NULL),
        m_preloadEffects(NULL),
        m_mute(false) {
    // init OpenAL
    CCOpenAL::init();
    
    // create mutex
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    
    // init mpg123
    mpg123_init();
    
    // schedule update timer
    CCScheduler* scheduler = CCDirector::sharedDirector()->getScheduler();
    if(scheduler) {
        scheduler->scheduleSelector(schedule_selector(SimpleAudioEngine_openal::update),
                                    this, 0, false);
    }
}
    
SimpleAudioEngine_openal::~SimpleAudioEngine_openal() {
    // unschedule update
    CCScheduler* scheduler = CCDirector::sharedDirector()->getScheduler();
    if(scheduler) {
        scheduler->unscheduleSelector(schedule_selector(SimpleAudioEngine_openal::update), this);
    }
    
    // release audio player and effect
    for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
		CC_SAFE_RELEASE(*iter);
	}
	for(EffectPtrList::iterator iter = m_preloadEffects.begin(); iter != m_preloadEffects.end(); iter++) {
		releaseEffect(*iter);
	}
    
    // release mutex
	pthread_mutex_destroy(&m_mutex);
    
    // end mpg123
	mpg123_exit();
    
    // end openal
	CCOpenAL::destroy();
    
    // nullify singleton
    s_instance = NULL;
}
    
void SimpleAudioEngine_openal::releaseEffect(Effect* effect) {
    if(effect->buffer)
        free(effect->buffer);
    free((void*)effect);
}
    
void SimpleAudioEngine_openal::update(float dt) {
	pthread_mutex_lock(&m_mutex);
    
	// remove pending audios
	for(AudioPlayerPtrList::iterator iter1 = m_pendingRemoveAudios.begin(); iter1 != m_pendingRemoveAudios.end(); iter1++) {
		for(AudioPlayerPtrList::iterator iter2 = m_audios.begin(); iter2 != m_audios.end(); iter2++) {
			if(*iter1 == *iter2) {
				// must remember, erase will let iter move forward so release should
				// be called first
				CC_SAFE_RELEASE(*iter2);
				m_audios.erase(iter2);
				break;
			}
		}
	}
    
	// clear pending remove list
	m_pendingRemoveAudios.clear();
    
	// update every player
    for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
		(*iter)->update();
	}
    
	pthread_mutex_unlock(&m_mutex);
}
    
void SimpleAudioEngine_openal::onAudioStop(CCAudioPlayer* player) {
    pthread_mutex_lock(&m_mutex);
    m_pendingRemoveAudios.push_back(player);
    pthread_mutex_unlock(&m_mutex);
}

bool SimpleAudioEngine_openal::hasAudio(const string& path) {
    for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
        if((*iter)->getStream()->isSame(path)) {
            return true;
        }
    }
    
    return false;
}
    
bool SimpleAudioEngine_openal::hasPreloadedEffect(const string& path) {
    for(EffectPtrList::iterator iter = m_preloadEffects.begin(); iter != m_preloadEffects.end(); iter++) {
        if(path == (*iter)->path) {
            return true;
        }
    }
    
    return false;
}
    
SimpleAudioEngine_openal::Effect* SimpleAudioEngine_openal::getPreloadedEffect(const string& path)  {
    for(EffectPtrList::iterator iter = m_preloadEffects.begin(); iter != m_preloadEffects.end(); iter++) {
        if(path == (*iter)->path) {
            return *iter;
        }
    }
    
    return NULL;
}
    
SimpleAudioEngine_openal::Effect* SimpleAudioEngine_openal::getPreloadedEffectById(int soundId) {
    for(EffectPtrList::iterator iter = m_preloadEffects.begin(); iter != m_preloadEffects.end(); iter++) {
        if(soundId == (*iter)->soundId) {
            return *iter;
        }
    }
    
    return NULL;
}
    
void SimpleAudioEngine_openal::preloadBackgroundMusic(const char* pszFilePath) {
    // do nothing
}

void SimpleAudioEngine_openal::playBackgroundMusic(const char* pszFilePath, bool bLoop) {
    pthread_mutex_lock(&m_mutex);
    
    string path = pszFilePath;
	if(!hasAudio(path)) {
		CCAudioStream* audio = NULL;
		if(CCUtils::endsWith(path, ".wav") || CCUtils::endsWith(path, ".WAV")) {
			audio = CCWavStream::create(path);
		} else if(CCUtils::endsWith(path, ".mp3") || CCUtils::endsWith(path, ".MP3")) {
			audio = CCMp3Stream::create(path);
		} else if(CCUtils::endsWith(path, ".ogg") || CCUtils::endsWith(path, ".OGG")) {
			audio = CCOggStream::create(path);
		} else {
			CCLOGWARN("playBackgroundMusic: Unsupported audio type: %s", pszFilePath);
		}
        
		if(audio) {
			audio->setup();
            CCAudioPlayer* player = CCAudioPlayer::create(audio);
            player->retain();
			player->setLoopCount(bLoop ? -1 : 0);
			player->play(m_backgroundVolume);
			m_audios.push_back(player);
		}
	}
    
	pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::stopBackgroundMusic(bool bReleaseData) {
    pthread_mutex_lock(&m_mutex);
	
	for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
		if(!(*iter)->getStream()->isSingleBuffer())
			(*iter)->stop();
	}
	
	pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::pauseBackgroundMusic() {
    pthread_mutex_lock(&m_mutex);
	
	for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
		if(!(*iter)->getStream()->isSingleBuffer())
			(*iter)->pause();
	}
	
	pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::resumeBackgroundMusic() {
    pthread_mutex_lock(&m_mutex);
	
	for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
		if(!(*iter)->getStream()->isSingleBuffer())
			(*iter)->resume();
	}
	
	pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::rewindBackgroundMusic() {
    pthread_mutex_lock(&m_mutex);
	
	for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
		if(!(*iter)->getStream()->isSingleBuffer()) {
			(*iter)->stop();
            (*iter)->getStream()->reset();
        }
	}
	
	pthread_mutex_unlock(&m_mutex);
}

bool SimpleAudioEngine_openal::willPlayBackgroundMusic() {
    return true;
}

bool SimpleAudioEngine_openal::isBackgroundMusicPlaying() {
    pthread_mutex_lock(&m_mutex);
	
    bool ret = false;
	for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
		if(!(*iter)->getStream()->isSingleBuffer() && (*iter)->isPlaying()) {
			ret = true;
            break;
        }
	}
	
	pthread_mutex_unlock(&m_mutex);
    
	return ret;
}

float SimpleAudioEngine_openal::getBackgroundMusicVolume() {
    return m_backgroundVolume;
}

void SimpleAudioEngine_openal::setBackgroundMusicVolume(float volume) {
    m_backgroundVolume = volume;
    
	pthread_mutex_lock(&m_mutex);
    
	for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
		if(!(*iter)->getStream()->isSingleBuffer())
			(*iter)->setVolume(volume);
	}
    
	pthread_mutex_unlock(&m_mutex);
}

float SimpleAudioEngine_openal::getEffectsVolume() {
    return m_effectVolume;
}

void SimpleAudioEngine_openal::setEffectsVolume(float volume) {
    m_effectVolume = volume;
    
	pthread_mutex_lock(&m_mutex);
    
	for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
		if((*iter)->getStream()->isSingleBuffer())
			(*iter)->setVolume(volume);
	}
    
	pthread_mutex_unlock(&m_mutex);
}

unsigned int SimpleAudioEngine_openal::playEffect(const char* pszFilePath, bool bLoop) {
    pthread_mutex_lock(&m_mutex);
    
	// get effect
    string path = pszFilePath;
	Effect* effect = getPreloadedEffect(path);
	if(!effect) {
		preloadEffect(pszFilePath);
		effect = getPreloadedEffect(path);
	}
	
	// if fail to load effect, return
	// if ok, create audio stream and player
	if(!effect) {
		CCLOGWARN("Failed to load effect path: %s", pszFilePath);
	} else {
		CCAudioStream* audio = NULL;
		switch(effect->type) {
			case MP3:
				audio = CCMp3Stream::create(effect->buffer, effect->length);
				break;
			case WAV:
				audio = CCWavStream::create(effect->buffer, effect->length);
				break;
			case OGG:
				audio = CCOggStream::create(effect->buffer, effect->length);
				break;
			default:
				CCLOGWARN("preloadEffect: Unsupported audio type: %d", effect->type);
				break;
		}
		
		if(audio) {
			audio->setup();
            CCAudioPlayer* player = CCAudioPlayer::create(audio);
            player->retain();
			player->play(m_effectVolume);
            player->setLoopCount(bLoop ? -1 : 0);
            player->setSoundId(effect->soundId);
			m_audios.push_back(player);
		}
	}
    
	pthread_mutex_unlock(&m_mutex);
    
    return effect ? effect->soundId : 0;
}

void SimpleAudioEngine_openal::pauseEffect(unsigned int nSoundId) {
    pthread_mutex_lock(&m_mutex);
    
    for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
        CCAudioPlayer* player = *iter;
        if(player->getSoundId() == nSoundId) {
            player->pause();
            break;
        }
    }
    
    pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::pauseAllEffects() {
    pthread_mutex_lock(&m_mutex);
    
    for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
        CCAudioPlayer* player = *iter;
        if(player->getStream()->isSingleBuffer()) {
            player->pause();
        }
    }
    
    pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::resumeEffect(unsigned int nSoundId) {
    pthread_mutex_lock(&m_mutex);
    
    for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
        CCAudioPlayer* player = *iter;
        if(player->getSoundId() == nSoundId) {
            player->resume();
            break;
        }
    }
    
    pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::resumeAllEffects() {
    pthread_mutex_lock(&m_mutex);
    
    for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
        CCAudioPlayer* player = *iter;
        if(player->getStream()->isSingleBuffer()) {
            player->resume();
        }
    }
    
    pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::stopEffect(unsigned int nSoundId) {
    pthread_mutex_lock(&m_mutex);
    
    for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
        CCAudioPlayer* player = *iter;
        if(player->getSoundId() == nSoundId) {
            player->stop();
            break;
        }
    }
    
    pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::stopAllEffects() {
    pthread_mutex_lock(&m_mutex);
    
    for(AudioPlayerPtrList::iterator iter = m_audios.begin(); iter != m_audios.end(); iter++) {
        CCAudioPlayer* player = *iter;
        if(player->getStream()->isSingleBuffer()) {
            player->stop();
        }
    }
    
    pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::preloadEffect(const char* pszFilePath) {
    pthread_mutex_lock(&m_mutex);
    
    string path = pszFilePath;
	if(!hasPreloadedEffect(path)) {
		// load effect raw data
		unsigned long length;
        unsigned char* buf = CCFileUtils::sharedFileUtils()->getFileData(pszFilePath, "rb", &length);
		
		// create effect
		Effect* effect = (Effect*)calloc(1, sizeof(Effect));
		effect->buffer = (char*)buf;
		effect->length = (size_t)length;
		effect->path = path;
        effect->soundId = s_nextSoundId++;
		if(CCUtils::endsWith(path, ".wav") || CCUtils::endsWith(path, ".WAV")) {
			effect->type = WAV;
		} else if(CCUtils::endsWith(path, ".mp3") || CCUtils::endsWith(path, ".MP3")) {
			effect->type = MP3;
		} else if(CCUtils::endsWith(path, ".ogg") || CCUtils::endsWith(path, ".OGG")) {
			effect->type = OGG;
		}
		
		// add effect
		m_preloadEffects.push_back(effect);
	}
    
	pthread_mutex_unlock(&m_mutex);
}

void SimpleAudioEngine_openal::unloadEffect(const char* pszFilePath) {
    pthread_mutex_lock(&m_mutex);
    
    string path = pszFilePath;
    for(EffectPtrList::iterator iter = m_preloadEffects.begin(); iter != m_preloadEffects.end(); iter++) {
        Effect* effect = *iter;
        if(path == effect->path) {
            // release, erase and break
            releaseEffect(effect);
            m_preloadEffects.erase(iter);
            break;
        }
    }
    
    pthread_mutex_unlock(&m_mutex);
}
    
} // end of namespace WiSound

#endif // #ifdef BACKEND_OPENAL