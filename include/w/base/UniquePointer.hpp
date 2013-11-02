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

#ifndef LIBW_UNIQUEPOINTER
#define LIBW_UNIQUEPOINTER

#include <w/base/Class.hpp>

namespace w
{
    /**
    * @class UniquePointer
    *
    * Class that holds reference to a pointer within lifetime.
    */
    template <class T> class UniquePointer
    {
    public:
        UNCOPYABLE(UniquePointer);

        UniquePointer(T* instance, bool isArray = false):
            instance_(instance),
            isArray_(isArray)
        {
        }

        ~UniquePointer()
        {
            if (instance_ != NULL)
            {
                if (isArray_ == false)
                {
                    delete instance_;
                }
                else
                {
                    delete [] instance_;
                }
                instance_ = NULL;
            }
        }

        T* pointer() const
        {
            return instance_;
        }

    private:
        T* instance_;
        bool isArray_;
    };
}

#endif
