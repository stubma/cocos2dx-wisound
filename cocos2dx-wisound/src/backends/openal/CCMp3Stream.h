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

#ifndef __CCMp3Stream_h__
#define __CCMp3Stream_h__

#include "CCAudioStream.h"
#include "mpg123.h"

using namespace std;

namespace WiSound {

/// mp3 audio stream
class CCMp3Stream : public CCAudioStream {
private:
	/// mp3 resource handle
	mpg123_handle* m_handle;

private:
	static ssize_t mp3Read(void* fp, void* buffer, size_t count);
	static off_t mp3Seek(void* fp, off_t offset, int mode);
	static void mp3Cleanup(void* fp);

protected:
	CCMp3Stream(const string& path);
	
    /**
     * Create a memory buffer audio stream
     *
     * @param buffer buffer
     * @param length buffer data length
     * @param release true means this buffer should be mananged
     */
    CCMp3Stream(char* buffer, size_t length, bool release);
    
	/// @see CCAudioStream::reset
	virtual void reset();

	/// @see CCAudioStream::read
	virtual int read(char* buffer, size_t length);

	/// @see CCAudioStream::initInfo
	virtual void initInfo();

public:
	static CCMp3Stream* create(const string& path);
    static CCMp3Stream* create(char* buffer, size_t length, bool release = false);
    
	virtual ~CCMp3Stream();
};
    
} // end of namespace WiSound

#endif // __CCMp3Stream_h__

#endif // #if BACKEND_OPENAL
