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

#include "w/Log.hpp"
#include "w/AudioAsset.hpp"
#include "AudioEnginePrivate.hpp"

namespace w
{
    AudioAsset::AudioAsset(const std::string& filename):
        resource_(AudioEnginePrivate::get(filename))
    {
        resource_->increment();
    }

    AudioAsset::AudioAsset(AudioAsset const& r):
        resource_(r.resource_)
    {
        resource_->increment();
    }

    AudioAsset::~AudioAsset()
    {
        resource_->decrement();
        resource_ = NULL;
    }

    AudioAsset& AudioAsset::operator=(AudioAsset const& r)
    {
        if (this != &r)
        {
            resource_ = r.resource_;
            resource_->increment();
        }
        return *this;
    }

    void AudioAsset::play(float volume)
    {
        // TODO: create tracker sample from this resource
    }

    void AudioAsset::setVolume(float volume)
    {
        // TODO: set volume to all playing tracker samples from this asset
    }

    void AudioAsset::fadeOut(unsigned int fadeOutTime)
    {
        // TODO: fade all playing tracker samples from this asset
    }
}
