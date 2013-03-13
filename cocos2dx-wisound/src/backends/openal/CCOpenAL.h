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

#ifndef __CCOpenAL_h__
#define __CCOpenAL_h__

#include <stdbool.h>
#include <stdlib.h>
#if ANDROID || WINDOWS
    #include "AL/al.h"
    #include "AL/alc.h"
#elif IOS || MACOSX
    #import <OpenAL/al.h>
    #import <OpenAL/alc.h>
#endif

/**
 * @class CCOpenAL
 *
 * 封装一些OpenAL的常用例程
 */
class CCOpenAL {
private:
	/// true means initialized
	static bool s_inited;

	/// OpenAL device
	static ALCdevice* s_device;

	/// OpenAL context
	static ALCcontext* s_context;

public:
	/**
	 * Initialize OpenAL
	 *
	 * @param deviceArguments Arguments supplied to native device. Optional and default is NULL
	 * @param contextFrequency Frequency for mixing output buffer, in units of Hz (Common values
	 * 		include 11025, 22050, and 44100). Optional and default is 44100
	 * @param contextRefresh Refresh intervalls, in units of Hz. Optional and default is 60
	 * @param contextSynchronized Flag, indicating a synchronous context. Optional and default is false
	 */
	static void init(const char* deviceArguments = NULL,
			int contextFrequency = 44100, int contextRefresh = 60, bool contextSynchronized = false);

	/**
	 * Destroy OpenAL resources
	 */
	static void destroy();

	/**
	 * Get an OpenAL source
	 *
	 * @return source id, or 0 means failed
	 */
	static ALuint obtainSource();

	/**
	 * Free an OpenAL source
	 *
	 * @param source source id
	 */
	static void freeSource(ALuint source);
};

#endif // __CCOpenAL_h__

#endif // #if BACKEND_OPENAL
