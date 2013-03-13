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

#ifndef __SimpleAudioEngine_openal_h__
#define __SimpleAudioEngine_openal_h__

#include "SimpleAudioEngine.h"
#include "CCAudioPlayer.h"
#include <string>

using namespace std;

namespace WiSound {

/// iOS implementation of SimpleAudioEngine
class SimpleAudioEngine_openal : public SimpleAudioEngine {
    friend class CCAudioPlayer;
    
private:
    /// audio type
    enum Format {
        MP3,
        OGG,
        WAV
    };
    
	/// effect structure
	struct Effect {
		/// buffer
		char* buffer;
		
		/// length
		size_t length;
		
		/// path
		string path;
		
		/// audio type
		Format type;
        
        /// sound id
        int soundId;
	};
    
private:
    /// true means mute
	bool m_mute;
    
	/// volume of background music
	float m_backgroundVolume;
    
	/// volume of effect sound
	float m_effectVolume;
    
	/// current audio stream which is playing
    typedef vector<CCAudioPlayer*> AudioPlayerPtrList;
	AudioPlayerPtrList m_audios;
    
	/// audio stream need to be removed
	AudioPlayerPtrList m_pendingRemoveAudios;
    
	/// effect preloaded
    typedef vector<Effect*> EffectPtrList;
	EffectPtrList m_preloadEffects;
    
	/// mutex
	pthread_mutex_t m_mutex;
    
private:
    /// invoked by CCAudioPlayer when audio is stopped
    void onAudioStop(CCAudioPlayer* player);
    
    /// is audio already loaded?
    bool hasAudio(const string& path);
    
    /// is effect already loaded?
    bool hasPreloadedEffect(const string& path);
    
    /// get effect by path, or null if not preloaded
    Effect* getPreloadedEffect(const string& path);
    
    /// get effect by sound id, or null if not preloaded
    Effect* getPreloadedEffectById(int soundId);
    
    /// release an effect
    void releaseEffect(Effect* effect);
    
public:
    SimpleAudioEngine_openal();
    virtual ~SimpleAudioEngine_openal();
    
    /// @see CCObject::update
    virtual void update(float dt);
    
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

#endif // __SimpleAudioEngine_openal_h__

#endif // #if BACKEND_OPENAL