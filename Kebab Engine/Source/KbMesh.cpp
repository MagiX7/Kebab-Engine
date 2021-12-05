#include "KbMesh.h"

KbMesh::KbMesh(std::string path) : Resource(ResourceType::MESH)
{
	textureMetaPath = "";
	SetPath(path);
}

KbMesh::KbMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) : Resource(ResourceType::MESH)
{
	textureMetaPath = "";
	SetData(vertices, indices);
}

KbMesh::~KbMesh()
{
	//delete vertexBuffer;
	vertexBuffer = nullptr;

	//delete indexBuffer;
	indexBuffer = nullptr;

	vertices.clear();
	indices.clear();
	
	path.clear();
	name.clear();
}

void KbMesh::SetData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	this->vertices = vertices;
	this->indices = indices;

	SetUpMesh();
}

void KbMesh::BeginDraw()
{
	vertexBuffer->Bind();
	indexBuffer->Bind();
}

void KbMesh::EndDraw()
{
	vertexBuffer->Unbind();
	indexBuffer->Unbind();
}

void KbMesh::DrawVertexNormals(const float& size, const float3& color)
{
	glBegin(GL_LINES);

	glColor3f(color.x, color.y, color.z);

	for (int i = 0; i < vertices.size(); ++i)
	{
		float3 pos = vertices[i].position;
		glVertex3f(pos.x, pos.y, pos.z);

		/*if (isKbGeometry)
		{
			float3 normal = vertices[i].normal;
			normal.Normalize();

			glVertex3f(pos.x + normal.x * normalsVertexSize, pos.y, pos.z);

			glVertex3f(pos.x, pos.y, pos.z);
			glVertex3f(pos.x, pos.y + normal.y * normalsVertexSize, pos.z);

			glVertex3f(pos.x, pos.y, pos.z);
			glVertex3f(pos.x, pos.y, pos.z + normal.z * normalsVertexSize);
		}
		else*/
		{
			float3 n;
			n.x = (vertices[i].position.x + vertices[i].normal.x * size);
			n.y = (vertices[i].position.y + vertices[i].normal.y * size);
			n.z = (vertices[i].position.z + vertices[i].normal.z * size);
			glVertex3f(n.x, n.y, n.z);
		}
	}

	glColor3f(1, 1, 1);

	glEnd();
}

void KbMesh::DrawTriangleNormals(const float& size, const float3& color)
{
	glBegin(GL_LINES);

	glColor3f(color.x, color.y, color.z);

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		float3 v1 = vertices[indices[i]].position;
		float3 v2 = vertices[indices[i + 1]].position;
		float3 v3 = vertices[indices[i + 2]].position;

		float3 tNorm = math::Cross(v2 - v1, v3 - v1);
		tNorm.Normalize();
		//tNorm *= 0.5f;

		float3 tCent;
		tCent.x = (v1.x + v2.x + v3.x) / 3;
		tCent.y = (v1.y + v2.y + v3.y) / 3;
		tCent.z = (v1.z + v2.z + v3.z) / 3;

		glVertex3f(tCent.x, tCent.y, tCent.z);
		glVertex3f(tCent.x + tNorm.x * size, tCent.y + tNorm.y * size, tCent.z + tNorm.z * size);
	}

	glColor3f(1, 1, 1);

	glEnd();
}

void KbMesh::SetPath(const std::string& path)
{
	this->path = path;

	int start = path.find_last_of('\\');
	if (start <= 0)
		start = path.find_last_of('/');

	int end = path.find(".");
	name = path.substr(start + 1, end - start - 1);
}

void KbMesh::SetName(const std::string& name)
{
	this->name = name;
}

void KbMesh::SetOwnerName(const std::string& n)
{
	ownerName = n;
}

void KbMesh::SetUpMesh()
{
	vertexBuffer = new VertexBuffer();
	vertexBuffer->SetData(vertices);

	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}