#ifndef __WINTER_INDEX_BUFFER_H__
#define __WINTER_INDEX_BUFFER_H__

namespace winter
{
namespace base
{

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;
    inline unsigned int count() const {return count_;}
    
private:
    unsigned int renderer_id_;
    unsigned int count_;
};

} // namespace base
} // namespace winter
#endif