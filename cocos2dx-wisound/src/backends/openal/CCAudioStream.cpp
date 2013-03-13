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

#include "CCAudioStream.h"
#include "CCMemoryInputStream.h"

namespace WiSound {

CCAudioStream::CCAudioStream(const string& path) :
		m_path(path),
		m_singleBuffer(false),
		m_bitsPerSample(0),
		m_channel(0),
		m_sampleRate(0),
		m_stream(NULL) {
	m_stream = CCAssetInputStream::create(path);
	m_stream->retain();
	m_stream->open();
}
    
CCAudioStream::CCAudioStream(char* buffer, size_t length, bool release) :
        m_singleBuffer(true),
        m_bitsPerSample(0),
        m_channel(0),
        m_sampleRate(0),
        m_stream(NULL) {
    m_stream = CCMemoryInputStream::create(buffer, length, release);
    m_stream->retain();
    m_stream->open();
}

CCAudioStream::~CCAudioStream() {
	// free stream
	m_stream->close();
	m_stream->release();
	m_stream = NULL;
}

bool CCAudioStream::isSame(const string& path) {
    return m_path == path;
}

void CCAudioStream::setup() {
	// init
	initInfo();
}

int CCAudioStream::read(char* buffer, size_t length) {
	return m_stream->read(buffer, length);
}

void CCAudioStream::skip(size_t length) {
	m_stream->seek(length, SEEK_CUR);
}

void CCAudioStream::reset() {
	m_stream->seek(0, SEEK_SET);
}
    
} // end of namespace WiSound

#endif // #if BACKEND_OPENAL
