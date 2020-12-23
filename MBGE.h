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
		void FillBuffer(char* Data);
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
	class Mesh
	{
		//alla koordinater är givna i lokala koordinater med ett implicit centrum av (0,0,0)

	};
	class Model
	{
	private:
		//eftersom position och eventuellt vinklar är sådant som vi inte nödvändigtvis vill kunna ändra utan vidare
		MBMath::MBVector3<float> Position = MBMath::MBVector3<float>(0, 0, 0);
		std::vector<Mesh> ModelMeshes = {};
		//en models position är given i world coordinater
	};
	class ElementBufferObject
	{
	private:
		unsigned int ObjectHandle = 0;
	public:
		ElementBufferObject(unsigned int NumberOfElements, unsigned int* Data);
		void DrawTriangles();
		void Bind();
		void UnBind();
	};
	class Camera
	{
	private:
		MBMath::MBVector3<float> Facing = MBMath::MBVector3<float>(0, 0, 1);
		MBMath::MBVector3<float> Normal = MBMath::MBVector3<float>(0, 1, 0);
		float FieldOfViewY = 45;
		float FieldOfViewX = 45;
	public:
		bool IsOrtoGraphic = false;

		MBMath::MBVector3<float> Position = MBMath::MBVector3<float>(0, 0, 0);
		Camera();
		MBMath::MBMatrix<float> GetTransformationMatrix();
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
		void AddModel(Model* ModelToAdd);
		std::string GetResourceFolder();
		void Update();
		void CreateWindow();
	};
};