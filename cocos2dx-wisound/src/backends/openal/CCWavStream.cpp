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

#include "CCWavStream.h"
#include <stdio.h>
#include "CCMoreMacros.h"

namespace WiSound {

CCWavStream::CCWavStream(const string& path) :
		CCAudioStream(path),
		m_dataStartPosition(0) {
}
    
CCWavStream::CCWavStream(char* buffer, size_t length, bool release) :
        CCAudioStream(buffer, length, release),
        m_dataStartPosition(0) {
}

CCWavStream::~CCWavStream() {
}

CCWavStream* CCWavStream::create(const string& path) {
	CCWavStream* s = new CCWavStream(path);
	return (CCWavStream*)s->autorelease();
}
    
CCWavStream* CCWavStream::create(char* buffer, size_t length, bool release) {
    CCWavStream* s = new CCWavStream(buffer, length, release);
    return (CCWavStream*)s->autorelease();
}

void CCWavStream::initInfo() {
	// read header
	wyWavHeader header;
	read((char*)&header, sizeof(wyWavHeader));

	// do byte endian conversion
	header.fmtSize = letoh32(header.fmtSize);
	header.format = letoh16(header.format);
	header.channels = letoh16(header.channels);
	header.samplesPerSec = letoh32(header.samplesPerSec);
	header.bytesPerSec = letoh32(header.bytesPerSec);
	header.blockAlign = letoh16(header.blockAlign);
	header.bitsPerSample = letoh16(header.bitsPerSample);

	// do validation
	if(memcmp("RIFF", header.riff, 4)) {
		CCLOGERROR("Wav RIFF header not found");
	} else if(memcmp("WAVE", header.wave, 4)) {
		CCLOGERROR("Wav WAVE chunk not found");
	} else if(memcmp("fmt ", header.fmt, 4)) {
		CCLOGERROR("Wav fmt chunk not found");
	} else if(header.format != 1) {
		CCLOGERROR("Wav file must be PCM format");
	} else if(header.channels != 1 && header.channels != 2) {
		CCLOGERROR("Wav file must has 1 or 2 channels");
	} else if(header.bitsPerSample != 16 && header.bitsPerSample != 8) {
		CCLOGERROR("Wav file must use 8 or 16 bit sample");
	} else {
		// skip fmt chunk
		skip(header.fmtSize - 16);

		// read following chunk until we found data chunk
		wyChunkHeader chunk;
		size_t length = read((char*)&chunk, sizeof(wyChunkHeader));
		while(length == sizeof(wyChunkHeader)) {
			chunk.dataSize = letoh32(chunk.dataSize);
			if(memcmp("data", chunk.data, 4)) {
				skip(chunk.dataSize);
				read((char*)&chunk, sizeof(wyChunkHeader));
			} else {
				break;
			}
		}

		// validate data chunk
		if(memcmp("data", chunk.data, 4)) {
			CCLOGERROR("Wav data chunk not found");
		} else {
			// save info
			m_bitsPerSample = header.bitsPerSample;
			m_channel = header.channels;
			m_sampleRate = header.samplesPerSec;
			m_dataStartPosition = m_stream->getPosition();
		}
	}
}

void CCWavStream::reset() {
	m_stream->seek(m_dataStartPosition, SEEK_SET);
}

} // end of namespace WiSound

#endif // #if BACKEND_OPENAL
