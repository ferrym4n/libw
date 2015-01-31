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

#include "w/graphics/Stride.hpp"
#include "RendererPrivate.hpp"
#include "TextureAssetPrivate.hpp"
#include "MeshAssetPrivate.hpp"
#include "PolygonAssetPrivate.hpp"
#include "FrameBufferPrivate.hpp"
#include "ShaderProgramAssetPrivate.hpp"
#include "w/graphics/MeshAsset.hpp"
#include "w/base/Log.hpp"
#include <vector>
#include <string>

namespace w
{
    namespace graphics
    {
        #define BUFFER_OFFSET(i) ((char*)NULL + (i))

        RendererPrivate::RendererPrivate():
            Referenced()
        {
        }

        RendererPrivate::~RendererPrivate()
        {
        }

        void RendererPrivate::setBlend(bool value)
        {
            if(value == true)
            {
                glEnable(GL_BLEND);
            }
            else
            {
                glDisable(GL_BLEND);
            }
        }

        void RendererPrivate::setBlendFunction(Renderer::BlendMode::Enum source, Renderer::BlendMode::Enum destination)
        {
            GLenum s;
            GLenum d;
            switch(source)
            {
                case Renderer::BlendMode::One:
                {
                    s = GL_ONE;
                    break;
                }
                case Renderer::BlendMode::SrcAlpha:
                {
                    s = GL_SRC_ALPHA;
                    break;
                }
                case Renderer::BlendMode::OneMinusSrcAlpha:
                {
                    s = GL_ONE_MINUS_SRC_ALPHA;
                }
            }
            switch(destination)
            {
                case Renderer::BlendMode::One:
                {
                    d = GL_ONE;
                    break;
                }
                case Renderer::BlendMode::SrcAlpha:
                {
                    d = GL_SRC_ALPHA;
                    break;
                }
                case Renderer::BlendMode::OneMinusSrcAlpha:
                {
                    d = GL_ONE_MINUS_SRC_ALPHA;
                }
            }
            glBlendFunc(s, d);
        }

        void RendererPrivate::setBlendFunctionSeparate(Renderer::BlendMode::Enum sourceRGB,
                Renderer::BlendMode::Enum destinationRGB,
                Renderer::BlendMode::Enum sourceAlpha,
                Renderer::BlendMode::Enum destinationAlpha)
        {
            GLenum sRGB = GL_ONE;
            GLenum dRGB = GL_ONE;
            GLenum sA = GL_ONE;
            GLenum dA = GL_ONE;
            switch(sourceRGB)
            {
                case Renderer::BlendMode::One:
                {
                    sRGB = GL_ONE;
                    break;
                }
                case Renderer::BlendMode::SrcAlpha:
                {
                    sRGB = GL_SRC_ALPHA;
                    break;
                }
                case Renderer::BlendMode::OneMinusSrcAlpha:
                {
                    sRGB = GL_ONE_MINUS_SRC_ALPHA;
                }
            }
            switch(destinationRGB)
            {
                case Renderer::BlendMode::One:
                {
                    dRGB = GL_ONE;
                    break;
                }
                case Renderer::BlendMode::SrcAlpha:
                {
                    dRGB = GL_SRC_ALPHA;
                    break;
                }
                case Renderer::BlendMode::OneMinusSrcAlpha:
                {
                    dRGB = GL_ONE_MINUS_SRC_ALPHA;
                }
            }
            switch(sourceAlpha)
            {
                case Renderer::BlendMode::One:
                {
                    sA = GL_ONE;
                    break;
                }
                case Renderer::BlendMode::SrcAlpha:
                {
                    sA = GL_SRC_ALPHA;
                    break;
                }
                case Renderer::BlendMode::OneMinusSrcAlpha:
                {
                    sA = GL_ONE_MINUS_SRC_ALPHA;
                }
            }
            switch(destinationAlpha)
            {
                case Renderer::BlendMode::One:
                {
                    dA = GL_ONE;
                    break;
                }
                case Renderer::BlendMode::SrcAlpha:
                {
                    dA = GL_SRC_ALPHA;
                    break;
                }
                case Renderer::BlendMode::OneMinusSrcAlpha:
                {
                    dA = GL_ONE_MINUS_SRC_ALPHA;
                }
            }
            glBlendFuncSeparate(sRGB, dRGB, sA, dA);
        }

        void RendererPrivate::setDepthTest(bool value)
        {
            if(value == true)
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
        }

