#pragma once
#include <MBMatrix.h>
#include <string>
#define MBGE_BASE_TYPE double 
class GLFWwindow;
namespace MBGE
{
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
		void Bind();
		void SetUniform4f(std::string UniformName,float x,float y, float z, float w);
		void SetUniform4i(std::string UniformName, int x, int y, int z, int w);
		void SetUniform1i(std::string UniformName, int x);
		void SetUniform1f(std::string UniformName, float x);
		void SetUniformMat4f(std::string UniformName, float* RowMajorData);
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
		//void DrawTriangles();
		void Bind();
		void UnBind();
	};
	class Mesh
	{
		//alla koordinater är givna i lokala koordinater med ett implicit centrum av (0,0,0)
	private:
		std::vector<unsigned char> VerticesData = {};
		unsigned int VertexSize = 0;
		unsigned int VerticesCount = 0;
		unsigned int DrawVerticesCount = 0;
		VertexArrayObject VAO;
		VertexLayout Layout;
		VertexBuffer Buffer;
		ElementBufferObject ArrayObject;
	public:
		//TODO fixa faktiska copy semantic etc för mesh objektet
		Mesh(int VerticesToLoadCount, int VertexToLoadSize, void* VertexToLoadData, int ArrayObjectSize, unsigned int* ArrayObjectData);
		void Rotate(float DegressToRotate, MBMath::MBVector3<float> AxisToRotate);
		void Draw();
		unsigned int NumberOfVertices();
		~Mesh();
	};
	class Model
	{
	private:
		//eftersom position och eventuellt vinklar är sådant som vi inte nödvändigtvis vill kunna ändra utan vidare
		MBMath::MBVector3<float> WorldPosition = MBMath::MBVector3<float>(0, 0, 0);
		//MBMath::
		std::vector<Mesh> ModelMeshes = {};
		//en models position är given i world coordinater
	public:
		void Rotate(float DegressToRotate, MBMath::MBVector3<float> AxisToRotate);
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
		float FieldOfViewY = 45;
		float FieldOfViewX = 45;
	public:
		MBMath::MBVector3<float> GetRotation();
		void SetRotation(float XaxisRotation, float YaxisRotation, float ZAxisRotation);
		void SetRotation(MBMath::MBVector3<float> NewRotation);
		MBMath::MBVector3<float> GetDirection();
		MBMath::MBVector3<float> GetRightAxis();
		MBMath::MBVector3<float> GetUpAxis();
		bool IsOrtoGraphic = false;
		MBMath::MBVector3<float> WorldSpaceCoordinates = MBMath::MBVector3<float>(0, 0, -4);
		void SetFrustum(float NearPlane, float FarPlane, float XMin, float XMax, float YMin, float YMax);
		Camera();
		MBMath::MBMatrix4<float> GetTransformationMatrix();
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

	public:
		Camera CameraObject = Camera();
		MBGraphicsEngine();
		bool GetKeyUp(unsigned int KeyCode);
		bool GetKey(unsigned int KeyCode);
		void AddModel(Model* ModelToAdd);
		std::string GetResourceFolder();
		void Update();
		void CreateWindow();
	};
};