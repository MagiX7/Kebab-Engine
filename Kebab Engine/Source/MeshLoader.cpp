#include "Application.h"
#include "Renderer3D.h"

#include "MeshLoader.h"
#include "TextureLoader.h"
#include "ResourceManager.h"

#include "MainScene.h"
#include "Editor.h"
#include "FileSystem.h"

#include "PanelAssets.h"

#include "TextureLoader.h"

#include "ComponentMaterial.h"

#include "Model.h"
#include "KbCube.h"
#include "KbPyramid.h"
#include "KbPlane.h"
#include "KbSphere.h"
#include "KbCylinder.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <queue>

#include "mmgr/mmgr.h"

#define ASSETS_DIR "Assets/Resources/"
#define CUSTOM_DIR "Library/Meshes/"
#define CUSTOM_EXTENSION ".kbmesh"

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

GameObject* MeshLoader::LoadModel(const std::string& path, bool loadOnScene)
{
    int start = path.find_last_of('/') + 1;
    if (start == 0) start = path.find_last_of("\\") + 1;
    int end = path.find('.');

    std::string name = path.substr(start, end - start);

    GameObject* baseGO = new GameObject(name);
    
    
    std::shared_ptr<Resource> res = ResourceManager::GetInstance()->IsAlreadyLoaded(path.c_str());
    
    if (res)
    {
        KbModel* model = (KbModel*)res.get();
        int i = 0;
        for (auto& mesh : model->GetMeshes())
        {
            GameObject* go = new GameObject(mesh->GetOwnerName());
            ComponentMesh* meshComp = (ComponentMesh*)go->CreateComponent(ComponentType::MESH);
            meshComp->SetMesh(mesh);
            baseGO->AddChild(go);
            go->SetParent(baseGO);
        }
    }
    else
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_CONSOLE("ERROR::ASSIMP:: %s", import.GetErrorString());
            return nullptr;
        }
        directory = path.substr(0, path.find_last_of('/'));
        
        std::shared_ptr<Resource> model = ResourceManager::GetInstance()->CreateNewResource(path.c_str(), ResourceType::MODEL);
        KbModel* newModel = (KbModel*)model.get();
        ProcessNode(scene->mRootNode, scene, baseGO, name, path, newModel);

        SaveModelCustomFormat(baseGO, newModel->uuid);
    
        
        app->editor->assetsPanel->AddAsset(baseGO);
    }

    app->scene->AddGameObject(baseGO);
    return baseGO;

    /*if (loadOnScene)
    {
        app->scene->AddGameObject(baseGO);
        app->editor->assetsPanel->AddAsset(baseGO);
        return baseGO;
    }
    else
    {
        delete baseGO;
        return nullptr;
    }*/
}

void MeshLoader::ProcessNode(aiNode* node, const aiScene* scene, GameObject* baseGO, const std::string& nameBaseGO, const std::string& path, KbModel* model)
{
    for (int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene, baseGO, nameBaseGO, path, model);
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

            ProcessNode(node->mChildren[i], scene, go, nameBaseGO, path, model);
            //app->scene->AddGameObject(go);
        }
        else
        {
            ProcessNode(node->mChildren[i], scene, baseGO, nameBaseGO, path, model);
        }
    }
}

//std::vector<Tex> MeshLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
//{
//    std::vector<Tex> textures;
//    /*for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//    {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//        Texture texture;
//        texture.id = TextureFromFile(str.C_Str(), directory);
//        texture.type = typeName;
//        texture.path = str.C_Str();
//        textures.push_back(texture);
//    }*/
//    return textures;
//}

