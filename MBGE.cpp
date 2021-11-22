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
	//Vertexshader
	VertexShader::VertexShader(std::string const& SourcePath)
	{
		std::ifstream t(SourcePath);
		t.seekg(0, std::ios::end);
		size_t size = std::filesystem::file_size(SourcePath);
		std::string buffer(size, ' ');
		t.seekg(0);
		t.read(&buffer[0], size);
		//nu har vi läst in filen
		const char* ShaderSourcePointer = buffer.c_str();
		m_ShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_ShaderHandle, 1, &ShaderSourcePointer, NULL);
		glCompileShader(m_ShaderHandle);
		//checka om det lyckades
		int  success;
		char infoLog[512];
		glGetShaderiv(m_ShaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_ShaderHandle, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
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
		std::ifstream t(SourcePath);
		t.seekg(0, std::ios::end);
		size_t size = t.tellg();
		std::string buffer(size, ' ');
		t.seekg(0);
		t.read(&buffer[0], size);
		//nu har vi läst in filen
		const char* ShaderSourcePointer = buffer.c_str();
		m_ShaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(m_ShaderHandle, 1, &ShaderSourcePointer, NULL);
		glCompileShader(m_ShaderHandle);
		//checka om det lyckades
		int  success;
		char infoLog[512];
		glGetShaderiv(m_ShaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_ShaderHandle, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
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
		std::ifstream t(SourcePath);
		t.seekg(0, std::ios::end);
		size_t size = t.tellg();
		std::string buffer(size, ' ');
		t.seekg(0);
		t.read(&buffer[0], size);
		//nu har vi läst in filen
		const char* ShaderSourcePointer = buffer.c_str();
		m_ShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_ShaderHandle, 1, &ShaderSourcePointer, NULL);
		glCompileShader(m_ShaderHandle);
		//checka om det lyckades
		int  success;
		char infoLog[512];
		glGetShaderiv(m_ShaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_ShaderHandle, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
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
	void ShaderProgram::SetUniform4f(std::string const& UniformName, float x, float y, float z, float w)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		if (UniformLocation == -1)
		{
			//invalid locatiopn printa
			std::cout << "Invalid uniform: " << UniformName<<std::endl;
		}
		glUniform4f(UniformLocation, x, y, z, w);
		glCheckError();
	}
	void ShaderProgram::SetUniform4i(std::string const& UniformName, int x, int y, int z, int w)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		if (UniformLocation == -1)
		{
			//invalid locatiopn printa
			std::cout << "Invalid uniform: " << UniformName << std::endl;
		}
		glUniform4f(UniformLocation, x, y, z, w);
		glCheckError();
	}
	void ShaderProgram::SetUniform1i(std::string const& UniformName, int x)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		if (UniformLocation == -1)
		{
			//invalid locatiopn printa
			std::cout << "Invalid uniform: " << UniformName << std::endl;
		}
		glUniform1i(UniformLocation, x);
		glCheckError();
	}
	void ShaderProgram::SetUniform1f(std::string const& UniformName, float x)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		if (UniformLocation == -1)
		{
			//invalid locatiopn printa
			std::cout << "Invalid uniform: " << UniformName << std::endl;
		}
		glUniform1f(UniformLocation, x);
		glCheckError();
	}
	void ShaderProgram::SetUniformMat4f(std::string const& UniformName, const float* RowMajorData)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		if (UniformLocation == -1)
		{
			//invalid locatiopn printa
			std::cout << "Invalid uniform: " << UniformName << std::endl;
		}
		glUniformMatrix4fv(UniformLocation, 1, GL_TRUE, RowMajorData);
		glCheckError();
	}
	void ShaderProgram::SetUniformVec3(std::string const& UniformName, float x, float y, float z)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		if (UniformLocation == -1)
		{
			//invalid locatiopn printa
			std::cout << "Invalid uniform: " << UniformName << std::endl;
		}
		glUniform3f(UniformLocation, x, y, z);
		glCheckError();
	}
	void ShaderProgram::SetUniformVec4(std::string const& UniformName, float x, float y, float z,float w)
	{
		int UniformLocation = glGetUniformLocation(m_ProgramHandle, UniformName.c_str());
		if (UniformLocation == -1)
		{
			//invalid locatiopn printa
			std::cout << "Invalid uniform: " << UniformName << std::endl;
		}
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
		//vi ska alltid anta att början av meshen är 
		//vi börjar med att definiera layouten som beror på vilka av våra grejer som är definierade
		unsigned int NumberOfVertexColors = AssimpMesh->GetNumColorChannels();
		unsigned int NumberOfTextureCoords = AssimpMesh->GetNumUVChannels();
		//vi har alltid en position
		Layout.AddAttribute(sizeof(float), 3, DataTypes::Float);
		VerticesCount = AssimpMesh->mNumVertices;
		*OutMaterialIndex = AssimpMesh->mMaterialIndex;
		//borde egentligen kolla för om vi saknar attributer vi vill ha
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
		//huruvida vi har ben attributer läggs tills på slutet
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
		//vi vet att storleken av arrayen alltid är 3 * antalet faces då vi gjort om dem till trianglar
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
		//´nu tar vi och faktiskt skapar vertexdatan
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
			//om offseten är mindre än än helt vertex lägger vi till så att den blir det, så vi kan lägga till fler attributes utan att hela systemet bryter samman
			if (Offset%VertexSize != 0)
			{
				Offset += (VertexSize - (Offset % VertexSize));
			}
		}
		//har vi bendata vill vi lägga in det i vektorn med
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
		//TODO lägg till så att vi kan fixa bena
	}
	void Mesh::Rotate(float DegreesToRotate, MBMath::MBVector3<float> const& RotationAxis)
	{
		unsigned int Offset = 0;
		for (size_t i = 0; i < VerticesCount; i++)
		{

			//TODO detta är kinda sussy, borde se över hur jag egentligen tänkt med detta..
			//vektor borde också kunna erättas med en string inser jag också
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
		//TODO omarbeta hela strukture, inser att vi vill nog göra det här på ett väldig annorlunda sätt
		//vi behöver binda rätt textures
		//Material* MaterialToUse = AssociatedModel->GetMaterial(MaterialIndex);
		//MaterialToUse->SetUniforms();
		VAO.Bind();
		Buffer.Bind();
		ArrayObject.Bind();
		float* TestPointer = (float*)&VerticesData[0];
		Buffer.FillBuffer(0, VerticesCount * VertexSize, &VerticesData[0]);
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
	//		//TODO denna kod använad inte längre men har inte faktikst kollar om det här stämmer
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
	//TODO lägg till support för material attributes som inte bara är textures
	Material::Material(void* MaterialData, std::string ModelDirectory, MBGraphicsEngine* AttachedEngine)
	{
		//AssciatedGraphicsEngine = AttachedEngine;
		aiMaterial* AssimpMaterial = (aiMaterial*)MaterialData;
		//vi går igenom allat textures och ser om den har någon
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
		std::cout << "Namnet på materialet är: " << AssimpMaterial->GetName().C_Str() << " och har " << NumberOfTextures << " Textures" << std::endl;

		//vi laddar dem olika texturesen som vi letar efter, har den ej det så laddar vi istället andra specifika värden
		std::vector<std::string> DiffuseTextures = GetTexturePaths(MaterialData, aiTextureType_DIFFUSE);
		if (DiffuseTextures.size() != 0)
		{
			//vi är bara intresserade av att ta den första textyuren, egentligen kan det finbnas fler, men vi tar det sen
			DiffuseTexture = ModelDirectory + DiffuseTextures[0];
		}
		else
		{
			//då är vi kanske intresserade av att ta fram färger etc?
		}
		std::vector<std::string> SpecularTextures = GetTexturePaths(MaterialData, aiTextureType_SPECULAR);
		if (SpecularTextures.size() != 0)
		{
			SpecularTexture = ModelDirectory + SpecularTextures[0];
		}
		else
		{
			//då är vi kanske intresserade av att ta fram färger etc?
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
		//förutsätter att vi redan har en bunden shader med rätt inställningar
		//laddar inte texutresen vi behöver
		//TODO optimesera det här så att det bara händer i konstrukturn och garantera att enginene aldrig deletar data som ett existerande ogbjekt använder
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

	//BEGIN UniformBundle
	std::unique_ptr<UniformValue> GetUniformValue(std::vector<std::unique_ptr<UniformValue>>&& ArrayValues)
	{
		return(std::unique_ptr<UniformValue>(new UniformValue_Array(ArrayValues)));
	}
	std::unique_ptr<UniformValue> GetUniformValue(std::map<std::string, std::unique_ptr<UniformValue>>&& MapValues)
	{
		return(std::unique_ptr<UniformValue>(new UniformValue_Aggregate(MapValues)));
	}
	std::unique_ptr<UniformValue> GetUniformValue(float Value)
	{
		return(std::unique_ptr<UniformValue>(new UniformValue_Float(Value)));
	}
	std::unique_ptr<UniformValue> GetUniformValue(int Value)
	{
		return(std::unique_ptr<UniformValue>(new UniformValue_Int(Value)));
	}
	std::unique_ptr<UniformValue> GetUniformValue(MBMath::MBVector3<float> const& Value)
	{
		return(std::unique_ptr<UniformValue>(new UniformValue_Vec3(Value)));
	}
	std::unique_ptr<UniformValue> GetUniformValue(float x, float y, float z)
	{
		return(std::unique_ptr<UniformValue>(new UniformValue_Vec3(x,y,z)));
	}
	std::unique_ptr<UniformValue> GetUniformValue(MBMath::MBMatrix4<float> const& Value)
	{
		return(std::unique_ptr<UniformValue>(new UniformValue_Mat4(Value)));
	}
	
	
	void swap(UniformBundle& LeftBundle, UniformBundle& RightBundle)
	{
		std::swap(LeftBundle, RightBundle);
	}
	UniformBundle::UniformBundle(UniformBundle&& BundleToSteal) noexcept
	{
		std::swap(*this, BundleToSteal);
	}
	UniformBundle::UniformBundle(UniformBundle const& BundleToCopy)
	{
		for (auto const& Value : BundleToCopy.m_UniformMap)
		{
			m_UniformMap[Value.first] = std::move(Value.second->Copy());
		}
	}
	UniformBundle& UniformBundle::operator=(UniformBundle BundleToSteal)
	{
		std::swap(*this, BundleToSteal);
		return(*this);
	}
	void UniformBundle::SetUniforms(ShaderProgram* ProgramToModify)
	{
		for (auto& Value : m_UniformMap)
		{
			Value.second->SetValue(Value.first, ProgramToModify);
		}
	}
	void UniformBundle::SetUniform_Float(std::string const& UniformName, double FloatValue)
	{
		//throwa exception om inte finns?
		if (m_UniformMap.find(UniformName) != m_UniformMap.end())
		{
			m_UniformMap[UniformName]->SetFloat(FloatValue);
		}
		else
		{
			m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Float(FloatValue)));
		}
	}
	void UniformBundle::SetUniform_Int(std::string const& UniformName, int IntegerValue)
	{
		if (m_UniformMap.find(UniformName) != m_UniformMap.end())
		{
			m_UniformMap[UniformName]->SetInt(IntegerValue);
		}
		else
		{
			m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Int(IntegerValue)));
		}
	}
	void UniformBundle::SetUniform_Vec3(std::string const& UniformName, float x, float y, float z)
	{
		if (m_UniformMap.find(UniformName) != m_UniformMap.end())
		{
			m_UniformMap[UniformName]->SetVec3(x,y,z);
		}
		else
		{
			m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Vec3(x,y,z)));
		}
	}
	void UniformBundle::SetUniform_Vec3(std::string const& UniformName, MBMath::MBStaticVector3<float> const& VectorToSet)
	{
		if (m_UniformMap.find(UniformName) != m_UniformMap.end())
		{
			m_UniformMap[UniformName]->SetVec3(VectorToSet[0],VectorToSet[1],VectorToSet[2]);
		}
		else
		{
			m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Vec3(VectorToSet[0],VectorToSet[1],VectorToSet[2])));
		}
	}
	void UniformBundle::SetUniform_Mat4(std::string const& UniformName, MBMath::MBMatrix4<float> const& MatrixToSet)
	{
		if (m_UniformMap.find(UniformName) != m_UniformMap.end())
		{
			m_UniformMap[UniformName]->SetMat4(MatrixToSet);
		}
		else
		{
			m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Mat4(MatrixToSet)));
		}
	}
	void UniformBundle::AddUniformVector(std::string const& VectorName)
	{
		//borde den kolla hurvida den redan finns?
		m_UniformMap[VectorName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Array()));
	}
	void UniformBundle::AddAggregateType(std::string const& UniformName)
	{
		m_UniformMap[UniformName] = std::move(std::unique_ptr<UniformValue>(new UniformValue_Aggregate()));
	}
	
	UniformValue& UniformBundle::GetUniform(std::string const& UniformName)
	{
		return(*m_UniformMap.at(UniformName));
	}
	void UniformBundle::AddUniform(std::string const& UniformName, std::unique_ptr<UniformValue> ValueToAdd)
	{
		m_UniformMap[UniformName] = std::move(ValueToAdd);
	}


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
		//vi ser vad som händer om vi kör ingen interpolering
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
					//vi har bara en key, rätt konstigt, så vi sätter dem till samma
					LowerKey.Rotation = HigherKey.Rotation;
					HigherKey.KeyTime = (TimeInSec + 1) * TicksPerSec;
					LowerKey.KeyTime = 0;
					//assert(false);
				}
				break;
			}
			if (i == RotationKeys.size() - 1)
			{
				//innebär antingen att vi bara har en key, eller att vi gått över tiden, oavsett vad så går vi till början
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
					//vi har bara en key, rätt konstigt, så vi sätter dem till samma
					LowerKey.Scaling = HigherKey.Scaling;
					HigherKey.KeyTime = (TimeInSec+1)*TicksPerSec;
					LowerKey.KeyTime = 0;
					//assert(false);
				}
				break;
			}
			if (i == ScalingKeys.size() - 1)
			{
				//innebär antingen att vi bara har en key, eller att vi gått över tiden, oavsett vad så går vi till början
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
					//vi har bara en key, rätt konstigt, så vi sätter dem till samma
					LowerKey.Position = HigherKey.Position;
					HigherKey.KeyTime = (TimeInSec + 1) * TicksPerSec;
					LowerKey.KeyTime = 0;
					//assert(false);
				}
				break;
			}
			if (i == TranslationKeys.size() - 1)
			{
				//innebär antingen att vi bara har en key, eller att vi gått över tiden, oavsett vad så går vi till början
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
		//lägger till meshes
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
		//lägger till nodes
		TopNode = Node(LoadedScene->mRootNode);
		m_NumberOfNodes = TopNode.GetNumberOfChildren() + 1;
		//lägger till materials
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
		//börjar med att lägg till våra ben till en mapp
		//vi vi sätter ben 0 att vara "noll benet" som är enhetsmatrisen så alla vertices kan ha lika många ben
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
		//lägger till nodes, populatar ben data som meshesen behöver
		TopNode = Node(LoadedScene->mRootNode);
		m_NumberOfNodes = TopNode.GetNumberOfChildren() + 1;
		InverseGlobalMatrix = TopNode.GetLocalTransformation().GetInverse();
		//lägger till animationer
		for (size_t i = 0; i < LoadedScene->mNumAnimations; i++)
		{
			Animations.push_back(ModelAnimation(LoadedScene->mAnimations[i]));
		}
		//lägger till meshes
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
		//lägger till materials
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
		AssociatedEngine->CameraObject.SetModelMatrix(MBMath::MBMatrix4<float>());
		AssociatedEngine->CameraObject.Update();
		MBMath::MBMatrix4<float> StandardMatrix = MBMath::MBMatrix4<float>();
		ShaderToUse->SetUniformMat4f("BoneTransforms[0]", StandardMatrix.GetContinousData());
		for (auto const& CurrentBone : m_BoneMap)
		{
			size_t TransformationIndex = NodeIDMap[CurrentBone.second.BoneID];
			//MBMath::MBMatrix4<float> BoneTransform = AssociatedModel->InverseGlobalMatrix * NodeTransformation * AssociatedBone->OffsetMatrix;
			MBMath::MBMatrix4<float> NewTransformation = InverseGlobalMatrix * Transformations[TransformationIndex] * CurrentBone.second.OffsetMatrix;
			ShaderToUse->SetUniformMat4f("BoneTransforms[" + std::to_string(CurrentBone.second.BoneIndex) + "]",NewTransformation.GetContinousData());
		}
		//Bonesen är uppdaterade, nu till rita alla meshes
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
			AssociatedEngine->CameraObject.Update();
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PixelWidth, PixelHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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
	Camera::Camera(MBGraphicsEngine* EngineToAttach)
	{
		AssociatedGraphicsEngine = EngineToAttach;
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
	MBMath::MBVector3<float> Camera::GetRotation()
	{
		return(Rotation);
	}
	void Camera::SetRotation(float XAxisRotation, float YAxisRotation, float ZAxisRotation)
	{
		//kanske finns ett bättre mindre ass sätt att räklna ut den på
		//TODO fundera på optimiseringen av SetRotation, borde gå att kunna använda en sluten formel
		/*
		MBMath::MBMatrix<float> BaseMatrix(3);
		BaseMatrix(2, 2) = -1;
		MBMath::MBMatrix<float> XRotationMatrix =BaseMatrix* MBMath::MBVector3<float>::GetRotationMatrix(XAxisRotation, MBMath::MBVector3<float>(1, 0, 0)) * BaseMatrix.Transpose();
		MBMath::MBMatrix<float> YRotationMatrix =BaseMatrix* MBMath::MBVector3<float>::GetRotationMatrix(YAxisRotation, MBMath::MBVector3<float>(0, 1, 0)) * BaseMatrix.Transpose();
		MBMath::MBMatrix<float> ZRotationMatrix =BaseMatrix* MBMath::MBVector3<float>::GetRotationMatrix(ZAxisRotation, MBMath::MBVector3<float>(0, 0, 1)) * BaseMatrix.Transpose();
		MBMath::MBMatrix<float> RotationMatrix = XRotationMatrix * YRotationMatrix * ZRotationMatrix;
		//givet en rotation så roterar vi "baskoordinatena" med den
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
		//std::cout << "Matrix intermediary" << std::endl;
		//std::cout << MatrixIntermediary << std::endl;
		//std::cout << "Inverse:" << std::endl << MBMath::GetInverse<float, 3>(MatrixIntermediary) << std::endl;
		MatrixIntermediary = MBMath::GetInverse<float,3>(MatrixIntermediary).Transpose();
		NormalMatrix = MBMath::MBMatrix4<float>(MatrixIntermediary);
	}
	void Camera::Update()
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
		std::shared_ptr<ShaderProgram> CurrentShader = AssociatedGraphicsEngine->GetCurrentShader();
		CurrentShader->Bind();
		CurrentShader->SetUniformMat4f("Model", ModelMatrix.GetContinousData());
		CurrentShader->SetUniformMat4f("View", ViewMatrix.GetContinousData());
		CurrentShader->SetUniformMat4f("Projection", ProjectionMatrix.GetContinousData());
		CurrentShader->SetUniformMat4f("NormalMatrix", NormalMatrix.GetContinousData());
		CurrentShader->SetUniformVec3("ViewPos", WorldSpaceCoordinates[0], WorldSpaceCoordinates[1], WorldSpaceCoordinates[2]);
	}
	MBMath::MBMatrix4<float> Camera::GetTransformationMatrix()
	{
		//först tar vi fram projektionsmatrisen för när man ska komma till vår position i worldspace
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
		MBMath::MBMatrix4<float> TransformationMatrix = ProjectionMatrix * BaseChangeMatrix * TranslationMatrix*ModelMatrix;
		return(TransformationMatrix);
	}
	//Lightningsource
	LightSource::LightSource(MBGraphicsEngine* AttachedEngine)
	{
		AssociatedGraphicsEngine = AttachedEngine;
	}
	void LightSource::SetLightning(int Position)
	{
		/*
			vec3 Color;
			vec3 WorldPos;
			float AmbStr;
			float SpecStr;
			float SpecExp;
		*/
		std::string StringPosition = std::to_string(Position);
		std::shared_ptr<ShaderProgram> CurrentShader = AssociatedGraphicsEngine->GetCurrentShader();
		CurrentShader->SetUniformVec3("LightSources[" + StringPosition + "].WorldPos", WorldPosition[0], WorldPosition[1], WorldPosition[2]);
		CurrentShader->SetUniformVec3("LightSources[" + StringPosition + "].Color", LightColor[0], LightColor[1], LightColor[2]);
		CurrentShader->SetUniform1f("LightSources[" + StringPosition + "].AmbStr",AmbienceStrength);
		CurrentShader->SetUniform1f("LightSources[" + StringPosition + "].SpecStr",SpecularStrength);
		CurrentShader->SetUniform1f("LightSources[" + StringPosition + "].SpecExp",SpecularExp);
	}
	//Texture
	Texture::Texture(std::string FilePath)
	{
		stbi_set_flip_vertically_on_load(true);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(FilePath.c_str(), &width, &height, &nrChannels, 0);
		glGenTextures(1, &TextureHandle);
		glBindTexture(GL_TEXTURE_2D, TextureHandle);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		//TODO ta en titt och se vilka texture optionas jag faktiskt vill ha
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned int ChannelType = GL_RGBA;
		if (nrChannels < 4)
		{
			ChannelType = GL_RGB;
		}
		if (nrChannels == 1)
		{
			ChannelType = GL_RED;
		}
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, ChannelType, width, height, 0, ChannelType, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture " << FilePath << std::endl;
		}
		stbi_image_free(data);
		glCheckError();
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
		//	//behövs den här ens?
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
		//	//nu tar vi och faktiskt uppdaterar våran shader
		//	//ShaderProgram* CurrentShader = AssociatedModel->AssociatedEngine->GetCurrentShader();
		//	//for (size_t i = 0; i < AssociatedModel->BoneOffsetList.size(); i++)
		//	//{
		//	//	CurrentShader->SetUniformMat4f("BoneTransforms[" + std::to_string(i) + "]", AssociatedModel->BoneOffsetList[i].GetContinousDataPointer());
		//	//}
		//}
		////tar tar vi och faktiskt ritar våra fina meshes
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
		//finns något sussy här, här gör vi i den här ordningen medan i p_FillAnimationTransformations så gör vi andra...
		//tror knappast vi kan anta att det alltid permuterar? får checka med fler models om det stämmer
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
	bool MBGraphicsEngine::GetKey(unsigned int KeyCode)
	{
		return(glfwGetKey(Window, KeyCode) == GLFW_PRESS);
	}
	bool MBGraphicsEngine::GetKeyUp(unsigned int KeyCode)
	{
		return(glfwGetKey(Window, KeyCode) == GLFW_RELEASE);
	}
	MBGraphicsEngine::MBGraphicsEngine()
	{
		//test för att se att vi kan starta ett window
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		Window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
		if (Window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(Window);
		gl3wInit();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//nu kan vi göra grejer som kräver en opengl kontext
		PostProcessingFrameBuffer = new FrameBuffer(800, 600);
		LoadShader("Default_PPS", "./Resources/Shaders/PostProcessing/PostProcessingVert.vert", "./Resources/Shaders/PostProcessing/PostProcessingFrag.frag");
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

		PBS_VertexArray = new VertexArrayObject();
		PBS_VertexArray->Bind();
		PBS_VertexBuffer = new VertexBuffer(VBTypes::StaticDraw, 16 * sizeof(unsigned int), PBS_Vertices);
		PBS_ElementBuffer = new ElementBufferObject(6, PBS_Indexes);

		PBS_Layout = new VertexLayout();
		PBS_Layout->AddAttribute(sizeof(float), 2, MBGE::DataTypes::Float);
		PBS_Layout->AddAttribute(sizeof(float), 2, MBGE::DataTypes::Float);
		PBS_VertexBuffer->Bind();
		PBS_Layout->Bind();
		PBS_VertexArray->UnBind();
	}
	LightSource* MBGraphicsEngine::AddLightSource()
	{
		LightSource* NewLightSource = new LightSource(this);
		LightSources.push_back(NewLightSource);
		return(NewLightSource);
	}
	void MBGraphicsEngine::UpdateUniforms()
	{
		CameraObject.Update();
		for (size_t i = 0; i < LightSources.size(); i++)
		{
			LightSources[i]->SetLightning(i);
		}
	}
	void MBGraphicsEngine::DrawPostProcessing()
	{
		std::shared_ptr<ShaderProgram> PPS_Shader = GetShader(PostProcessingShaderID);
		PPS_Shader->Bind();
		PPS_Shader->SetUniform1i("RenderedImage", 0);
		PPS_Shader->SetUniform1i("DepthStencilTexture", 1);
		PBS_VertexArray->Bind();
		PBS_VertexBuffer->Bind();
		PBS_ElementBuffer->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
	}
	void MBGraphicsEngine::PollEvents()
	{
		glfwPollEvents();
	}
	void MBGraphicsEngine::Update()
	{
		//nu bindar vi får color buffer som den vi ska rita, och ritar med post processing shadern
		PostProcessingFrameBuffer->BindColorBuffer(0);
		PostProcessingFrameBuffer->BindDepthBuffer(1);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		DrawPostProcessing();

		glfwSwapBuffers(Window);
		//Kanske inte behövs
		//CameraObject.Update();
		//kanske inte ens behövs clearas
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
		//här gör vi koden där vi skapar den nya moddelen
		Model* NewModel = new Model(ModelPath,MaterialAttributes,this);
		LoadedModels.push_back(NewModel);
		return(NewModel);
	}

}