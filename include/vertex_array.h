#ifndef __VERTEX_ARRAY__
#define __VERTEX_ARRAY__
#include "vertex_buffer.h"
#include <vector>
#include <GL/glew.h>

struct VertexBufferElement
{
    unsigned int count;
    unsigned int type;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:          return 4;
        case GL_UNSIGNED_INT:   return 4;
        case GL_UNSIGNED_BYTE:  return 1;
        }
        throw "unsupported type";
        return 0;
    }
}; 

class VertexBufferLayout
{
public :
    VertexBufferLayout() : stride_(0) {};
    ~VertexBufferLayout() {};

    template <typename T>
    void Push(unsigned int count);
    

    inline unsigned int stride() const { return stride_;}
    inline const std::vector<VertexBufferElement>& elements() const { return elements_;}
private:
    std::vector<VertexBufferElement> elements_;
    unsigned int stride_;
};


class VertexArray
{
public:
    VertexArray();
    ~VertexArray(); 
    
    void Bind() const;
    void Unbind() const;
    void AddBuffer(const VertexBuffer& vertex_buffer, const VertexBufferLayout& layout);
private:
    unsigned int renderer_id_;
};

#endif