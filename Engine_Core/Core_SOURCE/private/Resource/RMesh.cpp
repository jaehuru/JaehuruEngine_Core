#include "Resource/RMesh.h"

RMesh::FMeshData::FMeshData()
	: mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, vertices{}
	, indices{}
{

}

RMesh::FMeshData::~FMeshData()
{

}

RMesh::RMesh()
	: RResource(EResourceType::RMesh)
{

}

RMesh::~RMesh()
{

}

HRESULT RMesh::Save(const wstring& path)
{
	return S_OK;
}

HRESULT RMesh::Load(const wstring& path)
{

	return S_OK;
}

void RMesh::Serialize(json& jsonObject) const
{
    RResource::Serialize(jsonObject);
    jsonObject["Topology"] = static_cast<int>(mData.mTopology);

    json verticesArray = json::array();
    for (const auto& vertex : mData.vertices)
    {
        json vertexJson;
        vertexJson["PositionX"] = vertex.pos.x;
        vertexJson["PositionY"] = vertex.pos.y;
        vertexJson["PositionZ"] = vertex.pos.z;
        vertexJson["TexCoordX"] = vertex.uv.x;
        vertexJson["TexCoordY"] = vertex.uv.y;
        verticesArray.push_back(vertexJson);
    }
    jsonObject["Vertices"] = verticesArray;

    json indicesArray = json::array();
    for (const auto& index : mData.indices)
    {
        indicesArray.push_back(index);
    }
    jsonObject["Indices"] = indicesArray;
}

void RMesh::Deserialize(const json& jsonObject)
{
    RResource::Deserialize(jsonObject);
    mData.mTopology = static_cast<D3D11_PRIMITIVE_TOPOLOGY>(jsonObject["Topology"]);

    const json& verticesArray = jsonObject["Vertices"];
    for (const auto& vertexJson : verticesArray)
    {
        FVertex vertex;
        vertex.pos.x = vertexJson["PositionX"];
        vertex.pos.y = vertexJson["PositionY"];
        vertex.pos.z = vertexJson["PositionZ"];
        vertex.uv.x = vertexJson["TexCoordX"];
        vertex.uv.y = vertexJson["TexCoordY"];
        mData.vertices.push_back(vertex);
    }

    const json& indicesArray = jsonObject["Indices"];
    for (const auto& index : indicesArray)
    {
        mData.indices.push_back(index);
    }

    // Recreate buffers after deserialization
    CreateVB(mData.vertices);
    CreateIB(mData.indices);
}

bool RMesh::CreateVB(const vector<FVertex>& vertices)
{
	mData.vertices = vertices;
	return mVB.Create(vertices);
}

bool RMesh::CreateIB(const vector<UINT>& indices)
{
	mData.indices = indices;
	return mIB.Create(indices);
}

void RMesh::SetVertexBufferParams(UINT vertexCount, D3D11_INPUT_ELEMENT_DESC* layout, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength)
{
	mInputLayout.CreateInputLayout(vertexCount, layout, pShaderBytecodeWithInputSignature, BytecodeLength);
}

void RMesh::Bind()
{
	mInputLayout.Bind();

	mVB.Bind();
	mIB.Bind();

	GetDevice()->BindPrimitiveTopology(mData.mTopology);
}
