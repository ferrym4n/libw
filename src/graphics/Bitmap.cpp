/**
 * libw
 *
 * Copyright (C) 2013-2015 Woima Solutions
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

#include "w/graphics/Bitmap.hpp"
#include "BitmapPrivate.hpp"

namespace w
{
    namespace graphics
    {
        Bitmap::Bitmap(unsigned int width, unsigned int height, Bitmap::Format::Enum format):
            ReferencedPointer<BitmapPrivate>(new BitmapPrivate(width, height, format))
        {
        }
        
        Bitmap::Bitmap(const std::string & file):
            ReferencedPointer<BitmapPrivate>(new BitmapPrivate(file))
        {
        }
        
        Bitmap::Bitmap(Bitmap const& r):
            ReferencedPointer<BitmapPrivate>(r.pointer())
        {
        }

        Bitmap::~Bitmap()
        {
        }
        
        Bitmap& Bitmap::operator=(Bitmap const& r)
        {
            ReferencedPointer::operator=(r);
            return *this;
        }

        unsigned int Bitmap::width() const
        {
            return pointer()->width();
        }
        
        unsigned int Bitmap::height() const
        {
            return pointer()->height();
        }
        
        Bitmap::Format::Enum Bitmap::format() const
        {
            return pointer()->format();
        }

        char* Bitmap::data() const
        {
            return pointer()->data();
        }
    }
}
