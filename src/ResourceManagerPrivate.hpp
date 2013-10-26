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

#ifndef LIBW_RESOURCEMANAGERPRIVATE
#define LIBW_RESOURCEMANAGERPRIVATE

#include "Resource.hpp"
#include "Referenced.hpp"
#include "Mutex.hpp"
#include "FileHandle.hpp"
#include <map>
#include <string>
#include <sigc++/connection.h>
#include <vector>
#ifdef ANDROID
    #include <jni.h>
    #include <sys/types.h>
    #include <android/asset_manager.h>
    #include <android/asset_manager_jni.h>
#endif

namespace w
{
    class ResourceManagerPrivate: public Referenced
    {
    public:
        ~ResourceManagerPrivate();
        static Resource* getResource(const std::string& id);
        static void setResource(const std::string& id, Resource* resource);
        static FileHandle* getFileHandle(const std::string& filename);
        #ifdef ANDROID
            static AAssetManager* androidAssetManager();
            static void setAndroidAssetManager(AAssetManager* androidAssetManager);
        #endif

    private:
        friend class ResourceManager;
        ResourceManagerPrivate(const std::string & basePath);
        void handleResourceDestroy(unsigned int);
        static ResourceManagerPrivate* singleton_;
        std::string basePath_;
        Mutex mutex_;
        std::map<std::string, Resource*> resources_;

        // resourceConnections_ note:
        // We don't hold reference count in ResourceManager
        // so we delete the listening connection when resource
        // is freed by the user.
        std::map<unsigned int, sigc::connection> resourceConnections_;
        #ifdef ANDROID
            static AAssetManager* androidAssetManager_;
        #endif
    };
}

#endif
