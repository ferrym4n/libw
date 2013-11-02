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

#include "w/base/ResourceManagerPrivate.hpp"
#include "w/base/File.hpp"
#include "w/base/Lock.hpp"
#include <w/base/Exception.hpp>
#include <w/base/Log.hpp>
#include <utility> // std::pair
#include <functional>
using std::make_pair;

namespace w
{
    ResourceManagerPrivate* ResourceManagerPrivate::singleton_ = NULL;
    #ifdef ANDROID
        AAssetManager* ResourceManagerPrivate::androidAssetManager_ = NULL;
    #endif

    ResourceManagerPrivate::ResourceManagerPrivate(const std::string& basePath):
        basePath_(basePath)
    {
        if (singleton_ != NULL)
        {
            LOGE("ResourceManagerPrivate (==ResourceManager in public API!) exists already.");
            throw Exception("ResourceManagerPrivate exists already.");
        }
        singleton_ = this;
    }

    ResourceManagerPrivate::~ResourceManagerPrivate()
    {
        if (singleton_->resources_.size() == 0)
        {
            LOGI("ResourceManager: all freed.");
        }
        else
        {
            LOGI("ResourceManager: existing references:");
            std::map<std::string, Resource*>::iterator i;
            for (i = singleton_->resources_.begin(); i != singleton_->resources_.end(); i++)
            {
                std::string id = i->first;
                Resource* resource = i->second;
                LOGI(" -(id:%s)(refcount:%d)", id.c_str(), resource->referenceCount());
            }
        }
        singleton_ = NULL;
    }

    Resource* ResourceManagerPrivate::getResource(const std::string& id)
    {
        if (singleton_ == NULL)
        {
            throw Exception("ResourceManager needed to load resources.");
        }

        Lock lock(singleton_->mutex_);

        Resource* r = NULL;
        std::string key(id);
        std::map<std::string, Resource*>::const_iterator i = singleton_->resources_.find(key);
        if (i != singleton_->resources_.end())
        {
            r = i->second;
        }
        return r;
    }

    void ResourceManagerPrivate::setResource(const std::string& id, Resource* resource)
    {
        Lock lock(singleton_->mutex_);
        std::pair<std::string, Resource*> tmp0 = std::make_pair(id, resource);
        singleton_->resources_.insert(tmp0);

        sigc::connection connection = resource->destroy.connect(
            sigc::mem_fun(singleton_, &ResourceManagerPrivate::handleResourceDestroy));

        std::pair<unsigned int, sigc::connection> tmp1 = std::make_pair(resource->id(), connection);
        singleton_->resourceConnections_.insert(tmp1);

    }

    void ResourceManagerPrivate::handleResourceDestroy(unsigned int id)
    {
        Lock lock(singleton_->mutex_);

        // Remove Resource from loaded resources list and signal listener list
        std::map<std::string, Resource*>::iterator i;
        for (i = singleton_->resources_.begin(); i != singleton_->resources_.end(); i++)
        {
            Resource* loadedResource = i->second;
            if (loadedResource->id() == id)
            {
                // Remove Resource's destroy signal connection
                std::map<unsigned int, sigc::connection>::iterator i2;
                i2 = singleton_->resourceConnections_.find(id);
                singleton_->resourceConnections_.erase(i2);

                // Resource will delete itself, look Referenced.hpp
                singleton_->resources_.erase(i);

                break;
            }
        }
    }

    FileHandle* ResourceManagerPrivate::getFileHandle(const std::string& filename)
    {
        #ifdef ANDROID
            return new FileHandle(filename, androidAssetManager_);
        #elif __linux__
            return new FileHandle(singleton_->basePath_ + "/" + filename);
        #elif __APPLE__
            NSBundle *b = [NSBundle mainBundle];
            NSString *dir = [b resourcePath];
            char const* tmp = [dir UTF8String];
            return new FileHandle(std::string(tmp) + "/" + filename);
        #endif
    }

    #ifdef ANDROID
        AAssetManager* ResourceManagerPrivate::androidAssetManager()
        {
            if (androidAssetManager_ == NULL)
            {
                throw Exception("androidAssetManager not set.");
            }
            return androidAssetManager_;
        }

        void ResourceManagerPrivate::setAndroidAssetManager(AAssetManager* assetManager)
        {
            androidAssetManager_ = assetManager;
        }
    #endif
}