ComponentMesh* MeshLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* baseGO, const std::string& nameBaseGO, const std::string& path, KbModel* model)
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
        else LOG_CONSOLE("\nMesh doesn't have a material with DIFFUSE texture. If you have it, drag and drop it.");
    }

    ComponentMaterial* mat = (ComponentMaterial*)baseGO->CreateComponent(ComponentType::MATERIAL);
    std::shared_ptr<Resource> tex = ResourceManager::GetInstance()->CreateNewResource(imageName.c_str(), ResourceType::TEXTURE);
    mat->AddTexture((Texture*)tex.get());
   
    ComponentMesh* meshComp = (ComponentMesh*)baseGO->CreateComponent(ComponentType::MESH);
    KbMesh* m = new KbMesh(vertices, indices);
    m->SetOwnerName(baseGO->GetName());
    meshComp->SetMesh(m);
    model->AddMesh(m);

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
    KbMesh* mesh = nullptr;
    ComponentMaterial* matComp = nullptr;

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

            mesh = new KbCube(/*go->GetUuid(), */ { -0.5, -0.5, -0.5 }, {1,1,1});

            /*go = new GameObject(name.c_str());
            comp = new KbCube({ 0,0,0 }, { 1,1,1 }, go);*/
            break;
        }

        case KbGeometryType::PYRAMID:
        {
            static int numPyr = 0;
            name = "Pyramid";
            if (numPyr > 0) name += " " + std::to_string(numPyr);
            numPyr++;

            mesh = new KbPyramid({ 0,0,0 }, 2, 1);

            /*go = new GameObject(name.c_str());
            comp = new KbPyramid({ 0,0,0 }, 5, 3, go);*/
            break;
        }

        case KbGeometryType::PLANE:
        {
            static int numPlane = 0;
            name = "Plane";
            if (numPlane > 0) name += " " + std::to_string(numPlane);
            numPlane++;
            
            mesh = new KbPlane({ -0.5f,0.0f, -0.5f }, { 1,1 });

            /*go = new GameObject(name.c_str());
            comp = new KbPlane({ -1,0,0 }, { 2,1 }, go);*/
            break;
        }
        
        case KbGeometryType::SPHERE:
        {
            static int numSphere = 0;
            name = "Sphere";
            if (numSphere > 0) name += " " + std::to_string(numSphere);
            numSphere++;

            mesh = new KbSphere({ -0.5f,-0.5f,-0.5f }, 1, 15, 15);

            /*go = new GameObject(name.c_str());
            comp = new KbSphere({ 0,0,0 }, 1, 20, 20, go);*/
            break;
        }

        case KbGeometryType::CYLINDER:
        {
            static int numCyl = 0;
            name = "Cylinder";
            if (numCyl > 0) name = name += " " + std::to_string(numCyl);
            numCyl++;

            mesh = new KbCylinder({ -0.5f,-0.5f ,-0.5f }, 1, 1, 15);

            /*go = new GameObject(name.c_str());
            comp = new KbCylinder({ 0,0,0 }, 2, 5, 10, go);*/
            break;
        }
    }
    
    if (mesh)
    {
        go = new GameObject(name);
        ComponentMesh* meshComp = new ComponentMesh(go);
        go->AddComponent(meshComp);
        
        meshComp->SetMesh(mesh);

        /*ComponentMaterial* mat = (ComponentMaterial*)*/go->CreateComponent(ComponentType::MATERIAL);    

        app->scene->AddGameObject(go);
    }
    

    return go;
}

void MeshLoader::SaveMeshCustomFormat(KbMesh* mesh, const std::string& name)
{
    unsigned int ranges[2] = { mesh->vertices.size(), mesh->indices.size() };

    uint size = sizeof(ranges) + sizeof(Vertex) * mesh->vertices.size() + sizeof(uint32_t) * mesh->indices.size();

    char* fileBuffer = new char[size];
    char* cursor = fileBuffer;

    unsigned int bytes = sizeof(ranges);
    memcpy(cursor, ranges, bytes);
    cursor += bytes;


    bytes = sizeof(Vertex) * mesh->vertices.size();
    memcpy(cursor, mesh->vertices.data(), bytes);
    cursor += bytes;

    bytes = sizeof(uint32_t) * mesh->indices.size();
    memcpy(cursor, mesh->indices.data(), bytes);
    cursor += bytes;

    std::string n = CUSTOM_DIR + name + "_" + std::to_string(mesh->uuid) + CUSTOM_EXTENSION;
    //mesh->SetMeshPath(n);
    mesh->SetLibraryPath(n);

    app->fileSystem->Save(n.c_str(), fileBuffer, size);

    delete[] fileBuffer;
}

void MeshLoader::SaveMeshCustomFormat(ComponentMesh* mesh)
{
    SaveMeshCustomFormat(mesh->GetMesh(), mesh->GetParent()->GetName());
}

