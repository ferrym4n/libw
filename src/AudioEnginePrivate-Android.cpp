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

#include "w/AudioEngine.hpp"
#include "w/Exception.hpp"
#include "AudioEnginePrivate.hpp"
#include "Lock.hpp"
#include "w/Log.hpp"

namespace w
{
    AudioEnginePrivate* AudioEnginePrivate::singleton_ = NULL;

    AudioEnginePrivate::AudioEnginePrivate(float volumeAtStart, const std::string& assetPath):
        audioResourceManager_(assetPath),
        tracker_(1.0f),
        volumeAtStart_(volumeAtStart)
    {
        if (singleton_ != NULL)
        {
            throw Exception("AudioEngine exists! Create only one AudioEngine per process!");
        }

        // TODO: set-up android audio sink

        // All done
        singleton_ = this;
    }

    AudioEnginePrivate::~AudioEnginePrivate()
    {
        tracker_.shutdown();
        while (tracker_.shutdownIsDone() == false)
        {
        }
    }

    void AudioEnginePrivate::writeCallback(size_t size)
    {
        // TODO: write to sink
    }

    AudioEnginePrivate::State::Enum AudioEnginePrivate::state()
    {
        return singleton_->state_;
    }

    void AudioEnginePrivate::setVolume(float volume)
    {
        // TODO
    }

    float AudioEnginePrivate::volume()
    {
        return 1.0f; // TODO
    }

    AudioResource* AudioEnginePrivate::get(const std::string& file)
    {
        return singleton_->audioResourceManager_.get(file);
    }

    bool AudioEnginePrivate::play(AudioResource* resource, bool volume, bool looping)
    {
        TrackerSample* tmp = new TrackerSample(resource, volume, looping);

        tmp->increment();
        bool r = singleton_->tracker_.place(tmp);
        tmp->decrement();

        return r;
    }
}
