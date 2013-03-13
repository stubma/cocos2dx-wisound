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

#include "CCOggStream.h"

namespace WiSound {

CCOggStream::CCOggStream(const string& path) :
		CCAudioStream(path),
		m_oggFile(new OggVorbis_File()) {
}
    
CCOggStream::CCOggStream(char* buffer, size_t length, bool release) :
        CCAudioStream(buffer, length, release),
        m_oggFile(new OggVorbis_File()) {
}

CCOggStream::~CCOggStream() {
	if(m_oggFile) {
		ov_clear(m_oggFile);
		delete m_oggFile;
		m_oggFile = NULL;
	}
}

CCOggStream* CCOggStream::create(const string& path) {
	CCOggStream* s = new CCOggStream(path);
	return (CCOggStream*)s->autorelease();
}
    
CCOggStream* CCOggStream::create(char* buffer, size_t length, bool release) {
    CCOggStream* s = new CCOggStream(buffer, length, release);
    return (CCOggStream*)s->autorelease();
}

size_t CCOggStream::oggRead(void* ptr, size_t size, size_t nmemb, void* datasource) {
	CCAssetInputStream* stream = (CCAssetInputStream*)datasource;
	return stream->read((char*)ptr, size * nmemb) / size;
}

int CCOggStream::oggSeek(void* datasource, ogg_int64_t offset, int whence) {
	CCAssetInputStream* stream = (CCAssetInputStream*)datasource;
	return stream->seek(offset, whence);
}

int CCOggStream::oggClose(void* datasource) {
	return 0;
}

long CCOggStream::oggTell(void* datasource) {
	CCAssetInputStream* stream = (CCAssetInputStream*)datasource;
	return stream->getPosition();
}

void CCOggStream::reset() {
	ov_pcm_seek(m_oggFile, 0);
}

void CCOggStream::initInfo() {
	// open ogg file with stream
	static ov_callbacks callback = {
		oggRead,
		oggSeek,
		oggClose,
		oggTell
	};
	ov_open_callbacks(m_stream, m_oggFile, 0, 0, callback);

	// get ogg info
	vorbis_info* info = ov_info(m_oggFile, -1);
	m_channel = info->channels;
	m_sampleRate = info->rate;
	m_bitsPerSample = 16;
}

int CCOggStream::read(char* buffer, size_t length) {
	int bitstream;
	int read = 0;
	while(read != length) {
		int ret = ov_read(m_oggFile, buffer + read, length - read, &bitstream);
		if(ret == OV_HOLE)
			continue;
		if(ret == OV_EBADLINK || ret == OV_EINVAL || ret == 0)
			return read;
		read += ret;
	}

	return read;
}

} // end of namespace WiSound

#endif // #if BACKEND_OPENAL
