/**
 * libw
 *
 * Copyright (C) 2012-2014 Woima Solutions
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

#include "w/storage/Storage.hpp"
#include "StoragePrivate.hpp"
#include <w/base/Exception.hpp>

namespace w
{
    StoragePrivate* Storage::singleton_ = NULL;

    Storage::Storage(std::string const& id)
    {
        if (singleton_ == NULL)
        {
            singleton_ = new StoragePrivate(id);
            try
            {
                singleton_->load();
            }
            catch (Exception& e)
            {
                LOGE("Could not load Storage:\n%s\n", e.what());
            }
        }
        else
        {
            throw Exception("Only one Storage can exist in one application.");
        }
    }

    Storage::~Storage()
    {
        if (singleton_ != NULL)
        {
            delete singleton_;
        }
        singleton_ = NULL;
    }

    bool Storage::hasInt(const std::string& id)
    {
        bool r = 0;
        if (singleton_ == NULL)
        {
            throw Exception("Create Storage singleton!");
        }
        r = singleton_->hasInt(id);
        return r;
    }

    void Storage::setInt(const std::string& id, int value)
    {
        if (singleton_ == NULL)
        {
            throw Exception("Create Storage singleton!");
        }
        singleton_->setInt(id, value);
        singleton_->save();
    }

    int Storage::getInt(const std::string& id)
    {
        if (singleton_ == NULL)
        {
            throw Exception("Create Storage singleton!");
        }
        int r = singleton_->getInt(id);

        return r;
    }

    bool Storage::hasString(const std::string& id)
    {
        bool r = 0;
        if (singleton_ == NULL)
        {
            throw Exception("Create Storage singleton!");
        }
        r = singleton_->hasString(id);
        return r;
    }

    void Storage::setString(const std::string& id, const std::string& value)
    {
        if (singleton_ == NULL)
        {
            throw Exception("Create Storage singleton!");
        }
        singleton_->setString(id, value);
        singleton_->save();
    }

    std::string Storage::getString(const std::string& id)
    {
        if (singleton_ == NULL)
        {
            throw Exception("Create Storage singleton!");
        }
        return singleton_->getString(id);
    }

    std::string Storage::getString(const std::string& id, const std::string& defaultValue)
    {
        if (singleton_ == NULL)
        {
            throw Exception("Create Storage singleton!");
        }
        return singleton_->getString(id, defaultValue);
    }
    
    void Storage::remove(const std::string &key)
    {
        if (singleton_ == NULL)
        {
            throw Exception("Create Storage singleton!");
        }
        singleton_->remove(key);
    }
}
