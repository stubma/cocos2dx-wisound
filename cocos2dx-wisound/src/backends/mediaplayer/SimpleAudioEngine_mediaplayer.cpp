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
#ifdef BACKEND_MEDIAPLAYER

#include "SimpleAudioEngine_mediaplayer.h"
#include "SimpleAudioEngineJni.h"
#include "opensl/SimpleAudioEngineOpenSL.h"
#include "cocos2d.h"
#include <jni/JniHelper.h>
#include <jni.h>

#define I9100_MODEL "GT-I9100"
#define CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"
#define METHOD_NAME "getDeviceModel"

USING_NS_CC;
using namespace std;

namespace WiSound {

// i9100 flag
static bool s_bI9100 = false;

// shared instance
static SimpleAudioEngine* s_instance = NULL;

static string getFullPathWithoutAssetsPrefix(const char* pszFilename) {
	// Changing file path to full path
    string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszFilename);
    
    // Removing `assets` since it isn't needed for the API of playing sound.
    size_t pos = fullPath.find("assets/");
    if (pos == 0) {
    	fullPath = fullPath.substr(strlen("assets/"));
    }
    
    return fullPath;
}
    
SimpleAudioEngine* SimpleAudioEngine::sharedEngine() {
    if(!s_instance) {
        s_instance = new SimpleAudioEngine_mediaplayer();
    }
    return s_instance;
}
    
void SimpleAudioEngine::end() {
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->end();
	} else {
		endJNI();
	}
	
    CC_SAFE_RELEASE(s_instance);
}
    
SimpleAudioEngine_mediaplayer::SimpleAudioEngine_mediaplayer() {
	// get getDeviceModel method
	JniMethodInfo methodInfo;
	jstring jstr;
	if(JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, METHOD_NAME, "()Ljava/lang/String;")) {
		jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	}
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	
	// if i9100, use i9100 compatible mode
	const char* deviceModel = methodInfo.env->GetStringUTFChars(jstr, NULL);
	if (strcmp(I9100_MODEL, deviceModel) == 0) {
		s_bI9100 = true;
	}
    
	// release
	methodInfo.env->ReleaseStringUTFChars(jstr, deviceModel);
	methodInfo.env->DeleteLocalRef(jstr);
}
    
SimpleAudioEngine_mediaplayer::~SimpleAudioEngine_mediaplayer() {
	if(s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->end();
	}
	
    s_instance = NULL;
}
       
void SimpleAudioEngine_mediaplayer::preloadBackgroundMusic(const char* pszFilePath) {
    string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
    preloadBackgroundMusicJNI(fullPath.c_str());
}

void SimpleAudioEngine_mediaplayer::playBackgroundMusic(const char* pszFilePath, bool bLoop) {
	string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
    playBackgroundMusicJNI(fullPath.c_str(), bLoop);
}

void SimpleAudioEngine_mediaplayer::stopBackgroundMusic(bool bReleaseData) {
	stopBackgroundMusicJNI();
}

void SimpleAudioEngine_mediaplayer::pauseBackgroundMusic() {
	pauseBackgroundMusicJNI();
}

void SimpleAudioEngine_mediaplayer::resumeBackgroundMusic() {
	resumeBackgroundMusicJNI();
}

void SimpleAudioEngine_mediaplayer::rewindBackgroundMusic() {
	rewindBackgroundMusicJNI();
}

bool SimpleAudioEngine_mediaplayer::willPlayBackgroundMusic() {
    return true;
}

bool SimpleAudioEngine_mediaplayer::isBackgroundMusicPlaying() {
	return isBackgroundMusicPlayingJNI();
}

float SimpleAudioEngine_mediaplayer::getBackgroundMusicVolume() {
	return getBackgroundMusicVolumeJNI();
}

void SimpleAudioEngine_mediaplayer::setBackgroundMusicVolume(float volume) {
	setBackgroundMusicVolumeJNI(volume);
}

float SimpleAudioEngine_mediaplayer::getEffectsVolume() {
	if (s_bI9100) {
		return SimpleAudioEngineOpenSL::sharedEngine()->getEffectsVolume();
	} else {
		return getEffectsVolumeJNI();
	}
}

void SimpleAudioEngine_mediaplayer::setEffectsVolume(float volume) {
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->setEffectsVolume(volume);
	} else {
		setEffectsVolumeJNI(volume);
	}
}

unsigned int SimpleAudioEngine_mediaplayer::playEffect(const char* pszFilePath, bool bLoop) {
	string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
	if (s_bI9100) {
		return SimpleAudioEngineOpenSL::sharedEngine()->playEffect(fullPath.c_str(), bLoop);
	} else {
		return playEffectJNI(fullPath.c_str(), bLoop);
	}
}

void SimpleAudioEngine_mediaplayer::pauseEffect(unsigned int nSoundId) {
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->pauseEffect(nSoundId);
	} else {
		pauseEffectJNI(nSoundId);
	}
}

void SimpleAudioEngine_mediaplayer::pauseAllEffects() {
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->pauseAllEffects();
	} else {
		pauseAllEffectsJNI();
	}
}

void SimpleAudioEngine_mediaplayer::resumeEffect(unsigned int nSoundId) {
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->resumeEffect(nSoundId);
	} else {
		resumeEffectJNI(nSoundId);
	}
}

void SimpleAudioEngine_mediaplayer::resumeAllEffects() {
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->resumeAllEffects();
	} else {
		resumeAllEffectsJNI();
	}
}

void SimpleAudioEngine_mediaplayer::stopEffect(unsigned int nSoundId) {
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->stopEffect(nSoundId);
	} else {
		stopEffectJNI(nSoundId);
	}
}

void SimpleAudioEngine_mediaplayer::stopAllEffects() {
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->stopAllEffects();
	} else {
		stopAllEffectsJNI();
	}
}

void SimpleAudioEngine_mediaplayer::preloadEffect(const char* pszFilePath) {
	string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->preloadEffect(fullPath.c_str());
	} else {
		preloadEffectJNI(fullPath.c_str());
	}
}

void SimpleAudioEngine_mediaplayer::unloadEffect(const char* pszFilePath) {
	string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
	if (s_bI9100) {
		SimpleAudioEngineOpenSL::sharedEngine()->unloadEffect(fullPath.c_str());
	} else {
		unloadEffectJNI(fullPath.c_str());
	}
}
    
} // end of namespace WiSound

#endif // #ifdef BACKEND_MEDIAPLAYER