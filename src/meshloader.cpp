#include "meshloader.hpp"

#include "mesh.hpp"

#include "lib/assimp.hpp"

#include <iostream>
#include <vector>

std::unique_ptr<WorldObject> MeshLoader::load(std::string path)
{
    std::unique_ptr<WorldObject> root(new WorldObject());
    if (MeshLoader::load(path, *root.get())) {
        return root;
    } else {
        return nullptr;
    }
}

bool MeshLoader::load(std::string path, WorldObject & object)
{
    Assimp::Importer importer;

    aiScene const * scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Quality);

    if (!scene) {
        std::cerr << "MeshLoader::load: " << importer.GetErrorString() << std::endl;
        return false;
    }

    std::function<void(aiNode *, aiScene const *)> traverse;
    traverse = [&traverse, &object](aiNode * node, aiScene const * scene) -> void
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            auto mesh = std::make_shared<Mesh>();

            aiMesh * aimesh = scene->mMeshes[node->mMeshes[i]];

            for (unsigned int i = 0; i < aimesh->mNumVertices; i++) {
                aiVector3D aiposition = aimesh->mVertices[i];
                aiVector3D ainormal = aimesh->mNormals[i];
                mesh->positions.push_back(glm::vec3(aiposition.x, aiposition.y, aiposition.z));
                mesh->normals.push_back(glm::vec3(ainormal.x, ainormal.y, ainormal.z));
            }

            for(unsigned int i = 0; i < aimesh->mNumFaces; i++) {
                aiFace aiface = aimesh->mFaces[i];
                for(unsigned int j = 0; j < aiface.mNumIndices; j++) {
                    mesh->indices.push_back(aiface.mIndices[j]);
                }
            }

            mesh->update_positions = !mesh->positions.empty();
            mesh->update_normals = !mesh->normals.empty();
            mesh->update_indices = !mesh->indices.empty();

            object.add(mesh);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            traverse(node->mChildren[i], scene);
        }
    };

    traverse(scene->mRootNode, scene);

    object.update_world_transform();

    return true;
}
