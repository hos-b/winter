#ifndef __WINTER_MESH_H__
#define __WINTER_MESH_H__

#include "framework/base/abstract_types.h"
#include "framework/base/vertex_buffer.h"
#include "framework/base/vertex_array.h"
#include "framework/base/index_buffer.h"

#include <string>
#include <GL/glew.h>

/*********************************************************
* Mesh class:                                            *
* handles mesh creation and rendering. memory management *
* for mesh data (vertices, indices) must be done in the  *
* derived class or the scope that uses a mesh instance.  *
* Use:                                                   *
* define buffer layout, create mesh, render.             *
**********************************************************/

namespace winter
{
namespace mesh
{

class Mesh : public base::Renderable
{
public:
    Mesh(const std::string& name);
    ~Mesh() override;

    void CreateMesh(const void* vertices, unsigned int vertex_count, unsigned int *indices, unsigned int index_count);
    void OnRender() override;
    void DestroyMesh();
    inline bool initialized() { return initialized_; }
    
    template<typename T>
    void AddVertexBufferElement(unsigned int count) { buffer_layout_.Push<T>(count); }

private:
    bool initialized_;
    std::string mesh_name_;
    base::VertexBufferLayout buffer_layout_;

    base::VertexBuffer* vertex_buffer_;
    base::IndexBuffer* index_buffer_;
    base::VertexArray* vertex_array_;
};

}// namespace mesh
}// namespace winter

#endif