KbMesh* MeshLoader::LoadMeshCustomFormat(const std::string& fileName, GameObject* parent)
{
    std::string name;
    int start = fileName.find_last_of('\\');
    if (start <= 0)
        start = fileName.find_last_of('/');

    if (start <= 0)
        name = CUSTOM_DIR + fileName;
    else
        name = fileName;
    
    KbMesh* mesh = new KbMesh(name.c_str());

    char* buffer = nullptr;
    if (app->fileSystem->Load(name.c_str(), &buffer) > 0)
        LOG_CONSOLE("KBMESH LOADED SUCCESSFULLY");

    char* cursor = buffer;
    unsigned int ranges[2];

    unsigned int bytes = sizeof(ranges);
    memcpy(ranges, cursor, bytes);
    cursor += bytes;

    //mesh->indices.resize(ranges[0]);
    //mesh->vertices.resize(ranges[1]);
    std::vector<Vertex> vertices;
    vertices.resize(ranges[0]);

    std::vector<uint32_t> indices;
    indices.resize(ranges[1]);

    // Load vertices
    bytes = sizeof(Vertex) * vertices.size();
    memcpy(vertices.data(), cursor, bytes);
    cursor += bytes;

    // Load indices
    bytes = sizeof(uint32_t) * indices.size();
    memcpy(indices.data(), cursor, bytes);
    cursor += bytes;

    mesh->SetData(vertices, indices);

    delete[] buffer;
    vertices.clear();
    indices.clear();

    return mesh;
}

void MeshLoader::SaveModelCustomFormat(GameObject* go, int modelUuid)
{
    int ran = 0;
    std::queue<GameObject*> q;
    std::vector<std::pair<GameObject*, std::string>> gosMeshes;

    //q.push(go);
    for (auto& child : go->GetChilds())
        q.push(child);

    while (!q.empty())
    {
        GameObject* curr = q.front();
        q.pop();
        ComponentMesh* m = (ComponentMesh*)curr->GetComponent(ComponentType::MESH);

        if (m)
        {
            std::pair<GameObject*, std::string> g;
            g.first = curr;
            g.second = m->GetMesh()->GetPath();
            gosMeshes.push_back(g);
            ran++;
        }

        if (curr->GetChilds().size() > 0)
            for (auto& c : curr->GetChilds())
                q.push(c);
    }

    modelValue = json_value_init_object();
    JSON_Object* modelObj = json_value_get_object(modelValue);

    JSON_Value* arrValue = json_value_init_array();
    JSON_Array* arr = json_value_get_array(arrValue);


    if(go != app->scene->GetRoot()) json_object_set_string(modelObj, "base name", go->GetName().c_str());

    if(go->GetParent() != app->scene->GetRoot()) json_object_set_number(modelObj, "parent uuid", go->GetUuid());
    json_object_set_number(modelObj, "uuid", go->GetUuid());
    json_object_set_value(modelObj, "Meshes", arrValue);

    ComponentTransform* tr = (ComponentTransform*)go->GetComponent(ComponentType::TRANSFORM);
    json_object_set_number(modelObj, "pos x", tr->GetTranslation().x);
    json_object_set_number(modelObj, "pos y", tr->GetTranslation().y);
    json_object_set_number(modelObj, "pos z", tr->GetTranslation().z);

    json_object_set_number(modelObj, "rot x", tr->GetRotation().x);
    json_object_set_number(modelObj, "rot y", tr->GetRotation().y);
    json_object_set_number(modelObj, "rot z", tr->GetRotation().z);
    json_object_set_number(modelObj, "rot w", tr->GetRotation().w);

    json_object_set_number(modelObj, "scale x", tr->GetScale().x);
    json_object_set_number(modelObj, "scale y", tr->GetScale().y);
    json_object_set_number(modelObj, "scale z", tr->GetScale().z);

    for (int i = 0; i < gosMeshes.size(); ++i)
    {
        JSON_Value* value = json_value_init_object();
        JSON_Object* obj = json_object(value);

        json_object_set_number(obj, "parent uuid", gosMeshes[i].first->GetParent()->GetUuid());
        json_object_set_number(obj, "owner uuid", gosMeshes[i].first->GetUuid());
        json_object_set_string(obj, "owner name", gosMeshes[i].first->GetName().c_str());
        json_object_set_string(obj, "mesh path", gosMeshes[i].second.c_str());

        ComponentMaterial* mat = (ComponentMaterial*)gosMeshes[i].first->GetComponent(ComponentType::MATERIAL);
        std::string texPath;

        if (mat->GetCurrentTexture() != nullptr)
            texPath = mat->GetCurrentTexture()->GetPath();

        json_object_set_string(obj, "texture path", texPath.size() > 0 ? texPath.c_str() : "Checkers");
        
        json_array_append_value(arr, value);
    }
    //json_serialize_to_file_pretty(modelValue, "Library/Models/model.json");

    int size = json_serialization_size_pretty(modelValue);
    char* buffer = new char[size];
    json_serialize_to_buffer(modelValue, buffer, size);


    std::string path = "Library/Models/" + go->GetName() + "_" + std::to_string(modelUuid) + ".kbmodel";
    json_serialize_to_file_pretty(modelValue, path.c_str());
    //json_serialize_to_file_pretty(modelValue, "Library/Models/model.json");

    //if (app->fileSystem->Save(path.c_str(), &buffer, size) > 0)
    //    LOG_CONSOLE("SALUDOS");

    delete[] buffer;

    //json_value_free(modelValue);
    
}

