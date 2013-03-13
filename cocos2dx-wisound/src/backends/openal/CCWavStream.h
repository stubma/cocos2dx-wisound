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

#ifndef __CCWavStream_h__
#define __CCWavStream_h__

#include "CCAudioStream.h"

using namespace std;

namespace WiSound {

/// wav stream
class CCWavStream : public CCAudioStream {
private:
    /// header of wav file, in little endian
    struct wyWavHeader {
        char riff[4]; //'RIFF'
        unsigned int riffSize;
        char wave[4]; //'WAVE'
        char fmt[4]; //'fmt '
        unsigned int fmtSize;
        unsigned short format;
        unsigned short channels;
        unsigned int samplesPerSec;
        unsigned int bytesPerSec;
        unsigned short blockAlign;
        unsigned short bitsPerSample;
    };
    
    /// header of chunk
    struct wyChunkHeader {
        char data[4];
        unsigned int dataSize;
    };
    
private:
	/// offset position of audio data in file
	int m_dataStartPosition;

protected:
	CCWavStream(const string& path);
	
    /**
     * Create a memory buffer audio stream
     *
     * @param buffer buffer
     * @param length buffer data length
     * @param release true means this buffer should be mananged
     */
    CCWavStream(char* buffer, size_t length, bool release);
    
	/// @see CCAudioStream::reset
	virtual void reset();

	/// @see CCAudioStream::initInfo
	virtual void initInfo();

public:
	static CCWavStream* create(const string& path);
    static CCWavStream* create(char* buffer, size_t length, bool release = false);
    
	virtual ~CCWavStream();
};
    
} // end of namespace WiSound

#endif // __CCWavStream_h__

#endif // #if BACKEND_OPENAL
