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

#ifndef __CCOggStream_h__
#define __CCOggStream_h__

#include "CCAudioStream.h"
#include "ivorbisfile.h"

using namespace std;

namespace WiSound {

/// ogg stream
class CCOggStream : public CCAudioStream {
private:
	/// OGG file structure
	OggVorbis_File* m_oggFile;

private:
	/*
	 * ogg file redirect routine
	 */

	static size_t oggRead(void* ptr, size_t size, size_t nmemb, void* datasource);
	static int oggSeek(void* datasource, ogg_int64_t offset, int whence);
	static int oggClose(void* datasource);
	static long oggTell(void* datasource);

protected:	
	CCOggStream(const string& path);
	
    /**
     * Create a memory buffer audio stream
     *
     * @param buffer buffer
     * @param length buffer data length
     * @param release true means this buffer should be mananged
     */
    CCOggStream(char* buffer, size_t length, bool release);
    
	/// @see CCAudioStream::reset
	virtual void reset();

	/// @see CCAudioStream::initInfo
	virtual void initInfo();

	/// @see CCAudioStream::read
	virtual int read(char* buffer, size_t length);

public:
	static CCOggStream* create(const string& path);
    static CCOggStream* create(char* buffer, size_t length, bool release = false);
    
	virtual ~CCOggStream();
};
    
} // end of namespace WiSound

#endif // __CCOggStream_h__

#endif // #if BACKEND_OPENAL
