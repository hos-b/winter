#include "framework/vertex_array.h"
#include "framework/debug.h"

template<>
void VertexBufferLayout::Push<float>(unsigned int count)
{
    elements_.push_back({ count, GL_FLOAT, GL_FALSE });
    stride_ += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    elements_.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
    stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    elements_.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
    stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);;
}


VertexArray::VertexArray()
{
    GLDebug(glGenVertexArrays(1, &renderer_id_));
}
VertexArray::~VertexArray()
{
    GLDebug(glDeleteVertexArrays(1, &renderer_id_));
}
void VertexArray::Bind() const
{
    GLDebug(glBindVertexArray(renderer_id_));
}
void VertexArray::Unbind() const
{
    GLDebug(glBindVertexArray(0));
}
void VertexArray::AddBuffer(const VertexBuffer& vertex_buffer, const VertexBufferLayout& layout)
{
    Bind();
    vertex_buffer.Bind();
    const auto& elements = layout.elements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GLDebug(glEnableVertexAttribArray(i));
        GLDebug(glVertexAttribPointer(i, element.count, element.type, 
                element.normalized, layout.stride(), reinterpret_cast<const void*>(offset) ));
                
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}