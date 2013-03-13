/*
 * Copyright (c) 2010 WiYun Inc.
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
#if BACKEND_OPENAL

#ifndef __CCAudioPlayer_h__
#define __CCAudioPlayer_h__

#include "CCAudioStream.h"
#if ANDROID || WINDOWS
    #include "AL/al.h"
    #include "AL/alc.h"
#elif IOS || MACOSX
    #import <OpenAL/al.h>
    #import <OpenAL/alc.h>
#endif

USING_NS_CC;
using namespace std;

namespace WiSound {

/**
 * @class CCAudioPlayer
 *
 * Audio player for background and effect music
 */
class CCAudioPlayer : public CCObject {
private:
    /// audio stream
    CCAudioStream* m_stream;
    
    /// loop count, 0 means no loop, -1 mean infinite, > 0 means loop count. by default it is zero
	int m_loop;
    
	/// true means playing
	bool m_playing;
    
	/// true means paused
	bool m_paused;
    
    /// sound id which is played by this player, or 0 if not set
    int m_soundId;
    
    /// OpenAL buffer id list
	ALuint* m_buffers;
    
    /// audio source id
	ALuint m_source;
    
	/// time elapsed after playing
	float m_renderedSeconds;
    
	/// capacity of audio buffer, in seconds
	float m_secondsPerBuffer;
    
	/// temp buffer, only used for multi-buffer mode
	char* m_tempBuffer;
    
protected:
    /**
     * Constructor
     *
     * @param stream audio stream
     */
    CCAudioPlayer(CCAudioStream* stream);
    
public:
    virtual ~CCAudioPlayer();
    
    /**
     * Create an audio player for a stream
     *
     * @param stream audio stream
     * @return audio player
     */
    static CCAudioPlayer* create(CCAudioStream* stream);
    
    /**
	 * 开始播放这个音频流
	 *
	 * @param volume 音量, 0表示最小, 1表示最大
	 */
	virtual void play(float volume);
    
	/**
	 * 停止播放这个音频流
	 */
	virtual void stop();
    
	/**
	 * 暂停播放
	 */
	virtual void pause();
    
	/**
	 * 恢复播放
	 */
	virtual void resume();
    
    /**
     * Get audio stream
     *
     * @return audio stream object
     */
    CCAudioStream* getStream() { return m_stream; }
    
    /**
	 * 是否正在播放
	 *
	 * @return true表示正在播放
	 */
	bool isPlaying() { return m_playing; }
    
	/**
	 * 是否暂停了
	 *
	 * @return true表示已经暂停
	 */
	bool isPaused() { return m_paused; }
    
	/**
	 * 是否需要循环
	 *
	 * @return true表示需要循环
	 */
	bool isLoop() { return m_loop != 0; }
    
	/**
	 * 设置音量
	 *
	 * @param volume 音量
	 */
	void setVolume(float volume);
    
    /**
	 * 填充一个缓冲区
	 *
	 * @param buffer 缓冲区id
	 * @return 成功返回true, 失败返回false
	 */
	bool fill(ALuint buffer);
    
    /**
	 * 根据音频信息得到OpenAL格式常量
	 *
	 * @return OpenAL格式常量
	 */
	ALenum getALFormat();
    
    /**
	 * 设置循环次数
	 *
	 * @param loop 0表示不循环, -1表示无限循环, 大于0表示循环次数
	 */
	void setLoopCount(int loop) { m_loop = loop; }
    
    /// get sound id
    int getSoundId() { return m_soundId; }
    
    /// set sound id
    void setSoundId(int sid) { m_soundId = sid; }
    
    /**
	 * 更新播放缓冲或其它状态
	 */
	void update();
};
    
}

#endif // __CCAudioPlayer_h__

#endif // #if BACKEND_OPENAL
