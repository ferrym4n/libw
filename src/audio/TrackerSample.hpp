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

#ifndef LIBW_TRACKERSAMPLE
#define LIBW_TRACKERSAMPLE

#include "AudioResource.hpp"
#include "Referenced.hpp"
#include <stdint.h>

namespace w
{
    class TrackerSample: public Referenced
    {
    public:
        static unsigned int const BytesPerSample = 2;

        TrackerSample(AudioResource* resource, float volume, bool looping);
        ~TrackerSample();
        float volume();
        void setVolume(float volume);
        int16_t sample(bool& end);
        void fadeOut(unsigned int fadeTimeMilliseconds);

    protected:
        float volume_;
        AudioResource* resource_;
        unsigned int byteSize_;
        unsigned int byteLocation_;
        bool looping_;

        struct FadeOut
        {
            bool on_;
            unsigned int start_;
            float ramp_; // applied every sample() call to volume_ during fadeout
        } fadeOut_;
    };
}

#endif
