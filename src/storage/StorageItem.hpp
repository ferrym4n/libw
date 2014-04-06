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

#ifndef LIBW_STORAGE_STORAGEITEM
#define LIBW_STORAGE_STORAGEITEM

#include <w/base/Exception.hpp>
#include <w/base/Log.hpp>
#include <string>

namespace w
{
    namespace StorageItemConst
    {
        static const char Delimiter = ',';
        static const std::string Int = std::string("Int");
        static const std::string String = std::string("String");
        static const std::string Undefined = std::string("Undefined");
    };

    class StorageItem
    {
    public:
        struct Type
        {
            enum Enum
            {
                Undefined = 0,
                Int = 1,
                String = 2
            };
        };

        static std::string typeToString(Type::Enum type)
        {
            if(type == Type::Int)
            {
                return StorageItemConst::Int;
            }
            if(type == Type::String)
            {
                return StorageItemConst::String;
            }
            else
            {
                return StorageItemConst::Undefined;
            }
        }

        StorageItem(StorageItem::Type::Enum type, const std::string& key, const std::string& value):
            type_(type),
            key_(key),
            value_(value)
        {
            LOGD("Creating StorageItem: (%s, %s, %s)", typeToString(type).c_str(), key_.c_str(), value_.c_str());

            // Check that key is valid
            if(key_.length() < 1)
            {
                throw Exception("StrorageItem key length must be > 0");
            }
        }

        ~StorageItem()
        {
        }

        Type::Enum type()
        {
            return type_;
        }

        std::string key()
        {
            return key_;
        }

        std::string value()
        {
            return value_;
        }

        void setValue(const std::string & value)
        {
            value_ = value;
        }

        std::string serialize()
        {
            std::string r;

            if(type() == StorageItem::Type::Int)
            {
                r += StorageItemConst::Int;
            }
            else if(type() == StorageItem::Type::String)
            {
                r += StorageItemConst::String;
            }
            else
            {
                throw Exception("StoragePrivate::serialize() Unknown type.");
            }

            r += StorageItemConst::Delimiter;
            r += key();
            r += StorageItemConst::Delimiter;
            r += value();

            LOGD("StorageItem::serialized value: %s", value().c_str());

            return r;
        }

        static StorageItem* deserialize(const std::string& itemData)
        {
            StorageItem* r = 0;
            LOGD("StorageItem::deserialize: <%s>", itemData.c_str());

            // Check type
            std::string tmpInt = itemData.substr(0, StorageItemConst::Int.size());
            std::string tmpString = itemData.substr(0, StorageItemConst::String.size());
            if(StorageItemConst::Int.compare(tmpInt) == 0)
            {
                // Key
                std::string tmp = itemData.substr(StorageItemConst::Int.size() + 1, itemData.length());
                unsigned int keyEndIndex = tmp.find(StorageItemConst::Delimiter);
                std::string key = tmp.substr(0, keyEndIndex);

                // Data
                std::string value = tmp.substr(keyEndIndex + 1, tmp.size());

                r = new StorageItem(StorageItem::Type::Int, key, value);
            }
            else if(StorageItemConst::String.compare(tmpString) == 0)
            {
                // Key
                std::string tmp = itemData.substr(StorageItemConst::String.size() + 1, itemData.length());
                unsigned int keyEndIndex = tmp.find(StorageItemConst::Delimiter);
                std::string key = tmp.substr(0, keyEndIndex);

                // Data
                std::string value = tmp.substr(keyEndIndex + 1, tmp.size());

                r = new StorageItem(StorageItem::Type::String, key, value);
            }
            else
            {
                throw Exception("Storage deserialize(), unknown item type");
            }

            return r;
        }

        private:
            StorageItem::Type::Enum type_;
            std::string key_;
            std::string value_;
    };
}

#endif
