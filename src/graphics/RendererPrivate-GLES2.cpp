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

#include "Common.hpp"
#include "RendererPrivate.hpp"
#include "TextureAssetPrivate.hpp"
#include "MeshAssetPrivate.hpp"
#include "PolygonAssetPrivate.hpp"
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

        void RendererPrivate::draw(const TextureAsset & texture, const MeshAsset & mesh, const ShaderProgramAsset & shaderProgram)
        {
            // Use given mesh
            mesh.private_->bind();

            // Find out needed data for shader
            const std::vector<StrideComponent>& uniforms = mesh.private_->strideComponents();

            // Bind shader symbols to mesh data
            std::vector<std::string> shaderSymbols;
            for (std::vector<StrideComponent>::const_iterator i = uniforms.begin(); i != uniforms.end(); i++)
            {
                GLint shaderSymbolLocation = shaderProgram.private_->attribute((*i).shaderSymbolName);
                glEnableVertexAttribArray(shaderSymbolLocation);
                glVertexAttribPointer(
                    shaderSymbolLocation,
                    (*i).numberOfComponents,
                    (*i).type,
                    GL_FALSE,
                    (*i).strideLength * sizeof(GLfloat),
                    (GLvoid*)((*i).strideOffset * sizeof(GLfloat)));
            }

            // Use given texture
            texture.private_->bind();

            // TODO: probably some nice API for next lines:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);

            // Draw
            glDrawArrays(GL_TRIANGLES, 0, mesh.private_->vertexCount());
        }

        void RendererPrivate::draw(const PolygonAsset & polygon, const ShaderProgramAsset & shaderProgram)
        {
            LOG

            // Fake test data
            static const GLfloat g_vertex_buffer_data[] = {
                        0.0f,  0.0f, 0.0f,
                        10.0f,  10.0f, 0.0f
            };

            GLuint vertexbuffer;
            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

            // Shader attributes
            std::string tmp("xyz");
            GLint positionXyz = shaderProgram.private_->attribute(tmp);
            glEnableVertexAttribArray(positionXyz);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(positionXyz, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            // TODO: probably some nice API for next lines:
            glDisable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);

            // Draw the line
            glDrawArrays(GL_LINES, 0, 2);
            glDisableVertexAttribArray(positionXyz);
            glDeleteBuffers(1, &vertexbuffer);
        }

        void RendererPrivate::drawLine(float p0x, float p0y, float p1x, float p1y, const ShaderProgramAsset & shaderProgram)
        {
            LOG
            std::string tmp("xyz");
            GLint positionXyz = shaderProgram.private_->attribute(tmp);

            GLfloat vertices[] =
            {
                0.0f, 0.0f, 0.0f,
                10.0f, 10.0f, 0.0f
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
            unsigned int vertexStride = 0; // separate VBOs
            glVertexAttribPointer(positionXyz, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glDrawArrays(GL_LINES, 0,  2);

            // Delete data
            glDeleteBuffers(1, &vbo);
        }

        /*void RendererPrivate::drawLine(float p0x, float p0y, float p1x, float p1y, const ShaderProgramAsset & shaderProgram)
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
            unsigned int vertexStride = 0; // separate VBOs
            glVertexAttribPointer(positionXyz, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glDrawArrays(GL_LINES, 0,  2);

            // Delete data
            glDeleteBuffers(1, &vbo);
        }*/
    }
}
