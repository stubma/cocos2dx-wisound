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

namespace WiSound {
    
// shared instance
static SimpleAudioEngine* s_instance = NULL;
    
SimpleAudioEngine* SimpleAudioEngine::sharedEngine() {
    if(!s_instance) {
        s_instance = new SimpleAudioEngine_mediaplayer();
    }
    return s_instance;
}
    
void SimpleAudioEngine::end() {
    CC_SAFE_RELEASE(s_instance);
}
    
SimpleAudioEngine_mediaplayer::SimpleAudioEngine_mediaplayer() {
}
    
SimpleAudioEngine_mediaplayer::~SimpleAudioEngine_mediaplayer() {
    s_instance = NULL;
}
       
void SimpleAudioEngine_mediaplayer::preloadBackgroundMusic(const char* pszFilePath) {
    
}

void SimpleAudioEngine_mediaplayer::playBackgroundMusic(const char* pszFilePath, bool bLoop) {
    
}

void SimpleAudioEngine_mediaplayer::stopBackgroundMusic(bool bReleaseData) {
    
}

void SimpleAudioEngine_mediaplayer::pauseBackgroundMusic() {
    
}

void SimpleAudioEngine_mediaplayer::resumeBackgroundMusic() {
    
}

void SimpleAudioEngine_mediaplayer::rewindBackgroundMusic() {
    
}

bool SimpleAudioEngine_mediaplayer::willPlayBackgroundMusic() {
    
}

bool SimpleAudioEngine_mediaplayer::isBackgroundMusicPlaying() {
    
}

float SimpleAudioEngine_mediaplayer::getBackgroundMusicVolume() {
    
}

void SimpleAudioEngine_mediaplayer::setBackgroundMusicVolume(float volume) {
    
}

float SimpleAudioEngine_mediaplayer::getEffectsVolume() {
    
}

void SimpleAudioEngine_mediaplayer::setEffectsVolume(float volume) {
    
}

unsigned int SimpleAudioEngine_mediaplayer::playEffect(const char* pszFilePath, bool bLoop) {
}

void SimpleAudioEngine_mediaplayer::pauseEffect(unsigned int nSoundId) {
    
}

void SimpleAudioEngine_mediaplayer::pauseAllEffects() {
    
}

void SimpleAudioEngine_mediaplayer::resumeEffect(unsigned int nSoundId) {
    
}

void SimpleAudioEngine_mediaplayer::resumeAllEffects() {
    
}

void SimpleAudioEngine_mediaplayer::stopEffect(unsigned int nSoundId) {
    
}

void SimpleAudioEngine_mediaplayer::stopAllEffects() {
    
}

void SimpleAudioEngine_mediaplayer::preloadEffect(const char* pszFilePath) {
    
}

void SimpleAudioEngine_mediaplayer::unloadEffect(const char* pszFilePath) {
    
}
    
} // end of namespace WiSound

#endif // #ifdef BACKEND_MEDIAPLAYER