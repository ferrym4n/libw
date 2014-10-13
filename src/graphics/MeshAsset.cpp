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

#include "w/graphics/MeshAsset.hpp"
#include "MeshAssetPrivate.hpp"

namespace w
{
    namespace graphics
    {
        MeshAsset::MeshAsset(const std::vector<StrideComponent> & strideComponents, float* vertexData, unsigned int vertexCount):
            private_(new MeshAssetPrivate(strideComponents, vertexData, vertexCount))
        {
            private_->increment();
        }

        MeshAsset::MeshAsset(float width, float height, float uStart, float uEnd, float vStart, float vEnd, float wOffset, float hOffset):
            private_(new MeshAssetPrivate(width, height, uStart, uEnd, vStart, vEnd, wOffset, hOffset))
        {
            private_->increment();
        }

        MeshAsset::MeshAsset(MeshAsset const& r):
            private_(r.private_)
        {
            private_->increment();
        }

        MeshAsset::~MeshAsset()
        {
            private_->decrement();
        }

        MeshAsset& MeshAsset::operator=(MeshAsset const& r)
        {
            if (this != &r)
            {
                private_ = r.private_;
                private_->increment();
            }
            return *this;
        }

        void MeshAsset::setData(const std::vector<StrideComponent> & strideComponents, float* vertexData, unsigned int vertexCount)
        {
            private_->setData(strideComponents, vertexData, vertexCount);
        }
    }
}
