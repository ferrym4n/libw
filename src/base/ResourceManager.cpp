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

#include "w/base/ResourceManager.hpp"
#include "w/base/ResourceManagerPrivate.hpp"
#include <string>

namespace w
{
    #ifdef ANDROID
        ResourceManager::ResourceManager(AAssetManager* androidAssetManager):
            private_(new ResourceManagerPrivate("."))
        {
            private_->setAndroidAssetManager(androidAssetManager);
        }
    #elif linux
        ResourceManager::ResourceManager(const std::string& basePath):
            private_(new ResourceManagerPrivate(basePath))
        {
        }
    #elif __APPLE__
        ResourceManager::ResourceManager():
            private_(new ResourceManagerPrivate("."))
        {
        }
    #endif

    ResourceManager::ResourceManager(ResourceManager const& r):
        private_(r.private_)
    {
        private_->increment();
    }

    ResourceManager::~ResourceManager()
    {
        private_->decrement();
    }

    ResourceManager& ResourceManager::operator=(ResourceManager const& r)
    {
        if (this != &r)
        {
            private_ = r.private_;
            private_->increment();
        }
        return *this;
    }
}
