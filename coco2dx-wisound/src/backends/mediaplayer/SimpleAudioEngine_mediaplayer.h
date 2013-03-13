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

#ifndef __SimpleAudioEngine_mediaplayer_h__
#define __SimpleAudioEngine_mediaplayer_h__

#include "SimpleAudioEngine.h"
#include <string>

using namespace std;

namespace WiSound {

/// iOS implementation of SimpleAudioEngine
class SimpleAudioEngine_mediaplayer : public SimpleAudioEngine {    
public:
    SimpleAudioEngine_mediaplayer();
    virtual ~SimpleAudioEngine_mediaplayer();
    
    // from SimpleAudioEngine
    virtual void preloadBackgroundMusic(const char* pszFilePath);
    virtual void playBackgroundMusic(const char* pszFilePath, bool bLoop);
    virtual void stopBackgroundMusic(bool bReleaseData);
    virtual void pauseBackgroundMusic();
    virtual void resumeBackgroundMusic();
    virtual void rewindBackgroundMusic();
    virtual bool willPlayBackgroundMusic();
    virtual bool isBackgroundMusicPlaying();
    virtual float getBackgroundMusicVolume();
    virtual void setBackgroundMusicVolume(float volume);
    virtual float getEffectsVolume();
    virtual void setEffectsVolume(float volume);
    virtual unsigned int playEffect(const char* pszFilePath, bool bLoop);
    virtual void pauseEffect(unsigned int nSoundId);
    virtual void pauseAllEffects();
    virtual void resumeEffect(unsigned int nSoundId);
    virtual void resumeAllEffects();
    virtual void stopEffect(unsigned int nSoundId);
    virtual void stopAllEffects();
    virtual void preloadEffect(const char* pszFilePath);
    virtual void unloadEffect(const char* pszFilePath);
};
    
} // end of namespace WiSound

#endif // __SimpleAudioEngine_mediaplayer_h__

#endif // #if BACKEND_MEDIAPLAYER