#pragma once
#include <MBMatrix.h>
#include <string>
#include <unordered_map>
#define MBGE_BASE_TYPE double 
class GLFWwindow;
namespace MBGE
{
	//predekleration av klasser som pointers
	class Model;
	class MBGraphicsEngine;
	enum class VBTypes : unsigned int
	{
		StaticDraw,
		DynamicDraw,
		StreamDraw 
	};
	enum class DataTypes
	{
		Float,
		Int
	};
	class ShaderProgram;
	class VertexShader
	{
		friend class ShaderProgram;
	private:
		unsigned int ShaderHandle = 0;
	public: 
		VertexShader(std::string ShaderSourcePath);
		~VertexShader();
	};
	class GeometryShader
	{
		friend class ShaderProgram;
	private:
		unsigned int ShaderHandle = 0;
	public:
		GeometryShader(std::string ShaderSourcePath);
		~GeometryShader();
	};
	class FragmentShader
	{
		friend class ShaderProgram;
	private:
		unsigned int ShaderHandle = 0;
	public:
		FragmentShader(std::string ShaderSourcePath);
		~FragmentShader();
	};
	class ShaderProgram
	{
	private:
		unsigned int ProgramHandle = 0;
	public:
		ShaderProgram(VertexShader VerShader, FragmentShader FragShader);
		ShaderProgram(VertexShader VerShader, GeometryShader GeomShader,FragmentShader FragShader);
		void Bind();
		void SetUniform4f(std::string UniformName,float x,float y, float z, float w);
		void SetUniform4i(std::string UniformName, int x, int y, int z, int w);
		void SetUniform1i(std::string UniformName, int x);
		void SetUniform1f(std::string UniformName, float x);
		void SetUniformMat4f(std::string UniformName, float* RowMajorData);
		void SetUniformVec3(std::string Uniformname, float x, float y, float z);
		void SetUniformVec4(std::string Uniformname, float x, float y, float z,float w);
		~ShaderProgram();
	};
	struct VertexAttributeStruct
	{
		int SizeOfElements;
		int NumberOfElements;
		DataTypes DataType;
	};
	class VertexLayout
	{
	private:
		int TotalSize = 0;
		std::vector<VertexAttributeStruct> Attributes = {};
	public:
		void AddAttribute(int SizeOfElement, int NumberOfElements, DataTypes DataType);
		unsigned int VertexSize();
		void Bind();
	};
	class VertexBuffer
	{
	private:
		unsigned int BufferHandle = 0;
		unsigned int BufferSize = 0;
		unsigned int CurrentDrawType = 0;
	public:
		//semantiken av att kopiera implicit eller så makar inte för mycket sense
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(const VertexBuffer&) = delete;
		unsigned int SizeOfBuffer();
		VertexBuffer();
		VertexBuffer(VBTypes BufferType, unsigned long long InitialSize,  void* InitialData);
		void Bind();
		void Unbind();
		void DrawTriangles();
		void FillBuffer(unsigned int Offset,unsigned int NumberOfBytes,void* Data);
		void ResizeBuffer(unsigned int NewSize,void* NewData);
		~VertexBuffer();
	};
	class VertexArrayObject
	{
	private:
		unsigned int ObjectHandle = 0;
	public:
		VertexArrayObject();
		void Bind();
		void UnBind();
		~VertexArrayObject();
	};
	class Vertex
	{
	private:
		unsigned char* VertexData = nullptr;
		unsigned int DataSize = 0;
		void Swap(Vertex& OtherVertex);
	public:
		Vertex() {};
		MBMath::MBVector3<float> Position;
		Vertex(Vertex& VertexToCopy);
		Vertex(Vertex&& VertexToMove) noexcept;
		Vertex& operator=(Vertex VertexToCopy) noexcept
		{
			Position = VertexToCopy.Position;
			Swap(VertexToCopy);
		}
		Vertex& operator=(Vertex&& VertexToMove) noexcept
		{
			Position = VertexToMove.Position;
			Swap(VertexToMove);
		}
		Vertex(int NumberOfBytes, void* Data);
		~Vertex();
	};
	class ElementBufferObject
	{
	private:
		unsigned int ObjectHandle = 0;
	public:
		ElementBufferObject(unsigned int NumberOfElements, unsigned int* Data);
		ElementBufferObject();
		//void DrawTriangles();
		void FillBuffer(unsigned int Offset, unsigned int NumberOfBytes, void* Data);
		void ResizeBuffer(unsigned int NewSize, void* NewData);
		void Bind();
		void UnBind();
		~ElementBufferObject();
	};
	enum class VertexAttributes
	{
		Tangent,
		Bitangent,
		VertexColors,
		VertexNormal,
		TextureCoordinates
	};
	enum class MaterialAttribute
	{
		DiffuseTexture,
		NormalTexture,
		SpecularTexture
	};
	//TODO Typedefa pointersen till grejerna, void pointers är lite läskiga
	struct Vec3Data
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};
	class Mesh
	{
		//alla koordinater är givna i lokala koordinater med ett implicit centrum av (0,0,0)
	private:
		std::vector<unsigned char> VerticesData = {};
		unsigned int VertexSize = 0;
		unsigned int VerticesCount = 0;
		unsigned int DrawVerticesCount = 0;
		unsigned int MaterialIndex = -1;
		//bygger på antagandet att varje Mesh kan ha en olik layout inom en model
		VertexArrayObject VAO;
		VertexLayout Layout;
		VertexBuffer Buffer;
		ElementBufferObject ArrayObject;
		Model* AssociatedModel = nullptr;
		std::vector<Vec3Data> SavedPositionData = {};
	public:
		//TODO fixa faktiska copy semantic etc för mesh objektet
		Mesh(int VerticesToLoadCount, int VertexToLoadSize, void* VertexToLoadData, int ArrayObjectSize, unsigned int* ArrayObjectData);
		Mesh(void* MeshObject,std::vector<VertexAttributes> AttributeOrder, Model* ModelToBelong);
		void Rotate(float DegressToRotate, MBMath::MBVector3<float> AxisToRotate);
		void Draw();
		void SavePositions();
		void RestorePositions();
		void TransformPositions(MBMath::MBMatrix4<float> Transformation);
		unsigned int NumberOfVertices();
		~Mesh();
	};
	//TODO skapa copysemantics och liknande för nod objektet, är shallow copy just nu
	class Node
	{
	private:
		Node* ParentNode = nullptr;
		std::vector<Node*> ChildNodes = {};
		std::vector<unsigned int> MeshIndexes = {};
		MBMath::MBMatrix4<float> LocalTransformation = MBMath::MBMatrix4<float>();
		Model* AssociatedModel = nullptr;
		void Swap(Node& OtherNode);
	public:
		Node& operator=(Node OtherNode)
		{
			this->Swap(OtherNode);
			return(*this);
		}
		Node() {};
		Node(void* NodeData, Node* ParentNode,Model* ModelToBelong);
		void Draw(MBMath::MBMatrix4<float> ParentrTransformation);
		//kommer med detta behöva en egentlig copy constructor och etc
		~Node();
	};
	class Material
	{
	private:
		std::vector<std::string> GetTexturePaths(void* MaterialData, int TextureType);
		MBGraphicsEngine* AssciatedGraphicsEngine;
	public:
		std::vector<MaterialAttribute> MaterialShaderAttributes = { MaterialAttribute::DiffuseTexture,MaterialAttribute::NormalTexture,MaterialAttribute::SpecularTexture };
		std::string DiffuseTexture = "";
		MBMath::MBVector3<float> Color = MBMath::MBVector3<float>(0, 0, 0);
		std::string SpecularTexture = "";
		float SpecularStrength = -1;
		float SpecularExp = -1;
		std::string NormalTexture = "";
		Material(void* MaterialData,std::string PathToModel,MBGraphicsEngine* AttachedEngine);
		Material(void* MaterialData,std::string PathToModel,std::vector<MaterialAttribute> NewMaterialAttributes,MBGraphicsEngine* AttachedEngine);
		void SetUniforms();
	};
	class Model
	{
	private:
		//eftersom position och eventuellt vinklar är sådant som vi inte nödvändigtvis vill kunna ändra utan vidare
		MBMath::MBVector3<float> WorldPosition = MBMath::MBVector3<float>(0, 0, 0);
		std::vector<Mesh*> ModelMeshes = {};
		//en models position är given i world coordinater
		std::vector<Material> ModelMaterials = {};
		ShaderProgram* ModelShaderProgram = nullptr;
		Node TopNode;
		std::vector<MaterialAttribute> ModelShaderAttributes = {MaterialAttribute::DiffuseTexture,MaterialAttribute::NormalTexture,MaterialAttribute::SpecularTexture};
		//eftersom vi inte vill ha assimp i headefilen så passas en voidpointer utifall jag byter backend
		void ProcessNode(void* Node, void* Scene);
	public:
		std::string ModelShader = "";
		MBGraphicsEngine* AssociatedEngine = nullptr;
		void Rotate(float DegressToRotate, MBMath::MBVector3<float> AxisToRotate);
		void Draw();
		Mesh* GetMesh(unsigned int MeshIndex);
		Material* GetMaterial(unsigned int MaterialIndex);
		Model(std::string ModelPath,MBGraphicsEngine* AttachedEngine);
		Model(std::string ModelPath,std::vector<MaterialAttribute> MaterialAttributes,MBGraphicsEngine* AttachedEngine);
		//void SetRotation(float XRotation, float YRotation, float ZRotation);
		//MBMath::MBVector3<float> GetRotation();
	};
	class Camera
	{
	private:
		MBMath::MBVector3<float> Facing = MBMath::MBVector3<float>(0, 0, -1);
		MBMath::MBVector3<float> RightAxis = MBMath::MBVector3<float>(1, 0, 0);
		MBMath::MBVector3<float> UpAxis = MBMath::MBVector3<float>(0, 1, 0);
		MBMath::MBMatrix4<float> ProjectionMatrix = MBMath::MBMatrix4<float>();
		MBMath::MBVector3<float> Rotation = MBMath::MBVector3<float>(0, 0, 0);
		MBMath::MBMatrix4<float> ModelMatrix = MBMath::MBMatrix4<float>();
		MBMath::MBMatrix4<float> NormalMatrix = MBMath::MBMatrix4<float>();
		float FieldOfViewY = 45;
		float FieldOfViewX = 45;
		MBGraphicsEngine* AssociatedGraphicsEngine = nullptr;
	public:
		MBMath::MBVector3<float> WorldSpaceCoordinates = MBMath::MBVector3<float>(0, 0, -4);

		MBMath::MBVector3<float> GetRotation();
		void SetRotation(float XaxisRotation, float YaxisRotation, float ZAxisRotation);
		void SetRotation(MBMath::MBVector3<float> NewRotation);
		void SetModelMatrix(MBMath::MBMatrix4<float> ModelMatrix);
		void Update();
		MBMath::MBVector3<float> GetDirection();
		MBMath::MBVector3<float> GetRightAxis();
		MBMath::MBVector3<float> GetUpAxis();
		bool IsOrtoGraphic = false;
		void SetFrustum(float NearPlane, float FarPlane, float XMin, float XMax, float YMin, float YMax);
		Camera();
		Camera(MBGraphicsEngine* EngineToAttach);
		MBMath::MBMatrix4<float> GetTransformationMatrix();
	};
	class LightSource
	{
	private:
		MBGraphicsEngine* AssociatedGraphicsEngine;
	public:
		MBMath::MBVector3<float> WorldPosition = MBMath::MBVector3<float>(0, 0, 0);
		MBMath::MBVector3<float> LightColor = MBMath::MBVector3<float>(1, 1, 1);
		float AmbienceStrength = 0.1;
		float SpecularExp = 32;
		float SpecularStrength = 0.3;
		void SetLightning(int Position);
		LightSource(MBGraphicsEngine* AttachedEngine);
	};
	class Texture
	{
	private:
		unsigned int TextureHandle = 0;
	public:
		Texture(std::string FilePath);
		void Bind(int TextureLocation);
		void UnBind(int TextureLocation);
		~Texture();
	};

	class MBGraphicsEngine
	{
	private:
		GLFWwindow* Window;
		std::string ResourceFolder = "";
		std::vector<Model*> LoadedModels = {};
		std::string CurrentShaderID = "";
		std::unordered_map<std::string, ShaderProgram*> LoadedShaders = {};
		std::unordered_map<std::string, Texture*> LoadedTextures = {};
		std::vector<LightSource*> LightSources = {};
	public:
		Camera CameraObject = Camera(this);
		void UpdateUniforms();
		ShaderProgram* GetCurrentShader();
		LightSource* AddLightSource();
		void SetCurrentShader(std::string ShaderID);
		MBGraphicsEngine();
		bool GetKeyUp(unsigned int KeyCode);
		bool GetKey(unsigned int KeyCode);
		Model* LoadModel(std::string ModelPath,std::vector<MaterialAttribute> MaterialAttributes);
		ShaderProgram* LoadShader(std::string ShaderID,std::string VertexShaderPath,std::string FragmentShaderPath);
		ShaderProgram* LoadShader(std::string ShaderID,std::string VertexShaderPath,std::string GeometryShaderPath,std::string FragmentShaderPath);
		ShaderProgram* GetShader(std::string ShaderID);
		Texture* GetTexture(std::string TextureFilePath);
		std::string GetResourceFolder();
		void Update();
		void CreateWindow();
	};
};