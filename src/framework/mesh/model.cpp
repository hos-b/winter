#include "framework/mesh/model.h"
#include <iostream>

namespace winter
{
namespace mesh
{

Model::Model(const std::string& local_tex_dir) : local_tex_dir_(local_tex_dir)
{
	min_x_ = max_x_ = 0;
	min_y_ = max_y_ = 0;
	min_z_ = max_z_ = 0;
}
Model::~Model(){
	for (unsigned int i = 0; i < mesh_list_.size(); i++)
	{
		if(mesh_list_[i])
			delete mesh_list_[i];
		mesh_list_[i] = nullptr;
	}
	for (unsigned int i = 0; i < texture_list_.size(); i++)
	{
		if(texture_list_[i])
			delete texture_list_[i];
		texture_list_[i] = nullptr;
	}
}
/* sets the directory which will be scanned for textures upon loading the models */
void Model::SetLocalTextureDirectory(const std::string &local_tex_dir){
	local_tex_dir_ = local_tex_dir;
}
/* loads the model, including all the meshes and textures (1 texture pre mesh only) */
void Model::LoadModel(const std::string& path){
	Assimp::Importer importer;
	std::cout << "loading file -> " << std::flush;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | 
												   aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene){
		std::cout << "failed to load model " << path << ":\n"
				  << importer.GetErrorString() << std::endl;
		return;
	}
	std::cout << "loading root node -> " << std::flush;
	LoadNode(scene->mRootNode, scene);
	std::cout << "loading textures..." << std::endl;
	LoadMaterials(scene);
	// std::cout << "loaded " << mesh_list_.size() << " meshes" << std::endl;
	std::cout << "model dimensions :\n"
			  << "x: [" << min_x_ << "," << max_x_ << "], "
			  << "y: [" << min_y_ << "," << max_y_ << "], "
			  << "z: [" << min_z_ << "," << max_z_ << "]\n";
}
/* renders all the meshes in the model */
void Model::OnRender() {
	for (size_t i = 0; i < mesh_list_.size(); i++)
	{
		unsigned int t_index = texture_indices_[i];
		texture_list_[t_index]->Bind(1);
		mesh_list_[i]->OnRender();
	}
}
/* recursively loads a node and its subnodes in the assim structure */
void Model::LoadNode(aiNode *node, const aiScene *scene){
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		LoadNode(node->mChildren[i], scene);
}
/* loads a mesh in a node */
void Model::LoadMesh(aiMesh *mesh, const aiScene *scene){
	(void)scene;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++){
		vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
		UpdateDimensions(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->mTextureCoords[0]) // if first texture exists TODO: handle all
			vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
		else
			vertices.insert(vertices.end(), { 0.0f , 0.0f });
		vertices.insert(vertices.end(), {-mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z}); // TODO: fix reverted normals
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		indices.reserve(face.mNumIndices);
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.emplace_back(face.mIndices[j]);
	}
	
	std::string mesh_name = mesh->mName == aiString("") ? std::string("mesh_") + std::to_string(mesh_list_.size()) : mesh->mName.C_Str();
	mesh::Mesh *new_mesh = new mesh::Mesh(mesh_name);
	new_mesh->AddVertexBufferElement<float>(3);
	new_mesh->AddVertexBufferElement<float>(2);
	new_mesh->AddVertexBufferElement<float>(3);
	new_mesh->CreateMesh(&vertices[0], vertices.size() * sizeof(float), &indices[0], indices.size());
	mesh_list_.emplace_back(new_mesh);
	texture_indices_.emplace_back(mesh->mMaterialIndex);
}
/* loads the textures */
void Model::LoadMaterials(const aiScene *scene){
	unsigned int count = 0;
	texture_list_.resize(scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials ; i++)
	{
		aiMaterial *material = scene->mMaterials[i];
		texture_list_[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE)!=0)
		{
			aiString path;
			if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("/");
				std::string file_name = std::string(path.data).substr(idx + 1);
				std::string tex_path = local_tex_dir_ + "/" + file_name;
				// std::cout << "loading " << tex_path << std::endl;
				texture_list_[i] = new base::Texture(tex_path, GL_REPEAT, true);
				texture_list_[i]->Bind(1);
				if (!texture_list_[i]->loaded())
				{
					delete texture_list_[i];
					texture_list_[i] = nullptr;
					std::cout << "failed to load texture at " << tex_path << std::endl;
				}
			}
		}
		// if still nullptr after all that -> set to default texture
		if (!texture_list_[i])
		{
			texture_list_[i] = new base::Texture("../res/textures/plain.png", GL_REPEAT, true);
			count++;
		}
	}
	// std::cout << "used default for " << count << "/" << texture_list_.size() << " textures" << std::endl;
}

/* function to keep track of the model dimensions */
void Model::UpdateDimensions(double x, double y, double z)
{
	if (x < min_x_)
		min_x_ = x;
	else if (x > max_x_)
		max_x_ = x;

	if (y < min_y_)
		min_y_ = y;
	else if (y > max_y_)
		max_y_ = y;
	
	if (z < min_z_)
		min_z_ = z;
	else if (z > max_z_)
		max_z_ = z;
}

} // namespace mesh
} // namespace winter
