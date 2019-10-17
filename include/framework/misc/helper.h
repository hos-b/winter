#ifndef __WINTER_HELPER_H__
#define __WINTER_HELPER_H__

#include <glm/glm.hpp>
namespace winter
{
namespace helper
{

void CalculatePhongNormals(unsigned int* indices, unsigned int index_count, 
                     		 float* vbuffer, unsigned int vbuffer_size, 
                     		 unsigned int layout_length, unsigned int normal_offset )
{
    // going over faces
    for (unsigned int i = 0; i < index_count; i += 3 ) {
        unsigned int in0 = indices[i]   * layout_length;
        unsigned int in1 = indices[i+1] * layout_length;
        unsigned int in2 = indices[i+2] * layout_length;
        glm::vec3 vec1(vbuffer[in1]   - vbuffer[in0],
                       vbuffer[in1+1] - vbuffer[in0+1],
                       vbuffer[in1+2] - vbuffer[in0+2]);
		glm::vec3 vec2(vbuffer[in2]   - vbuffer[in0],
                       vbuffer[in2+1] - vbuffer[in0+1],
                       vbuffer[in2+2] - vbuffer[in0+2]);
		glm::vec3 normal = glm::cross(vec1, vec2);
		normal = glm::normalize(normal);
		// getting normal index for all 3 vbuffer involved
		in0 += normal_offset;
		in1 += normal_offset;
		in2 += normal_offset;
		// adding it to each vertex' normal
		vbuffer[in0] += normal.x; vbuffer[in0+1] += normal.y; vbuffer[in0+2] += normal.z; 
		vbuffer[in1] += normal.x; vbuffer[in1+1] += normal.y; vbuffer[in1+2] += normal.z;
		vbuffer[in2] += normal.x; vbuffer[in2+1] += normal.y; vbuffer[in2+2] += normal.z;
	}
	// normalizing normals
	for (unsigned int i = 0; i < vbuffer_size / layout_length; i++){
		unsigned int n_offset = i * layout_length + normal_offset;
		glm::vec3 vec(vbuffer[n_offset], vbuffer[n_offset + 1], vbuffer[n_offset + 2]);
		vec = glm::normalize(vec);
		vbuffer[n_offset] = vec.x; vbuffer[n_offset + 1] = vec.y; vbuffer[n_offset + 2] = vec.z;
	}
}

} // namespace util
} // namespace winter


#endif