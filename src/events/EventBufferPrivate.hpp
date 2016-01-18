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

#ifndef LIBW_EVENTS_EVENTBUFFERPRIVATE
#define LIBW_EVENTS_EVENTBUFFERPRIVATE

#include <w/base/Class.hpp>
#include <w/base/Referenced.hpp>
#include <w/base/Mutex.hpp>
#include <w/graphics/Window.hpp>
#include "w/events/Event.hpp"
#if defined(__linux__) && !defined(__ANDROID__)
    #include <X11/keysym.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
#endif
#include <list>
#include <map>

namespace w
{
    namespace events
    {
        class EventBufferPrivate: public Referenced
        {
        public:
            UNCOPYABLE(EventBufferPrivate)

            EventBufferPrivate(graphics::Window const& window);
            void add(Event* event);
            Event* pop();

        private:
            Mutex mutex_;
            std::list<Event*> events_;

            #if defined(__linux__) && !defined(__ANDROID__)
                void pollXEvent();
                Display* xDisplay_;
            #endif
        };
    }
}

#endif
