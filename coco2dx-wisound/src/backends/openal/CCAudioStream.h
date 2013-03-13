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

#ifndef __CCAudioStream_h__
#define __CCAudioStream_h__

#include "cocos2d.h"
#include "CCAssetInputStream.h"
#include <string>

USING_NS_CC;
using namespace std;

namespace WiSound {

/**
 * stream of an audio file
 */
class CCAudioStream : public CCObject {
protected:
	/// stream of audio file
	CCAssetInputStream* m_stream;

    /// true means only creating one buffer to load whole audio at once
	bool m_singleBuffer;

    /// bits every sample
	int m_bitsPerSample;

    /// channel count
	int m_channel;
    
    /// sample rate
	int m_sampleRate;

    /// path of audio file
	string m_path;

protected:
	/**
	 * invoked by setup, to initial some info of audio file, subclass
     * should implement this
	 */
	virtual void initInfo() = 0;
	
	/**
	 * create audio stream by path
	 *
	 * @param path path of audio file
	 */
	CCAudioStream(const string& path);
    
    /**
     * Create a memory buffer audio stream
     *
     * @param buffer buffer
     * @param length buffer data length
     * @param release true means this buffer should be mananged
     */
    CCAudioStream(char* buffer, size_t length, bool release);
	
public:
	virtual ~CCAudioStream();
    
    /**
	 * read some data
	 *
	 * @param buffer buffer
	 * @param length max length to read
	 * @return actual bytes read
	 */
	virtual int read(char* buffer, size_t length);
    
	/**
	 * skip some bytes
	 *
	 * @param length bytes to be skipped
	 */
	virtual void skip(size_t length);
    
	/**
	 * back to first byte
	 */
	virtual void reset();
    
    /**
     * initialization, invoked once before playing audio stream.
     * it calls initInfo to do real work and subclass should override
     * initInfo
	 */
	void setup();
	
	/**
	 * Set audio file path
	 *
	 * @param path audio file path
	 */
	void setPath(const string& path) { m_path = path; }
    
	/**
	 * check path is same or not
	 *
	 * @param path path of audio file
	 * @return true means the path is same as path of current stream
	 */
	bool isSame(const string& path);
    
    ///use single buffer or not
	bool isSingleBuffer() { return m_singleBuffer; }
    
    /// Get bits per sample
    int getBitsPerSample() { return m_bitsPerSample; }
    
    ///Get channel count
	int getChannel() { return m_channel; }
    
    /// Get sample rate
	int getSampleRate() { return m_sampleRate; }
};

} // end of namespace WiSound

#endif // __CCAudioStream_h__

#endif // #if BACKEND_OPENAL
