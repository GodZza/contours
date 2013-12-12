//
//   Copyright 2013 Pixar
//
//   Licensed under the Apache License, Version 2.0 (the "Apache License")
//   with the following modification; you may not use this file except in
//   compliance with the Apache License and the following modification to it:
//   Section 6. Trademarks. is deleted and replaced with:
//
//   6. Trademarks. This License does not grant permission to use the trade
//      names, trademarks, service marks, or product names of the Licensor
//      and its affiliates, except as required to comply with Section 4(c) of
//      the License and to reproduce the content of the NOTICE file.
//
//   You may obtain a copy of the Apache License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the Apache License with the above modification is
//   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
//   KIND, either express or implied. See the Apache License for the specific
//   language governing permissions and limitations under the Apache License.
//

#include "../osd/cpuGLVertexBuffer.h"

#include "../osd/opengl.h"

#include <string.h>

namespace OpenSubdiv {
namespace OPENSUBDIV_VERSION {

OsdCpuGLVertexBuffer::OsdCpuGLVertexBuffer(int numElements, int numVertices)
    : _numElements(numElements), _numVertices(numVertices),
      _vbo(0), _cpuBuffer(0), _dataDirty(true) {
}

OsdCpuGLVertexBuffer::~OsdCpuGLVertexBuffer() {

    delete[] _cpuBuffer;
    glDeleteBuffers(1, &_vbo);
}

OsdCpuGLVertexBuffer *
OsdCpuGLVertexBuffer::Create(int numElements, int numVertices) {
    OsdCpuGLVertexBuffer *instance =
        new OsdCpuGLVertexBuffer(numElements, numVertices);
    if (instance->allocate()) return instance;
    delete instance;
    return NULL;
}

void
OsdCpuGLVertexBuffer::UpdateData(const real *src, int startVertex, int numVertices) {

    memcpy(_cpuBuffer + startVertex * GetNumElements(), src, GetNumElements() * numVertices * sizeof(real));
    _dataDirty = true;
}

int
OsdCpuGLVertexBuffer::GetNumElements() const {

    return _numElements;
}

int
OsdCpuGLVertexBuffer::GetNumVertices() const {

    return _numVertices;
}

real*
OsdCpuGLVertexBuffer::BindCpuBuffer() {
    _dataDirty = true; // caller might modify data
    return _cpuBuffer;
}

GLuint
OsdCpuGLVertexBuffer::BindVBO() {
    if (not _dataDirty)
        return _vbo;

    int size = GetNumElements() * GetNumVertices() * sizeof(real);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size, _cpuBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _dataDirty = false;
    return _vbo;
}

bool
OsdCpuGLVertexBuffer::allocate() {
    _cpuBuffer = new real[GetNumElements() * GetNumVertices()];
    _dataDirty = true;
    int size = GetNumElements() * GetNumVertices() * sizeof(real);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

}  // end namespace OPENSUBDIV_VERSION
}  // end namespace OpenSubdiv