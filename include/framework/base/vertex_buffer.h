#ifndef __WINTER_VERTEX_BUFFER_H__
#define __WINTER_VERTEX_BUFFER_H__


namespace winter
{
namespace base
{

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
    inline unsigned int count() const {return count_;}
private:
    unsigned int renderer_id_;
    unsigned int count_;
};

} // namespace base
} // namespace shader
#endif