        void RendererPrivate::setCullFace(bool value)
        {
            if(value == true)
            {
                glEnable(GL_CULL_FACE);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
        }

        void RendererPrivate::setColorMask(bool r, bool g, bool b, bool a)
        {
            glColorMask(r, g, b, a);
        }

        void RendererPrivate::draw(const TextureAsset & texture, const MeshAsset & mesh, const ShaderProgramAsset & shaderProgram)
        {
            // Use given mesh
            mesh.private_->bind();

            // Find out needed data for shader
            const std::vector<StrideComponent>& uniforms = mesh.private_->strideComponents();
            unsigned int strideByteSize = mesh.private_->strideByteSize();

            // Bind shader symbols to mesh data
            std::vector<std::string> shaderSymbols;
            for (std::vector<StrideComponent>::const_iterator i = uniforms.begin(); i != uniforms.end(); i++)
            {
                GLint shaderSymbolLocation = shaderProgram.private_->attribute((*i).shaderSymbolName);
                glEnableVertexAttribArray(shaderSymbolLocation);
                GLenum type = 0;
                switch((*i).type)
                {
                    case StrideType::Float32:
                    {
                        type = GL_FLOAT;
                        break;
                    }
                    default:
                    {
                        throw Exception("StrideType unknown");
                        break;
                    }
                }

                glVertexAttribPointer(
                    shaderSymbolLocation,
                    (*i).numberOfComponents,
                    type,
                    GL_FALSE,
                    strideByteSize,
                    (GLvoid*)((*i).byteOffset));
            }

            // Use given texture
            texture.private_->bind();

            // Draw
            glDrawArrays(GL_TRIANGLES, 0, mesh.private_->vertexCount());
        }

        void RendererPrivate::draw(const PolygonAsset & polygon, const ShaderProgramAsset & shaderProgram)
        {
            unsigned int pointCount = polygon.private_->pointCount();
            polygon.private_->bind();

            // Shader attributes
            std::string tmp("xyz");
            GLint positionXyz = shaderProgram.private_->attribute(tmp);
            glEnableVertexAttribArray(positionXyz);
            glVertexAttribPointer(positionXyz, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            // Draw the line
            glDrawArrays(GL_LINE_STRIP, 0, pointCount);
            glDisableVertexAttribArray(positionXyz);
        }

        void RendererPrivate::draw(FrameBuffer const& frameBuffer, MeshAsset const& mesh, ShaderProgramAsset const& shaderProgram)
        {
            // Use given mesh
            mesh.private_->bind();

            // Find out needed data for shader
            const std::vector<StrideComponent>& uniforms = mesh.private_->strideComponents();
            unsigned int strideByteSize = mesh.private_->strideByteSize();

            // Bind shader symbols to mesh data
            std::vector<std::string> shaderSymbols;
            for (std::vector<StrideComponent>::const_iterator i = uniforms.begin(); i != uniforms.end(); i++)
            {
                GLint shaderSymbolLocation = shaderProgram.private_->attribute((*i).shaderSymbolName);
                glEnableVertexAttribArray(shaderSymbolLocation);
                GLenum type = 0;
                switch((*i).type)
                {
                    case StrideType::Float32:
                    {
                        type = GL_FLOAT;
                        break;
                    }
                    default:
                    {
                        throw Exception("StrideType unknown");
                        break;
                    }
                }

                glVertexAttribPointer(
                    shaderSymbolLocation,
                    (*i).numberOfComponents,
                    type,
                    GL_FALSE,
                    strideByteSize,
                    (GLvoid*)((*i).byteOffset));
            }

            // Use given texture
            ((FrameBufferPrivate*)(frameBuffer.pointer()))->bindAsTexture();

            // Draw
            glDrawArrays(GL_TRIANGLES, 0, mesh.private_->vertexCount());
        }

        void RendererPrivate::drawLine(float p0x, float p0y, float p1x, float p1y, const ShaderProgramAsset & shaderProgram)
        {
            LOG
            std::string tmp("xyz");
            GLint positionXyz = shaderProgram.private_->attribute(tmp);

            GLfloat vertices[] =
            {
                p0x, p0y, 0.0f,
                p1x, p1y, 0.0f
            };

            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW); // 2 points having each 3 floats

            // TODO: probably some nice API for next lines:
            glDisable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);

            // Draw
            glEnableVertexAttribArray(positionXyz);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glVertexAttribPointer(positionXyz, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glDrawArrays(GL_LINES, 0,  2);

            // Delete data
            glDeleteBuffers(1, &vbo);
        }
    }
}
