#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__


class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;
    unsigned int count() const;
    
private:
    unsigned int renderer_id_;
    unsigned int count_;
};

#endif