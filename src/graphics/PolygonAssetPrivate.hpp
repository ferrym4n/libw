/**
 * libw
 *
 * Copyright (C) 2013-2014 Woima Solutions
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

#ifndef LIBW_GRAPHICS_POLYGONASSETPRIVATE
#define LIBW_GRAPHICS_POLYGONASSETPRIVATE

#include "w/graphics/Stride.hpp"
#include "w/graphics/PolygonAsset.hpp"
#include <w/base/Referenced.hpp>
#include <w/base/Class.hpp>
#include <w/math/Eigen.hpp>
#ifdef __linux__ // & Android
    #include <GLES2/gl2.h>
#else // APPLE
    #include <OpenGLES/ES2/gl.h>
#endif
#include <string>
#include <vector>

namespace w
{
    namespace graphics
    {
        class PolygonAssetPrivate: public Referenced
        {
        public:
            UNCOPYABLE(PolygonAssetPrivate)
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW

            PolygonAssetPrivate(const std::vector<PolygonAsset::Point> & data,
                const Eigen::Vector4f & color);
            virtual ~PolygonAssetPrivate();

            unsigned int pointCount() const;
            const std::vector<StrideComponent>& strideComponents() const;
            Eigen::Vector4f color();

            // Android, linux, iOS. All use GLES2
            void bind();

        private:
            Mutex mutex_;
            void loadGPUData();
            Eigen::Vector4f color_;
            std::vector<PolygonAsset::Point>* tmpData_;
            unsigned int dataSize_;

            // Android, linux, iOS. All use GLES2
            GLuint vbo_;
            std::vector<StrideComponent> strideComponents_;
            unsigned int pointCount_;
        };
    }
}

#endif
