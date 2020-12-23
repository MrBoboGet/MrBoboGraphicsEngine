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
	void ElementBufferObject::DrawTriangles()
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glCheckError();
	}
	//Mesh

	//Model

	//Camera
	Camera::Camera()
	{

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
	}
	void MBGraphicsEngine::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(Window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glCheckError();
	}

}