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

#include "CCMp3Stream.h"
#include <stdio.h>

namespace WiSound {

CCMp3Stream::CCMp3Stream(const string& path) :
		CCAudioStream(path),
		m_handle(NULL) {
}
    
CCMp3Stream::CCMp3Stream(char* buffer, size_t length, bool release) :
        CCAudioStream(buffer, length, release),
        m_handle(NULL) {
}

CCMp3Stream::~CCMp3Stream() {
	mpg123_close(m_handle);
	mpg123_delete(m_handle);
	m_handle = NULL;
}

CCMp3Stream* CCMp3Stream::create(const string& path) {
	CCMp3Stream* s = new CCMp3Stream(path);
	return (CCMp3Stream*)s->autorelease();
}
    
CCMp3Stream* CCMp3Stream::create(char* buffer, size_t length, bool release) {
    CCMp3Stream* s = new CCMp3Stream(buffer, length, release);
    return (CCMp3Stream*)s->autorelease();
}

off_t CCMp3Stream::mp3Seek(void* fp, off_t offset, int mode) {
	CCAssetInputStream* stream = (CCAssetInputStream*)fp;
	return stream->seek(offset, mode);
}

void CCMp3Stream::mp3Cleanup(void* fp) {
}

void CCMp3Stream::reset() {
	mpg123_seek(m_handle, 0, SEEK_SET);
}

int CCMp3Stream::read(char* buffer, size_t length) {
	size_t count = 0;
	int error = mpg123_read(m_handle, (unsigned char*)buffer, length, &count);
    if (error == MPG123_DONE) {
        return count;
    } else if (error != MPG123_OK) {
		CCLOGERROR("CCMp3Stream::read: %s", mpg123_strerror(m_handle));
        return -1;
	}

    return count;
}

void CCMp3Stream::initInfo() {
	// create handle
	int error;
	m_handle = mpg123_new(NULL, &error);
	if(m_handle == NULL){
		CCLOGERROR("CCMp3Stream::initInfo 1: %s", mpg123_plain_strerror(error));
		return;
	}

	// redirect IO routines
	error = mpg123_replace_reader_handle(m_handle, mp3Read, mp3Seek, mp3Cleanup);
	if (error == MPG123_ERR) {
		CCLOGERROR("CCMp3Stream::initInfo 2: %s", mpg123_strerror(m_handle));
		return;
	}

	/*
	 * Now mpg123 is being prepared for feeding. The main loop will read chunks from
	 * stdin and feed them to mpg123 then take decoded data as available to write to stdout.
	 */
	error = mpg123_open_handle(m_handle, m_stream);
	if (error == MPG123_ERR) {
		CCLOGERROR("CCMp3Stream::initInfo 3: %s", mpg123_strerror(m_handle));
		return;
	}

	// get audio info
	long sampleRate;
	int channel;
	int format;
	error = mpg123_getformat(m_handle, &sampleRate, &channel, &format);
	if (error != MPG123_OK) {
		CCLOGERROR("CCMp3Stream::initInfo 4: %s", mpg123_strerror(m_handle));
		return;
	}

	// Ensure that this output format will not change (it could, when we allow it)
	mpg123_format_none(m_handle);
	mpg123_format(m_handle, sampleRate, channel, MPG123_ENC_SIGNED_16);

	// save info
	m_sampleRate = sampleRate;
	m_channel = channel;
	m_bitsPerSample = 16;
}
    
} // end of namespace WiSound

#endif // #if BACKEND_OPENAL
