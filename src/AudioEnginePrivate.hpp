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

#ifndef LIBW_AUDIOENGINEPRIVATE
#define LIBW_AUDIOENGINEPRIVATE

#include "Mutex.hpp"
#include "AudioResourceManager.hpp"
#include "Tracker.hpp"
#include "TrackerSample.hpp"
#include <w/Class.hpp>
#include <string>
#ifdef __linux__
    #include <pulse/pulseaudio.h>
#elif __ANDROID__
#elif __APPLE__
    #import <AudioToolbox/AudioToolbox.h>
    #import <AVFoundation/AVFoundation.h>
#endif

namespace w
{
    class AudioEnginePrivate
    {
    public:
        UNCOPYABLE(AudioEnginePrivate)
        friend class AudioEngine;
#if __APPLE__
        constexpr static float const VolumeOffThreshold = 0.001f;
#else
        static float const VolumeOffThreshold = 0.001f;
#endif
        
        struct State
        {
            enum Enum
            {
                Start,
                Starting,
                Run,
                ShuttingDown,
                End
            };
        };

        static State::Enum state();
        static void setVolume(float volume);
        static float volume();
        static AudioResource* get(const std::string& file);
        static bool play(AudioResource* resource, bool volume, bool looping);

#ifdef __linux__
        void writeCallback(size_t size);
#elif __ANDROID__

#elif __APPLE__
        void writeCallback(size_t size, SInt16* targetBuffer);
#endif

    private:
        AudioEnginePrivate(float volumeAtStart, const std::string& assetPath);
        ~AudioEnginePrivate();
        static AudioEnginePrivate* singleton_;
        AudioResourceManager audioResourceManager_;
        Tracker tracker_;
        State::Enum state_;
        Mutex mutex_;
        float volumeAtStart_;

        #ifdef __ANDROID__
            //
        #elif __APPLE__
            void setupAudioUnitSession();
            void setupAudioGraph(UInt32 busCount);
            void audioRouteChangeListenerCallback (void* inUserData,
                AudioSessionPropertyID inPropertyID, UInt32 inPropertyValueSize,
                const void* inPropertyValue);
            AudioStreamBasicDescription streamFormat_;
            AudioComponentInstance ioUnit_;
        #elif __linux__
            // pulse
            void configureStream();
            pa_threaded_mainloop* mainloop_;
            pa_context* context_;
            pa_stream* stream_;
        #endif
    };
}

#endif
