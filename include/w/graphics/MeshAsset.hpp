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

#ifndef LIBW_GRAPHICS_MESHASSET
#define LIBW_GRAPHICS_MESHASSET

#include <w/base/Class.hpp>

namespace w
{
    class MeshAsset
    {
    public:
        COPYABLE(MeshAsset)

        /**
         * Creates a rectangular mesh.
         *
         * @note You must have GL context to create this class!
         *
         * @param [in]  w       Width of the created mesh
         * @param [in]  h       Height of the created mesh
         * @param [in]  uStart  Texture u coodinate left
         * @param [in]  uEnd    Texture u coodinate right
         * @param [in]  vStart  Texture v coodinate bottom
         * @param [in]  vEnd    Texture u coodinate ceiling
         */
        MeshAsset(float w, float h, float uStart, float uEnd, float vStart, float vEnd);
        ~MeshAsset();
        float width() const;
        float height() const;

    private:
        friend class RendererPrivate;
        class MeshAssetPrivate* private_;
    };
}

#endif
