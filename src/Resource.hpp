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

#ifndef LIBW_RESOURCE
#define LIBW_RESOURCE

#include "Referenced.hpp"
#include <string>

namespace w
{
    class Resource: public Referenced
    {
    public:
        Resource(const std::string& filename):
            Referenced(),
            filename_(filename)
        {
        }

        virtual ~Resource()
        {
        }

        const std::string& filename()  const
        {
            return filename_;
        }

        std::string& Resource::basePath()
        {
            char const* r = 0;

            #ifdef __APPLE__
                NSBundle *b = [NSBundle mainBundle];
                NSString *dir = [b resourcePath];
                r = [dir UTF8String];
            #elif __linux__
                r = ".";
            #endif

            return std::string(r) + std::string("/");
        }

    private:
        std::string filename_;
    };
}

#endif
