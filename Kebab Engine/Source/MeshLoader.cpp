#include "Application.h"
#include "MeshLoader.h"
#include "TextureLoader.h"

#include "Cube.h"
#include "Pyramid.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"

//#include "assimp/cimport.h"
//#include "assimp/scene.h"
//#include "assimp/postprocess.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mmgr/mmgr.h"

#define ASSETS_DIR "Assets/3D Models/"

MeshLoader* MeshLoader::instance = nullptr;

MeshLoader* MeshLoader::GetInstance()
{
    if (instance == nullptr)
        instance = new MeshLoader();

    return instance;
}

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{   
}

void MeshLoader::CleanUp()
{
    delete(instance);
    instance = nullptr;
}

GameObject* MeshLoader::LoadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_CONSOLE("ERROR::ASSIMP:: %s", import.GetErrorString());
        return nullptr;
    }
    directory = path.substr(0, path.find_last_of('/'));

    int start = path.find_last_of('/') + 1;
    if (start == 0) start = path.find_last_of("\\") + 1;
    int end = path.find('.');

    std::string name = path.substr(start, end - start);

    GameObject* baseGO = new GameObject(name);

    ProcessNode(scene->mRootNode, scene, baseGO, name);

    app->scene->AddGameObject(baseGO);

    return baseGO;
}

void MeshLoader::ProcessNode(aiNode* node, const aiScene* scene, GameObject* baseGO, std::string nameBaseGO)
{
    for (int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene, baseGO, nameBaseGO);
    }
    for (int i = 0; i < node->mNumChildren; ++i)
    {
        if (node->mChildren[i]->mNumMeshes > 0)
        {
            GameObject* go = new GameObject(node->mChildren[i]->mName.C_Str());
            ComponentTransform* tr = (ComponentTransform*)go->GetComponent(ComponentType::TRANSFORM);
            aiVector3D p, s;
            aiQuaternion r;
            node->mChildren[i]->mTransformation.Decompose(s, r, p);

            float3 pos = { p.x, p.y, p.z };
            float3 scale = { s.x,s.y,s.z };
            Quat rot = { r.x,r.y,r.z,r.w };
            tr->SetTranslation(pos);
            tr->SetScale(scale);
            tr->SetRotation(rot);


            go->SetParent(baseGO);
            if (baseGO) baseGO->AddChild(go);

            ProcessNode(node->mChildren[i], scene, go, nameBaseGO);
            //app->scene->AddGameObject(go);
        }
        else
        {
            ProcessNode(node->mChildren[i], scene, baseGO, nameBaseGO);
        }
    }
    
    /*
    // Process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene, baseGO);
    }
    // Then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        if(baseGO->GetChilds().size() > i)
            ProcessNode(node->mChildren[i], scene, baseGO->GetChilds()[i]);
        else
            ProcessNode(node->mChildren[i], scene, baseGO);
    }
    */
}

std::vector<Tex> MeshLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Tex> textures;
    /*for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }*/
    return textures;
}

ComponentMesh* MeshLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* baseGO, std::string nameBaseGO)
{
    //GameObject* go = new GameObject(mesh->mName.C_Str());

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    //std::vector<float2> texCoords;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }
        
        if (mesh->mTextureCoords[0])
        {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    
    std::string imageName;
    if (scene->mNumMaterials > 0)
    {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

        aiString n;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &n);
        //if(n.length <= 0) LOG_CONSOLE("\nCurrent mesh %s doesn't have a diffuse texture", mesh->mName.C_Str());
        //mat->GetTexture(aiTextureType_BASE_COLOR, 0, &n);
        //if (n.length <= 0) LOG_CONSOLE("\nCurrent mesh %s doesn't have a base color texture\n", mesh->mName.C_Str());

        std::string name;
        name = n.C_Str();

        if (name.size() > 0)
        {
            int start = name.find_last_of('\\');
            if (start == 0)
                start = name.find_last_of('/');

            imageName = ASSETS_DIR + nameBaseGO + "/";
            imageName += name.substr(start + 1);
        }
    }
        

    /*int start = 0;
    if (name.size() > 0)
    {
        start = name.find_last_of('\\');
        if (start == 0)
            start = name.find_last_of('/');

        imageName = ASSETS_DIR + nameBaseGO + "/";
        imageName += name.substr(start + 1);
    }*/
    /*else
    {
        imageName = ASSETS_DIR + nameBaseGO + "/";
        imageName += name.substr(start + 1);
    }*/


    ComponentMesh* meshComp = (ComponentMesh*)baseGO->CreateComponent(ComponentType::MESH);
    meshComp->SetData(vertices, indices, TextureLoader::GetInstance()->LoadTexture(imageName.c_str()));

    LOG_CONSOLE("\nSuccesfully loaded mesh %s from %s: %i vertices, %i indices", baseGO->GetName().c_str(), nameBaseGO.c_str(), vertices.size(), indices.size());

    return meshComp;
}

