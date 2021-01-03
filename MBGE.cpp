#include <vector>
#include <fstream>

#include <MBGE.h>

#include <gl3w/include/GL/gl3w.h>
#include <glfw/include/GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>
#include <assert.h>
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
	VertexShader::VertexShader(std::string SourcePath)
	{
		std::ifstream t(SourcePath);
		t.seekg(0, std::ios::end);
		size_t size = t.tellg();
		std::string buffer(size, ' ');
		t.seekg(0);
		t.read(&buffer[0], size);
		//nu har vi läst in filen
		const char* ShaderSourcePointer = buffer.c_str();
		ShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(ShaderHandle, 1, &ShaderSourcePointer, NULL);
		glCompileShader(ShaderHandle);
		//checka om det lyckades
		int  success;
		char infoLog[512];
		glGetShaderiv(ShaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ShaderHandle, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
	}
	VertexShader::~VertexShader()
	{
		glDeleteShader(ShaderHandle);
		glCheckError();
	}
	//geometry shader
	GeometryShader::GeometryShader(std::string SourcePath)
	{
		std::ifstream t(SourcePath);
		t.seekg(0, std::ios::end);
		size_t size = t.tellg();
		std::string buffer(size, ' ');
		t.seekg(0);
		t.read(&buffer[0], size);
		//nu har vi läst in filen
		const char* ShaderSourcePointer = buffer.c_str();
		ShaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(ShaderHandle, 1, &ShaderSourcePointer, NULL);
		glCompileShader(ShaderHandle);
		//checka om det lyckades
		int  success;
		char infoLog[512];
		glGetShaderiv(ShaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ShaderHandle, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
	}
	GeometryShader::~GeometryShader()
	{
		glDeleteShader(ShaderHandle);
	}
	//fragmentShader
	FragmentShader::FragmentShader(std::string SourcePath)
	{
		std::ifstream t(SourcePath);
		t.seekg(0, std::ios::end);
		size_t size = t.tellg();
		std::string buffer(size, ' ');
		t.seekg(0);
		t.read(&buffer[0], size);
		//nu har vi läst in filen
		const char* ShaderSourcePointer = buffer.c_str();
		ShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(ShaderHandle, 1, &ShaderSourcePointer, NULL);
		glCompileShader(ShaderHandle);
		//checka om det lyckades
		int  success;
		char infoLog[512];
		glGetShaderiv(ShaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ShaderHandle, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
	}
	FragmentShader::~FragmentShader()
	{
		glDeleteShader(ShaderHandle);
		glCheckError();
	}
	//ShaderProgram
	ShaderProgram::ShaderProgram(VertexShader VerShader, FragmentShader FragShader)
	{
		glCheckError();
		ProgramHandle = glCreateProgram();
		glAttachShader(ProgramHandle, VerShader.ShaderHandle);
		glAttachShader(ProgramHandle, FragShader.ShaderHandle);
		glLinkProgram(ProgramHandle);
		int  success;
		char infoLog[512];
		glGetProgramiv(ProgramHandle, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ProgramHandle, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
	}
	ShaderProgram::ShaderProgram(VertexShader VerShader, GeometryShader GeomShader, FragmentShader FragShader)
	{
		glCheckError();
		ProgramHandle = glCreateProgram();
		glAttachShader(ProgramHandle, VerShader.ShaderHandle);
		glAttachShader(ProgramHandle, FragShader.ShaderHandle);
		glAttachShader(ProgramHandle, GeomShader.ShaderHandle);
		glLinkProgram(ProgramHandle);
		int  success;
		char infoLog[512];
		glGetProgramiv(ProgramHandle, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ProgramHandle, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}
		glCheckError();
	}
	void ShaderProgram::Bind()
	{
		glUseProgram(ProgramHandle);
		glCheckError();
	}
	void ShaderProgram::SetUniform4f(std::string UniformName, float x, float y, float z, float w)
	{
		int UniformLocation = glGetUniformLocation(ProgramHandle, UniformName.c_str());
		glUniform4f(UniformLocation, x, y, z, w);
		glCheckError();
	}
	void ShaderProgram::SetUniform4i(std::string UniformName, int x, int y, int z, int w)
	{
		int UniformLocation = glGetUniformLocation(ProgramHandle, UniformName.c_str());
		glUniform4f(UniformLocation, x, y, z, w);
		glCheckError();
	}
	void ShaderProgram::SetUniform1i(std::string UniformName, int x)
	{
		int UniformLocation = glGetUniformLocation(ProgramHandle, UniformName.c_str());
		glUniform1i(UniformLocation, x);
		glCheckError();
	}
	void ShaderProgram::SetUniform1f(std::string UniformName, float x)
	{
		int UniformLocation = glGetUniformLocation(ProgramHandle, UniformName.c_str());
		glUniform1f(UniformLocation, x);
		glCheckError();
	}
	void ShaderProgram::SetUniformMat4f(std::string UniformName, float* RowMajorData)
	{
		int UniformLocation = glGetUniformLocation(ProgramHandle, UniformName.c_str());
		glUniformMatrix4fv(UniformLocation, 1, GL_TRUE, RowMajorData);
		glCheckError();
	}
	void ShaderProgram::SetUniformVec3(std::string UniformName, float x, float y, float z)
	{
		int UniformLocation = glGetUniformLocation(ProgramHandle, UniformName.c_str());
		glUniform3f(UniformLocation, x, y, z);
		glCheckError();
	}
	void ShaderProgram::SetUniformVec4(std::string UniformName, float x, float y, float z,float w)
	{
		int UniformLocation = glGetUniformLocation(ProgramHandle, UniformName.c_str());
		glUniform4f(UniformLocation, x, y, z,w);
		glCheckError();
	}
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(ProgramHandle);
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
			glVertexAttribPointer(i, CurrentAttrib.NumberOfElements, GL_FLOAT, GL_FALSE, TotalSize, (void*)CurrentOffset);
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
		glBindBuffer(GL_ARRAY_BUFFER, BufferHandle);
		glCheckError();
	}
	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glCheckError();
	}
	VertexBuffer::VertexBuffer(VBTypes BufferType, unsigned long long InitialSize, void* InitialData)
	{
		BufferSize = InitialSize;
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
		CurrentDrawType = DrawType;
		glGenBuffers(1, &BufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, BufferHandle);
		glBufferData(GL_ARRAY_BUFFER, InitialSize, InitialData, DrawType);
		glCheckError();
	}
	VertexBuffer::VertexBuffer()
	{
		CurrentDrawType = GL_DYNAMIC_DRAW;
		glGenBuffers(1, &BufferHandle);
		glCheckError();
	}
	unsigned int VertexBuffer::SizeOfBuffer()
	{
		return(BufferSize);
	}
	void VertexBuffer::ResizeBuffer(unsigned int NewSize,void* Data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, BufferHandle);
		glBufferData(GL_ARRAY_BUFFER, NewSize, Data, CurrentDrawType);
		glCheckError();
	}
	void VertexBuffer::FillBuffer(unsigned int Offset, unsigned int NumberOfBytes, void* Data)
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
		glDeleteBuffers(1, &BufferHandle);
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
	ElementBufferObject::ElementBufferObject(unsigned int NumberOfElements,unsigned int* Data)
	{
		glGenBuffers(1, &ObjectHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ObjectHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumberOfElements * sizeof(int), Data, GL_STATIC_DRAW);
		glCheckError();
	}
	void ElementBufferObject::ResizeBuffer(unsigned int NumberOfElements, void* Data)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ObjectHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumberOfElements *sizeof(int), Data, GL_STATIC_DRAW);
		glCheckError();
	}
	void ElementBufferObject::FillBuffer(unsigned int Offset, unsigned int NumberOfBytes, void* Data)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, Offset, NumberOfBytes, Data);
		glCheckError();
	}
	ElementBufferObject::ElementBufferObject()
	{
		glGenBuffers(1, &ObjectHandle);
		glCheckError();
	}
	ElementBufferObject::~ElementBufferObject()
	{
		glDeleteBuffers(1, &ObjectHandle);
		glCheckError();
	}
	void ElementBufferObject::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ObjectHandle);
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
		unsigned char* TempPointer = this->VertexData;
		this->VertexData = OtherVertex.VertexData;
		OtherVertex.VertexData = TempPointer;
	}
	Vertex::Vertex(Vertex& OtherVertex)
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
		DataSize = OtherVertex.DataSize;
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
	Mesh::Mesh(void* MeshDataObject,std::vector<VertexAttributes> AttributeOrder,Model* ModelToBelong)
		: Buffer(), ArrayObject(),VAO(),Layout()
	{
		const int TextureDimensions = 2;
		AssociatedModel = ModelToBelong;
		aiMesh* AssimpMesh = (aiMesh*)MeshDataObject;
		//vi ska alltid anta att början av meshen är 
		//vi börjar med att definiera layouten som beror på vilka av våra grejer som är definierade
		unsigned int NumberOfVertexColors = AssimpMesh->GetNumColorChannels();
		unsigned int NumberOfTextureCoords = AssimpMesh->GetNumUVChannels();
		//vi har alltid en position
		Layout.AddAttribute(sizeof(float), 3, DataTypes::Float);
		VerticesCount = AssimpMesh->mNumVertices;
		MaterialIndex = AssimpMesh->mMaterialIndex;
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
				assert(AssimpMesh->mFaces[i].mNumIndices == 3);
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
		}
		//nu är all data innladdad, vi tar och sätter vårt buffer object likaså
		Buffer.Bind();
		Buffer.ResizeBuffer(TotalData, &VerticesData[0]);
		Layout.Bind();
		VAO.UnBind();
		//TODO lägg till så att vi kan fixa bena
	}
	void Mesh::Rotate(float DegreesToRotate, MBMath::MBVector3<float> RotationAxis)
	{
		unsigned int Offset = 0;
		for (size_t i = 0; i < VerticesCount; i++)
		{
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
		Material* MaterialToUse = AssociatedModel->GetMaterial(MaterialIndex);
		MaterialToUse->SetUniforms();
		VAO.Bind();
		Buffer.Bind();
		ArrayObject.Bind();
		float* TestPointer = (float*)&VerticesData[0];
		Buffer.FillBuffer(0, VerticesCount * VertexSize, &VerticesData[0]);
		glDrawElements(GL_TRIANGLES, DrawVerticesCount, GL_UNSIGNED_INT, 0);
		VAO.UnBind();
		glCheckError();
	}
	void Mesh::SavePositions()
	{
		unsigned int Offset = 0;
		SavedPositionData = std::vector<Vec3Data>(VerticesCount);
		for (size_t i = 0; i < VerticesCount; i++)
		{
			Vec3Data NewPosition;
			float* DataPointer = (float*)&VerticesData[Offset];
			NewPosition.x = DataPointer[0];
			NewPosition.y = DataPointer[1];
			NewPosition.z = DataPointer[2];
			SavedPositionData[i] = NewPosition;
			Offset += VertexSize;
		}
	}
	void Mesh::RestorePositions()
	{
		unsigned int Offset = 0;
		for (size_t i = 0; i < VerticesCount; i++)
		{
			float* DataPointer = (float*)&VerticesData[Offset];
			DataPointer[0] = SavedPositionData[i].x;
			DataPointer[1] = SavedPositionData[i].y;
			DataPointer[2] = SavedPositionData[i].z;
			Offset += VertexSize;
		}
	}
	void Mesh::TransformPositions(MBMath::MBMatrix4<float> Transformation)
	{
		unsigned int Offset = 0;
		for (size_t i = 0; i < VerticesCount; i++)
		{
			float* DataPointer = (float*)&VerticesData[Offset];
			MBMath::MBVector3<float> VectorIntermediary = MBMath::MBVector3<float>(DataPointer[0], DataPointer[1], DataPointer[2]);
			VectorIntermediary = Transformation * VectorIntermediary;
			DataPointer[0] = VectorIntermediary[0];
			DataPointer[1] = VectorIntermediary[1];
			DataPointer[2] = VectorIntermediary[2];
			Offset += VertexSize;
		}
	}
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
			if (NewString.length!= 0)
			{
				ReturnValue.push_back(std::string(NewString.C_Str()));
			}
		}
		return(ReturnValue);
	}
	Material::Material(void* MaterialData, std::string ModelDirectory,MBGraphicsEngine* AttachedEngine)
	{
		AssciatedGraphicsEngine = AttachedEngine;
		aiMaterial* AssimpMaterial = (aiMaterial*)MaterialData;
		//vi går igenom allat textures och ser om den har någon
		int NumberOfTextures = 0;
		std::vector<aiTextureType> TextureTypes = { aiTextureType::aiTextureType_AMBIENT,aiTextureType::aiTextureType_AMBIENT_OCCLUSION,aiTextureType::aiTextureType_BASE_COLOR,
		aiTextureType::aiTextureType_DIFFUSE,aiTextureType::aiTextureType_DIFFUSE_ROUGHNESS,aiTextureType::aiTextureType_DISPLACEMENT,aiTextureType::aiTextureType_EMISSION_COLOR,
		aiTextureType::aiTextureType_EMISSIVE,aiTextureType::aiTextureType_HEIGHT,aiTextureType::aiTextureType_LIGHTMAP,aiTextureType::aiTextureType_METALNESS,aiTextureType::aiTextureType_NONE,
		aiTextureType::aiTextureType_NORMALS,aiTextureType::aiTextureType_NORMAL_CAMERA,aiTextureType::aiTextureType_OPACITY,aiTextureType::aiTextureType_REFLECTION,aiTextureType::aiTextureType_SHININESS,
		aiTextureType::aiTextureType_SPECULAR,aiTextureType::aiTextureType_UNKNOWN,aiTextureType::_aiTextureType_Force32Bit
		};
		for (size_t i = 0; i < TextureTypes.size(); i++)
		{
			NumberOfTextures += AssimpMaterial->GetTextureCount(TextureTypes[i]);
		}
		std::cout << "Namnet på materialet är: " << AssimpMaterial->GetName().C_Str() <<" och har "<<NumberOfTextures<<" Textures" << std::endl;
		
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
	Material::Material(void* MaterialData,std::string PathToModel, std::vector<MaterialAttribute> NewMaterialAttributes, MBGraphicsEngine* AttachedEngine)
	{
		NewMaterialAttributes = NewMaterialAttributes;
		AssciatedGraphicsEngine = AttachedEngine;
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
	}
	void Material::SetUniforms()
	{
		//förutsätter att vi redan har en bunden shader med rätt inställningar
		//laddar inte texutresen vi behöver
		//TODO optimesera det här så att det bara händer i konstrukturn och garantera att enginene aldrig deletar data som ett existerande ogbjekt använder
		ShaderProgram* CurrentShader = AssciatedGraphicsEngine->GetCurrentShader();
		CurrentShader->Bind();
		if (VectorContains<MaterialAttribute>(MaterialShaderAttributes,MaterialAttribute::DiffuseTexture))
		{
			Texture* DiffuseToUse = AssciatedGraphicsEngine->GetTexture(DiffuseTexture);
			DiffuseToUse->Bind(0);
			CurrentShader->SetUniform1i("FragUni.UseDiffTex", true);
			CurrentShader->SetUniform1i("DiffuseTex", 0);
		}
		else
		{
			CurrentShader->SetUniform1i("FragUni.UseDiffTex", false);
		}
		if (VectorContains<MaterialAttribute>(MaterialShaderAttributes, MaterialAttribute::SpecularTexture))
		{
			Texture* SpecToUse = AssciatedGraphicsEngine->GetTexture(SpecularTexture);
			SpecToUse->Bind(1);
			CurrentShader->SetUniform1i("FragUni.UseSpecTex", true);
			CurrentShader->SetUniform1i("SpecTex", 1);
		}
		else
		{
			CurrentShader->SetUniform1i("FragUni.UseSpecTex", false);
		}
		if (VectorContains<MaterialAttribute>(MaterialShaderAttributes, MaterialAttribute::NormalTexture))
		{
			Texture* NormalToUse = AssciatedGraphicsEngine->GetTexture(NormalTexture);
			NormalToUse->Bind(2);
			CurrentShader->SetUniform1i("FragUni.UseNormTex", true);
			CurrentShader->SetUniform1i("NormalTex", 2);
		}
		else
		{
			CurrentShader->SetUniform1i("FragUni.UseNormTex", false);
		}
	}
	//Model
	void Model::Rotate(float DegreesToRotate, MBMath::MBVector3<float> RotationAxis)
	{
		//roterar helt enkelt varje mesh motsvarande grader
		for (size_t i = 0; i < ModelMeshes.size(); i++)
		{
			(*ModelMeshes[i]).Rotate(DegreesToRotate, RotationAxis);
		}
	}
	void Model::ProcessNode(void* Node, void* Scene)
	{
		aiNode* AssimpNode = (aiNode*)Node;
		aiScene* AssimpScene = (aiScene*)Scene;
		//rekursivt lägger till 
	}
	Model::Model(std::string ModelPath,MBGraphicsEngine* AttachedEngine)
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
			ModelMeshes.push_back(new Mesh(LoadedScene->mMeshes[i], StandardAttributeOrder,this));
			NumberOfTriangels += ModelMeshes.back()->NumberOfVertices();
		}
		std::cout << "Number of Triangels "<<NumberOfTriangels/3<<std::endl;
		//lägger till nodes
		TopNode = Node(LoadedScene->mRootNode, nullptr,this);
		//lägger till materials
		unsigned int NumberOfMaterials = LoadedScene->mNumMaterials;
		for (size_t i = 0; i < NumberOfMaterials; i++)
		{
			ModelMaterials.push_back(Material(LoadedScene->mMaterials[i], ModelPath.substr(0, ModelPath.find_last_of("/")+1),AssociatedEngine));
		}
	}
	Model::Model(std::string ModelPath, std::vector<MaterialAttribute> MaterialAttributes, MBGraphicsEngine* AttachedEngine)
	{
		//hur vi faktiskt tar och laddar in moddelen
		AssociatedEngine = AttachedEngine;
		std::vector<VertexAttributes> StandardAttributeOrder = { VertexAttributes::TextureCoordinates,VertexAttributes::Tangent,VertexAttributes::Bitangent,VertexAttributes::VertexNormal };
		Assimp::Importer Importer;
		const aiScene* LoadedScene = Importer.ReadFile(ModelPath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
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
			ModelMeshes.push_back(new Mesh(LoadedScene->mMeshes[i], StandardAttributeOrder, this));
			NumberOfTriangels += ModelMeshes.back()->NumberOfVertices();
		}
		std::cout << "Number of Triangels " << NumberOfTriangels / 3 << std::endl;
		//lägger till nodes
		TopNode = Node(LoadedScene->mRootNode, nullptr, this);
		//lägger till materials
		unsigned int NumberOfMaterials = LoadedScene->mNumMaterials;
		for (size_t i = 0; i < NumberOfMaterials; i++)
		{
			ModelMaterials.push_back(Material(LoadedScene->mMaterials[i], ModelPath.substr(0, ModelPath.find_last_of("/") + 1),MaterialAttributes, AssociatedEngine));
			ModelMaterials.back().MaterialShaderAttributes = MaterialAttributes;
		}
	}

	Mesh* Model::GetMesh(unsigned int MeshIndex)
	{
		return(ModelMeshes[MeshIndex]);
	}
	Material* Model::GetMaterial(unsigned int MaterialIndex)
	{
		return(&ModelMaterials[MaterialIndex]);
	}
	void Model::Draw()
	{
		//vi callar helt enkelt nodens draw funktion, som ritar grejen rekursivt
		//TODO optimera så vi inte behöver getta shadern varje frame
		AssociatedEngine->SetCurrentShader(ModelShader);
		TopNode.Draw(MBMath::MBMatrix4<float>());
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
		std::cout << "New projection matirx:"<<std::endl << NewProjectionMatrix << std::endl;
		NewProjectionMatrix.PrintWolframMatrix();
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
	void Camera::SetRotation(MBMath::MBVector3<float> NewRotation)
	{
		SetRotation(NewRotation[0], NewRotation[1], NewRotation[2]);
	}
	void Camera::SetModelMatrix(MBMath::MBMatrix4<float> NewModelMatrix)
	{
		ModelMatrix = NewModelMatrix;
		MBMath::MBMatrix<float> MatrixIntermediary = MBMath::MBMatrix<float>(3);
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				MatrixIntermediary(i, j) = ModelMatrix(i, j);
			}
		}
		MatrixIntermediary = MatrixIntermediary.InvertedMatrix().Transpose();
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
		ShaderProgram* CurrentShader = AssociatedGraphicsEngine->GetCurrentShader();
		CurrentShader->Bind();
		CurrentShader->SetUniformMat4f("Model", ModelMatrix.GetContinousDataPointer());
		CurrentShader->SetUniformMat4f("View", ViewMatrix.GetContinousDataPointer());
		CurrentShader->SetUniformMat4f("Projection", ProjectionMatrix.GetContinousDataPointer());
		CurrentShader->SetUniformMat4f("NormalMatrix", NormalMatrix.GetContinousDataPointer());
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
		ShaderProgram* CurrentShader = AssociatedGraphicsEngine->GetCurrentShader();
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
		std::swap(AssociatedModel, OtherNode.AssociatedModel);
	}
	Node::Node(void* NodeData, Node* NewParentNode, Model* ModelToBelong)
	{
		AssociatedModel = ModelToBelong;
		ParentNode = NewParentNode;
		aiNode* AssimpNode = (aiNode*)NodeData;
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
			Node* NewNode = new Node(AssimpNode->mChildren[i], this, ModelToBelong);
			ChildNodes.push_back(NewNode);
		}
	}
	void Node::Draw(MBMath::MBMatrix4<float> ParentTransformation)
	{
		MBMath::MBMatrix4<float> ModelMatrix = LocalTransformation * ParentTransformation;

		//MBMath::MBMatrix4<float> ModelMatrix = MBMath::MBMatrix4<float>();
		if (MeshIndexes.size() > 0)
		{
			//uppdaterar kameran och modell matrixen
			AssociatedModel->AssociatedEngine->CameraObject.SetModelMatrix(ModelMatrix);
			AssociatedModel->AssociatedEngine->CameraObject.Update();
		}
		for (size_t i = 0; i < MeshIndexes.size(); i++)
		{
			Mesh* MeshToDraw = AssociatedModel->GetMesh(MeshIndexes[i]);
			MeshToDraw->Draw();
		}
		for (size_t i = 0; i < ChildNodes.size(); i++)
		{
			ChildNodes[i]->Draw(ModelMatrix);
		}
	}
	Node::~Node()
	{
		for (size_t i = 0; i < ChildNodes.size(); i++)
		{
			delete ChildNodes[i];
		}
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
	void MBGraphicsEngine::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(Window);
		//Kanske inte behövs
		//CameraObject.Update();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();
	}
	ShaderProgram* MBGraphicsEngine::LoadShader(std::string ShaderID, std::string VertexShaderPath, std::string FragmentShaderPath)
	{
		VertexShader NewVertexShader = VertexShader(VertexShaderPath);
		FragmentShader NewFragmentShader = FragmentShader(FragmentShaderPath);
		ShaderProgram* NewShader = new ShaderProgram(NewVertexShader,NewFragmentShader);
		LoadedShaders[ShaderID] = NewShader;
		return(NewShader);
	}
	ShaderProgram* MBGraphicsEngine::LoadShader(std::string ShaderID, std::string VertexShaderPath, std::string GeometryShaderPath,std::string FragmentShaderPath)
	{
		VertexShader NewVertexShader = VertexShader(VertexShaderPath);
		FragmentShader NewFragmentShader = FragmentShader(FragmentShaderPath);
		GeometryShader NewGeometryShader = GeometryShader(GeometryShaderPath);
		ShaderProgram* NewShader = new ShaderProgram(NewVertexShader, NewGeometryShader,NewFragmentShader);
		LoadedShaders[ShaderID] = NewShader;
		return(NewShader);
	}
	Texture* MBGraphicsEngine::GetTexture(std::string TextureFilePath)
	{
		if (LoadedTextures.find(TextureFilePath) != LoadedTextures.end())
		{
			return(LoadedTextures[TextureFilePath]);
		}
		else
		{
			Texture* NewTexture = new Texture(TextureFilePath);
			LoadedTextures[TextureFilePath] = NewTexture;
			return(NewTexture);
		}
	}
	void MBGraphicsEngine::SetCurrentShader(std::string ShaderID)
	{
		CurrentShaderID = ShaderID;
		LoadedShaders[ShaderID]->Bind();
	}
	ShaderProgram* MBGraphicsEngine::GetCurrentShader()
	{
		return(LoadedShaders[CurrentShaderID]);
	}
	ShaderProgram* MBGraphicsEngine::GetShader(std::string ShaderId)
	{
		ShaderProgram* ReturnValue = nullptr;
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