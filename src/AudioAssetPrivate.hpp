/**
 * libw
 *
 * Copyright (C) 2013 Woima Solutions
 *
 * This software is provided 'as-is', without any express or implied warranty. In
 * no event will the authors be held liable for any damages arising from the use
 * of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including
 * commercial applications, and to alter it and redistribute it freely, subject to
 * the following restrictions:
 *
 * 1) The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a product,
 *    an acknowledgment in the product documentation is appreciated.
 *
 * 2) Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3) This notice may not be removed or altered from any source distribution.
 *
 * @author antti.peuhkurinen@woimasolutions.com
 */

#ifndef LIBW_AUDIOASSETPRIVATE
#define LIBW_AUDIOASSETPRIVATE

#include "w/Class.hpp"
#include "AudioResource.hpp"
#include "Referenced.hpp"
#include "TrackerSample.hpp"
#include <string>
#include <sigc++/connection.h>

namespace w
{
    class AudioAssetPrivate: public Referenced
    {
    public:
        UNCOPYABLE(AudioAssetPrivate)

        AudioAssetPrivate(const std::string& filename, bool parallelPlay, bool looping);
        ~AudioAssetPrivate();
        bool play(float volume);
        void setVolume(float volume);
        void fadeOut(unsigned int fadeOutTimeMilliseconds);

    private:
        AudioResource* resource_;
        Mutex mutex_;
        bool parallerPlay_;
        bool looping_;
    };
}

#endif
