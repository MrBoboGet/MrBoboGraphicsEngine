#include <vector>
#include <fstream>

#include <MBGE.h>

#include <gl3w/include/GL/gl3w.h>
#include <glfw/include/GLFW/glfw3.h>
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>
#include <stb/stb_image.h>
#include <assert.h>
#include <filesystem>
#include <stack>
#include <MBUtility/MBFiles.h>

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 
namespace MBGE
{
	unsigned int h_CreateShader(const void* ShaderData,GLint DataSize,unsigned int ShaderType)
	{
		unsigned int ReturnValue = 0;
		const char* ShaderSourcePointer =(const char*) ShaderData;
		ReturnValue = glCreateShader(ShaderType);
		glShaderSource(ReturnValue, 1, &ShaderSourcePointer, &DataSize);
		glCompileShader(ReturnValue);
		//checka om det lyckades
		int  success;
		char infoLog[512];
		glGetShaderiv(ReturnValue, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ReturnValue, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
		return(ReturnValue);
	}
	//Vertexshader
	VertexShader::VertexShader(std::string const& SourcePath)
	{
		std::string ShaderData = MBUtility::ReadWholeFile(SourcePath);
		m_ShaderHandle = h_CreateShader(ShaderData.data(), ShaderData.size(), GL_VERTEX_SHADER);
	}
	VertexShader::VertexShader(const void* Data, size_t DataSize)
	{
		m_ShaderHandle = h_CreateShader(Data, DataSize, GL_VERTEX_SHADER);
	}
	VertexShader::VertexShader(VertexShader&& ShaderToSteal) noexcept
	{
		std::swap(m_ShaderHandle, ShaderToSteal.m_ShaderHandle);
	}
	VertexShader& VertexShader::operator=(VertexShader&& ShaderToSteal) noexcept
	{
		std::swap(m_ShaderHandle, ShaderToSteal.m_ShaderHandle);
		return(*this);
	}
	VertexShader::~VertexShader()
	{
		if (m_ShaderHandle != 0)
		{
			glDeleteShader(m_ShaderHandle);
		}
		glCheckError();
	}
	//geometry shader
	GeometryShader::GeometryShader(std::string const& SourcePath)
	{
		std::string ShaderData = MBUtility::ReadWholeFile(SourcePath);
		m_ShaderHandle = h_CreateShader(ShaderData.data(), ShaderData.size(), GL_GEOMETRY_SHADER);
	}
	GeometryShader::GeometryShader(const void* Data, size_t DataSize)
	{
		m_ShaderHandle = h_CreateShader(Data, DataSize, GL_GEOMETRY_SHADER);
	}
	GeometryShader::GeometryShader(GeometryShader&& ShaderToSteal) noexcept
	{
		std::swap(m_ShaderHandle, ShaderToSteal.m_ShaderHandle);
	}
	GeometryShader& GeometryShader::operator=(GeometryShader&& ShaderToSteal) noexcept
	{
		std::swap(m_ShaderHandle, ShaderToSteal.m_ShaderHandle);
		return(*this);
	}
	GeometryShader::~GeometryShader()
	{
		if (m_ShaderHandle != 0)
		{
			glDeleteShader(m_ShaderHandle);
		}
	}
	//fragmentShader
	FragmentShader::FragmentShader(std::string const& SourcePath)
	{
		std::string ShaderData = MBUtility::ReadWholeFile(SourcePath);
		m_ShaderHandle = h_CreateShader(ShaderData.data(), ShaderData.size(), GL_FRAGMENT_SHADER);
	}
	FragmentShader::FragmentShader(const void* Data, size_t DataSize)
	{
		m_ShaderHandle = h_CreateShader(Data, DataSize, GL_FRAGMENT_SHADER);
	}
	FragmentShader::FragmentShader(FragmentShader&& ShaderToSteal) noexcept
	{
		std::swap(m_ShaderHandle, ShaderToSteal.m_ShaderHandle);
	}
	FragmentShader& FragmentShader::operator=(FragmentShader&& ShaderToSteal) noexcept
	{
		std::swap(m_ShaderHandle, ShaderToSteal.m_ShaderHandle);
		return(*this);
	}
	FragmentShader::~FragmentShader()
	{
		if (m_ShaderHandle != 0)
		{
			glDeleteShader(m_ShaderHandle);
		}
		glCheckError();
	}
	//ShaderProgram
	ShaderProgram::ShaderProgram(VertexShader const& VerShader, FragmentShader const& FragShader)
	{
		glCheckError();
		m_ProgramHandle = glCreateProgram();
		glAttachShader(m_ProgramHandle, VerShader.m_ShaderHandle);
		glAttachShader(m_ProgramHandle, FragShader.m_ShaderHandle);
		glLinkProgram(m_ProgramHandle);
		int  success;
		char infoLog[512];
		glGetProgramiv(m_ProgramHandle, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ProgramHandle, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
	}
	ShaderProgram::ShaderProgram(ShaderProgram&& ShaderToSteal) noexcept
	{
		std::swap(m_ProgramHandle, ShaderToSteal.m_ProgramHandle);
	}
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& ShaderToSteal) noexcept
	{
		std::swap(m_ProgramHandle, ShaderToSteal.m_ProgramHandle);
		return(*this);
	}
	ShaderProgram::ShaderProgram(VertexShader const& VerShader, GeometryShader const& GeomShader, FragmentShader const& FragShader)
	{
		glCheckError();
		m_ProgramHandle = glCreateProgram();
		glAttachShader(m_ProgramHandle, VerShader.m_ShaderHandle);
		glAttachShader(m_ProgramHandle, FragShader.m_ShaderHandle);
		glAttachShader(m_ProgramHandle, GeomShader.m_ShaderHandle);
		glLinkProgram(m_ProgramHandle);
		int  success;
		char infoLog[512];
		glGetProgramiv(m_ProgramHandle, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ProgramHandle, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
	}
	void ShaderProgram::Bind()
	{
		glUseProgram(m_ProgramHandle);
		glCheckError();
	}
	constexpr bool MBGE_CHECK_UNIFORM_VALIDITY = false;
	void ShaderProgram::p_HandleInvalidPosition(int PositionToCheck, std::string const& UniformName)
	{
		if (PositionToCheck == -1)
		{
			if constexpr (MBGE_CHECK_UNIFORM_VALIDITY)
			{
				std::cout << "Invalid uniform: " << UniformName << std::endl;
			}
		}
	}
	void ShaderProgram::SetUniform4f(std::string const& UniformName, float x, float y, float z, float w)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		p_HandleInvalidPosition(UniformLocation, UniformName);
		glUniform4f(UniformLocation, x, y, z, w);
		glCheckError();
	}
	void ShaderProgram::SetUniform4i(std::string const& UniformName, int x, int y, int z, int w)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		p_HandleInvalidPosition(UniformLocation, UniformName);
		glUniform4f(UniformLocation, x, y, z, w);
		glCheckError();
	}
	void ShaderProgram::SetUniform1i(std::string const& UniformName, int x)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		p_HandleInvalidPosition(UniformLocation, UniformName);
		glUniform1i(UniformLocation, x);
		glCheckError();
	}
	void ShaderProgram::SetUniform1f(std::string const& UniformName, float x)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		p_HandleInvalidPosition(UniformLocation, UniformName);
		glUniform1f(UniformLocation, x);
		glCheckError();
	}
	void ShaderProgram::SetUniformMat4f(std::string const& UniformName, const float* RowMajorData)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		p_HandleInvalidPosition(UniformLocation, UniformName);
		glUniformMatrix4fv(UniformLocation, 1, GL_TRUE, RowMajorData);
		glCheckError();
	}
	void ShaderProgram::SetUniformVec3(std::string const& UniformName, float x, float y, float z)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		p_HandleInvalidPosition(UniformLocation, UniformName);
		glUniform3f(UniformLocation, x, y, z);
		glCheckError();
	}
	void ShaderProgram::SetUniformVec4(std::string const& UniformName, float x, float y, float z,float w)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		p_HandleInvalidPosition(UniformLocation, UniformName);
		glUniform4f(UniformLocation, x, y, z,w);
		glCheckError();
	}
	void ShaderProgram::PrintActiveAttributesAndUniforms()
	{
		GLint i;
		GLint count;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 32; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(m_ProgramHandle, GL_ACTIVE_ATTRIBUTES, &count);
		printf("Active Attributes: %d\n", count);

		for (i = 0; i < count; i++)
		{
			glGetActiveAttrib(m_ProgramHandle, (GLuint)i, bufSize, &length, &size, &type, name);

			printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
		}

		glGetProgramiv(m_ProgramHandle, GL_ACTIVE_UNIFORMS, &count);
		printf("Active Uniforms: %d\n", count);

		for (i = 0; i < count; i++)
		{
			glGetActiveUniform(m_ProgramHandle, (GLuint)i, bufSize, &length, &size, &type, name);

			printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
		}
	}
	ShaderProgram::~ShaderProgram()
	{
		if (m_ProgramHandle != 0)
		{
			glDeleteProgram(m_ProgramHandle);
		}
		glCheckError();
	}
	//VertexLayout
	void VertexLayout::AddAttribute(int SizeOfElement, int NumberOfElements, DataTypes DataType)
	{
		TotalSize += SizeOfElement * NumberOfElements;
		Attributes.push_back(VertexAttributeStruct{SizeOfElement,NumberOfElements,DataType});
	}
	void VertexLayout::Bind()
	{
		int CurrentOffset = 0;
		for (size_t i = 0; i < Attributes.size(); i++)
		{
			unsigned int ElementType = 0;
			VertexAttributeStruct CurrentAttrib = Attributes[i];
			if (CurrentAttrib.DataType == DataTypes::Float)
			{
				ElementType = GL_FLOAT;
			}
			else  if(CurrentAttrib.DataType == DataTypes::Int)
			{
				ElementType = GL_INT;
			}
			if (ElementType == GL_FLOAT)
			{
				glVertexAttribPointer(i, CurrentAttrib.NumberOfElements, GL_FLOAT, GL_FALSE, TotalSize, (void*)CurrentOffset);
			}
			else if(ElementType == GL_INT)
			{
				glVertexAttribIPointer(i, CurrentAttrib.NumberOfElements, GL_INT, TotalSize, (void*)CurrentOffset);
			}
			CurrentOffset += CurrentAttrib.NumberOfElements * CurrentAttrib.SizeOfElements;
			glEnableVertexAttribArray(i);
		}
		glCheckError();
	}
	unsigned int VertexLayout::VertexSize()
	{
		return(TotalSize);
	}
	//vertexbuffer
	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferHandle);
		glCheckError();
	}
	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glCheckError();
	}
	VertexBuffer::VertexBuffer(VBTypes BufferType, unsigned long long InitialSize, const void* InitialData)
	{
		m_BufferSize = InitialSize;
		unsigned int DrawType = 0;
		if (BufferType == VBTypes::DynamicDraw)
		{
			DrawType = GL_DYNAMIC_DRAW;
		}
		else if(BufferType == VBTypes::StaticDraw)
		{
			DrawType = GL_STATIC_DRAW;
		}
		else if(BufferType == VBTypes::StreamDraw)
		{
			DrawType = GL_STREAM_DRAW;
		}
		m_CurrentDrawType = DrawType;
		glGenBuffers(1, &m_BufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferHandle);
		glBufferData(GL_ARRAY_BUFFER, InitialSize, InitialData, DrawType);
		glCheckError();
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& BufferToSteal) noexcept
	{
		std::swap(m_BufferHandle, BufferToSteal.m_BufferHandle);
		std::swap(m_BufferSize, BufferToSteal.m_BufferSize);
		std::swap(m_CurrentDrawType, BufferToSteal.m_CurrentDrawType);
	}
	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& BufferToSteal) noexcept
	{
		std::swap(m_BufferHandle, BufferToSteal.m_BufferHandle);
		std::swap(m_BufferSize, BufferToSteal.m_BufferSize);
		std::swap(m_CurrentDrawType, BufferToSteal.m_CurrentDrawType);
		return(*this);
	}
	VertexBuffer::VertexBuffer()
	{
		m_CurrentDrawType = GL_DYNAMIC_DRAW;
		glGenBuffers(1, &m_BufferHandle);
		glCheckError();
	}
	unsigned int VertexBuffer::SizeOfBuffer()
	{
		return(m_BufferSize);
	}
	void VertexBuffer::ResizeBuffer(unsigned int NewSize,const void* Data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferHandle);
		glBufferData(GL_ARRAY_BUFFER, NewSize, Data, m_CurrentDrawType);
		glCheckError();
	}
	void VertexBuffer::FillBuffer(unsigned int Offset, unsigned int NumberOfBytes, const void* Data)
	{
		glBufferSubData(GL_ARRAY_BUFFER, Offset, NumberOfBytes, Data);
		glCheckError();
	}
	void VertexBuffer::DrawTriangles()
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glCheckError();
	}
	VertexBuffer::~VertexBuffer()
	{
		if (m_BufferHandle != 0)
		{
			glDeleteBuffers(1, &m_BufferHandle);
		}
		glCheckError();
	}
	//VertexArrayObject
	VertexArrayObject::VertexArrayObject()
	{
		glGenVertexArrays(1, &ObjectHandle);
		glCheckError();
	}
	void VertexArrayObject::Bind()
	{
		glBindVertexArray(ObjectHandle);
		glCheckError();
	}
	void VertexArrayObject::UnBind()
	{
		glBindVertexArray(0);
		glCheckError();
	}
	VertexArrayObject::~VertexArrayObject()
	{
		glDeleteVertexArrays(1, &ObjectHandle);
		glCheckError();
	}
	//ElementBufferObject
	ElementBufferObject::ElementBufferObject(unsigned int NumberOfElements,const unsigned int* Data)
	{
		glGenBuffers(1, &m_ObjectHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ObjectHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumberOfElements * sizeof(int), Data, GL_STATIC_DRAW);
		glCheckError();
	}
	void ElementBufferObject::ResizeBuffer(unsigned int NumberOfElements, const void* Data)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ObjectHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumberOfElements *sizeof(int), Data, GL_STATIC_DRAW);
		glCheckError();
	}
	void ElementBufferObject::FillBuffer(unsigned int Offset, unsigned int NumberOfBytes, const void* Data)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, Offset, NumberOfBytes, Data);
		glCheckError();
	}

	ElementBufferObject::ElementBufferObject(ElementBufferObject&& ObjectToSteal) noexcept
	{
		std::swap(m_ObjectHandle, ObjectToSteal.m_ObjectHandle);
	}
	ElementBufferObject& ElementBufferObject::operator=(ElementBufferObject&& ObjectToCopy) noexcept
	{
		std::swap(m_ObjectHandle, ObjectToCopy.m_ObjectHandle);
		return(*this);
	}
	ElementBufferObject::ElementBufferObject()
	{
		glGenBuffers(1, &m_ObjectHandle);
		glCheckError();
	}
	ElementBufferObject::~ElementBufferObject()
	{
		glDeleteBuffers(1, &m_ObjectHandle);
		glCheckError();
	}
	void ElementBufferObject::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ObjectHandle);
		glCheckError();
	}
	void ElementBufferObject::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glCheckError();
	}
	/*
	void ElementBufferObject::DrawTriangles()
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glCheckError();
	}
	*/
	//Vertex
	Vertex::Vertex(int NumberOfBytes, void* Data)
	{
		DataSize = NumberOfBytes;
		VertexData = (unsigned char*)malloc(NumberOfBytes);
		unsigned char* NewData = (unsigned char*)Data;
		for (size_t i = 0; i < NumberOfBytes; i++)
		{
			VertexData[i] = NewData[i];
		}
	}
	void Vertex::Swap(Vertex& OtherVertex)
	{
		std::swap(VertexData, OtherVertex.VertexData);
		std::swap(DataSize, OtherVertex.DataSize);
		std::swap(Position, OtherVertex.Position);
	}
	Vertex::Vertex(Vertex const& OtherVertex)
	{
		//skapar ny data likla med den andras storlek
		DataSize = OtherVertex.DataSize;
		VertexData = (unsigned char*)malloc(OtherVertex.DataSize);
		for (size_t i = 0; i < DataSize; i++)
		{
			VertexData[i] = OtherVertex.VertexData[i];
		}
	}
	Vertex::Vertex(Vertex&& OtherVertex) noexcept
	{
		//skapar ny data likla med den andras storlek
		Swap(OtherVertex);
	}
	Vertex::~Vertex()
	{
		if (VertexData != nullptr)
		{
			free(VertexData);
		}
	}
	//Mesh
	Mesh::Mesh()
	{

	}
	void Mesh::FillMeshData(std::vector<VertexAttributes> const& Attributes, const void* VertexData, size_t VertexDataSize, const void* FaceData, size_t FaceDataSize)
	{
		VAO.Bind();
		
		Buffer.Bind();

		Layout = p_GetVertexLayout(Attributes);
		Layout.Bind();

		Buffer.ResizeBuffer(VertexDataSize, VertexData);
		ArrayObject.Bind();
		ArrayObject.ResizeBuffer(FaceDataSize, FaceData);
		VAO.UnBind();
		DrawVerticesCount = 6;
		VertexSize = Layout.VertexSize();
		VerticesCount = VertexDataSize / VertexSize;
		//legacy, borde inte beh�vas l�ngre men men
		VerticesData = std::vector<unsigned char>((unsigned char*)VertexData, ((unsigned char*)VertexData) + VertexDataSize);
	}
	VertexLayout Mesh::p_GetVertexLayout(std::vector<VertexAttributes> const& AttributeOrder)
	{
		VertexLayout ReturnValue;
		for (size_t i = 0; i < AttributeOrder.size(); i++)
		{
			VertexAttributes CurrentAttribute = AttributeOrder[i];
			if (CurrentAttribute == VertexAttributes::TextureCoordinates)
			{
				ReturnValue.AddAttribute(4, 2, DataTypes::Float);
			}
			else if (CurrentAttribute == VertexAttributes::Position)
			{
				ReturnValue.AddAttribute(4, 3, DataTypes::Float);
			}
		}
		return(ReturnValue);
	}
	Mesh::Mesh(int VerticesToLoadCount, int VertexToLoadSize, void* VertexToLoadData, int ArrayObjectSize, unsigned int* ArrayObjectData)
		: Buffer(VBTypes::DynamicDraw, VerticesToLoadCount* VertexToLoadSize, VertexToLoadData), ArrayObject(ArrayObjectSize, ArrayObjectData)
	{
		VAO.Bind();
		Buffer.Bind();
		Layout.AddAttribute(4, 3, DataTypes::Float);
		Layout.AddAttribute(4, 2, DataTypes::Float);
		Layout.Bind();
		ArrayObject.Bind();
		VAO.UnBind();
		DrawVerticesCount = ArrayObjectSize;
		VertexSize = VertexToLoadSize;
		VerticesCount = VerticesToLoadCount;
		unsigned int TotalDataLength = VerticesToLoadCount * VertexToLoadSize;
		//VerticesData = (unsigned char*)malloc(TotalDataLength);
		VerticesData = std::vector<unsigned char>(TotalDataLength, 0);
		unsigned char* NewData = (unsigned char*)VertexToLoadData;
		for (size_t i = 0; i < TotalDataLength; i++)
		{
			VerticesData[i] = NewData[i];
		}
	}
	Mesh::Mesh(void* MeshDataObject,std::vector<VertexAttributes> const& AttributeOrder, std::unordered_map<std::string, Bone> const& BoneMap,size_t* OutMaterialIndex)
		: Buffer(), ArrayObject(),VAO(),Layout()
	{
		const int TextureDimensions = 2;
		//AssociatedModel = ModelToBelong;
		aiMesh* AssimpMesh = (aiMesh*)MeshDataObject;
		//vi ska alltid anta att b�rjan av meshen �r 
		//vi b�rjar med att definiera layouten som beror p� vilka av v�ra grejer som �r definierade
		unsigned int NumberOfVertexColors = AssimpMesh->GetNumColorChannels();
		unsigned int NumberOfTextureCoords = AssimpMesh->GetNumUVChannels();
		//vi har alltid en position
		Layout.AddAttribute(sizeof(float), 3, DataTypes::Float);
		VerticesCount = AssimpMesh->mNumVertices;
		*OutMaterialIndex = AssimpMesh->mMaterialIndex;
		//borde egentligen kolla f�r om vi saknar attributer vi vill ha
		for (size_t i = 0; i < AttributeOrder.size(); i++)
		{
			switch (AttributeOrder[i])
			{
			case VertexAttributes::Bitangent:
				Layout.AddAttribute(sizeof(float), 3, DataTypes::Float);
				break;
			case VertexAttributes::Tangent:
				Layout.AddAttribute(sizeof(float), 3, DataTypes::Float);
				break;
			case VertexAttributes::VertexColors:
				Layout.AddAttribute(sizeof(float), 4*NumberOfVertexColors, DataTypes::Float);
				break;
			case VertexAttributes::TextureCoordinates:
				Layout.AddAttribute(sizeof(float), TextureDimensions*NumberOfTextureCoords, DataTypes::Float);
				break;
			case VertexAttributes::VertexNormal:
				Layout.AddAttribute(sizeof(float), 3, DataTypes::Float);
				break;
			default:
				//blir ett error egentligen
				break;
			}
		}
		//huruvida vi har ben attributer l�ggs tills p� slutet
		int BoneAttributesSize = 0;
		if (BoneMap.size() != 0)
		{
			for (size_t i = 0; i < MBGE_BONEPERVERTEX; i++)
			{
				Layout.AddAttribute(sizeof(int), 1, MBGE::DataTypes::Int);
				Layout.AddAttribute(sizeof(float), 1, MBGE::DataTypes::Float);
				BoneAttributesSize += (sizeof(int) + sizeof(float));
			}
		}
		VAO.Bind();
		VertexSize = Layout.VertexSize();
		//vi vet att storleken av arrayen alltid �r 3 * antalet faces d� vi gjort om dem till trianglar
		DrawVerticesCount = AssimpMesh->mNumFaces * 3;
		unsigned int* ArrayBufferData = (unsigned int*)malloc(sizeof(unsigned int)*3 * (long long)AssimpMesh->mNumFaces);
		unsigned int FaceOffset = 0;
		for (size_t i = 0; i < AssimpMesh->mNumFaces; i++)
		{
			for (size_t j = 0; j < AssimpMesh->mFaces[i].mNumIndices; j++)
			{

				ArrayBufferData[FaceOffset] = AssimpMesh->mFaces[i].mIndices[j];
				FaceOffset += 1;
			}
		}
		ArrayObject.Bind();
		ArrayObject.ResizeBuffer(DrawVerticesCount, ArrayBufferData);
		free(ArrayBufferData);
		//�nu tar vi och faktiskt skapar vertexdatan
		unsigned int TotalData = VertexSize * VerticesCount;
		VerticesData = std::vector<unsigned char>(TotalData, 0);
		int Offset = 0;
		for (size_t i = 0; i < VerticesCount; i++)
		{
			//vi ska alltid ha med positionwn
			float* VertexPositions = (float*)&VerticesData[Offset];
			for (size_t j = 0; j < 3; j++)
			{
				VertexPositions[j] = AssimpMesh->mVertices[i][j];
			}
			Offset += sizeof(float) * 3;
			for (size_t j = 0; j < AttributeOrder.size(); j++)
			{
				float* FloatPointer = (float*)&VerticesData[Offset];
				switch (AttributeOrder[j])
				{
				case VertexAttributes::Bitangent:
					for (size_t k = 0; k < 3; k++)
					{
						FloatPointer[k] = AssimpMesh->mBitangents[i][k];
					}
					Offset += sizeof(float) * 3;
					//Layout.AddAttribute(sizeof(float), 3, DataTypes::Float);
					break;
				case VertexAttributes::Tangent:
					for (size_t k = 0; k < 3; k++)
					{
						FloatPointer[k] = AssimpMesh->mTangents[i][k];
					}
					Offset += sizeof(float) * 3;
					//Layout.AddAttribute(sizeof(float), 3, DataTypes::Float);
					break;
				case VertexAttributes::VertexColors:
					for (size_t k = 0; k < NumberOfVertexColors; k++)
					{
						for (size_t L = 0; L < 4;L++)
						{
							FloatPointer[k*4+L] = AssimpMesh->mColors[k][i][L];
						}
					}
					Offset += (sizeof(float) * 4)*NumberOfVertexColors;
					//Layout.AddAttribute(sizeof(float), 4 * NumberOfVertexColors, DataTypes::Float);
					break;
				case VertexAttributes::TextureCoordinates:
					for (size_t k = 0; k < NumberOfTextureCoords; k++)
					{
						for (size_t L = 0; L < TextureDimensions;L++)
						{
							FloatPointer[k * TextureDimensions + L] = AssimpMesh->mTextureCoords[k][i][L];
						}
					}
					Offset += (sizeof(float) * TextureDimensions) * NumberOfTextureCoords;
					//Layout.AddAttribute(sizeof(float), 3 * NumberOfTextureCoords, DataTypes::Float);
					break;
				case VertexAttributes::VertexNormal:
					for (size_t k = 0; k < 3; k++)
					{
						FloatPointer[k] = AssimpMesh->mNormals[i][k];
					}
					Offset += sizeof(float) * 3;
					//Layout.AddAttribute(sizeof(float), 3, DataTypes::Float);
					break;
				default:
					//blir ett error egentligen
					break;
				}
			}
			//om offseten �r mindre �n �n helt vertex l�gger vi till s� att den blir det, s� vi kan l�gga till fler attributes utan att hela systemet bryter samman
			if (Offset%VertexSize != 0)
			{
				Offset += (VertexSize - (Offset % VertexSize));
			}
		}
		//har vi bendata vill vi l�gga in det i vektorn med
		//if (AssociatedModel->BoneMap.size()!= 0)
		if (BoneMap.size() != 0)
		{
			std::vector<char> BonesPerVertex = std::vector<char>(VerticesCount, 0);
			for (size_t i = 0; i < AssimpMesh->mNumBones; i++)
			{
				Bone MBBone = BoneMap.at(std::string(AssimpMesh->mBones[i]->mName.C_Str()));
				for (size_t j = 0; j < MBBone.Weights.size(); j++)
				{
					int VertexToModify = MBBone.Weights[j].VertexIndex;
					char VertexToModifyPreviousBoneCount = BonesPerVertex[VertexToModify];
					BonesPerVertex[VertexToModify] += 1;
					//if (BonesPerVertex[VertexToModify] > MBGE_BONEPERVERTEX)
					//{
					//	int Hej = BonesPerVertex[VertexToModify];
					//	int Hej2 = BonesPerVertex[VertexToModify];
					//}
					int VertexToModifyBoneDataOffset = VertexToModify * VertexSize+(VertexSize-BoneAttributesSize) + VertexToModifyPreviousBoneCount * (sizeof(int) + sizeof(float));
					int* BoneIndexPtr = (int*)&VerticesData[VertexToModifyBoneDataOffset];
					float* BoneWeightPtr = (float*)&VerticesData[VertexToModifyBoneDataOffset+sizeof(int)];
					*BoneIndexPtr = MBBone.BoneIndex;
					*BoneWeightPtr = MBBone.Weights[j].Weight;
				}
			}
		}
 		Buffer.Bind();
		Buffer.ResizeBuffer(TotalData, &VerticesData[0]);
		Layout.Bind();
		VAO.UnBind();
		//TODO l�gg till s� att vi kan fixa bena
	}
	void Mesh::Rotate(float DegreesToRotate, MBMath::MBVector3<float> const& RotationAxis)
	{
		unsigned int Offset = 0;
		for (size_t i = 0; i < VerticesCount; i++)
		{

			//TODO detta �r kinda sussy, borde se �ver hur jag egentligen t�nkt med detta..
			//vektor borde ocks� kunna er�ttas med en string inser jag ocks�
			float* NumberData = (float*)&VerticesData[Offset];
			MBMath::MBVector3<float> VectorIntermediary(NumberData[0], NumberData[1], NumberData[2]);
			VectorIntermediary.Rotate(DegreesToRotate, RotationAxis);
			if (Offset == 0)
			{
				//std::cout << VectorIntermediary << std::endl;
				
			}
			NumberData[0] = VectorIntermediary[0];
			NumberData[1] = VectorIntermediary[1];
			NumberData[2] = VectorIntermediary[2];
			Offset += VertexSize;
		}
	}
	void Mesh::Draw()
	{
		//TODO omarbeta hela strukture, inser att vi vill nog g�ra det h�r p� ett v�ldig annorlunda s�tt
		//vi beh�ver binda r�tt textures
		//Material* MaterialToUse = AssociatedModel->GetMaterial(MaterialIndex);
		//MaterialToUse->SetUniforms();
		VAO.Bind();
		Buffer.Bind();
		ArrayObject.Bind();
		//eftersom mesh datan mer eller mindre aldrig �ndras s� beh�ver vi inte fylla om den hela tiden
		//float* TestPointer = (float*)&VerticesData[0];
		//Buffer.FillBuffer(0, VerticesCount * VertexSize, &VerticesData[0]);
		glDrawElements(GL_TRIANGLES, DrawVerticesCount, GL_UNSIGNED_INT, 0);
		VAO.UnBind();
		glCheckError();
	}
	//void Mesh::SavePositions()
	//{
	//	unsigned int Offset = 0;
	//	SavedPositionData = std::vector<Vec3Data>(VerticesCount);
	//	for (size_t i = 0; i < VerticesCount; i++)
	//	{
	//		Vec3Data NewPosition;
	//		float* DataPointer = (float*)&VerticesData[Offset];
	//		NewPosition.x = DataPointer[0];
	//		NewPosition.y = DataPointer[1];
	//		NewPosition.z = DataPointer[2];
	//		SavedPositionData[i] = NewPosition;
	//		Offset += VertexSize;
	//	}
	//}
	//void Mesh::RestorePositions()
	//{
	//	unsigned int Offset = 0;
	//	for (size_t i = 0; i < VerticesCount; i++)
	//	{
	//		float* DataPointer = (float*)&VerticesData[Offset];
	//		DataPointer[0] = SavedPositionData[i].x;
	//		DataPointer[1] = SavedPositionData[i].y;
	//		DataPointer[2] = SavedPositionData[i].z;
	//		Offset += VertexSize;
	//	}
	//}
	
	
	
	//void Mesh::TransformPositions(MBMath::MBMatrix4<float> const& Transformation)
	//{
	//	unsigned int Offset = 0;
	//	for (size_t i = 0; i < VerticesCount; i++)
	//	{
	//		//TODO denna kod anv�nad inte l�ngre men har inte faktikst kollar om det h�r st�mmer
	//		float* DataPointer = (float*)&VerticesData[Offset];
	//		MBMath::MBStaticVector<float,4> VectorIntermediary;
	//		VectorIntermediary[0] = DataPointer[0];
	//		VectorIntermediary[1] = DataPointer[1];
	//		VectorIntermediary[2] = DataPointer[2];
	//		VectorIntermediary[3] = 1;
	//		VectorIntermediary = Transformation*VectorIntermediary;
	//		DataPointer[0] = VectorIntermediary[0];
	//		DataPointer[1] = VectorIntermediary[1];
	//		DataPointer[2] = VectorIntermediary[2];
	//		Offset += VertexSize;
	//	}
	//}
	
	
	
	unsigned int Mesh::NumberOfVertices()
	{
		return(VerticesCount);
	}
	Mesh::~Mesh()
	{

	}
	//Material
	std::vector<std::string> Material::GetTexturePaths(void* MaterialData, int TextureType)
	{
		std::vector<std::string> ReturnValue = {};
		aiMaterial* AssimpMaterial = (aiMaterial*)MaterialData;
		aiTextureType AssimpTextureType = (aiTextureType)TextureType;
		unsigned int NumberOfTextures = AssimpMaterial->GetTextureCount(AssimpTextureType);
		for (size_t i = 0; i <NumberOfTextures; i++)
		{
			aiString NewString;
AssimpMaterial->GetTexture(AssimpTextureType, i, &NewString);
if (NewString.length != 0)
{
	ReturnValue.push_back(std::string(NewString.C_Str()));
}
		}
		return(ReturnValue);
	}
	//TODO l�gg till support f�r material attributes som inte bara �r textures
	Material::Material(void* MaterialData, std::string ModelDirectory, MBGraphicsEngine* AttachedEngine)
	{
		//AssciatedGraphicsEngine = AttachedEngine;
		aiMaterial* AssimpMaterial = (aiMaterial*)MaterialData;
		//vi g�r igenom allat textures och ser om den har n�gon
		int NumberOfTextures = 0;
		std::vector<aiTextureType> TextureTypes = { aiTextureType::aiTextureType_AMBIENT,aiTextureType::aiTextureType_AMBIENT_OCCLUSION,aiTextureType::aiTextureType_BASE_COLOR,
		aiTextureType::aiTextureType_DIFFUSE,aiTextureType::aiTextureType_DIFFUSE_ROUGHNESS,aiTextureType::aiTextureType_DISPLACEMENT,aiTextureType::aiTextureType_EMISSION_COLOR,
		aiTextureType::aiTextureType_EMISSIVE,aiTextureType::aiTextureType_HEIGHT,aiTextureType::aiTextureType_LIGHTMAP,
			aiTextureType::aiTextureType_METALNESS,aiTextureType::aiTextureType_NONE,	aiTextureType::aiTextureType_NORMALS,
			aiTextureType::aiTextureType_NORMAL_CAMERA,aiTextureType::aiTextureType_OPACITY,
			aiTextureType::aiTextureType_REFLECTION,aiTextureType::aiTextureType_SHININESS,
		aiTextureType::aiTextureType_SPECULAR,aiTextureType::aiTextureType_UNKNOWN };
		for (size_t i = 0; i < TextureTypes.size(); i++)
		{
			NumberOfTextures += AssimpMaterial->GetTextureCount(TextureTypes[i]);
		}
		std::cout << "Namnet p� materialet �r: " << AssimpMaterial->GetName().C_Str() << " och har " << NumberOfTextures << " Textures" << std::endl;

		//vi laddar dem olika texturesen som vi letar efter, har den ej det s� laddar vi ist�llet andra specifika v�rden
		std::vector<std::string> DiffuseTextures = GetTexturePaths(MaterialData, aiTextureType_DIFFUSE);
		if (DiffuseTextures.size() != 0)
		{
			//vi �r bara intresserade av att ta den f�rsta textyuren, egentligen kan det finbnas fler, men vi tar det sen
			DiffuseTexture = ModelDirectory + DiffuseTextures[0];
		}
		else
		{
			//d� �r vi kanske intresserade av att ta fram f�rger etc?
		}
		std::vector<std::string> SpecularTextures = GetTexturePaths(MaterialData, aiTextureType_SPECULAR);
		if (SpecularTextures.size() != 0)
		{
			SpecularTexture = ModelDirectory + SpecularTextures[0];
		}
		else
		{
			//d� �r vi kanske intresserade av att ta fram f�rger etc?
		}
		std::vector<std::string> NormalTextures = GetTexturePaths(MaterialData, aiTextureType_NORMALS);
		if (NormalTextures.size() != 0)
		{
			NormalTexture = ModelDirectory + NormalTextures[0];
		}
		p_LoadTextures(AttachedEngine);
	}
	template<typename T> bool VectorContains(const std::vector<T>& VectorToCheck, T Value)
	{
		for (size_t i = 0; i < VectorToCheck.size(); i++)
		{
			if (VectorToCheck[i] == Value)
			{
				return(true);
			}
		}
		return(false);
	}
	Material::Material(void* MaterialData, std::string PathToModel, std::vector<MaterialAttribute> NewMaterialAttributes, MBGraphicsEngine* AttachedEngine)
	{
		MaterialShaderAttributes = NewMaterialAttributes;
		//AssciatedGraphicsEngine = AttachedEngine;
		if (VectorContains<MaterialAttribute>(NewMaterialAttributes, MaterialAttribute::DiffuseTexture));
		{
			DiffuseTexture = PathToModel + "Diffuse.png";
		}
		if (VectorContains<MaterialAttribute>(NewMaterialAttributes, MaterialAttribute::SpecularTexture));
		{
			SpecularTexture = PathToModel + "Specular.png";
		}
		if (VectorContains<MaterialAttribute>(NewMaterialAttributes, MaterialAttribute::NormalTexture));
		{
			NormalTexture = PathToModel + "Normal.png";
		}
		p_LoadTextures(AttachedEngine);
	}
	void Material::p_LoadTextures(MBGraphicsEngine* AssociatedEngine)
	{
		MaterialTextures.push_back(AssociatedEngine->GetTexture(DiffuseTexture));
		MaterialTextures.push_back(AssociatedEngine->GetTexture(SpecularTexture));
		MaterialTextures.push_back(AssociatedEngine->GetTexture(NormalTexture));
	}
	void Material::SetUniforms(ShaderProgram* ProgramToModify)
	{
		//f�ruts�tter att vi redan har en bunden shader med r�tt inst�llningar
		//laddar inte texutresen vi beh�ver
		//TODO optimesera det h�r s� att det bara h�nder i konstrukturn och garantera att enginene aldrig deletar data som ett existerande ogbjekt anv�nder
		//ShaderProgram* CurrentShader = AssciatedGraphicsEngine->GetCurrentShader();
		//CurrentShader->Bind();
		//Uniforms.SetUniforms(ProgramToModify);


		//Legacy grejer
		ProgramToModify->SetUniform1i("FragUni.UseDiffTex", false);
		ProgramToModify->SetUniform1i("FragUni.UseSpecTex", false);
		ProgramToModify->SetUniform1i("FragUni.UseNormTex", false);

		for (size_t i = 0; i < MaterialShaderAttributes.size();i++)
		{
			MaterialAttribute CurrentTextureType = MaterialShaderAttributes[i];
			MaterialTextures[i]->Bind(i);
			if (CurrentTextureType == MaterialAttribute::DiffuseTexture)
			{
				ProgramToModify->SetUniform1i("FragUni.UseDiffTex", true);
				ProgramToModify->SetUniform1i("DiffuseTex", i);
			}
			else if (CurrentTextureType == MaterialAttribute::SpecularTexture)
			{
				ProgramToModify->SetUniform1i("FragUni.UseSpecTex", true);
				ProgramToModify->SetUniform1i("SpecTex", i);
			}
			else if (CurrentTextureType == MaterialAttribute::NormalTexture)
			{
				ProgramToModify->SetUniform1i("FragUni.UseNormTex", true);
				ProgramToModify->SetUniform1i("NormalTex", i);
			}
		}
	}
	//END Material

	//BEGIN UniformValue
	void swap(UniformValue& LeftValue, UniformValue& RightValue) noexcept
	{
		std::swap(LeftValue.m_Type, RightValue.m_Type);
		std::swap(LeftValue.m_DataPointer, RightValue.m_DataPointer);
	}
	void UniformValue::p_FreeData()
	{
		if (m_DataPointer == nullptr)
		{
			return;
		}
		if (m_Type == DataTypes::Float)
		{
			delete &p_GetData<float>();
		}
		else if (m_Type == DataTypes::Int)
		{
			delete& p_GetData<int>();
		}
		else if (m_Type == DataTypes::Matrix4)
		{
			delete& p_GetData<MBMath::MBMatrix4<float>>();
		}
		else if (m_Type == DataTypes::Vec3)
		{
			delete& p_GetData<MBMath::MBVector3<float>>();
		}
		else if (m_Type == DataTypes::Vec4)
		{
			delete& p_GetData<MBMath::MBStaticVector<float,4>>();
		}
		else if (m_Type == DataTypes::Array)
		{
			delete& p_GetData<std::vector<UniformValue>>();
		}
		else if (m_Type == DataTypes::Struct)
		{
			delete& p_GetData<std::map<std::string,UniformValue>>();
		}
		else if (m_Type == DataTypes::Null)
		{
			assert(m_DataPointer == nullptr);
		}
		else
		{
			throw std::domain_error("Corrupted UniformValue data");
		}
	}
	void* UniformValue::p_CloneData() const
	{
		if (m_Type == DataTypes::Float)
		{
			return(new float(p_GetData<float>()));
		}
		else if (m_Type == DataTypes::Int)
		{
			return(new float(p_GetData<int>()));
		}
		else if (m_Type == DataTypes::Matrix4)
		{
			return(new MBMath::MBMatrix4<float>(p_GetData<MBMath::MBMatrix4<float>>()));
		}
		else if (m_Type == DataTypes::Vec3)
		{
			return(new MBMath::MBVector3<float>(p_GetData<MBMath::MBVector3<float>>()));
		}
		else if (m_Type == DataTypes::Vec4)
		{
			return(new MBMath::MBStaticVector<float,4>(p_GetData<MBMath::MBStaticVector<float,4>>()));
		}
		else if (m_Type == DataTypes::Array)
		{
			return(new std::vector<UniformValue>(p_GetData<std::vector<UniformValue>>()));
		}
		else if (m_Type == DataTypes::Struct)
		{
			return(new std::map<std::string,UniformValue>(p_GetData<std::map<std::string,UniformValue>>()));
		}
		else if (m_Type == DataTypes::Null)
		{
			assert(m_DataPointer == nullptr);
			return(nullptr);
		}
		else
		{
			throw std::domain_error("Corrupted UniformValue data");
		}
	}
	UniformValue::UniformValue(UniformValue const& ValueToCopy)
	{
		m_Type = ValueToCopy.m_Type;
		m_DataPointer = ValueToCopy.p_CloneData();
	}
	UniformValue::UniformValue(UniformValue&& ValueToSteal) noexcept
	{
		swap(*this, ValueToSteal);
	}
	UniformValue& UniformValue::operator=(UniformValue ValueToSteal)
	{
		swap(*this,ValueToSteal);
		return(*this);
	}

	UniformValue::UniformValue(float InitialFloatValue)
	{
		p_GenericInitialize<float>();
		p_GetData<float>() = InitialFloatValue;
		m_Type = DataTypes::Float;
	}
	UniformValue::UniformValue(int InitialIntValue)
	{
		p_GenericInitialize<int>();
		p_GetData<int>() = InitialIntValue;
		m_Type = DataTypes::Int;
	}
	UniformValue::UniformValue(MBMath::MBVector3<float> InitialVectorValue)
	{
		p_GenericInitialize<MBMath::MBVector3<float>>();
		p_GetData<MBMath::MBVector3<float>>() = std::move(InitialVectorValue);
		m_Type = DataTypes::Vec3;
	}
	UniformValue::UniformValue(MBMath::MBStaticVector<float, 4> InitialVectorValue)
	{
		p_GenericInitialize<MBMath::MBStaticVector<float, 4>>();
		p_GetData<MBMath::MBStaticVector<float, 4>>() = std::move(InitialVectorValue);
		m_Type = DataTypes::Vec4;
	}
	UniformValue::UniformValue(MBMath::MBMatrix4<float> InitialMatrixValue)
	{
		p_GenericInitialize<MBMath::MBMatrix4<float>>();
		p_GetData<MBMath::MBMatrix4<float>>() = std::move(InitialMatrixValue);
		m_Type = DataTypes::Matrix4;
	}
	UniformValue::UniformValue(std::vector<UniformValue> InitialArrayValue)
	{
		p_GenericInitialize<std::vector<UniformValue>>();
		p_GetData<std::vector<UniformValue>>() = std::move(InitialArrayValue);
		m_Type = DataTypes::Array;
	}
	UniformValue::UniformValue(std::map<std::string, UniformValue> InitialStructData)
	{
		p_GenericInitialize<std::map<std::string, UniformValue>>();
		p_GetData<std::map<std::string, UniformValue>>() = std::move(InitialStructData);
		m_Type = DataTypes::Struct;
	}
	//UniformValue::UniformValue(DataTypes InitialDataType)
	//{
	//	
	//}

	void UniformValue::UpdateUniforms(std::string const& NamePrefix, ShaderProgram* ProgramToUpdate) const
	{
		if (m_Type == DataTypes::Float)
		{
			ProgramToUpdate->SetUniform1f(NamePrefix, p_GetData<float>());
		}
		else if (m_Type == DataTypes::Int)
		{
			ProgramToUpdate->SetUniform1i(NamePrefix, p_GetData<int>());
		}
		else if (m_Type == DataTypes::Vec3)
		{
			MBMath::MBVector3<float> const& InternalVectorData = p_GetData<MBMath::MBVector3<float>>();
			ProgramToUpdate->SetUniformVec3(NamePrefix,InternalVectorData[0], InternalVectorData[1], InternalVectorData[2]);
		}
		else if (m_Type == DataTypes::Vec4)
		{
			auto const& InternalVectorData = p_GetData<MBMath::MBStaticVector<float,4>>();
			ProgramToUpdate->SetUniformVec4(NamePrefix, InternalVectorData[0], InternalVectorData[1], InternalVectorData[2],InternalVectorData[3]);
		}
		else if (m_Type == DataTypes::Matrix4)
		{
			MBMath::MBMatrix4<float> const& InternalVectorData = p_GetData<MBMath::MBMatrix4<float>>();
			ProgramToUpdate->SetUniformMat4f(NamePrefix, InternalVectorData.GetContinousData());
		}
		else if (m_Type == DataTypes::Array)
		{
			std::vector<UniformValue> const& InternalVectorData = p_GetData<std::vector<UniformValue>>();
			for (size_t i = 0; i < InternalVectorData.size();i++)
			{
				UniformValue const& Value = InternalVectorData[i];
				if (Value.GetType() != DataTypes::Null)
				{
					Value.UpdateUniforms(NamePrefix + "[" + std::to_string(i) + "]", ProgramToUpdate);
				}
			}
		}
		else if (m_Type == DataTypes::Struct)
		{
			std::map<std::string,UniformValue> const& InternalStructData = p_GetData<std::map<std::string, UniformValue>>();
			for (auto const& Value : InternalStructData)
			{
				if (Value.second.GetType() != DataTypes::Null)
				{
					std::string NewName = NamePrefix;
					if (NamePrefix != "")
					{
						NewName += ".";
					}
					NewName += Value.first;
					Value.second.UpdateUniforms(NewName, ProgramToUpdate);
				}
			}
		}
		else if (m_Type == DataTypes::Null)
		{
			//throw std::domain_error("Cant update uniforms for null UniformValue");
			//inget g�rs, inget h�nder
		}
		else
		{
			throw std::domain_error("Corrupted UniformValue");
		}
	}
	void UniformValue::UpdateUniforms(UniformValue& ValuesToUpdate) const
	{
		if (m_Type == DataTypes::Float)
		{
			ValuesToUpdate.SetFloat(p_GetData<float>());
		}
		else if (m_Type == DataTypes::Int)
		{
			ValuesToUpdate.SetInt(p_GetData<int>());
		}
		else if (m_Type == DataTypes::Vec3)
		{
			ValuesToUpdate.SetVec3(p_GetData<MBMath::MBVector3<float>>());
		}
		else if (m_Type == DataTypes::Vec4)
		{
			ValuesToUpdate.SetVec4(p_GetData<MBMath::MBStaticVector<float,4>>());
		}
		else if (m_Type == DataTypes::Matrix4)
		{
			ValuesToUpdate.SetMat4(p_GetData<MBMath::MBMatrix4<float>>());
		}
		else if (m_Type == DataTypes::Array)
		{
			std::vector<UniformValue> const& InternalVectorData = p_GetData<std::vector<UniformValue>>();
			for (size_t i = 0; i < InternalVectorData.size();i++)
			{
				UniformValue const& Value = InternalVectorData[i];
				if (Value.GetType() != DataTypes::Null)
				{
					ValuesToUpdate.SetIndex(i,Value);
				}
			}
		}
		else if (m_Type == DataTypes::Struct)
		{
			std::map<std::string, UniformValue> const& InternalStructData = p_GetData<std::map<std::string, UniformValue>>();
			for (auto const& Value : InternalStructData)
			{
				if (Value.second.GetType() != DataTypes::Null)
				{
					ValuesToUpdate.SetValue(Value.first, Value.second);
				}
			}
		}
		else if (m_Type == DataTypes::Null)
		{
			throw std::domain_error("Cant update uniforms for null UniformValue");
		}
		else
		{
			throw std::domain_error("Corrupted UniformValue");
		}
	}
	void UniformValue::SetFloat(float FloatValue)
	{
		p_GenericSet<float>(DataTypes::Float, FloatValue);
	}
	float& UniformValue::GetFloat()
	{
		return(p_GenericGet<float>(DataTypes::Float));
	}
	void UniformValue::SetInt(int FloatValue)
	{
		p_GenericSet<int>(DataTypes::Int, FloatValue);
	}
	int& UniformValue::GetInt()
	{
		return(p_GenericGet<int>(DataTypes::Int));
	}
	MBMath::MBVector3<float>& UniformValue::GetVec3()
	{
		return(p_GenericGet<MBMath::MBVector3<float>>(DataTypes::Vec3));
	}
	void UniformValue::SetVec3(MBMath::MBStaticVector3<float> const& VectorToSet)
	{
		p_GenericSet<MBMath::MBVector3<float>>(DataTypes::Vec3, VectorToSet);
	}
	void UniformValue::SetVec4(MBMath::MBStaticVector<float, 4> const& VectorToSet)
	{
		p_GenericSet<MBMath::MBStaticVector<float,4>>(DataTypes::Vec4, VectorToSet);
	}
	MBMath::MBStaticVector<float,4>& UniformValue::GetVec4()
	{
		return(p_GenericGet<MBMath::MBStaticVector<float,4>>(DataTypes::Vec4));
	}
	void UniformValue::SetMat4(MBMath::MBMatrix4<float> const& MatrixToSet)
	{
		p_GenericSet<MBMath::MBMatrix4<float>>(DataTypes::Matrix4, MatrixToSet);
	}
	void UniformValue::SetIndex(size_t IndexToGet,UniformValue NewValue)
	{
		if (m_Type != DataTypes::Array)
		{
			*this = UniformValue(std::vector<UniformValue>());
		}
		std::vector<UniformValue>& InternalVector = p_GetData<std::vector<UniformValue>>();
		//implicit appendar listan tills det att den �r lika stor som indexet vi s�tter
		while (InternalVector.size() <= IndexToGet)
		{
			InternalVector.push_back(UniformValue());
		}
		InternalVector[IndexToGet] = std::move(NewValue);
	}
	UniformValue& UniformValue::GetIndex(size_t IndexToGet)
	{
		if (m_Type != DataTypes::Array)
		{
			throw std::domain_error("Object not of array type");
		}
		else
		{
			std::vector<UniformValue>& InternalVector = p_GetData<std::vector<UniformValue>>();
			//throwar error om ett v�rde inte finns vid indexet
			if (InternalVector.size() <= IndexToGet)
			{
				throw std::domain_error("Index out of range");
			}
			return(InternalVector[IndexToGet]);
		}
	}
	void UniformValue::SetValue(std::string const& ValueName, UniformValue ValueToAdd)
	{
		//Implicit convertar den till ett struct objekt
		if (m_Type != DataTypes::Struct)
		{
			*this = UniformValue(std::map<std::string,UniformValue>());
		}
		std::map<std::string, UniformValue>& InternalMap = p_GetData<std::map<std::string, UniformValue>>();
		//throwar error om ett v�rde inte finns vid indexet
		InternalMap[ValueName] = std::move(ValueToAdd);
	}
	UniformValue& UniformValue::GetValue(std::string const& ValueName)
	{
		if (m_Type != DataTypes::Struct)
		{
			throw std::domain_error("Object not of struct type");
		}
		else
		{
			std::map<std::string,UniformValue>& InternalMap = p_GetData<std::map<std::string, UniformValue>>();
			//throwar error om ett v�rde inte finns vid indexet
			if (InternalMap.find(ValueName) == InternalMap.end())
			{
				throw std::domain_error("Value doesn't exist");
			}
			return(InternalMap[ValueName]);
		}
	}
	bool UniformValue::HasValue(std::string const& ValueName)
	{
		if (m_Type != DataTypes::Struct)
		{
			throw std::domain_error("Object not of struct type");
		}
		std::map<std::string, UniformValue>& InternalMap = p_GetData<std::map<std::string, UniformValue>>();
		return(InternalMap.find(ValueName) != InternalMap.end());
	}
	MBMath::MBMatrix4<float>& UniformValue::GetMat4()
	{
		return(p_GenericGet<MBMath::MBMatrix4<float>>(DataTypes::Matrix4));
	}

	//END UniformValue

	//BEGIN UniformBundle
	//std::unique_ptr<UniformValue> GetUniformValue(std::vector<std::unique_ptr<UniformValue>>&& ArrayValues)
	//{
	//	return(std::unique_ptr<UniformValue>(new UniformValue_Array(ArrayValues)));
	//}
	//std::unique_ptr<UniformValue> GetUniformValue(std::map<std::string, std::unique_ptr<UniformValue>>&& MapValues)
	//{
	//	return(std::unique_ptr<UniformValue>(new UniformValue_Aggregate(MapValues)));
	//}
	//std::unique_ptr<UniformValue> GetUniformValue(float Value)
	//{
	//	return(std::unique_ptr<UniformValue>(new UniformValue_Float(Value)));
	//}
	//std::unique_ptr<UniformValue> GetUniformValue(int Value)
	//{
	//	return(std::unique_ptr<UniformValue>(new UniformValue_Int(Value)));
	//}
	//std::unique_ptr<UniformValue> GetUniformValue(MBMath::MBVector3<float> const& Value)
	//{
	//	return(std::unique_ptr<UniformValue>(new UniformValue_Vec3(Value)));
	//}
	//std::unique_ptr<UniformValue> GetUniformValue(float x, float y, float z)
	//{
	//	return(std::unique_ptr<UniformValue>(new UniformValue_Vec3(x,y,z)));
	//}
	//std::unique_ptr<UniformValue> GetUniformValue(MBMath::MBMatrix4<float> const& Value)
	//{
	//	return(std::unique_ptr<UniformValue>(new UniformValue_Mat4(Value)));
	//}
	
	
	//void swap(UniformBundle& LeftBundle, UniformBundle& RightBundle)
	//{
	//	std::swap(LeftBundle, RightBundle);
	//}
	//UniformBundle::UniformBundle(UniformBundle&& BundleToSteal) noexcept
	//{
	//	std::swap(*this, BundleToSteal);
	//}
	//UniformBundle::UniformBundle(UniformBundle const& BundleToCopy)
	//{
	//	for (auto const& Value : BundleToCopy.m_UniformMap)
	//	{
	//		m_UniformMap[Value.first] = std::move(Value.second->Copy());
	//	}
	//}
	//UniformBundle& UniformBundle::operator=(UniformBundle BundleToSteal)
	//{
	//	std::swap(*this, BundleToSteal);
	//	return(*this);
	//}
	//void UniformBundle::SetUniforms(ShaderProgram* ProgramToModify)
	//{
	//	for (auto& Value : m_UniformMap)
	//	{
	//		Value.second->SetValue(Value.first, ProgramToModify);
	//	}
	//}
	//void UniformBundle::SetUniform_Float(std::string const& UniformName, double FloatValue)
	//{
	//	//throwa exception om inte finns?
	//	if (m_UniformMap.find(UniformName) != m_UniformMap.end())
	//	{
	//		m_UniformMap[UniformName]->SetFloat(FloatValue);
	//	}
	//	else
	//	{
	//		m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Float(FloatValue)));
	//	}
	//}
	//void UniformBundle::SetUniform_Int(std::string const& UniformName, int IntegerValue)
	//{
	//	if (m_UniformMap.find(UniformName) != m_UniformMap.end())
	//	{
	//		m_UniformMap[UniformName]->SetInt(IntegerValue);
	//	}
	//	else
	//	{
	//		m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Int(IntegerValue)));
	//	}
	//}
	//void UniformBundle::SetUniform_Vec3(std::string const& UniformName, float x, float y, float z)
	//{
	//	if (m_UniformMap.find(UniformName) != m_UniformMap.end())
	//	{
	//		m_UniformMap[UniformName]->SetVec3(x,y,z);
	//	}
	//	else
	//	{
	//		m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Vec3(x,y,z)));
	//	}
	//}
	//void UniformBundle::SetUniform_Vec3(std::string const& UniformName, MBMath::MBStaticVector3<float> const& VectorToSet)
	//{
	//	if (m_UniformMap.find(UniformName) != m_UniformMap.end())
	//	{
	//		m_UniformMap[UniformName]->SetVec3(VectorToSet[0],VectorToSet[1],VectorToSet[2]);
	//	}
	//	else
	//	{
	//		m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Vec3(VectorToSet[0],VectorToSet[1],VectorToSet[2])));
	//	}
	//}
	//void UniformBundle::SetUniform_Mat4(std::string const& UniformName, MBMath::MBMatrix4<float> const& MatrixToSet)
	//{
	//	if (m_UniformMap.find(UniformName) != m_UniformMap.end())
	//	{
	//		m_UniformMap[UniformName]->SetMat4(MatrixToSet);
	//	}
	//	else
	//	{
	//		m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Mat4(MatrixToSet)));
	//	}
	//}
	//void UniformBundle::AddUniformVector(std::string const& VectorName)
	//{
	//	//borde den kolla hurvida den redan finns?
	//	m_UniformMap[VectorName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Array()));
	//}
	//void UniformBundle::AddAggregateType(std::string const& UniformName)
	//{
	//	m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Aggregate()));
	//}
	//
	//UniformValue& UniformBundle::GetUniform(std::string const& UniformName)
	//{
	//	return(*m_UniformMap.at(UniformName));
	//}
	//void UniformBundle::AddUniform(std::string const& UniformName, std::unique_ptr<UniformValue> ValueToAdd)
	//{
	//	m_UniformMap[UniformName] = std::move(ValueToAdd);
	//}


	//END UniformBundle
	//UniformValue_Array const& Material::GetUniformArray(std::string const& UniformName) const
	//{
	//
	//}
	//UniformValue_Aggregate const& Material::GetUniformStruct(std::string const& UniformName) const
	//{
	//
	//}
	//Bone
	Bone::Bone(void* AssimpData)
	{
		aiBone* AssimpBone = (aiBone*)AssimpData;
		Weights.reserve(AssimpBone->mNumWeights);
		for (size_t i = 0; i < AssimpBone->mNumWeights; i++)
		{
			Weights.push_back(BoneWeight{ AssimpBone->mWeights[i].mVertexId,AssimpBone->mWeights[i].mWeight});
		}
		BoneID = std::string(AssimpBone->mName.C_Str());
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				OffsetMatrix(i, j) = AssimpBone->mOffsetMatrix[i][j];
			}
		}
	}
	//NodeAnimation
	MBMath::MBMatrix4<float> NodeAnimation::GetInterpolatedRotation(float TimeInSec) const
	{
		//vi ser vad som h�nder om vi k�r ingen interpolering
		NodeAnimationRotationKey LowerKey;
		NodeAnimationRotationKey HigherKey;
		for (size_t i = 0; i < RotationKeys.size(); i++)
		{
			if (RotationKeys[i].KeyTime / TicksPerSec >= TimeInSec)
			{
				HigherKey = RotationKeys[i];
				//return(MBMath::MBMatrix4<float>(HigherKey.Rotation.GetRotationMatrix()));
				if (i != 0)
				{
					LowerKey = RotationKeys[i - 1];
				}
				else
				{
					//vi har bara en key, r�tt konstigt, s� vi s�tter dem till samma
					LowerKey.Rotation = HigherKey.Rotation;
					HigherKey.KeyTime = (TimeInSec + 1) * TicksPerSec;
					LowerKey.KeyTime = 0;
					//assert(false);
				}
				break;
			}
			if (i == RotationKeys.size() - 1)
			{
				//inneb�r antingen att vi bara har en key, eller att vi g�tt �ver tiden, oavsett vad s� g�r vi till b�rjan
				LowerKey = RotationKeys[0];
				if (RotationKeys.size() > 1)
				{
					HigherKey = RotationKeys[1];
				}
				else
				{
					HigherKey.KeyTime = LowerKey.KeyTime + 1 * TicksPerSec;
					HigherKey.Rotation = LowerKey.Rotation;
				}
				TimeInSec = fmod(TimeInSec, HigherKey.KeyTime / TicksPerSec);
				//assert(false);
			}
		}
		float KeyTimeDifference = (HigherKey.KeyTime - LowerKey.KeyTime) / TicksPerSec;
		float ElapsedTime = TimeInSec-(LowerKey.KeyTime / TicksPerSec);
		assert(ElapsedTime >= 0);
		float NormalizedTime = ElapsedTime / KeyTimeDifference;
		MBMath::Quaternion<float> NewRotation = LowerKey.Rotation.Slerp(HigherKey.Rotation, NormalizedTime);
		MBMath::MBMatrix4<float> ReturnValue = MBMath::MBMatrix4<float>(NewRotation.GetRotationMatrix());
		return(ReturnValue);
	}
	MBMath::MBMatrix4<float> NodeAnimation::GetInterpolatedScaling(float TimeInSec) const
	{
		NodeAnimationScalingKey LowerKey;
		NodeAnimationScalingKey HigherKey;
		for (size_t i = 0; i < ScalingKeys.size(); i++)
		{
			if (ScalingKeys[i].KeyTime / TicksPerSec >= TimeInSec)
			{
				HigherKey = ScalingKeys[i];
				if (i != 0)
				{
					LowerKey = ScalingKeys[i - 1];
				}
				else
				{
					//vi har bara en key, r�tt konstigt, s� vi s�tter dem till samma
					LowerKey.Scaling = HigherKey.Scaling;
					HigherKey.KeyTime = (TimeInSec+1)*TicksPerSec;
					LowerKey.KeyTime = 0;
					//assert(false);
				}
				break;
			}
			if (i == ScalingKeys.size() - 1)
			{
				//inneb�r antingen att vi bara har en key, eller att vi g�tt �ver tiden, oavsett vad s� g�r vi till b�rjan
				LowerKey = ScalingKeys[0];
				if (ScalingKeys.size() > 1)
				{
					HigherKey = ScalingKeys[1];
				}
				else
				{
					HigherKey.KeyTime = LowerKey.KeyTime + 1 * TicksPerSec;
					HigherKey.Scaling = LowerKey.Scaling;
				}
				TimeInSec = fmod(TimeInSec, HigherKey.KeyTime / TicksPerSec);
				//assert(false);
			}
		}
		float KeyTimeDifference = (HigherKey.KeyTime - LowerKey.KeyTime) / TicksPerSec;
		float ElapsedTime = TimeInSec-(LowerKey.KeyTime / TicksPerSec);
		assert(ElapsedTime >= 0);
		float NormalizedTime = ElapsedTime / KeyTimeDifference;
		MBMath::MBVector3<float> NewScale = MBMath::LinearInterpolation(LowerKey.Scaling,HigherKey.Scaling, NormalizedTime);
		MBMath::MBMatrix4<float> ReturnValue = MBMath::MBMatrix4<float>();
		//ska det vara minus kanske?
		ReturnValue(0, 0) = NewScale[0];
		ReturnValue(1, 1) = NewScale[1];
		ReturnValue(2, 2) = NewScale[2];
		return(ReturnValue);
	}
	MBMath::MBMatrix4<float> NodeAnimation::GetInterpolatedTranslation(float TimeInSec) const
	{
		NodeAnimationPositionKey LowerKey;
		NodeAnimationPositionKey HigherKey;
		for (size_t i = 0; i < TranslationKeys.size(); i++)
		{
			if (TranslationKeys[i].KeyTime/TicksPerSec >= TimeInSec)
			{
				HigherKey = TranslationKeys[i];
				if (i != 0)
				{
					LowerKey = TranslationKeys[i-1];
				}
				else
				{
					//vi har bara en key, r�tt konstigt, s� vi s�tter dem till samma
					LowerKey.Position = HigherKey.Position;
					HigherKey.KeyTime = (TimeInSec + 1) * TicksPerSec;
					LowerKey.KeyTime = 0;
					//assert(false);
				}
				break;
			}
			if (i == TranslationKeys.size() - 1)
			{
				//inneb�r antingen att vi bara har en key, eller att vi g�tt �ver tiden, oavsett vad s� g�r vi till b�rjan
				LowerKey = TranslationKeys[0];
				if (TranslationKeys.size() > 1)
				{
					HigherKey = TranslationKeys[1];
				}
				else
				{
					HigherKey.KeyTime = LowerKey.KeyTime + 1 * TicksPerSec;
					HigherKey.Position = LowerKey.Position;
				}
				TimeInSec = fmod(TimeInSec, HigherKey.KeyTime / TicksPerSec);
				//assert(false);
			}
		}
		float KeyTimeDifference = (HigherKey.KeyTime - LowerKey.KeyTime) / TicksPerSec;
		float ElapsedTime = TimeInSec-(LowerKey.KeyTime / TicksPerSec);
		assert(ElapsedTime >= 0);
		float NormalizedTime = ElapsedTime / KeyTimeDifference;
		MBMath::MBVector3<float> NewTranslation = MBMath::LinearInterpolation(LowerKey.Position,HigherKey.Position, NormalizedTime);
		MBMath::MBMatrix4<float> ReturnValue = MBMath::MBMatrix4<float>();
		//ska det vara minus kanske?
		ReturnValue(0, 3) = NewTranslation[0];
		ReturnValue(1, 3) = NewTranslation[1];
		ReturnValue(2, 3) = NewTranslation[2];
		return(ReturnValue);
	}
	NodeAnimation::NodeAnimation(void* AssimpData,double AnimationDuration,double NewTicksPerSec)
	{
		aiNodeAnim* AssimpNodeAnimation = (aiNodeAnim*)AssimpData;
		TickDuration = AnimationDuration;
		TicksPerSec = NewTicksPerSec;
		for (size_t i = 0; i < AssimpNodeAnimation->mNumPositionKeys; i++)
		{
			NodeAnimationPositionKey NewPositionKey;
			NewPositionKey.KeyTime = AssimpNodeAnimation->mPositionKeys[i].mTime;
			NewPositionKey.Position[0] = AssimpNodeAnimation->mPositionKeys[i].mValue[0];
			NewPositionKey.Position[1] = AssimpNodeAnimation->mPositionKeys[i].mValue[1];
			NewPositionKey.Position[2] = AssimpNodeAnimation->mPositionKeys[i].mValue[2];
			TranslationKeys.push_back(NewPositionKey);
		}
		for (size_t i = 0; i < AssimpNodeAnimation->mNumScalingKeys; i++)
		{
			NodeAnimationScalingKey NewScalingKey;
			NewScalingKey.KeyTime = AssimpNodeAnimation->mScalingKeys[i].mTime;
			NewScalingKey.Scaling[0] = AssimpNodeAnimation->mScalingKeys[i].mValue[0];
			NewScalingKey.Scaling[1] = AssimpNodeAnimation->mScalingKeys[i].mValue[1];
			NewScalingKey.Scaling[2] = AssimpNodeAnimation->mScalingKeys[i].mValue[2];
			ScalingKeys.push_back(NewScalingKey);
		}
		for (size_t i = 0; i < AssimpNodeAnimation->mNumRotationKeys; i++)
		{
			NodeAnimationRotationKey NewRotationKey;
			NewRotationKey.KeyTime = AssimpNodeAnimation->mRotationKeys[i].mTime;
			NewRotationKey.Rotation.a = AssimpNodeAnimation->mRotationKeys[i].mValue.w;
			NewRotationKey.Rotation.i = AssimpNodeAnimation->mRotationKeys[i].mValue.x;
			NewRotationKey.Rotation.j = AssimpNodeAnimation->mRotationKeys[i].mValue.y;
			NewRotationKey.Rotation.k = AssimpNodeAnimation->mRotationKeys[i].mValue.z;
			RotationKeys.push_back(NewRotationKey);
		}
	}
	MBMath::MBMatrix4<float> NodeAnimation::GetTransformation(float TimeInSec) const
	{
		MBMath::MBMatrix4<float> ReturnValue = GetInterpolatedTranslation(TimeInSec) * GetInterpolatedRotation(TimeInSec) * GetInterpolatedScaling(TimeInSec);
		return(ReturnValue);
	}
	//BEGIN SkeletonAnimation

	bool SkeletonAnimation::IsInAnimation(std::string const& StringToCheck) const
	{
		return(m_NodeAnimations.find(StringToCheck) != m_NodeAnimations.end());
	}
	MBMath::MBMatrix4<float> SkeletonAnimation::GetNodeTransformation(std::string const& NodeID, double TimeInSec) const
	{
		return(m_NodeAnimations.at(NodeID).GetTransformation(TimeInSec));
	}
	//END SkeletionAnimation

	//ModelAnimation
	ModelAnimation::ModelAnimation(void* AssimpData)
	{
		aiAnimation* AssimpAnimation = (aiAnimation*)AssimpData;
		TicksPerSec = AssimpAnimation->mTicksPerSecond;
		TickDuration = AssimpAnimation->mDuration;
		std::unordered_map<std::string, NodeAnimation> AnimationData = {};
		for (size_t i = 0; i < AssimpAnimation->mNumChannels; i++)
		{
			AnimationData[std::string(AssimpAnimation->mChannels[i]->mNodeName.C_Str())] = NodeAnimation(AssimpAnimation->mChannels[i],AssimpAnimation->mDuration,AssimpAnimation->mTicksPerSecond);
		}
		BoneAnimation = SkeletonAnimation(std::move(AnimationData));
	}
	//MBMath::MBMatrix4<float> ModelAnimation::GetNodeTransformation(float TimeInSec,std::string Node)
	//{
	//	NodeAnimation AssociatedNodeTransformation = NodeAnimations[Node];
	//	return(AssociatedNodeTransformation.GetTransformation(TimeInSec));
	//}


	//BEGIN Transform
	MBMath::MBMatrix4<float> Transform::GetModelMatrix() const
	{
		MBMath::MBMatrix4<float> ReturnValue;
		MBMath::MBMatrix4<float> ScalingMatrix;
		MBMath::MBMatrix4<float> RotationMatrix;
		
		ScalingMatrix(0,0) = m_Scalings[0];
		ScalingMatrix(1,1) = m_Scalings[1];
		ScalingMatrix(2,2) = m_Scalings[2];

		//RotationMatrix = MBMath::GetRotationMatrix<float>(m_Rotation[0], m_Rotation[1], m_Rotation[2]);
		RotationMatrix = MBMath::GetRotationMatrix<float>(-m_Rotation[1], m_Rotation[2], m_Rotation[0]);

		ReturnValue =  RotationMatrix*ScalingMatrix;
		ReturnValue(0, 3) = m_WorldPosition[0];
		ReturnValue(1, 3) = m_WorldPosition[1];
		ReturnValue(2, 3) = m_WorldPosition[2];

		return(ReturnValue);
		
	}
	void Transform::SetRotation(float XRotation, float YRotation, float ZRotation)
	{
		m_Rotation[0] = XRotation;
		m_Rotation[1] = YRotation;
		m_Rotation[2] = ZRotation;
	}
	void Transform::SetRotation(MBMath::MBVector3<float> const& RotationToSet)
	{
		m_Rotation = RotationToSet;
	}
	MBMath::MBVector3<float> Transform::GetRotation() const
	{
		return(m_Rotation);
	}
	void Transform::SetPosition(MBMath::MBVector3<float> const& PositionToSet)
	{
		m_WorldPosition = PositionToSet;
	}
	void Transform::SetPosition(float x, float y, float z)
	{
		m_WorldPosition[0] = x;
		m_WorldPosition[1] = y;
		m_WorldPosition[2] = z;
	}
	MBMath::MBVector3<float> Transform::GetPosition() const
	{
		return(m_WorldPosition);
	}
	void Transform::SetScaling(MBMath::MBVector3<float> const& PositionToSet)
	{
		m_Scalings = PositionToSet;
	}
	void Transform::SetScaling(float x, float y, float z)
	{
		m_Scalings[0] = x;
		m_Scalings[1] = y;
		m_Scalings[2] = z;
	}
	MBMath::MBVector3<float> Transform::GetScaling() const
	{
		return(m_Scalings);
	}
	//END Transform


	//Model
	void Model::Rotate(float DegreesToRotate, MBMath::MBVector3<float> const& RotationAxis)
	{
		//roterar helt enkelt varje mesh motsvarande grader
		for (size_t i = 0; i < ModelMeshes.size(); i++)
		{
			(*ModelMeshes[i]).Rotate(DegreesToRotate, RotationAxis);
		}
	}
	Model::Model(std::string const& ModelPath,MBGraphicsEngine* AttachedEngine)
	{
		//hur vi faktiskt tar och laddar in moddelen
		AssociatedEngine = AttachedEngine;
		std::vector<VertexAttributes> StandardAttributeOrder = { VertexAttributes::TextureCoordinates,VertexAttributes::Tangent,VertexAttributes::Bitangent,VertexAttributes::VertexNormal};
		Assimp::Importer Importer;
		const aiScene* LoadedScene = Importer.ReadFile(ModelPath, aiProcess_Triangulate| aiProcess_CalcTangentSpace);
		if (!LoadedScene || LoadedScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !LoadedScene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << std::endl;
			return;
		}
		//l�gger till meshes
		unsigned int NumberOfMeshes = LoadedScene->mNumMeshes;
		unsigned int NumberOfTriangels = 0;
		for (size_t i = 0; i < NumberOfMeshes; i++)
		{
			size_t MeshMaterialIndex = 0;
			ModelMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(LoadedScene->mMeshes[i], StandardAttributeOrder, m_BoneMap, &MeshMaterialIndex)));
			m_MeshMaterialIndexes.push_back(MeshMaterialIndex);
			NumberOfTriangels += ModelMeshes.back()->NumberOfVertices();
		}
		std::cout << "Number of Triangels "<<NumberOfTriangels/3<<std::endl;
		//l�gger till nodes
		TopNode = Node(LoadedScene->mRootNode);
		m_NumberOfNodes = TopNode.GetNumberOfChildren() + 1;
		//l�gger till materials
		unsigned int NumberOfMaterials = LoadedScene->mNumMaterials;
		for (size_t i = 0; i < NumberOfMaterials; i++)
		{
			ModelMaterials.push_back(Material(LoadedScene->mMaterials[i], ModelPath.substr(0, ModelPath.find_last_of("/")+1),AssociatedEngine));
		}
	}
	ModelAnimation* Model::GetCurrentAnimation()
	{
		if (Animations.size() > 0)
		{
			return(&Animations.front());
		}
		else
		{
			return(nullptr);
		}
	}
	void Model::SetShader(std::shared_ptr<ShaderProgram> ProgramToUse)
	{
		m_ModelShader = ProgramToUse;
	}
	Model::Model(std::string const& ModelPath, std::vector<MaterialAttribute> MaterialAttributes, MBGraphicsEngine* AttachedEngine)
	{
		//hur vi faktiskt tar och laddar in moddelen
		AssociatedEngine = AttachedEngine;
		std::vector<VertexAttributes> StandardAttributeOrder = { VertexAttributes::TextureCoordinates,VertexAttributes::Tangent,VertexAttributes::Bitangent,VertexAttributes::VertexNormal };
		Assimp::Importer Importer;
		//Importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		const aiScene* LoadedScene = Importer.ReadFile(ModelPath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
		if (!LoadedScene || LoadedScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !LoadedScene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << std::endl;
			return;
		}
		//b�rjar med att l�gg till v�ra ben till en mapp
		//vi vi s�tter ben 0 att vara "noll benet" som �r enhetsmatrisen s� alla vertices kan ha lika m�nga ben
		int BoneIndexCounter = 1;
		for (size_t i = 0; i < LoadedScene->mNumMeshes; i++)
		{
			for (size_t j = 0; j < LoadedScene->mMeshes[i]->mNumBones; j++)
			{
				aiBone* NewBone = LoadedScene->mMeshes[i]->mBones[j];
				std::string NewBoneID = std::string(NewBone->mName.C_Str());
				if (m_BoneMap.find(NewBoneID) == m_BoneMap.end())
				{
					m_BoneMap[NewBoneID] = Bone(NewBone);
					m_BoneMap[NewBoneID].BoneIndex = BoneIndexCounter;
					BoneIndexCounter += 1;
				}
			}
		}
		//BoneOffsetList = std::vector<MBMath::MBMatrix4<float>>(BoneIndexCounter, MBMath::MBMatrix4<float>());
		//l�gger till nodes, populatar ben data som meshesen beh�ver
		TopNode = Node(LoadedScene->mRootNode);
		m_NumberOfNodes = TopNode.GetNumberOfChildren() + 1;
		m_InverseGlobalMatrix = TopNode.GetLocalTransformation().GetInverse();
		//l�gger till animationer
		for (size_t i = 0; i < LoadedScene->mNumAnimations; i++)
		{
			Animations.push_back(ModelAnimation(LoadedScene->mAnimations[i]));
		}
		//l�gger till meshes
		unsigned int NumberOfMeshes = LoadedScene->mNumMeshes;
		unsigned int NumberOfTriangels = 0;
		for (size_t i = 0; i < NumberOfMeshes; i++)
		{
			size_t MeshMaterialIndex = 0;
			ModelMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(LoadedScene->mMeshes[i], StandardAttributeOrder, m_BoneMap,&MeshMaterialIndex)));
			m_MeshMaterialIndexes.push_back(MeshMaterialIndex);
			NumberOfTriangels += ModelMeshes.back()->NumberOfVertices();
		}
		std::cout << "Number of Triangels " << NumberOfTriangels / 3 << std::endl;
		//l�gger till materials
		unsigned int NumberOfMaterials = LoadedScene->mNumMaterials;
		for (size_t i = 0; i < NumberOfMaterials; i++)
		{
			ModelMaterials.push_back(Material(LoadedScene->mMaterials[i], ModelPath.substr(0, ModelPath.find_last_of("/") + 1),MaterialAttributes, AssociatedEngine));
			ModelMaterials.back().MaterialShaderAttributes = MaterialAttributes;
		}
	}

	//Mesh* Model::GetMesh(unsigned int MeshIndex)
	//{
	//	return(ModelMeshes[MeshIndex]);
	//}
	//Material* Model::GetMaterial(unsigned int MaterialIndex)
	//{
	//	return(&ModelMaterials[MaterialIndex]);
	//}
	void Model::p_DrawAnimation(ModelAnimation const& ModelAnimationToDraw,ShaderProgram* ShaderToUse, double AnimationTimeInSec)
	{
		std::vector<MBMath::MBMatrix4<float>> Transformations = TopNode.GetAnimationTransformations(ModelAnimationToDraw.BoneAnimation, AnimationTimeInSec);
		std::unordered_map<std::string, size_t> NodeIDMap = TopNode.GetNodeIDMap();

		AssociatedEngine->CameraObject.SetModelMatrix(ModelTransform.GetModelMatrix());
		AssociatedEngine->CameraObject.Update(m_ModelShader.get());

		MBMath::MBMatrix4<float> StandardMatrix = MBMath::MBMatrix4<float>();
		ShaderToUse->SetUniformMat4f("BoneTransforms[0]", StandardMatrix.GetContinousData());
		for (auto const& CurrentBone : m_BoneMap)
		{
			size_t TransformationIndex = NodeIDMap[CurrentBone.second.BoneID];
			//MBMath::MBMatrix4<float> BoneTransform = AssociatedModel->InverseGlobalMatrix * NodeTransformation * AssociatedBone->OffsetMatrix;
			MBMath::MBMatrix4<float> NewTransformation = m_InverseGlobalMatrix * Transformations[TransformationIndex] * CurrentBone.second.OffsetMatrix;
			ShaderToUse->SetUniformMat4f("BoneTransforms[" + std::to_string(CurrentBone.second.BoneIndex) + "]",NewTransformation.GetContinousData());
		}
		//Bonesen �r uppdaterade, nu till rita alla meshes
		for (size_t i = 0; i < ModelMeshes.size(); i++)
		{
			Material& MaterialToUse = ModelMaterials[m_MeshMaterialIndexes[i]];
			MaterialToUse.SetUniforms(m_ModelShader.get());
			ModelMeshes[i]->Draw();
		}
	}
	void Model::p_DrawDefault()
	{
		//std::vector<MBMath::MBMatrix4<float>> Transformations = TopNode.GetDefaultTransformations();
		std::shared_ptr<ShaderProgram> CurrentShader = AssociatedEngine->GetCurrentShader();
		MBMath::MBMatrix4<float> DefaultMatrix = MBMath::MBMatrix4<float>();
		for (auto const& CurrentBone : m_BoneMap)
		{
			CurrentShader->SetUniformMat4f("BoneTransforms[" + std::to_string(CurrentBone.second.BoneIndex) + "]", DefaultMatrix.GetContinousData());
		}
		p_DrawDefault_Node(&TopNode, MBMath::MBMatrix4<float>());
	}
	void Model::p_DrawDefault_Node(const Node* NodeToProcess, MBMath::MBMatrix4<float> ParentTransformation)
	{
		MBMath::MBMatrix4<float> ModelMatrix = NodeToProcess->LocalTransformation * ParentTransformation;
		if (NodeToProcess->MeshIndexes.size() > 0)
		{
			AssociatedEngine->CameraObject.SetModelMatrix(ModelMatrix);
			AssociatedEngine->CameraObject.Update(m_ModelShader.get());
		}
		for (size_t i = 0; i < NodeToProcess->MeshIndexes.size(); i++)
		{
			Mesh& MeshToDraw = *ModelMeshes[NodeToProcess->MeshIndexes[i]];
			Material& MaterialToUse = ModelMaterials[m_MeshMaterialIndexes[NodeToProcess->MeshIndexes[i]]];
			MaterialToUse.SetUniforms(m_ModelShader.get());
			MeshToDraw.Draw();
		}
		std::vector<Node> const& NodeChildren = NodeToProcess->GetChildren();
		for (size_t i = 0; i < NodeToProcess->GetChildren().size(); i++)
		{
			p_DrawDefault_Node(&NodeChildren[i], ModelMatrix);
		}
	}
	void Model::Draw()
	{		
		//AssociatedEngine->SetCurrentShader(m_ModelShader);
		m_ModelShader->Bind();

		if (m_BoneMap.size() != 0)
		//if (false)
		{
			AnimationIsPlaying = true;
			AnimationTime += AssociatedEngine->GetDeltaTimeInSec();

			ModelAnimation* CurrentAnimation = GetCurrentAnimation();
			if (AnimationTime > CurrentAnimation->GetDurationInSec())
			{
				AnimationTime = 0;
			}
			p_DrawAnimation(*CurrentAnimation, m_ModelShader.get(),AnimationTime);
			//TopNode.DrawAnimation(CurrentAnimation->BoneAnimation,AssociatedEngine->GetShader(ModelShader),AnimationTime);
		}
		else
		{
			//TopNode.Draw(MBMath::MBMatrix4<float>());
			p_DrawDefault();
		}
	}
	//BEGIN SpriteModel
	SpriteModel::SpriteModel(std::shared_ptr<Texture> Texture)
	{
		//m_AssociatedEngine = AssociatedEngine;
		m_SpriteTexture = Texture;

		float Width = 1;
		float AspectRation = 1;
		float Height = Width * AspectRation;
		float VertexData[20];
		//0
		VertexData[0] = -Width / 2;
		VertexData[1] = Height/2;
		VertexData[2] = 0;
		VertexData[3] = 0;
		VertexData[4] = 1;
		//
		//1
		VertexData[5] = Width / 2;
		VertexData[6] = Height / 2;
		VertexData[7] = 0;
		VertexData[8] = 1;
		VertexData[9] = 1;
		//
		//2
		VertexData[10] = -Width / 2;
		VertexData[11] = -Height / 2;
		VertexData[12] = 0;
		VertexData[13] = 0;
		VertexData[14] = 0;
		//
		//3
		VertexData[15] = Width / 2;
		VertexData[16] = -Height / 2;
		VertexData[17] = 0;
		VertexData[18] = 1;
		VertexData[19] = 0;
		//
		uint32_t FaceData[6] = { 0,1,2,1,2,3 };
		m_SpriteMesh.FillMeshData({ VertexAttributes::Position,VertexAttributes::TextureCoordinates }, VertexData, sizeof(VertexData), FaceData, 6);
	}
	void SpriteModel::SetShader(std::shared_ptr<ShaderProgram> ShaderToUse)
	{
		m_SpriteShader = ShaderToUse;
	}
	void SpriteModel::SetTexture(std::shared_ptr<Texture> TextureToUse)
	{
		m_SpriteTexture = TextureToUse;
	}
	void SpriteModel::Draw()
	{
		m_SpriteShader->Bind();
		//m_SpriteShader->SetUniform1i("OurTexture", 0);
		m_SpriteTexture->Bind(0);
		//m_SpriteShader->PrintActiveAttributesAndUniforms();
		//m_AssociatedEngine->CameraObject.SetModelMatrix(ModelTransform.GetModelMatrix());
		//m_AssociatedEngine->CameraObject.Update(m_SpriteShader.get());
		//m_SpriteShader.get()->SetUniformMat4f("Model", ModelTransform.GetModelMatrix().GetContinousData());

		m_SpriteMesh.Draw();
	}
	
	//END SpriteModel

	//framebuffer
	FrameBuffer::FrameBuffer(int Width,int Height)
	{
		const unsigned int PixelWidth = Width;
		const unsigned int PixelHeight = Height;
		glGenFramebuffers(1, &FrameBufferHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferHandle);
		// generate texture
		glGenTextures(1, &ColorTextureHandle);
		glBindTexture(GL_TEXTURE_2D, ColorTextureHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, PixelWidth, PixelHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glCheckError();
		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTextureHandle, 0);

		glCheckError();
		//depth buffer och stencil nu
		glGenTextures(1, &DepthStencilTextureHandle);
		glBindTexture(GL_TEXTURE_2D, DepthStencilTextureHandle);
		glCheckError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, PixelWidth, PixelHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glCheckError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glCheckError();
		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthStencilTextureHandle, 0);
		glCheckError();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Error creating framebuffer" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCheckError();
	}
	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferHandle);
		glCheckError();
	}
	void FrameBuffer::BindColorBuffer(int Index)
	{
		glActiveTexture(GL_TEXTURE0 + Index);
		glBindTexture(GL_TEXTURE_2D, ColorTextureHandle);
		glCheckError();
	}
	void FrameBuffer::BindDepthBuffer(int Index)
	{
		glActiveTexture(GL_TEXTURE0 + Index);
		glBindTexture(GL_TEXTURE_2D, DepthStencilTextureHandle);
		glCheckError();
	}
	//Camera
	Camera::Camera()
	{
	}
	MBMath::MBVector3<float> Camera::GetDirection() 
	{ 
		return(Facing * -1); 
	}
	MBMath::MBVector3<float> Camera::GetRightAxis()
	{
		return(RightAxis); 
	}
	MBMath::MBVector3<float> Camera::GetUpAxis() 
	{
		return(UpAxis); 
	}
	void Camera::SetFrustum(float NearPlane, float FarPlane, float XMin, float XMax, float YMin, float YMax)
	{
		MBMath::MBMatrix4<float> NewProjectionMatrix = MBMath::MBMatrix4<float>();
		NewProjectionMatrix(0, 0) = 2 * NearPlane / (XMax - XMin);
		NewProjectionMatrix(1, 1) = 2 * NearPlane / (YMax - YMin);
		NewProjectionMatrix(0, 2) = (XMax + XMin) / (XMax - XMin);
		NewProjectionMatrix(1, 2) = (YMax + YMin) / (YMax - YMin);
		NewProjectionMatrix(2, 2) = -(FarPlane + NearPlane) / (FarPlane - NearPlane);
		NewProjectionMatrix(2, 3) = -2 * FarPlane * NearPlane / (FarPlane - NearPlane);
		NewProjectionMatrix(3, 2) = -1;
		NewProjectionMatrix(3, 3) = 0;
		//std::cout << "New projection matirx:"<<std::endl << NewProjectionMatrix << std::endl;
		//NewProjectionMatrix.PrintWolframMatrix();
		ProjectionMatrix = NewProjectionMatrix;
	}
	void Camera::SetOrtographicProjection(float Width, float Height)
	{
		const float XMax = Width/2;
		const float XMin = -Width/2;
		const float YMax = Height/2;
		const float YMin = -Height/2;
		const float FarPlane = 10000;
		const float NearPlane = 0.01;
		MBMath::MBMatrix4<float> NewProjectionMatrix = MBMath::MBMatrix4<float>();
		NewProjectionMatrix(0, 0) = 2/(XMax-XMin);
		NewProjectionMatrix(1, 1) = 2/(YMax-YMin);
		NewProjectionMatrix(2, 2) = -2 / (FarPlane - NearPlane);	
		NewProjectionMatrix(0, 3) = -(XMax + XMin) / (XMax - XMin);
		NewProjectionMatrix(1, 3) = -(YMax + YMin) / (YMax - YMin);
		NewProjectionMatrix(2, 3) = -(FarPlane + NearPlane) / (FarPlane - NearPlane);
		NewProjectionMatrix(3, 3) = 1;
		//std::cout << "New projection matirx:"<<std::endl << NewProjectionMatrix << std::endl;
		//NewProjectionMatrix.PrintWolframMatrix();
		ProjectionMatrix = NewProjectionMatrix;
	}
	MBMath::MBVector3<float> Camera::GetRotation()
	{
		return(Rotation);
	}
	void Camera::SetRotation(float XAxisRotation, float YAxisRotation, float ZAxisRotation)
	{
		//kanske finns ett b�ttre mindre ass s�tt att r�klna ut den p�
		//TODO fundera p� optimiseringen av SetRotation, borde g� att kunna anv�nda en sluten formel
		/*
		MBMath::MBMatrix<float> BaseMatrix(3);
		BaseMatrix(2, 2) = -1;
		MBMath::MBMatrix<float> XRotationMatrix =BaseMatrix* MBMath::MBVector3<float>::GetRotationMatrix(XAxisRotation, MBMath::MBVector3<float>(1, 0, 0)) * BaseMatrix.Transpose();
		MBMath::MBMatrix<float> YRotationMatrix =BaseMatrix* MBMath::MBVector3<float>::GetRotationMatrix(YAxisRotation, MBMath::MBVector3<float>(0, 1, 0)) * BaseMatrix.Transpose();
		MBMath::MBMatrix<float> ZRotationMatrix =BaseMatrix* MBMath::MBVector3<float>::GetRotationMatrix(ZAxisRotation, MBMath::MBVector3<float>(0, 0, 1)) * BaseMatrix.Transpose();
		MBMath::MBMatrix<float> RotationMatrix = XRotationMatrix * YRotationMatrix * ZRotationMatrix;
		//givet en rotation s� roterar vi "baskoordinatena" med den
		Facing = RotationMatrix*(MBMath::MBVector<float>)MBMath::MBVector3<float>(0,0,-1);
		RightAxis = RotationMatrix*(MBMath::MBVector<float>)MBMath::MBVector3<float>(1, 0, 0);
		UpAxis = RotationMatrix*(MBMath::MBVector<float>)MBMath::MBVector3<float>(0, 1, 0);
		Rotation = MBMath::MBVector3<float>(XAxisRotation, YAxisRotation, ZAxisRotation);
		*/
		float Pi = 3.1415926535;
		float XRadians = XAxisRotation * Pi / 180;
		float YRadians = YAxisRotation * Pi / 180;
		float ZRadians = ZAxisRotation * Pi / 180;
		RightAxis = MBMath::MBVector3<float>(std::cos(YRadians) * std::cos(ZRadians), -std::cos(YRadians) * std::sin(ZRadians), -std::sin(YRadians));
		UpAxis = MBMath::MBVector3<float>(std::cos(ZRadians) * std::sin(XRadians) * std::sin(YRadians) + std::cos(XRadians) * std::sin(ZRadians),
			std::cos(XRadians) * std::cos(ZRadians) - std::sin(XRadians) * std::sin(YRadians) * std::sin(ZRadians), std::cos(YRadians) * std::sin(XRadians));
		Facing = MBMath::MBVector3<float>(-std::cos(XRadians) * std::cos(ZRadians) * std::sin(YRadians) + std::sin(XRadians) * std::sin(ZRadians),
			std::cos(ZRadians) * std::sin(XRadians) + std::cos(XRadians) * std::sin(YRadians) * std::sin(ZRadians), -std::cos(XRadians) * std::cos(YRadians));
		Rotation = MBMath::MBVector3<float>(XAxisRotation, YAxisRotation, ZAxisRotation);
	}
	void Camera::SetRotation(MBMath::MBVector3<float> const& NewRotation)
	{
		SetRotation(NewRotation[0], NewRotation[1], NewRotation[2]);
	}
	void Camera::SetModelMatrix(MBMath::MBMatrix4<float> const& NewModelMatrix)
	{
		ModelMatrix = NewModelMatrix;
		MBMath::MBStaticMatrix<float,3,3> MatrixIntermediary;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				MatrixIntermediary(i, j) = ModelMatrix(i, j);
			}
		}
		//Projection* View* Model
		//std::cout << "Matrix intermediary" << std::endl;
		//std::cout << MatrixIntermediary << std::endl;
		//std::cout << "Inverse:" << std::endl << MBMath::GetInverse<float, 3>(MatrixIntermediary) << std::endl;
		MatrixIntermediary = MBMath::GetInverse<float,3>(MatrixIntermediary).Transpose();
		NormalMatrix = MBMath::MBMatrix4<float>(MatrixIntermediary);
	}
	void Camera::Update(ShaderProgram* ShaderToUpdate)
	{
		Update(m_Uniforms);
		ShaderToUpdate->Bind();
		m_Uniforms.UpdateUniforms("", ShaderToUpdate);
	}
	void Camera::Update(UniformValue& ValuesToUpdate)
	{
		MBMath::MBMatrix4<float> ViewMatrix = GetViewMatrix();

		//ShaderToUpdate->Bind();
		//ShaderToUpdate->SetUniformMat4f("Model", ModelMatrix.GetContinousData());
		//ShaderToUpdate->SetUniformMat4f("View", ViewMatrix.GetContinousData());
		//ShaderToUpdate->SetUniformMat4f("Projection", ProjectionMatrix.GetContinousData());
		//ShaderToUpdate->SetUniformMat4f("NormalMatrix", NormalMatrix.GetContinousData());
		//ShaderToUpdate->SetUniformVec3("ViewPos", WorldSpaceCoordinates[0], WorldSpaceCoordinates[1], WorldSpaceCoordinates[2]);

		ValuesToUpdate.SetValue("Model",ModelMatrix);
		ValuesToUpdate.SetValue("View",ViewMatrix);
		ValuesToUpdate.SetValue("Projection",ProjectionMatrix);
		ValuesToUpdate.SetValue("NormalMatrix",NormalMatrix);
		ValuesToUpdate.SetValue("ViewPos",WorldSpaceCoordinates);
	}
	MBMath::MBMatrix4<float> Camera::GetViewMatrix()
	{
		MBMath::MBMatrix4<float> TranslationMatrix = MBMath::MBMatrix4<float>();
		TranslationMatrix(0, 3) = -WorldSpaceCoordinates[0];
		TranslationMatrix(1, 3) = -WorldSpaceCoordinates[1];
		TranslationMatrix(2, 3) = -WorldSpaceCoordinates[2];
		MBMath::MBMatrix4<float> BaseChangeMatrix = MBMath::MBMatrix4<float>();
		BaseChangeMatrix(0, 0) = RightAxis[0];
		BaseChangeMatrix(0, 1) = RightAxis[1];
		BaseChangeMatrix(0, 2) = RightAxis[2];
		BaseChangeMatrix(1, 0) = UpAxis[0];
		BaseChangeMatrix(1, 1) = UpAxis[1];
		BaseChangeMatrix(1, 2) = UpAxis[2];
		BaseChangeMatrix(2, 0) = Facing[0];
		BaseChangeMatrix(2, 1) = Facing[1];
		BaseChangeMatrix(2, 2) = Facing[2];
		MBMath::MBMatrix4<float> ViewMatrix = BaseChangeMatrix * TranslationMatrix;
		return(ViewMatrix);
	}
	MBMath::MBMatrix4<float> Camera::GetModelMatrix()
	{
		return(ModelMatrix);
	}
	MBMath::MBMatrix4<float> Camera::GetProjectionMatrix()
	{
		return(ProjectionMatrix);
	}
	MBMath::MBMatrix4<float> Camera::GetTransformationMatrix()
	{
		//f�rst tar vi fram projektionsmatrisen f�r n�r man ska komma till v�r position i worldspace
		MBMath::MBMatrix4<float> ViewMatrix = GetViewMatrix();
		MBMath::MBMatrix4<float> TransformationMatrix = ProjectionMatrix * ViewMatrix *ModelMatrix;
		return(TransformationMatrix);
	}
	//Lightningsource
	LightSource::LightSource()
	{
		//m_Bundle.AddUniform("LightSources",)
	}
	void LightSource::SetLightning(int Position,ShaderProgram* ProgramToUpdate)
	{
		/*
			vec3 Color;
			vec3 WorldPos;
			float AmbStr;
			float SpecStr;
			float SpecExp;
		*/
		ProgramToUpdate->Bind();
		SetLightning(Position, m_Uniforms);
		m_Uniforms.UpdateUniforms("",ProgramToUpdate);
	}
	void LightSource::SetLightning(int Position, UniformValue& ValuesToUpdate)
	{
		//std::string StringPosition = std::to_string(Position);
		//std::shared_ptr<ShaderProgram> CurrentShader = AssociatedGraphicsEngine->GetCurrentShader();

		//ProgramToUpdate->SetUniformVec3("LightSources[" + StringPosition + "].WorldPos", WorldPosition[0], WorldPosition[1], WorldPosition[2]);
		//ProgramToUpdate->SetUniformVec3("LightSources[" + StringPosition + "].Color", LightColor[0], LightColor[1], LightColor[2]);
		//ProgramToUpdate->SetUniform1f("LightSources[" + StringPosition + "].AmbStr", AmbienceStrength);
		//ProgramToUpdate->SetUniform1f("LightSources[" + StringPosition + "].SpecStr", SpecularStrength);
		//ProgramToUpdate->SetUniform1f("LightSources[" + StringPosition + "].SpecExp", SpecularExp);
		//ValuesToUpdate.GetValue("LightSources").SetIndex(Position)
		UniformValue NewUniformValue = UniformValue();
		NewUniformValue.SetValue("WorldPos", WorldPosition);
		NewUniformValue.SetValue("Color", LightColor);
		NewUniformValue.SetValue("AmbStr", AmbienceStrength);
		NewUniformValue.SetValue("SpecStr", SpecularStrength);
		NewUniformValue.SetValue("SpecExp", SpecularExp);
		UniformValue FinalValue = UniformValue();
		FinalValue.SetValue("LightSources", 1);
		FinalValue.GetValue("LightSources").SetIndex(Position,NewUniformValue);
		FinalValue.UpdateUniforms(ValuesToUpdate);
	}
	//BEGIN Texture
	Texture::Texture(std::string const& FilePath, uint64_t Options)
	{
		uint64_t GLType = -1;
		int Width = -1;
		int Height = -1;
		uint8_t* FileData = p_LoadFileData(FilePath, &GLType, &Width, &Height);
		p_LoadWithOptions(FileData, Width, Height, GLType, Options);
		stbi_image_free(FileData);
	}
	Texture::Texture(void* Data,int Width,int Height, TextureType DataType, uint64_t Options)
	{
		uint64_t GLType = -1;
		if (DataType == TextureType::RGB8)
		{
			GLType = GL_RGB;
		}
		else if (DataType == TextureType::RED)
		{
			GLType = GL_RED;
		}
		else if (DataType == TextureType::RGBA8)
		{
			GLType = GL_RGBA;
		}
		p_LoadWithOptions(Data, Width, Height, GLType, Options);
	}
	uint64_t Texture::p_TextureTypeToGLType(TextureType TypeToConvert)
	{
		uint64_t GLType = -1;
		if (TypeToConvert == TextureType::RGB8)
		{
			GLType = GL_RGB;
		}
		else if (TypeToConvert == TextureType::RED)
		{
			GLType = GL_RED;
		}
		else if (TypeToConvert == TextureType::RGBA8)
		{
			GLType = GL_RGBA;
		}
		if (GLType == -1)
		{
			throw  std::exception();
		}
		return(GLType);
	}
	uint8_t* Texture::p_LoadFileData(std::string const& FilePath, uint64_t* OutGLType, int* OutWidth, int* OutHeight)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned int ChannelType = GL_RGBA;
		int width, height, nrChannels;
		unsigned char* data = stbi_load(FilePath.c_str(), &width, &height, &nrChannels, 0);
		if (nrChannels < 4)
		{
			ChannelType = GL_RGB;
		}
		if (nrChannels == 1)
		{
			ChannelType = GL_RED;
		}
		*OutGLType = ChannelType;
		*OutWidth = width;
		*OutHeight = height;
		//stbi_image_free(data);
		if (!data)
		{
			std::cout << "Failed to load texture with path: " << FilePath << "\n";
		}
		//stbi_
		//std::string ReturnValue = std::string(data,)
		return((uint8_t*)data);
	}
	void Texture::p_LoadWithOptions(void* Data,int width,int height,uint64_t GL_Type,uint64_t Options)
	{
		stbi_set_flip_vertically_on_load(true);
		uint64_t FilterType = GL_LINEAR;
		if (Options & uint64_t(TextureOptions::NoFilter))
		{
			FilterType = GL_NEAREST;
		}
		//int width, height, nrChannels;
		//unsigned char* data = stbi_load(FilePath.c_str(), &width, &height, &nrChannels, 0);
		m_PixelWidth = width;
		m_PixelHeight = height;
		glGenTextures(1, &TextureHandle);
		glBindTexture(GL_TEXTURE_2D, TextureHandle);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		//TODO ta en titt och se vilka texture optionas jag faktiskt vill ha
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterType);
		//unsigned int ChannelType = GL_RGBA;
		//if (nrChannels < 4)
		//{
		//	ChannelType = GL_RGB;
		//}
		//if (nrChannels == 1)
		//{
		//	ChannelType = GL_RED;
		//}
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		if (Data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_Type, width, height, 0, GL_Type, GL_UNSIGNED_BYTE, Data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			//std::cout << "Failed to load texture " << FilePath << std::endl;
		}
		//stbi_image_free(data);
		glCheckError();
	}
	void Texture::FillTexture(void* Data, int Width, int Height, TextureType PixelFormat, uint64_t Options)
	{
		uint64_t GLDataType = p_TextureTypeToGLType(PixelFormat);
		glTexImage2D(GL_TEXTURE_2D, 0, GLDataType, Width, Height, 0, GLDataType, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glCheckError();
	}
	Texture::Texture(std::string FilePath)
	{
		uint64_t GLType = -1;
		int Width = -1;
		int Height = -1;
		uint8_t* FileData = p_LoadFileData(FilePath, &GLType, &Width, &Height);
		p_LoadWithOptions(FileData,Width,Height, GLType, (uint64_t)TextureOptions::LinearFilter);
		stbi_image_free(FileData);
	}
	void Texture::Bind(int TextureLocation)
	{
		glActiveTexture(GL_TEXTURE0 + TextureLocation);
		glBindTexture(GL_TEXTURE_2D, TextureHandle);
		glCheckError();
	}
	void Texture::UnBind(int TextureLocation)
	{
		glActiveTexture(GL_TEXTURE0 + TextureLocation);
		glBindTexture(GL_TEXTURE_2D, 0);
		glCheckError();
	}
	Texture::~Texture()
	{
		glDeleteTextures(1, &TextureHandle);
		glCheckError();
	}
	//END Texture

	//Node
	void Node::Swap(Node& OtherNode)
	{
		std::swap(ParentNode, OtherNode.ParentNode);
		std::swap(ChildNodes, OtherNode.ChildNodes);
		std::swap(MeshIndexes, OtherNode.MeshIndexes);
		std::swap(LocalTransformation, OtherNode.LocalTransformation);
		//std::swap(AssociatedModel, OtherNode.AssociatedModel);
		std::swap(NodeID, OtherNode.NodeID);
		std::swap(m_NodeIndex, OtherNode.m_NodeIndex);
		std::swap(m_NumberOfChildren, OtherNode.m_NumberOfChildren);
	}
	void Node::p_UpdateChildParents()
	{
		for (size_t i = 0; i < ChildNodes.size(); i++)
		{
			ChildNodes[i].ParentNode = this;
			ChildNodes[i].p_UpdateChildParents();
		}
	}

	Node::Node(void* NodeData, Node* NewParentNode,size_t* TotalNumberOfNodes )
	{
		//AssociatedModel = ModelToBelong;
		size_t PreviousNodeCount = *TotalNumberOfNodes;
		m_NodeIndex = PreviousNodeCount;
		ParentNode = NewParentNode;
		aiNode* AssimpNode = (aiNode*)NodeData;
		NodeID = std::string(AssimpNode->mName.C_Str());
		//AssociatedModel->NodeNames.push_back(NodeID);
		MeshIndexes.reserve(AssimpNode->mNumMeshes);
		for (size_t i = 0; i < AssimpNode->mNumMeshes; i++)
		{
			MeshIndexes.push_back(AssimpNode->mMeshes[i]);
		}
		//tar hand om transformationen
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				LocalTransformation(i, j) = AssimpNode->mTransformation[i][j];
			}
		}
		for (size_t i = 0; i < AssimpNode->mNumChildren; i++)
		{
			*TotalNumberOfNodes += 1;
			Node NewNode = Node(AssimpNode->mChildren[i],this, TotalNumberOfNodes);
			ChildNodes.push_back(std::move(NewNode));
		}
		m_NumberOfChildren = *TotalNumberOfNodes-PreviousNodeCount;
	}
	Node::Node(void* AssimpData)
	{
		size_t TotalNumberOfNodes = 0;
		*this = Node(AssimpData, nullptr, &TotalNumberOfNodes);
	}
	size_t Node::GetNumberOfChildren()
	{
		return(m_NumberOfChildren);
	}
	std::vector<Node> const& Node::GetChildren() const
	{
		return(ChildNodes);
	}
	void Node::DrawAnimation(SkeletonAnimation const& CurrentAnimation,ShaderProgram* ShaderToUpdate,double AnimationTime)
	{
		//if (ParentNode == nullptr)
		//{
		//	//beh�vs den h�r ens?
		//	//ModelAnimation* CurrentAnimation = AssociatedModel->GetCurrentAnimation();
		//	
		//	
		//	MBMath::MBMatrix4<float> NodeTransformation = LocalTransformation;
		//	
		//	
		//	if (CurrentAnimation.IsInAnimation(NodeID) && NodeID != "")
		//	{
		//		NodeTransformation = CurrentAnimation.GetNodeTransformation(NodeID,AnimationTime);
		//	}
		//
		//	//OBS global konfiguerad data som ska tas hand om av modelen
		//	//AssociatedModel->AssociatedEngine->CameraObject.SetModelMatrix(MBMath::MBMatrix4<float>());
		//	//AssociatedModel->AssociatedEngine->CameraObject.Update();
		//
		//	//TODO temp grejer
		//	//ShaderProgram* ShaderToUpdate = AssociatedModel->AssociatedEngine->GetCurrentShader();
		//	for (size_t i = 0; i < ChildNodes.size(); i++)
		//	{
		//		ChildNodes[i].UpdateBones(NodeTransformation, ShaderToUpdate);
		//	}
		//	//nu tar vi och faktiskt uppdaterar v�ran shader
		//	//ShaderProgram* CurrentShader = AssociatedModel->AssociatedEngine->GetCurrentShader();
		//	//for (size_t i = 0; i < AssociatedModel->BoneOffsetList.size(); i++)
		//	//{
		//	//	CurrentShader->SetUniformMat4f("BoneTransforms[" + std::to_string(i) + "]", AssociatedModel->BoneOffsetList[i].GetContinousDataPointer());
		//	//}
		//}
		////tar tar vi och faktiskt ritar v�ra fina meshes
		//for (size_t i = 0; i < MeshIndexes.size(); i++)
		//{
		//	Mesh* MeshToDraw = AssociatedModel->GetMesh(MeshIndexes[i]);
		//	MeshToDraw->Draw();
		//}
		//for (size_t i = 0; i < ChildNodes.size(); i++)
		//{
		//	ChildNodes[i].DrawAnimation(CurrentAnimation,AnimationTime);
		//}
	}
	void Node::UpdateBones(MBMath::MBMatrix4<float> const& ParentTransformation,SkeletonAnimation const& CurrentAnimation,double AnimationTime,ShaderProgram* ShaderToUpdate)
	{
		//ModelAnimation* CurrentAnimation = AssociatedModel->GetCurrentAnimation();
		MBMath::MBMatrix4<float> NodeTransformation = ParentTransformation;
		//nu uppdaterar vi faktiskt benet
		if (CurrentAnimation.IsInAnimation(NodeID) && NodeID != "")
		{
			//NodeTransformation = NodeTransformation * CurrentAnimation->GetNodeTransformation(AssociatedModel->GetAnimationTimeInSec(), NodeID);
			//NodeTransformation = ParentTransformation * LocalTransformation; verifierat att det fungerar med default posen
			NodeTransformation = NodeTransformation * CurrentAnimation.GetNodeTransformation(NodeID, AnimationTime);
		}
		else
		{
			NodeTransformation = NodeTransformation*LocalTransformation;
		}
		//if (AssociatedModel->BoneMap.find(NodeID) != AssociatedModel->BoneMap.end())
		//{
		//	Bone* AssociatedBone = &AssociatedModel->BoneMap[NodeID];
		//	MBMath::MBMatrix4<float> BoneTransform = AssociatedModel->InverseGlobalMatrix * NodeTransformation * AssociatedBone->OffsetMatrix;
		//	ShaderToUpdate->SetUniformMat4f("BoneTransforms[" + std::to_string(AssociatedBone->BoneIndex) + "]", BoneTransform.GetContinousDataPointer());
		//	//AssociatedModel->BoneOffsetList[AssociatedBone->BoneIndex] = BoneTransform;
		//	//std::cout << BoneTransform << std::endl;
		//}
		for (size_t i = 0; i < ChildNodes.size(); i++)
		{
			//ChildNodes[i].UpdateBones(NodeTransformation,ShaderToUpdate);
		}
	}
	void Node::p_FillDefaultTransformations(std::vector<MBMath::MBMatrix4<float>>& VectorToFill, MBMath::MBMatrix4<float> const& CurrentTransform)
	{
		//finns n�got sussy h�r, h�r g�r vi i den h�r ordningen medan i p_FillAnimationTransformations s� g�r vi andra...
		//tror knappast vi kan anta att det alltid permuterar? f�r checka med fler models om det st�mmer
		MBMath::MBMatrix4<float> ModelMatrix = LocalTransformation * CurrentTransform;
		VectorToFill[m_NodeIndex] = ModelMatrix;
		for (size_t i = 0; i <ChildNodes.size(); i++)
		{
			ChildNodes[i].p_FillDefaultTransformations(VectorToFill, ModelMatrix);
		}
	}
	void Node::p_FillAnimationTransformations(std::vector<MBMath::MBMatrix4<float>>& VectorToFill,MBMath::MBMatrix4<float> const& CurrentTransform, SkeletonAnimation const& ParentTransformation, double AnimationTime)
	{
		//ModelAnimation* CurrentAnimation = AssociatedModel->GetCurrentAnimation();
		MBMath::MBMatrix4<float> NodeTransformation = CurrentTransform;
		//nu uppdaterar vi faktiskt benet
		if (ParentTransformation.IsInAnimation(NodeID) && NodeID != "")
		{
			//NodeTransformation = NodeTransformation * CurrentAnimation->GetNodeTransformation(AssociatedModel->GetAnimationTimeInSec(), NodeID);
			//NodeTransformation = ParentTransformation * LocalTransformation; verifierat att det fungerar med default posen
			NodeTransformation = NodeTransformation * ParentTransformation.GetNodeTransformation(NodeID, AnimationTime);
		}
		else
		{
			NodeTransformation = NodeTransformation * LocalTransformation;
		}
		VectorToFill[m_NodeIndex] = NodeTransformation;
		//if (AssociatedModel->BoneMap.find(NodeID) != AssociatedModel->BoneMap.end())
		//{
		//	Bone* AssociatedBone = &AssociatedModel->BoneMap[NodeID];
		//	MBMath::MBMatrix4<float> BoneTransform = AssociatedModel->InverseGlobalMatrix * NodeTransformation * AssociatedBone->OffsetMatrix;
		//	ShaderToUpdate->SetUniformMat4f("BoneTransforms[" + std::to_string(AssociatedBone->BoneIndex) + "]", BoneTransform.GetContinousDataPointer());
		//	//AssociatedModel->BoneOffsetList[AssociatedBone->BoneIndex] = BoneTransform;
		//	//std::cout << BoneTransform << std::endl;
		//}
		for (size_t i = 0; i < ChildNodes.size(); i++)
		{
			ChildNodes[i].p_FillAnimationTransformations(VectorToFill,NodeTransformation,ParentTransformation, AnimationTime);
		}
	}
	std::vector<MBMath::MBMatrix4<float>> Node::GetDefaultTransformations()
	{
		std::vector<MBMath::MBMatrix4<float>> ReturnValue = std::vector<MBMath::MBMatrix4<float>>(m_NumberOfChildren + 1, MBMath::MBMatrix4<float>());
		p_FillDefaultTransformations(ReturnValue, MBMath::MBMatrix4<float>());
		return(ReturnValue);
	}
	std::vector<MBMath::MBMatrix4<float>> Node::GetAnimationTransformations(SkeletonAnimation const& CurrentAnimation, double AnimationTime)
	{
		std::vector<MBMath::MBMatrix4<float>> ReturnValue = std::vector<MBMath::MBMatrix4<float>>(m_NumberOfChildren + 1, MBMath::MBMatrix4<float>());
		p_FillAnimationTransformations(ReturnValue, MBMath::MBMatrix4<float>(), CurrentAnimation, AnimationTime);
		return(ReturnValue);
	}
	std::unordered_map<std::string, size_t> Node::GetNodeIDMap()
	{
		std::unordered_map<std::string, size_t> ReturnValue;
		p_FillNodeIDMap(ReturnValue);
		return(ReturnValue);
	}
	void Node::p_FillNodeIDMap(std::unordered_map<std::string, size_t>& MapToFill)
	{
		MapToFill[NodeID] = m_NodeIndex;
		for (size_t i = 0; i < ChildNodes.size(); i++)
		{
			ChildNodes[i].p_FillNodeIDMap(MapToFill);
		}
	}
	void Node::Draw(MBMath::MBMatrix4<float> const& ParentTransformation)
	{
		//MBMath::MBMatrix4<float> ModelMatrix = LocalTransformation* ParentTransformation;
		////MBMath::MBMatrix4<float> ModelMatrix = MBMath::MBMatrix4<float>();
		//if (MeshIndexes.size() > 0)
		//{
		//	//uppdaterar kameran och modell matrixen
		//
		//	//AssociatedModel->AssociatedEngine->CameraObject.SetModelMatrix(ModelMatrix);
		//	//AssociatedModel->AssociatedEngine->CameraObject.Update();
		//}
		//for (size_t i = 0; i < MeshIndexes.size(); i++)
		//{
		//	Mesh* MeshToDraw = AssociatedModel->GetMesh(MeshIndexes[i]);
		//	MeshToDraw->Draw();
		//}
		//for (size_t i = 0; i < ChildNodes.size(); i++)
		//{
		//	ChildNodes[i].Draw(ModelMatrix);
		//}
	}
	//MBGraphicsEngine
	//bool MBGraphicsEngine::GetKey(unsigned int KeyCode)
	//{
	//	return(glfwGetKey((GLFWwindow*)Window, KeyCode) == GLFW_PRESS);
	//}
	//bool MBGraphicsEngine::GetKeyUp(unsigned int KeyCode)
	//{
	//	return(glfwGetKey((GLFWwindow*)Window, KeyCode) == GLFW_RELEASE);
	//}
	MBGraphicsEngine::MBGraphicsEngine()
	{
		glfwInit();
	}
	KeyCode h_GLFWKeyToMBKey(int GLFWKey)
	{
		KeyCode ReturnValue = KeyCode::_END;
		if (GLFWKey >= 65 && GLFWKey <= 90)
		{
			ReturnValue = KeyCode(GLFWKey - 65);
		}
		//LeftShift, Space, Left, Right, Up, Down, Esc, Enter
		//TODO fixa detta genom att mappa med en statisk array typ, just nu �r jag bara lat
		if (GLFWKey == GLFW_KEY_LEFT_SHIFT)
		{
			ReturnValue = KeyCode::LeftShift;
		}
		else if (GLFWKey == GLFW_KEY_SPACE)
		{
			ReturnValue = KeyCode::Space;
		}
		else if (GLFWKey == GLFW_KEY_LEFT)
		{
			ReturnValue = KeyCode::Left;
		}
		else if (GLFWKey == GLFW_KEY_RIGHT)
		{
			ReturnValue = KeyCode::Right;
		}
		else if (GLFWKey == GLFW_KEY_UP)
		{
			ReturnValue = KeyCode::Up;
		}
		else if (GLFWKey == GLFW_KEY_DOWN)
		{
			ReturnValue = KeyCode::Down;
		}
		else if (GLFWKey == GLFW_KEY_ESCAPE)
		{
			ReturnValue = KeyCode::Esc;
		}
		else if (GLFWKey == GLFW_KEY_ENTER)
		{
			ReturnValue = KeyCode::Enter;
		}
		return(ReturnValue);
	}
	void MBGraphicsEngine::p_ResetInput()
	{
		std::lock_guard<std::mutex> Lock(m_InputMutex);
		for (size_t i = 0; i < (size_t)KeyCode::_END; i++)
		{
			if (m_KeyboardInputs[i] == KeyInputType::Pressed)
			{
				m_KeyboardInputs[i] = KeyInputType::Down;
			}
			if (m_KeyboardInputs[i] == KeyInputType::Released)
			{
				m_KeyboardInputs[i] = KeyInputType::Null;
			}
		}
	}
	void MBGraphicsEngine::GFLW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		MBGraphicsEngine* AssociatedEngine = (MBGraphicsEngine*)glfwGetWindowUserPointer(window);
		std::lock_guard<std::mutex> Lock(AssociatedEngine->m_InputMutex);
		if (action == GLFW_PRESS)
		{
			AssociatedEngine->m_KeyboardInputs[(size_t)h_GLFWKeyToMBKey(key)] = KeyInputType::Pressed;
		}
		else if (action == GLFW_RELEASE)
		{
			AssociatedEngine->m_KeyboardInputs[(size_t)h_GLFWKeyToMBKey(key)] = KeyInputType::Released;
		}
		else if (action == GLFW_REPEAT)
		{
			//g�r inget mer eller mindre
		}
		//std::cout << char(key);
	}
	bool MBGraphicsEngine::GetKeyDown(KeyCode KeyToCheck)
	{
		std::lock_guard<std::mutex> Lock(m_InputMutex);
		return(m_KeyboardInputs[(size_t)KeyToCheck] == KeyInputType::Down || m_KeyboardInputs[(size_t)KeyToCheck] == KeyInputType::Pressed);
	}
	bool MBGraphicsEngine::GetKeyPressed(KeyCode KeyToCheck)
	{
		std::lock_guard<std::mutex> Lock(m_InputMutex);
		return(m_KeyboardInputs[(size_t)KeyToCheck] == KeyInputType::Pressed);
	}
	bool MBGraphicsEngine::GetKeyReleased(KeyCode KeyToCheck)
	{
		std::lock_guard<std::mutex> Lock(m_InputMutex);
		return(m_KeyboardInputs[(size_t)KeyToCheck] == KeyInputType::Released);
	}
	void MBGraphicsEngine::p_LoadDefaultShaders()
	{
		const char* TrivialShaderFrag =
#include <Resources/DefaultShaders/TrivialTextureShader.frag>
			;
		const char* TrivialShaderVert =
#include <Resources/DefaultShaders/TrivialTextureShader.vert>
			;
		LoadedShaders["TrivialTextureShader"] = std::shared_ptr<ShaderProgram>(
			new ShaderProgram(VertexShader(TrivialShaderVert, strlen(TrivialShaderVert)), FragmentShader(TrivialShaderFrag, strlen(TrivialShaderFrag))));

		const char* CameraShaderFrag =
#include <Resources/DefaultShaders/CameraTextureShader.frag>
			;
		const char* CameraShaderVert =
#include <Resources/DefaultShaders/CameraTextureShader.vert>
			;
		LoadedShaders["CameraTextureShader"] = std::shared_ptr<ShaderProgram>(
			new ShaderProgram(VertexShader(CameraShaderVert, strlen(CameraShaderVert)), FragmentShader(CameraShaderFrag, strlen(CameraShaderFrag))));

	}
	void MBGraphicsEngine::WindowCreate(size_t Width, size_t Height, std::string const& MonitorName, bool FullScreen)
	{
		//test f�r att se att vi kan starta ett window
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		if (!FullScreen)
		{
			Window = glfwCreateWindow(Width, Height, MonitorName.c_str(), NULL, NULL);
		}
		else
		{
			Window = glfwCreateWindow(Width, Height, MonitorName.c_str(), glfwGetPrimaryMonitor(), NULL);
		}
		if (Window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		glfwSetWindowUserPointer((GLFWwindow*)Window, this);
		glfwMakeContextCurrent((GLFWwindow*)Window);
		glfwSetKeyCallback((GLFWwindow*)Window, MBGraphicsEngine::GFLW_KeyCallback);
		gl3wInit();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		//nu kan vi g�ra grejer som kr�ver en opengl kontext
		PostProcessingFrameBuffer = std::unique_ptr<FrameBuffer>(new FrameBuffer(Width, Height));
		//LoadShader("Default_PPS", "./Resources/Shaders/PostProcessing/PostProcessingVert.vert", "./Resources/Shaders/PostProcessing/PostProcessingFrag.frag");
		float PBS_Vertices[] =
		{
			-1,-1, 0,0,
			-1, 1, 0,1,
			 1, 1, 1,1,
			 1,-1, 1,0,
		};
		unsigned int PBS_Indexes[] =
		{
			0,1,2,
			0,2,3
		};

		PBS_VertexArray = std::unique_ptr<VertexArrayObject>(new VertexArrayObject());
		PBS_VertexArray->Bind();
		PBS_VertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(VBTypes::StaticDraw, 16 * sizeof(unsigned int), PBS_Vertices));
		PBS_ElementBuffer = std::unique_ptr<ElementBufferObject>(new ElementBufferObject(6, PBS_Indexes));

		PBS_Layout = std::unique_ptr<VertexLayout>(new VertexLayout());
		PBS_Layout->AddAttribute(sizeof(float), 2, MBGE::DataTypes::Float);
		PBS_Layout->AddAttribute(sizeof(float), 2, MBGE::DataTypes::Float);
		PBS_VertexBuffer->Bind();
		PBS_Layout->Bind();
		PBS_VertexArray->UnBind();

		//skapar default post processing shader
		const char* DefaultVertexData =
#include <Resources/Shaders/PostProcessing/PostProcessingDefault.vert>
			;
		const char* DefaultFragmentData =
#include <Resources/Shaders/PostProcessing/PostProcessingDefault.frag>
			;
		VertexShader DefaultVertex(DefaultVertexData, std::strlen(DefaultVertexData));
		FragmentShader DefaultFragment(DefaultFragmentData, std::strlen(DefaultFragmentData));
		std::shared_ptr<ShaderProgram> DefaultShader = std::shared_ptr<ShaderProgram>(new ShaderProgram(DefaultVertex, DefaultFragment));
		DefaultShader->Bind();
		DefaultShader->SetUniform1i("RenderedImage", 0);
		m_PBS_Shaders.push_back(DefaultShader);
		p_LoadDefaultShaders();
	}
	void MBGraphicsEngine::GetWindowSize(int* Width, int* Height)
	{
		glfwGetWindowSize((GLFWwindow*)Window, Width, Height);
	}
	void MBGraphicsEngine::AddPostProcessingShader(std::shared_ptr<ShaderProgram> NewPostProcessingShader)
	{
		if (!_DefaultRemoved)
		{
			m_PBS_Shaders[0] = NewPostProcessingShader;
		}
		else
		{
			m_PBS_Shaders.push_back(NewPostProcessingShader);
		}
	}
	LightSource* MBGraphicsEngine::AddLightSource()
	{
		LightSource* NewLightSource = new LightSource();
		LightSources.push_back(NewLightSource);
		return(NewLightSource);
	}
	void MBGraphicsEngine::UpdateUniforms(ShaderProgram* ProgramToUpdate)
	{
		CameraObject.Update(ProgramToUpdate);
		for (size_t i = 0; i < LightSources.size(); i++)
		{
			LightSources[i]->SetLightning(i,ProgramToUpdate);
		}
	}
	void MBGraphicsEngine::DrawPostProcessing()
	{
		//std::shared_ptr<ShaderProgram> PPS_Shader = GetShader(PostProcessingShaderID);
		//PPS_Shader->Bind();
		//PPS_Shader->SetUniform1i("RenderedImage", 0);
		//PPS_Shader->SetUniform1i("DepthStencilTexture", 1);
		PBS_VertexArray->Bind();
		PBS_VertexBuffer->Bind();
		PBS_ElementBuffer->Bind();
		//borde fixa en �kta pipeline greja f�r Post processing, men nu vill vi bara f� saker att fungera
		for (size_t i = 0; i < m_PBS_Shaders.size(); i++)
		{
			m_PBS_Shaders[i]->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}		
	}
	void MBGraphicsEngine::PollEvents()
	{
		glfwPollEvents();
	}
	void MBGraphicsEngine::Update()
	{
		//nu bindar vi f�r color buffer som den vi ska rita, och ritar med post processing shadern
		p_ResetInput();
		PostProcessingFrameBuffer->BindColorBuffer(0);
		PostProcessingFrameBuffer->BindDepthBuffer(1);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		
		//glClearColor(0, 0, 0, 0);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		DrawPostProcessing();

		glfwSwapBuffers((GLFWwindow*)Window);
		//Kanske inte beh�vs
		//CameraObject.Update();
		//kanske inte ens beh�vs clearas
		DeltaTime = (clock() - DeltaTimeTimer)/(double)CLOCKS_PER_SEC;
		DeltaTimeTimer = clock();
		PostProcessingFrameBuffer->Bind();
		if (FrameByFrame)
		{
			DeltaTime = 0.0166666;
		}
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCheckError();
	}
	std::shared_ptr<ShaderProgram> MBGraphicsEngine::LoadShader(std::string ShaderID, std::string VertexShaderPath, std::string FragmentShaderPath)
	{
		VertexShader NewVertexShader = VertexShader(VertexShaderPath);
		FragmentShader NewFragmentShader = FragmentShader(FragmentShaderPath);
		std::shared_ptr<ShaderProgram> NewShader =std::shared_ptr<ShaderProgram>(new ShaderProgram(NewVertexShader,NewFragmentShader));
		LoadedShaders[ShaderID] = NewShader;
		return(NewShader);
	}
	std::shared_ptr<ShaderProgram> MBGraphicsEngine::LoadShader(std::string ShaderID, std::string VertexShaderPath, std::string GeometryShaderPath,std::string FragmentShaderPath)
	{
		VertexShader NewVertexShader = VertexShader(VertexShaderPath);
		FragmentShader NewFragmentShader = FragmentShader(FragmentShaderPath);
		GeometryShader NewGeometryShader = GeometryShader(GeometryShaderPath);

		std::shared_ptr<ShaderProgram> NewShader = std::shared_ptr<ShaderProgram>(new ShaderProgram(NewVertexShader, NewGeometryShader,NewFragmentShader));
		LoadedShaders[ShaderID] = NewShader;
		return(NewShader);
	}
	std::shared_ptr<Texture> MBGraphicsEngine::GetTexture(std::string const& TextureFilePath)
	{
		if (LoadedTextures.find(TextureFilePath) != LoadedTextures.end())
		{
			return(LoadedTextures[TextureFilePath]);
		}
		else
		{
			std::shared_ptr<Texture> NewTexture = std::shared_ptr<Texture>(new Texture(TextureFilePath));
			LoadedTextures[TextureFilePath] = NewTexture;
			return(NewTexture);
		}
	}
	//void MBGraphicsEngine::SetCurrentShader(std::string ShaderID)
	//{
	//	CurrentShaderID = ShaderID;
	//	LoadedShaders[ShaderID]->Bind();
	//}
	std::shared_ptr<ShaderProgram> MBGraphicsEngine::GetCurrentShader()
	{
		return(m_CurrentShader);
	}
	void MBGraphicsEngine::SetCurrentShader(std::shared_ptr<ShaderProgram> ShaderToUse)
	{
		m_CurrentShader = ShaderToUse;
	}
	std::shared_ptr<ShaderProgram> MBGraphicsEngine::GetShader(std::string ShaderId)
	{
		std::shared_ptr<ShaderProgram> ReturnValue = nullptr;
		if (LoadedShaders.find(ShaderId) != LoadedShaders.end())
		{
			ReturnValue = LoadedShaders[ShaderId];
		}
		return(ReturnValue);
	}
	Model* MBGraphicsEngine::LoadModel(std::string ModelPath,std::vector<MaterialAttribute> MaterialAttributes)
	{
		//h�r g�r vi koden d�r vi skapar den nya moddelen
		Model* NewModel = new Model(ModelPath,MaterialAttributes,this);
		LoadedModels.push_back(NewModel);
		return(NewModel);
	}

}