//GameObject* MeshLoader::LoadPrimitive(PrimitiveType type)
//{
//    GameObject* go = nullptr;
//    Component* comp = nullptr;
//
//    std::string name;
//    switch (type)
//    {
//        case PrimitiveType::CUBE:
//        {
//            static int numCube = 0;
//            name = "Cube";
//            if(numCube > 0)
//                name += " " + std::to_string(numCube);
//            numCube++;
//
//            go = new GameObject(name.c_str());
//            comp = (ComponentMesh*)new Primitive(type, go);
//
//            //comp = new KbCube({ 0,0,0 }, { 1,1,1 }, go);            
//            break;
//        }
//
//        case PrimitiveType::CONE:
//        {
//            static int numPlane = 0;
//            name = "Cone";
//            if (numPlane > 0) name += " " + std::to_string(numPlane);
//            numPlane++;
//
//            /*go = new GameObject(name.c_str());
//            comp = new KbPlane({ -1,0,0 }, { 2,1 }, go);*/
//            break;
//        }
//        
//        case PrimitiveType::SPHERE:
//        {
//            static int numSphere = 0;
//            name = "Sphere";
//            if (numSphere > 0) name += " " + std::to_string(numSphere);
//            numSphere++;
//
//            /*go = new GameObject(name.c_str());
//            comp = new KbSphere({ 0,0,0 }, 1, 20, 20, go);*/
//            break;
//        }
//
//        case PrimitiveType::CYLINDER:
//        {
//            static int numCyl = 0;
//            name = "Cylinder";
//            if (numCyl > 0) name = name += " " + std::to_string(numCyl);
//            numCyl++;
//
//            /*go = new GameObject(name.c_str());
//            comp = new KbCylinder({ 0,0,0 }, 2, 5, 10, go);*/
//            break;
//        }
//
//    }
//
//    go = new GameObject(name.c_str());
//    comp = (ComponentMesh*)new Primitive(type, go);
//
//    if (go)
//    {
//        go->AddComponent(comp);
//        app->scene->AddGameObject(go);
//    }
//
//    return go;
//
//}

GameObject* MeshLoader::LoadKbGeometry(KbGeometryType type)
{
    GameObject* go = nullptr;
    Component* comp = nullptr;

    std::string name;
    switch (type)
    {
        case KbGeometryType::CUBE:
        {
            static int numCube = 0;
            name = "Cube";
            if(numCube > 0)
                name += " " + std::to_string(numCube);
            numCube++;

            go = new GameObject(name.c_str());
            comp = new KbCube({ 0,0,0 }, { 1,1,1 }, go);            
            break;
        }

        case KbGeometryType::PYRAMID:
        {
            static int numPyr = 0;
            name = "Pyramid";
            if (numPyr > 0) name += " " + std::to_string(numPyr);
            numPyr++;

            go = new GameObject(name.c_str());
            comp = new KbPyramid({ 0,0,0 }, 5, 3, go);
            break;
        }

        case KbGeometryType::PLANE:
        {
            static int numPlane = 0;
            name = "Plane";
            if (numPlane > 0) name += " " + std::to_string(numPlane);
            numPlane++;

            go = new GameObject(name.c_str());
            comp = new KbPlane({ -1,0,0 }, { 2,1 }, go);
            break;
        }
        
        case KbGeometryType::SPHERE:
        {
            static int numSphere = 0;
            name = "Sphere";
            if (numSphere > 0) name += " " + std::to_string(numSphere);
            numSphere++;

            go = new GameObject(name.c_str());
            comp = new KbSphere({ 0,0,0 }, 1, 20, 20, go);
            break;
        }

        case KbGeometryType::CYLINDER:
        {
            static int numCyl = 0;
            name = "Cylinder";
            if (numCyl > 0) name = name += " " + std::to_string(numCyl);
            numCyl++;

            go = new GameObject(name.c_str());
            comp = new KbCylinder({ 0,0,0 }, 2, 5, 10, go);
            break;
        }

    }

    if (go)
    {
        go->AddComponent(comp);
        app->scene->AddGameObject(go);
    }

    return go;
}