// Send the file name, NOT the path
GameObject* MeshLoader::LoadModelCustomFormat(const std::string& fileName)
{
    GameObject* ret = nullptr;

    std::string path = "Library/Models/" + fileName;

    char* buffer;
    if(app->fileSystem->Load(path.c_str(), &buffer) > 0)
    {
        modelValue = json_parse_string(buffer);
        JSON_Object* modelObj = json_value_get_object(modelValue);

        int s = fileName.find_last_of('/');
        int e = fileName.find_last_of('.');
        std::string n = fileName.substr(s + 1, e);

        int baseUuid = json_object_get_number(modelObj, "uuid");

        ret = new GameObject(n, baseUuid);
        app->scene->AddGameObject(ret);

        float3 p = { 0,0,0 };
        p.x = json_object_get_number(modelObj, "pos x");
        p.y = json_object_get_number(modelObj, "pos y");
        p.z = json_object_get_number(modelObj, "pos z");
        
        Quat r = { 0,0,0,1 };
        r.x = json_object_get_number(modelObj, "rot x");
        r.y = json_object_get_number(modelObj, "rot y");
        r.z = json_object_get_number(modelObj, "rot z");
        r.w = json_object_get_number(modelObj, "rot w");

        float3 sc = { 0,0,0 };
        sc.x = json_object_get_number(modelObj, "scale x");
        sc.y = json_object_get_number(modelObj, "scale y");
        sc.z = json_object_get_number(modelObj, "scale z");

        ComponentTransform* parentTr = (ComponentTransform*)ret->GetComponent(ComponentType::TRANSFORM);
        /*tr->SetTranslation(p);
        tr->SetRotation(r);
        tr->SetScale(sc);*/
        parentTr->SetLocalMatrix(float4x4::FromTRS(p, r, sc));


        JSON_Array* arr = json_object_get_array(modelObj, "Meshes");
        std::queue<GameObject*> q;

        int size = json_array_get_count(arr);
        for (int i = 0; i < size; ++i)
        {
            JSON_Object* obj = json_array_get_object(arr, i);
            int ownerUuid = json_object_get_number(obj, "owner uuid");
            int parentUuid = json_object_get_number(obj, "parent uuid");

            const char* ownerName = json_object_get_string(obj, "owner name");
            const char* meshPath = json_object_get_string(obj, "mesh path");
            const char* texPath = json_object_get_string(obj, "texture path");


            GameObject* owner = new GameObject(ownerName, ownerUuid);
            ComponentTransform* trans = (ComponentTransform*)owner->GetComponent(ComponentType::TRANSFORM);
            trans->SetLocalMatrix(parentTr->GetLocalMatrix());

            GameObject* parent = app->scene->GetGameObjectByUuid(parentUuid);
            if (parent)
            {
                parent->AddChild(owner);
            }

            ComponentMesh* meshComp = new ComponentMesh(owner, meshPath);

            KbMesh* mesh = LoadMeshCustomFormat(meshPath, owner);
            meshComp->SetData(mesh->vertices, mesh->indices);
            meshComp->SetParent(owner);

            ResourceManager::GetInstance()->CreateNewResource(texPath, ResourceType::TEXTURE);
            Texture* tex = TextureLoader::GetInstance()->LoadTextureCustomFormat(texPath);

            ComponentMaterial* matComp = new ComponentMaterial(owner);
            if(tex)
                matComp->AddTexture(tex);
            matComp->SetParent(owner);

            owner->AddComponent(meshComp);
            owner->AddComponent(matComp);
            //ret->AddChild(owner);
            owner->SetParent(ret);
            ret->SetGlobalAABB(*owner->GetGlobalAABB());
        }
    }
    //if (ret) app->scene->AddGameObject(ret);
    return ret;
}