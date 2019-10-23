#ifndef __WINTER_MODEL_H__
#define __WINTER_MODEL_H__

#include <vector>
#include <string>

#include "framework/mesh/mesh.h"
#include "framework/base/texture.h"
#include "framework/base/shader.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace winter
{
namespace mesh
{

class Model : public base::Renderable
{
public:
	Model(const std::string& local_tex_dir = "../res/textures");
	~Model();
	void LoadModel(const std::string& path);
	void OnRender() override;
	void SetLocalTextureDirectory(const std::string &local_tex_dir);

private:
	void LoadNode(aiNode *node, const aiScene *scene);
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterials(const aiScene *scene);
	void UpdateDimensions(double x, double y, double z);

	std::vector<mesh::Mesh*> mesh_list_;
	std::vector<base::Texture*> texture_list_;
	std::vector<unsigned int> texture_indices_;

	std::string local_tex_dir_;
	double min_z_, max_z_, min_y_, max_y_, min_x_, max_x_;
};

} // namespace mesh
} // namespace winter


#endif