#include "Model.h"



Model::Model(std::string modelPathName)
{
	Assimp::Importer importer;
	this->scene = importer.ReadFile(modelPathName,aiProcess_Triangulate | aiProcess_FlipUVs );
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	    return;
	}
	this->directory = modelPathName.substr(0,modelPathName.find_last_of('/'));
	this->processAssimpSceneTree(scene->mRootNode,scene);
}


Model::~Model()
{
}


void Model::Draw(Shader shader)
{
	for (int i=0; i<this->meshes.size();i++)
	{
		this->meshes[i].Draw(shader);
	}
};


void Model::processAssimpSceneTree(aiNode* node, const aiScene* scene)
{
	for (GLuint i=0; i<node->mNumMeshes;i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processAssimpMesh(mesh,scene));
	};
	for (GLuint i=0; i<node->mNumChildren;i++)
	{
		this->processAssimpSceneTree(node->mChildren[i],scene);
	}
};

Mesh Model::processAssimpMesh(aiMesh* mesh, const aiScene* scene)
{
	//This helper function will translate assimp model to our Mesh Class interface.
	std::vector<Mesh::vertex> vertices;
	std::vector<GLuint>       indices;
	std::vector<Texture>      textures;


	//Vertex Extraction
	for (int i=0; i<mesh->mNumVertices;i++)
	{
		Mesh::vertex vertex;
		glm::vec3 vector3;
		glm::vec2 vector2;

		vector3.x = mesh->mVertices[i].x;
		vector3.y = mesh->mVertices[i].y;
		vector3.z = mesh->mVertices[i].z;

		vertex.position=vector3;

		vector3.x = mesh->mNormals[i].x;
		vector3.y = mesh->mNormals[i].y;
		vector3.z = mesh->mNormals[i].z;

		vertex.normal = vector3;

		if (mesh->mTextureCoords[0])
		{
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = vector2;
		}
		vertices.push_back(vertex); //We store the vertex for the current mesh

	}
	//Indices for the previous vertex data
	for(GLuint i= 0; i<mesh->mNumFaces;i++)
	{
		aiFace face = mesh->mFaces[i];
		for(GLuint j=0; j< face.mNumIndices;j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex>=0)
	{
		aiMaterial* material            = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture>diffuseMaps = this->loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
		textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
		std::vector<Texture>specMaps    = this->loadMaterialTextures(material,aiTextureType_SPECULAR,"texture_specular");
		textures.insert(textures.end(),specMaps.begin(),specMaps.end());
	}
	return Mesh(vertices, indices,textures);
}
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> modelTextures;
	for (GLuint i = 0; i<mat->GetTextureCount(type);i++)
	{
		aiString str;
		mat->GetTexture(type,i,&str);
		Texture texture = Texture(str.C_Str());
		texture.setTextureType(typeName);
		texture.setTexturePath(str);
		modelTextures.push_back(texture);
	}
	return modelTextures;

}
