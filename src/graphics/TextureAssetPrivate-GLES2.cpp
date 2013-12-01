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

#include "TextureAssetPrivate.hpp"
#include "w/base/UniquePointer.hpp"
#include <w/base/Log.hpp>
#include <w/base/Exception.hpp>
#include "w/base/ResourceManagerPrivate.hpp"
#include <memory>

namespace w
{
    // Callback function for libpng
    static void read(png_structp p, png_bytep data, png_size_t length);

    TextureAssetPrivate::TextureAssetPrivate(const std::string& file):
        Resource(file),
        bytesPerPixel_(0),
        width_(0),
        height_(0),
        tmpData_(NULL)
    {
        loadFileData();
    }

    TextureAssetPrivate::~TextureAssetPrivate()
    {
        if (tmpData_ != NULL)
        {
            delete [] tmpData_;
        }
    }

    void TextureAssetPrivate::loadFileData()
    {
        static const int flags = PNG_TRANSFORM_STRIP_16 |
        PNG_TRANSFORM_GRAY_TO_RGB | PNG_TRANSFORM_PACKING |
        PNG_TRANSFORM_EXPAND;

        // Create libpng structs
        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        if (!png)
        {
            LOGE("Cannot open file %.!", filename().c_str());
            throw Exception("Couldn't create libpng read struct.");
        }

        png_infop info = png_create_info_struct(png);
        if (!info)
        {
            LOGE("Cannot open file %.!", filename().c_str());
            throw Exception("Couldn't create libpng info struct.");
        }

        // Errors, TODO: check if this is needed.
        if (setjmp(png_jmpbuf(png)))
        {
            LOGE("libpng error while reading file %s.", filename().c_str());
            throw Exception("Could not load png file!");
        }

        // Start reading the file
        UniquePointer<FileHandle> fileHandle(ResourceManagerPrivate::getFileHandle(filename()));

        // Read the image header and data
        png_set_read_fn(png, reinterpret_cast<void*>(fileHandle.pointer()), read);
        png_read_png(png, info, flags, 0);
        png_bytepp rows = png_get_rows(png, info);

        // Set pixel format
        if (png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB)
        {
            bytesPerPixel_ = 3;
        }
        else if (png_get_color_type(png, info) == PNG_COLOR_TYPE_RGBA)
        {
            bytesPerPixel_ = 4;
        }
        else
        {
            LOGE("Cannot open file %.!", filename().c_str());
            throw Exception("libpng unsupported color type.");
        }

        // Copy to continous memory
        width_ = png_get_image_width(png, info);
        height_ = png_get_image_height(png, info);
        tmpData_ = new char[width_ * height_ * bytesPerPixel_];
        for (unsigned int i = 0; i < height_; i++)
        {
            memcpy(&(tmpData_)[bytesPerPixel_ * width_ * i], rows[height_ - i - 1], width_ * bytesPerPixel_);
        }
    }

    void TextureAssetPrivate::bind()
    {
        loadGPUData();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId_);
    }

    void TextureAssetPrivate::loadGPUData()
    {
        LOCK

        if (tmpData_ == NULL)
        {
            return;
        }

        // Create texture
        GLenum format = bytesPerPixel_ == 3 ? GL_RGB : GL_RGBA;
        GLint minFilter = GL_LINEAR_MIPMAP_NEAREST;
        GLint magFilter = GL_LINEAR;

        glGenTextures(1, &textureId_);
        glBindTexture(GL_TEXTURE_2D, textureId_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D,
            0,
            format,
            width_,
            height_,
            0,
            format,
            GL_UNSIGNED_BYTE,
            tmpData_);

        if ((minFilter != GL_NEAREST && minFilter != GL_LINEAR) || (magFilter != GL_NEAREST && magFilter != GL_LINEAR))
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            LOGE("Error in texture filters!\n");
        }

        delete [] tmpData_;
        tmpData_ = NULL;
        LOGD("TextureAssetPrivate(), created with id:%d, size: %d x %d, bpp: %d", textureId_, width_, height_, bytesPerPixel_);
    }

    void read(png_structp fileHandlePointer, png_bytep data, png_size_t length)
    {
        FileHandle* fileHandle = reinterpret_cast<FileHandle*>(png_get_io_ptr(fileHandlePointer));
        fileHandle->read(reinterpret_cast<char*>(data), length);
    }
}
