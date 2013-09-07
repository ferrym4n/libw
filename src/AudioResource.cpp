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

#include "AudioResource.hpp"
#include <w/Log.hpp>
#include <w/Exception.hpp>
#include <string.h>

namespace w
{
    static unsigned getUnsigned(const unsigned char buf[])
    {
        return buf[0] + (buf[1] * 0x100) + (buf[2] * 0x10000) + (buf[3] * 0x1000000);
    }

    static unsigned getUnsignedShort(const unsigned char buf[])
    {
        return buf[0] + (buf[1] * 0x100);
    }

    AudioResource::AudioResource(const std::string& file):
        Resource(file),
        channels_(false),
        bit_(0),
        bytesPerSample_(0),
        rate_(0),
        sizeInBytes_(0),
        isSigned_(false),
        data_(NULL)
    {
        load(file);
    }

    AudioResource::~AudioResource()
    {
    }

    unsigned int AudioResource::channels() const
    {
        return channels_;
    }

    unsigned int AudioResource::bitPerSample(void) const
    {
        return bit_;
    }

    unsigned int AudioResource::bytesPerSample(void) const
    {
        return bytesPerSample_;
    }

    int AudioResource::playBackRate(void) const
    {
        return rate_;
    }

    unsigned int AudioResource::sizeInBytes(void) const
    {
        return sizeInBytes_;
    }

    bool AudioResource::isSigned(void) const
    {
        return isSigned_;
    }

    unsigned char const* AudioResource::data(void) const
    {
        return data_;
    }

    int16_t AudioResource::sample(unsigned int location, bool& end) const
    {
        int16_t r = 0;

        if (location >= (sizeInBytes_ - bytesPerSample_))
        {
            end = true;
        }
        else
        {
            // TODO: check this cast
            r = *(int*)(&data_[location]);
        }
        return r;
    }

    void AudioResource::load(const std::string& file)
    {
        FILE* fp = NULL;

        LOGI("Loading \"%s\"\n", file.c_str());

        fp = fopen(file.c_str(), "rb");
        if (fp != NULL)
        {
            unsigned char buf[256] = {0};
            unsigned int l = 0;

            // Header
            if (fread(buf, 1, 4, fp) != 4)
            {
                LOGE("Error in reading RIFF.\n");
                goto error;
            }

            if (strncmp((char*)buf, "RIFF", 4) != 0)
            {
                LOGI("RIFF not found.\n");
            }

            if (fread(buf, 1, 4, fp) != 4)
            {
                LOGE("Error in reading file size.\n");
                goto error;
            }

            if (fread(buf, 1, 8, fp) != 8)
            {
                LOGE("WAVEfmt could not be read.\n");
                goto error;
            }

            if (strncmp((char*)buf, "WAVEfmt", 7) != 0)
            {
                LOGI("WAVEfmt not found\n");
            }

            if (fread(buf, 1, 4, fp) != 4)
            {
                LOGE("Header size could not be read.\n");
                goto error;
            }
            unsigned int headerSize = getUnsigned(buf);

            // WORD  formatTag;
            // WORD  channels;
            // DWORD samplesPerSecond;
            // DWORD averageBytesPerSecond;
            // WORD  blockAlign;
            // WORD  bitsPerSample;
            // WORD  cbSize;
            if (fread(buf, 1, headerSize, fp) != headerSize)
            {
                LOGE("Error in reading header.\n");
                goto error;
            }

            unsigned short channels = getUnsignedShort(buf + 2);
            unsigned int samplesPerSecond = getUnsigned(buf + 4);
            unsigned short bitsPerSample = (headerSize >= 16 ? getUnsignedShort(buf + 14) : 0);

            /*LOGD("formatTag=%d\n", formatTag);
            LOGD("channels=%d\n", channels);
            LOGD("samplesPerSecond=%d\n", samplesPerSecond);
            LOGD("averageBytesPerSecond=%d\n", averageBytesPerSecond);
            LOGD("blockAlign=%d\n", blockAlign);
            LOGD("bitsPerSample=%d\n", bitsPerSample);
            LOGD("cbSize=%d\n", cbSize);*/

            // Data
            while (1)
            {
                if (fread(buf,1,4,fp) != 4)
                {
                    printf("Error while waiting for data.\n");
                    goto error;
                }

                if ((buf[0]=='d' || buf[0]=='D') && (buf[1]=='a' || buf[1]=='A') &&
                    (buf[2]=='t' || buf[2]=='T') && (buf[3]=='a' ||  buf[3]=='A'))
                {
                    // "DATA" block found
                    break;
                }
                else
                {
                    LOGI("Skipping block \"%c%c%c%c\"\n", buf[0], buf[1], buf[2], buf[3]);

                    if (fread(buf, 1, 4, fp)!=4)
                    {
                        LOGE("Skipping unknown block failed!\n");
                        goto error;
                    }

                    l = getUnsigned(buf);
                    if (fread(buf, 1, l, fp) != l)
                    {
                        LOGE("Skipping unknown block failed!\n");
                        goto error;
                    }
                }
            }

            if (fread(buf, 1, 4, fp) != 4)
            {
                LOGE("Data size could not be read.\n");
                goto error;
            }
            unsigned int dataSize = getUnsigned(buf);

            if (data_ != NULL)
            {
                LOGI("Replacing earlier data\n");
                delete data_;
                data_ = NULL;
            }

            data_ = new unsigned char [dataSize];
            if ((l = fread(data_, 1, dataSize, fp)) != dataSize)
            {
                LOGI("File ended before reading all data.\n");
                LOGI("%d (0x%x) bytes have been read.\n", l, l);
            }

            channels_ = channels;
            bit_ = bitsPerSample;
            bytesPerSample_ = bitsPerSample >> 3;
            sizeInBytes_ = dataSize;
            rate_ = samplesPerSecond;

            if (bitsPerSample == 8)
            {
                isSigned_ = false;
            }
            else
            {
                isSigned_= true;
            }

            fclose(fp);
            return;
        }
        else
        {
            LOGE("No file:%s\n", file.c_str());
        }
        return;

    error:
        if (fp != NULL)
        {
            fclose(fp);
        }
        throw Exception("Could not load audio asset!\n");
    }
}
