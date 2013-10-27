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

#include "ShaderProgramAssetPrivate.hpp"
#include "FileHandle.hpp"
#include "ResourceManagerPrivate.hpp"
#include "w/Log.hpp"
#include "UniquePointer.hpp"

namespace w
{
    static void checkGlError(const char* op)
    {
        for (GLint error = glGetError(); error; error = glGetError())
        {
            LOGE("after %s() glError (0x%x)\n", op, error);
        }
    }

    ShaderProgramAssetPrivate::ShaderProgramAssetPrivate(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename):
        Referenced(),
        programId_(0)
    {
        // Vertex
        UniquePointer<FileHandle> vertexFileHandle(ResourceManagerPrivate::getFileHandle(vertexShaderFilename));
        unsigned int vertexSourceLength = vertexFileHandle.pointer()->byteSize();
        UniquePointer<char> vertexSourceBuffer(new char[vertexSourceLength +1], true);
        vertexFileHandle.pointer()->read(vertexSourceBuffer.pointer(), vertexSourceLength);
        vertexSourceBuffer.pointer()[vertexSourceLength] = 0;

        // Fragment
        UniquePointer<FileHandle> fragmentFileHandle(ResourceManagerPrivate::getFileHandle(fragmentShaderFilename));
        unsigned int fragmentSourceLength = fragmentFileHandle.pointer()->byteSize();
        UniquePointer<char> fragmentSourceBuffer(new char[fragmentSourceLength +1], true);
        fragmentFileHandle.pointer()->read(fragmentSourceBuffer.pointer(), fragmentSourceLength);
        fragmentSourceBuffer.pointer()[fragmentSourceLength] = 0;

        // Compile to program
        programId_ = createProgram(vertexSourceBuffer.pointer(), fragmentSourceBuffer.pointer());
    }

    ShaderProgramAssetPrivate::~ShaderProgramAssetPrivate()
    {
    }

    GLint ShaderProgramAssetPrivate::uniform(const std::string& symbolName)
    {
        GLint location = glGetUniformLocation(programId_, symbolName.c_str());
        if (location < 0)
        {
            LOGE("ShaderProgramAssetPrivate::uniform(), no symbol: \"%s\"", symbolName.c_str());
            throw Exception("Failed to get uniform location");
        }
    }

    GLuint ShaderProgramAssetPrivate::createShader(GLenum shaderType, const char* pSource)
    {
        GLuint shader = glCreateShader(shaderType);
        if (shader)
        {
            glShaderSource(shader, 1, &pSource, NULL);
            glCompileShader(shader);
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled)
            {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen)
                {
                    char* buf = new char[infoLen];
                    if (buf)
                    {
                        glGetShaderInfoLog(shader, infoLen, NULL, buf);
                        LOGE("Could not compile shader %d: %s", shaderType, buf);
                        delete [] buf;
                    }
                    glDeleteShader(shader);
                    shader = 0;
                }
            }
        }
        return shader;
    }

    GLuint ShaderProgramAssetPrivate::createProgram(const char* vertexSource, const char* fragmentSource)
    {
        GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexSource);
        if (!vertexShader)
        {
            return 0;
        }

        GLuint pixelShader = createShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (!pixelShader)
        {
            return 0;
        }

        GLuint program = glCreateProgram();
        if (program)
        {
            glAttachShader(program, vertexShader);
            checkGlError("glAttachShader");
            glAttachShader(program, pixelShader);
            checkGlError("glAttachShader");
            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE)
            {
                GLint bufLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
                if (bufLength)
                {
                    char* buf = new char[bufLength];
                    if (buf)
                    {
                        glGetProgramInfoLog(program, bufLength, NULL, buf);
                        LOGE("Could not link program:\n%s\n", buf);
                        delete [] buf;
                    }
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }
}
