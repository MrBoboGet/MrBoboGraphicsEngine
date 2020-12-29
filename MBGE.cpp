#include <vector>
#include <MBGE.h>
#include <gl3w/include/GL/gl3w.h>
#include <glfw/include/GLFW/glfw3.h>
#include <fstream>
#include <stb/stb_image.h>
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
		glUniform1i(UniformLocation, x);
		glCheckError();
	}
	void ShaderProgram::SetUniformMat4f(std::string UniformName, float* RowMajorData)
	{
		int UniformLocation = glGetUniformLocation(ProgramHandle, UniformName.c_str());
		glUniformMatrix4fv(UniformLocation, 1, GL_TRUE, RowMajorData);
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
		VAO.Bind();
		Buffer.Bind();
		Buffer.FillBuffer(0, VerticesCount * VertexSize, &VerticesData[0]);
		glDrawElements(GL_TRIANGLES, DrawVerticesCount, GL_UNSIGNED_INT, 0);
		VAO.UnBind();
		glCheckError();
	}
	unsigned int Mesh::NumberOfVertices()
	{
		return(VerticesCount);
	}
	Mesh::~Mesh()
	{

	}
	//Model
	void Model::Rotate(float DegreesToRotate, MBMath::MBVector3<float> RotationAxis)
	{
		//roterar helt enkelt varje mesh motsvarande grader
		for (size_t i = 0; i < ModelMeshes.size(); i++)
		{
			ModelMeshes[i].Rotate(DegreesToRotate, RotationAxis);
		}
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

	}
	void Camera::SetRotation(MBMath::MBVector3<float> NewRotation)
	{
		SetRotation(NewRotation[0], NewRotation[1], NewRotation[2]);
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
		MBMath::MBMatrix4<float> TransformationMatrix = ProjectionMatrix * BaseChangeMatrix * TranslationMatrix;
		return(TransformationMatrix);
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
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture " << FilePath << std::endl;
		}
		stbi_image_free(data);
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
	void MBGraphicsEngine::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(Window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glCheckError();
	}

}