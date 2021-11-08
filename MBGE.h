#pragma once
#include <MBMatrix.h>
#include <string>
#include <unordered_map>
#include <time.h>
#include <thread>
#define MBGE_BASE_TYPE double 
class GLFWwindow;
#define MBGE_BONEPERVERTEX 5
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
		unsigned int m_ShaderHandle = 0;
	public: 
		VertexShader(VertexShader const&) = delete;
		VertexShader& operator=(VertexShader const&) = delete;

		VertexShader(VertexShader&& ShaderToSteal) noexcept;
		VertexShader& operator=(VertexShader&&) noexcept;

		VertexShader(std::string const& ShaderSourcePath);
		~VertexShader();
	};
	class GeometryShader
	{
		friend class ShaderProgram;
	private:
		unsigned int m_ShaderHandle = 0;
	public:
		GeometryShader(GeometryShader const&) = delete;
		GeometryShader& operator=(GeometryShader const&) = delete;

		GeometryShader(GeometryShader&& ShaderToSteal) noexcept;
		GeometryShader& operator=(GeometryShader&&) noexcept;

		GeometryShader(std::string const& ShaderSourcePath);
		~GeometryShader();
	};
	class FragmentShader
	{
		friend class ShaderProgram;
	private:
		unsigned int m_ShaderHandle = 0;
	public:
		FragmentShader(FragmentShader const&) = delete;
		FragmentShader& operator=(FragmentShader const&) = delete;

		FragmentShader(FragmentShader&& ShaderToSteal) noexcept;
		FragmentShader& operator=(FragmentShader&&) noexcept;

		FragmentShader(std::string const& ShaderSourcePath);
		~FragmentShader();
	};
	class ShaderProgram
	{
	private:
		unsigned int m_ProgramHandle = 0;
	public:
		ShaderProgram(ShaderProgram const&) = delete;
		ShaderProgram& operator=(ShaderProgram const&) = delete;

		ShaderProgram(ShaderProgram&& ShaderToSteal) noexcept;
		ShaderProgram& operator=(ShaderProgram&&) noexcept;

		ShaderProgram(VertexShader const& VerShader, FragmentShader const& FragShader);
		ShaderProgram(VertexShader const& VerShader, GeometryShader const& GeomShader,FragmentShader const& FragShader);
	
		void Bind();
		void SetUniform4f(std::string const& UniformName,float x,float y, float z, float w);
		void SetUniform4i(std::string const& UniformName, int x, int y, int z, int w);
		void SetUniform1i(std::string const& UniformName, int x);
		void SetUniform1f(std::string const& UniformName, float x);
		void SetUniformMat4f(std::string const& UniformName, float* RowMajorData);
		void SetUniformVec3(std::string const& Uniformname, float x, float y, float z);
		void SetUniformVec4(std::string const& Uniformname, float x, float y, float z,float w);
		void PrintActiveAttributesAndUniforms();
		
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
		unsigned int m_BufferHandle = 0;
		unsigned int m_BufferSize = 0;
		unsigned int m_CurrentDrawType = 0;
	public:
		//semantiken av att kopiera implicit eller så makar inte för mycket sense
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& BufferToSteal) noexcept;
		VertexBuffer& operator=(VertexBuffer&& BufferToSteal) noexcept;

		unsigned int SizeOfBuffer();
		VertexBuffer();
		VertexBuffer(VBTypes BufferType, unsigned long long InitialSize,  const void* InitialData);
		void Bind();
		void Unbind();
		void DrawTriangles();
		void FillBuffer(unsigned int Offset,unsigned int NumberOfBytes,const void* Data);
		void ResizeBuffer(unsigned int NewSize,const void* NewData);
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
		Vertex(Vertex const& VertexToCopy);
		Vertex(Vertex&& VertexToMove) noexcept;
		Vertex& operator=(Vertex VertexToCopy) noexcept
		{
			Swap(VertexToCopy);
		}
		Vertex& operator=(Vertex&& VertexToMove) noexcept
		{
			Swap(VertexToMove);
		}
		Vertex(int NumberOfBytes, void* Data);
		~Vertex();
	};
	class ElementBufferObject
	{
	private:
		unsigned int m_ObjectHandle = 0;
	public:
		ElementBufferObject(ElementBufferObject const& ObjectToCopy) = delete;
		ElementBufferObject& operator=(ElementBufferObject const& ObjectToCopy) = delete;

		ElementBufferObject(ElementBufferObject&& ObjectToSteal) noexcept;
		ElementBufferObject& operator=(ElementBufferObject&& ObjectToCopy) noexcept;

		ElementBufferObject(unsigned int NumberOfElements, const unsigned int* Data);
		ElementBufferObject();
		//void DrawTriangles();
		void FillBuffer(unsigned int Offset, unsigned int NumberOfBytes, const void* Data);
		void ResizeBuffer(unsigned int NewSize, const void* NewData);
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
	struct BoneWeight
	{
		unsigned int VertexIndex = -1;
		float Weight = 0;
	};
	class Bone
	{
	private:

	public:
		std::string BoneID = "";
		int BoneIndex = -1;
		MBMath::MBMatrix4<float> OffsetMatrix = MBMath::MBMatrix4<float>();
		std::vector<BoneWeight> Weights = {};
		Bone(void* AssimpBoneData);
		Bone() { };
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
		std::vector<Node> ChildNodes = {};
		std::vector<unsigned int> MeshIndexes = {};
		MBMath::MBMatrix4<float> LocalTransformation = MBMath::MBMatrix4<float>();
		Model* AssociatedModel = nullptr;
		std::string NodeID = "";
		void Swap(Node& OtherNode);

		void p_UpdateChildParents();
	public:
		Node(Node const& OtherNode) = delete;

		Node& operator=(Node OtherNode) noexcept
		{
			this->Swap(OtherNode);
			p_UpdateChildParents();
			return(*this);
		}
		Node(Node&& OtherNode) noexcept
		{
			Swap(OtherNode);
			p_UpdateChildParents();
		}
		Node() {};
		Node(void* NodeData, Node* ParentNode,Model* ModelToBelong);
		MBMath::MBMatrix4<float> GetLocalTransformation() { return(LocalTransformation); };
		void Draw(MBMath::MBMatrix4<float> ParentrTransformation);
		void DrawAnimation();
		void UpdateBones(MBMath::MBMatrix4<float> ParentTransformation);
		//kommer med detta behöva en egentlig copy constructor och etc
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
	struct NodeAnimationRotationKey
	{
		float KeyTime = 0;
		MBMath::Quaternion<float> Rotation;
	};
	struct NodeAnimationPositionKey
	{
		float KeyTime = 0;
		MBMath::MBVector3<float> Position;
	};
	struct NodeAnimationScalingKey
	{
		float KeyTime = 0;
		MBMath::MBVector3<float> Scaling;
	};
	class NodeAnimation
	{
		double TickDuration = 0;
		double TicksPerSec = 0;
		std::vector<NodeAnimationScalingKey> ScalingKeys = {};
		std::vector<NodeAnimationRotationKey> RotationKeys = {};
		std::vector<NodeAnimationPositionKey> TranslationKeys = {};
		MBMath::MBMatrix4<float> GetInterpolatedScaling(float TimeInSec);
		MBMath::MBMatrix4<float> GetInterpolatedRotation(float TimeInSec);
		MBMath::MBMatrix4<float> GetInterpolatedTranslation(float TimeInSec);
	public:
		NodeAnimation(void* AssimpData, double AnimationDuration, double TicksPerSec);
		MBMath::MBMatrix4<float> GetTransformation(float TimeInSec);
		NodeAnimation() {};
	};
	class ModelAnimation
	{
	private:
		double TickDuration = 0;
		double TicksPerSec = 0;
		std::unordered_map<std::string,NodeAnimation> NodeAnimations = {};
	public:
		bool IsInAnimation(std::string NodeID) { return(NodeAnimations.find(NodeID) != NodeAnimations.end()); };
		MBMath::MBMatrix4<float> GetNodeTransformation(float TimeInSec, std::string NodeId);
		double GetDurationInSec() { return(TickDuration / TicksPerSec); };
		ModelAnimation(void* AssimpData);
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
		std::vector<MaterialAttribute> ModelShaderAttributes = { MaterialAttribute::DiffuseTexture,MaterialAttribute::NormalTexture,MaterialAttribute::SpecularTexture };
		//eftersom vi inte vill ha assimp i headefilen så passas en voidpointer utifall jag byter backend
		void ProcessNode(void* Node, void* Scene);
		bool AnimationIsPlaying = false;
		double AnimationTime = 0;
		std::vector<ModelAnimation> Animations = {};
	public:
		int NumberOfNodes = 0;
		std::vector<std::string> NodeNames = {};
		ModelAnimation* GetCurrentAnimation();
		bool IsPlayingAnimation() { return(AnimationIsPlaying); };
		double GetAnimationTimeInSec(){return(AnimationTime);};
		MBMath::MBMatrix4<float> InverseGlobalMatrix = MBMath::MBMatrix4<float>();
		std::vector<MBMath::MBMatrix4<float>> BoneOffsetList = std::vector<MBMath::MBMatrix4<float>>(100, MBMath::MBMatrix4<float>());
		std::unordered_map<std::string, Bone> BoneMap = {};
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
	class FrameBuffer
	{
	private:
		unsigned int FrameBufferHandle = -1;
		unsigned int ColorTextureHandle = -1;
		unsigned int DepthStencilTextureHandle = -1;
	public:
		FrameBuffer(int Width,int Heigh);
		void Bind();
		void BindColorBuffer(int Index);
		void BindDepthBuffer(int Index);
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
		VertexArrayObject* PBS_VertexArray;
		VertexLayout* PBS_Layout;
		VertexBuffer* PBS_VertexBuffer;
		ElementBufferObject* PBS_ElementBuffer;
		FrameBuffer* PostProcessingFrameBuffer;
		void DrawPostProcessing();
		clock_t DeltaTimeTimer = clock();
		double DeltaTime = 0;
		//std::thread PollEventsThread;
	public:
		bool FrameByFrame = false;
		double GetDeltaTimeInSec() { return(DeltaTime); };
		std::string PostProcessingShaderID = "Default_PPS";
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
		void PollEvents();
		std::string GetResourceFolder();
		void Update();
		void CreateWindow();
	};
};