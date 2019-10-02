#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__


class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
private:
    unsigned int renderer_id_;
};

#endif