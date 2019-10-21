#include "framework/mesh/mesh.h"
#include "framework/misc/debug.h"
#include <cmath>
#include <iostream>

namespace winter
{
namespace mesh
{

Mesh::Mesh(const std::string& name):  mesh_name_(name)
{
    initialized_ = false;
    vertex_array_ = nullptr;
    vertex_buffer_ = nullptr;
    index_buffer_ = nullptr;
    shader_ = nullptr;
}
Mesh::~Mesh()
{
	DestroyMesh();
}

void Mesh::CreateMesh(const void* vertex_data, unsigned int buffer_size, unsigned int *indices, unsigned int index_count)
{
    vertex_buffer_ = new base::VertexBuffer(vertex_data, buffer_size);
    index_buffer_ = new base::IndexBuffer(indices, index_count);
    vertex_array_ = new base::VertexArray;
    // making sure the buffer layout is not empty
    WINTER_ASSERT(buffer_layout_.stride() != 0);
    // making sure the buffer layout is filled correctly
    WINTER_ASSERT( fmodf(buffer_size, buffer_layout_.stride()) == 0.0f );
    vertex_array_->AddBuffer(*vertex_buffer_, buffer_layout_);    
    vertex_buffer_->Unbind();
    index_buffer_->Unbind();
    vertex_array_->Unbind();
    initialized_=true;
}


void Mesh::OnRender()
{
    if(!initialized_)
    {
        std::cout << "attempting to draw uninitialized mesh " << mesh_name_ << std::endl;
        return;
    }
    if(shader_)
        shader_->Bind();
    vertex_array_->Bind();
    index_buffer_->Bind();
    GLDebug(glDrawElements(GL_TRIANGLES, index_buffer_->count(), GL_UNSIGNED_INT, nullptr));
}
void Mesh::DestroyMesh()
{
    delete vertex_array_;
    delete vertex_buffer_;
    delete index_buffer_;
    vertex_array_ = nullptr;
    vertex_buffer_ = nullptr;
    index_buffer_ = nullptr;
    shader_ = nullptr;
    initialized_ = false;
}

void Mesh::AssignShader(base::Shader* shader)
{
    shader_ = shader;
}

} // namespace mesh
} // namespace winter