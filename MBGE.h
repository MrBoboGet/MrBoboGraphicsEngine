#pragma once
#include <MBUtility/MBMatrix.h>
#include <string>
#include <unordered_map>
#include <time.h>
#include <thread>
#include <cstdint>
#include <map>
#include <mutex>
#include <cinttypes>
#include <stdint.h>
#define MBGE_BASE_TYPE double 
#define MBGE_BONEPERVERTEX 5
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
		Int,
		Matrix4,
		Array,
		Vec3,
		Vec4,
		Struct,
		Null,
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
		VertexShader(const void* Data,size_t DataSize);
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
		GeometryShader(const void* Data, size_t DataSize);
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
		FragmentShader(const void* Data, size_t DataSize);
		~FragmentShader();
	};
	class ShaderProgram
	{
	private:
		unsigned int m_ProgramHandle = 0;
		void p_HandleInvalidPosition(int PositionToCheck, std::string const& UniformName);
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
		void SetUniformMat4f(std::string const& UniformName, const float* RowMajorData);
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
		//semantiken av att kopiera implicit eller s� makar inte f�r mycket sense
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
		Position,
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
	//TODO Typedefa pointersen till grejerna, void pointers �r lite l�skiga
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
		size_t BoneIndex = -1;
		MBMath::MBMatrix4<float> OffsetMatrix = MBMath::MBMatrix4<float>();
		std::vector<BoneWeight> Weights = {};
		Bone(void* AssimpBoneData);
		Bone() { };
	};
	class Mesh
	{
		//alla koordinater �r givna i lokala koordinater med ett implicit centrum av (0,0,0)
	private:
		std::vector<unsigned char> VerticesData = {};
		unsigned int VertexSize = 0;
		unsigned int VerticesCount = 0;
		unsigned int DrawVerticesCount = 0;

		//unsigned int MaterialIndex = -1;


		//bygger p� antagandet att varje Mesh kan ha en olik layout inom en model
		VertexArrayObject VAO;
		VertexLayout Layout;
		VertexBuffer Buffer;
		ElementBufferObject ArrayObject;
		//Model* AssociatedModel = nullptr;
		//std::vector<Vec3Data> SavedPositionData = {};
		VertexLayout p_GetVertexLayout(std::vector<VertexAttributes> const& AttributeOrder);
	public:
		//TODO fixa faktiska copy semantic etc f�r mesh objektet
		Mesh(int VerticesToLoadCount, int VertexToLoadSize, void* VertexToLoadData, int ArrayObjectSize, unsigned int* ArrayObjectData);
		Mesh(void* MeshObject,std::vector<VertexAttributes> const& AttributeOrder, std::unordered_map<std::string,Bone> const& BoneMap, size_t* OutMaterialIndex);
		Mesh();
		void FillMeshData(std::vector<VertexAttributes> const& Attributes, const void* VertexData, size_t VertexDataSize, const void* FaceData, size_t FaceDataSize);

		void Rotate(float DegressToRotate, MBMath::MBVector3<float> const& AxisToRotate);
		void Draw();
		//void SavePositions();
		//void RestorePositions();
		//void TransformPositions(MBMath::MBMatrix4<float> const& Transformation);
		unsigned int NumberOfVertices();
		~Mesh();
	};
	//TODO skapa copysemantics och liknande f�r nod objektet, �r shallow copy just nu
	class NodeAnimation;
	class SkeletonAnimation;
	class Node
	{
	private:
		Node* ParentNode = nullptr;
		std::vector<Node> ChildNodes = {};
		size_t m_NumberOfChildren = 0;
		size_t m_NodeIndex = 0;
		std::string NodeID = "";
		void Swap(Node& OtherNode);

		void p_UpdateChildParents();
		void p_FillDefaultTransformations(std::vector<MBMath::MBMatrix4<float>>& VectorToFill,MBMath::MBMatrix4<float> const& CurrentTransform);
		void p_FillAnimationTransformations(std::vector<MBMath::MBMatrix4<float>>& VectorToFill,MBMath::MBMatrix4<float> const& CurrentTransform,SkeletonAnimation const& NodeAnimation, double AnimationTime);
		Node(void* AssimpData, Node* ParentNode, size_t* NumberOfChildren);
		void p_FillNodeIDMap(std::unordered_map<std::string, size_t>& MapToFill);
	public:
		Node(Node const& OtherNode) = delete;
		std::vector<size_t> MeshIndexes = {};
		MBMath::MBMatrix4<float> LocalTransformation = MBMath::MBMatrix4<float>();

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
		Node(void* AssimpData);

		MBMath::MBMatrix4<float> GetLocalTransformation() { return(LocalTransformation); };
		void Draw(MBMath::MBMatrix4<float> const& ParentrTransformation);
		void DrawAnimation(SkeletonAnimation const& NodeAnimation,ShaderProgram* ShaderToUpdate,double AnimationTime);



		std::vector<MBMath::MBMatrix4<float>> GetDefaultTransformations();
		std::vector<MBMath::MBMatrix4<float>> GetAnimationTransformations(SkeletonAnimation const& NodeAnimation,double AnimationTime);
		std::vector<Node> const& GetChildren() const; //Experimentel
		size_t GetNumberOfChildren();
		std::unordered_map<std::string, size_t> GetNodeIDMap();

		void UpdateBones(MBMath::MBMatrix4<float> const& ParentTransformation,SkeletonAnimation const& AnimationToUse,double AnimationTime,ShaderProgram* ShaderToUpdate);
		//kommer med detta beh�va en egentlig copy constructor och etc
	};
	class UniformValue
	{
	protected:
		//std::string m_UniformName = "";
		DataTypes m_Type = DataTypes::Null;
		void* m_DataPointer = nullptr;

		friend void swap(UniformValue& LeftValue, UniformValue& RightValue) noexcept;
		void p_FreeData();
		void* p_CloneData() const;

		template<typename T> 
		T& p_GetData()
		{
			return(*(T*)m_DataPointer);
		}
		template<typename T>
		T const& p_GetData() const
		{
			return(*(const T*)m_DataPointer);
		}
		template<typename T>
		void p_GenericInitialize()
		{
			//releasar inte
			m_DataPointer = new T();
		}

		template<typename T>
		void p_GenericSet(DataTypes TargetDataType, T const& ValueToSet)
		{
			if (m_Type == TargetDataType)
			{
				p_GetData<T>() = ValueToSet;
			}
			else
			{
				*this = UniformValue(ValueToSet);
			}
		}
		template<typename T>
		T& p_GenericGet(DataTypes TargetDataType)
		{
			if (m_Type != TargetDataType)
			{
				throw std::domain_error("trying to get data different from stored");
			}
			else
			{
				return(p_GetData<T>());
			}
		}
		template<typename T>
		T const& p_GenericGet(DataTypes TargetDataType) const
		{
			if (m_Type != TargetDataType)
			{
				throw std::domain_error("trying to get data different from stored");
			}
			else
			{
				return(p_GetData<T>());
			}
		}
	public:
		UniformValue(UniformValue const& ValueToCopy);
		UniformValue(UniformValue&& ValueToSteal) noexcept;
		UniformValue& operator=(UniformValue ValueToSteal);

		UniformValue() {};
		UniformValue(float InitialFloatValue);
		UniformValue(int InitialIntValue);
		UniformValue(MBMath::MBVector3<float> InitialVectorValue);
		UniformValue(MBMath::MBStaticVector<float,4> InitialVectorValue);
		UniformValue(MBMath::MBMatrix4<float>  InitialMatrixValue);
		UniformValue(std::vector<UniformValue>  InitialArrayValue);
		UniformValue(std::map<std::string, UniformValue> InitialStructData);
		//UniformValue(DataTypes InitialDataType);

		void UpdateUniforms(std::string const& NamePrefix, ShaderProgram* ProgramToUpdate) const;
		void UpdateUniforms(UniformValue& ValuesToUpdate) const;

		DataTypes GetType() const { return(m_Type); };
		
		void SetFloat(float FloatValue);
		float& GetFloat();
		
		void SetInt(int FloatValue);
		int& GetInt();

		void SetVec3(MBMath::MBStaticVector3<float> const& VectorToSet);
		MBMath::MBVector3<float>& GetVec3();

		void SetVec4(MBMath::MBStaticVector<float, 4> const& VectorToSet);
		MBMath::MBStaticVector<float, 4>& GetVec4();

		void SetMat4(MBMath::MBMatrix4<float> const& MatrixToSet);
		MBMath::MBMatrix4<float>& GetMat4();

		//OBS implicit omvandlar till en array
		void SetIndex(size_t IndexToSet, UniformValue NewValue);
		UniformValue& GetIndex(size_t IndexToGet);

		//OBS implicit omvandlar typen till en Aggregate
		void SetValue(std::string const& ValueName,UniformValue ValueToAdd);
		UniformValue& GetValue(std::string const& ValueName);
		bool HasValue(std::string const& ValueName);
		//bool HasValue(const char* ValueName);
		
		~UniformValue()
		{
			p_FreeData();
		}
		//void 
	};
	//class UniformValue_Float : public UniformValue
	//{
	//private:
	//public:
	//	float Value = 0;
	//	UniformValue_Float(float NewValue)
	//	{
	//		Value = NewValue;
	//		m_Type = DataTypes::Float;
	//	}
	//	virtual std::unique_ptr<UniformValue> Copy()
	//	{
	//		return(std::unique_ptr<UniformValue>(new UniformValue_Float(Value)));
	//	}
	//	void SetValue(std::string const& NamePrefix, ShaderProgram* ProgramToUpdate) override
	//	{
	//		ProgramToUpdate->SetUniform1f(NamePrefix, Value);
	//	}
	//	virtual void SetFloat(float FloatValue)
	//	{
	//		Value = FloatValue;
	//	}
	//	virtual float& GetFloat()
	//	{
	//		return(Value);
	//	}
	//	virtual ~UniformValue_Float()
	//	{
	//
	//	}
	//};
	//class UniformValue_Int : public UniformValue
	//{
	//private:
	//public:
	//	int Value = 0;
	//
	//	virtual std::unique_ptr<UniformValue> Copy()
	//	{
	//		return(std::unique_ptr<UniformValue>(new UniformValue_Int(Value)));
	//	}
	//
	//	void SetValue(std::string const& NamePrefix, ShaderProgram* ProgramToUpdate) override
	//	{
	//		ProgramToUpdate->SetUniform1i(NamePrefix, Value);
	//	}
	//	UniformValue_Int(int NewValue)
	//	{
	//		Value = NewValue;
	//		m_Type = DataTypes::Int;
	//	}
	//
	//	virtual void SetInt(int IntValue) override
	//	{
	//		Value = IntValue;
	//	}
	//	virtual int& GetInt() override
	//	{
	//		return(Value);
	//	}
	//	virtual ~UniformValue_Int()
	//	{
	//
	//	}
	//};
	//class UniformValue_Vec3 : public UniformValue
	//{
	//private:
	//public:
	//	MBMath::MBVector3<float> Data;
	//
	//	virtual std::unique_ptr<UniformValue> Copy()
	//	{
	//		return(std::unique_ptr<UniformValue>(new UniformValue_Vec3(Data)));
	//	}
	//
	//	void SetValue(std::string const& NamePrefix, ShaderProgram* ProgramToUpdate) override
	//	{
	//		ProgramToUpdate->SetUniformVec3(NamePrefix,Data[0],Data[1],Data[2]);
	//	}
	//	UniformValue_Vec3(MBMath::MBVector3<float> const& Value)
	//	{
	//		Data = Value;
	//		m_Type = DataTypes::Vec3;
	//	}
	//	UniformValue_Vec3(float x, float y, float z)
	//	{
	//		Data[0] = x;
	//		Data[1] = y;
	//		Data[2] = z;
	//		m_Type = DataTypes::Vec3;
	//	}
	//	virtual void SetVec3(float x, float y, float z) override
	//	{
	//		//Value = IntValue;
	//		Data[0] = x;
	//		Data[1] = y;
	//		Data[2] = z;
	//	}
	//	virtual void SetVec3(MBMath::MBVector3<float> const& NewData) override
	//	{
	//		Data = NewData;
	//	}
	//	virtual MBMath::MBVector3<float>& GetVec3() override
	//	{
	//		return(Data);
	//	}
	//	virtual ~UniformValue_Vec3()
	//	{
	//
	//	}
	//};
	//class UniformValue_Mat4 : public UniformValue
	//{
	//private:
	//public:
	//	MBMath::MBMatrix4<float> Value;
	//	
	//	virtual std::unique_ptr<UniformValue> Copy() override
	//	{
	//		return(std::unique_ptr<UniformValue>(new UniformValue_Mat4(Value)));
	//	}
	//
	//	void SetValue(std::string const& NamePrefix, ShaderProgram* ProgramToUpdate) override
	//	{
	//		ProgramToUpdate->SetUniformMat4f(NamePrefix, Value.GetContinousData());
	//	}
	//	UniformValue_Mat4(MBMath::MBMatrix4<float> const& ValuesToSet)
	//	{
	//		Value = ValuesToSet;
	//		m_Type = DataTypes::Matrix4;
	//	}
	//
	//	virtual void SetMat4(MBMath::MBMatrix4<float> const& IntValue) override
	//	{
	//		Value = IntValue;
	//	}
	//	virtual MBMath::MBMatrix4<float>& GetMat4() override
	//	{
	//		return(Value);
	//	}
	//	virtual ~UniformValue_Mat4()
	//	{
	//
	//	}
	//};
	//class UniformValue_Array : public UniformValue
	//{
	//private:
	//	//std::string m_TypeName = "";
	//	std::vector<std::unique_ptr<UniformValue>> m_Values = {};
	//public:
	//	virtual std::unique_ptr<UniformValue> Copy() override
	//	{
	//		return(std::unique_ptr<UniformValue>(new UniformValue_Array(m_Values)));
	//	}
	//	void SetValue(std::string const& NamePrefix, ShaderProgram* ProgramToUpdate) override
	//	{
	//		for (size_t i = 0; i < m_Values.size(); i++)
	//		{
	//			m_Values[i]->SetValue(NamePrefix + "[" + std::to_string(i) + "]", ProgramToUpdate);
	//		}
	//	}
	//	UniformValue_Array()
	//	{
	//		m_Type = DataTypes::Array;
	//	}
	//	UniformValue_Array(std::vector<std::unique_ptr<UniformValue>> const& NewValues)
	//	{
	//		for (size_t i = 0; i < NewValues.size(); i++)
	//		{
	//			m_Values.push_back(NewValues[i]->Copy());
	//		}
	//		m_Type = DataTypes::Array;
	//	}
	//	UniformValue_Array(std::vector<std::unique_ptr<UniformValue>>&& NewValues)
	//	{
	//		m_Values = std::move(NewValues);
	//		m_Type = DataTypes::Array;
	//	}
	//	//void AddValue();
	//	void PushValue(std::unique_ptr<UniformValue> ValueToAdd)
	//	{
	//		m_Values.push_back(std::move(ValueToAdd));
	//	}
	//	UniformValue& GetIndex(size_t Index) override
	//	{
	//		return(*m_Values.at(Index));
	//	}
	//
	//
	//	~UniformValue_Array()
	//	{
	//
	//	}
	//
	//};
	//class UniformValue_Aggregate : public UniformValue
	//{
	//private:
	//	std::map<std::string,std::unique_ptr<UniformValue>> m_Values = {};
	//public:
	//	//void AddValue(std::string const& Name);
	//	virtual std::unique_ptr<UniformValue> Copy() override
	//	{
	//		return(std::unique_ptr<UniformValue>(new UniformValue_Aggregate(m_Values)));
	//	}
	//
	//	void SetValue(std::string const& NamePrefix, ShaderProgram* ProgramToUpdate) override
	//	{
	//		for (auto& Values : m_Values)
	//		{
	//			Values.second->SetValue(NamePrefix + "." + Values.first, ProgramToUpdate);
	//		}
	//	}
	//	UniformValue_Aggregate()
	//	{
	//		m_Type = DataTypes::Struct;
	//	}
	//	UniformValue_Aggregate(std::map<std::string, std::unique_ptr<UniformValue>> const& NewValues)
	//	{
	//		for (auto const& Value : NewValues)
	//		{
	//			m_Values[Value.first] = Value.second->Copy();
	//		}
	//		m_Type = DataTypes::Struct;
	//	}
	//	UniformValue_Aggregate(std::map<std::string, std::unique_ptr<UniformValue>>&& NewValues)
	//	{
	//		m_Values = std::move(NewValues);
	//		m_Type = DataTypes::Struct;
	//	}
	//	UniformValue& GetValue(std::string const& VariableName) override
	//	{
	//		return(*m_Values.at(VariableName));
	//	}
	//	void AddValue(std::string const& VariableName,std::unique_ptr<UniformValue> ValueToAdd) override
	//	{
	//		m_Values[VariableName] = std::move(ValueToAdd);
	//	}
	//	~UniformValue_Aggregate()
	//	{
	//
	//	}
	//};
	//std::unique_ptr<UniformValue> GetUniformValue(std::vector<std::unique_ptr<UniformValue>>&& ArrayValues);
	//std::unique_ptr<UniformValue> GetUniformValue(std::map<std::string,std::unique_ptr<UniformValue>>&& MapValues);
	//std::unique_ptr<UniformValue> GetUniformValue(float Value);
	//std::unique_ptr<UniformValue> GetUniformValue(int Value);
	//std::unique_ptr<UniformValue> GetUniformValue(MBMath::MBVector3<float> const& Value);
	//std::unique_ptr<UniformValue> GetUniformValue(float x,float y,float z);
	//std::unique_ptr<UniformValue> GetUniformValue(MBMath::MBMatrix4<float> const& Value);
	//class UniformBundle
	//{
	//private:
	//	std::map<std::string, std::unique_ptr<UniformValue>> m_UniformMap = {};
	//public:
	//	friend void swap(UniformBundle& LeftBundle, UniformBundle& RightBundle);
	//
	//
	//	UniformBundle(UniformBundle&& BundleToSteal) noexcept;
	//	UniformBundle() {};
	//	UniformBundle(UniformBundle const& BundleToCopy);
	//	UniformBundle& operator=(UniformBundle BundleToSteal);
	//	void SetUniforms(ShaderProgram* ProgramModify);
	//
	//	void SetUniform_Float(std::string const& UniformName, double FloatValue);
	//	void SetUniform_Int(std::string const& UniformName, int IntegerValue);
	//	void SetUniform_Vec3(std::string const& UniformName, float x, float y, float z);
	//	void SetUniform_Vec3(std::string const& UniformName, MBMath::MBStaticVector3<float> const& VectorToSet);
	//	void SetUniform_Mat4(std::string const& UniformName, MBMath::MBMatrix4<float> const& MatrixToSet);
	//
	//	UniformValue& GetUniform(std::string const& UniformName);
	//	void AddUniform(std::string const& UniformName, std::unique_ptr<UniformValue> ValueToAdd);
	//
	//	void AddUniformVector(std::string const& VectorName);
	//	void AddAggregateType(std::string const& UniformName);
	//};
	class Texture;
	class Material
	{
	private:
		std::vector<std::string> GetTexturePaths(void* MaterialData, int TextureType);
		//MBGraphicsEngine* AssciatedGraphicsEngine;

		std::string DiffuseTexture = "";
		std::string SpecularTexture = "";
		std::string NormalTexture = "";

		MBMath::MBVector3<float> Color = MBMath::MBVector3<float>(0, 0, 0);
		float SpecularStrength = -1;
		float SpecularExp = -1;


		void p_LoadTextures(MBGraphicsEngine* AssociatedEngine);
	public:
		Material(void* MaterialData, std::string PathToModel, MBGraphicsEngine* AttachedEngine);
		Material(void* MaterialData, std::string PathToModel, std::vector<MaterialAttribute> NewMaterialAttributes, MBGraphicsEngine* AttachedEngine);


		std::vector<MaterialAttribute> MaterialShaderAttributes = { MaterialAttribute::DiffuseTexture,MaterialAttribute::SpecularTexture,MaterialAttribute::NormalTexture };
		std::vector<std::shared_ptr<Texture>> MaterialTextures = {};
		//UniformBundle Uniforms;

		void SetUniforms(ShaderProgram* ProgramModify);
		
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
		MBMath::MBMatrix4<float> GetInterpolatedScaling(float TimeInSec) const;
		MBMath::MBMatrix4<float> GetInterpolatedRotation(float TimeInSec) const;
		MBMath::MBMatrix4<float> GetInterpolatedTranslation(float TimeInSec) const;
	public:
		NodeAnimation(void* AssimpData, double AnimationDuration, double TicksPerSec);
		MBMath::MBMatrix4<float> GetTransformation(float TimeInSec) const;
		NodeAnimation() {};
	};
	class SkeletonAnimation
	{
		std::unordered_map<std::string, NodeAnimation> m_NodeAnimations = {};
	public:
		SkeletonAnimation() {};
		SkeletonAnimation(std::unordered_map<std::string, NodeAnimation>&& Data) { m_NodeAnimations = Data; };
		bool IsInAnimation(std::string const& StringToCheck) const;
		MBMath::MBMatrix4<float> GetNodeTransformation(std::string const& NodeID, double TimeInSec) const;
	};
	class ModelAnimation
	{
	private:
		double TickDuration = 0;
		double TicksPerSec = 0;
		//std::unordered_map<std::string,NodeAnimation> NodeAnimations = {};
	public:
		SkeletonAnimation BoneAnimation;
		//bool IsInAnimation(std::string NodeID) { return(BoneAnimation.IsInAnimation(NodeID)); };
		//MBMath::MBMatrix4<float> GetNodeTransformation(float TimeInSec, std::string NodeId);
		double GetDurationInSec() { return(TickDuration / TicksPerSec); };
		ModelAnimation(void* AssimpData);
	};

	class Transform
	{
	private:
		bool m_HasChanged = false;
		MBMath::MBVector3<float> m_WorldPosition = {0,0,0};
		MBMath::MBVector3<float> m_Scalings = {1,1,1};
		MBMath::MBVector3<float> m_Rotation = {0,0,0}; //euler vinklar

		
	public:
		MBMath::MBMatrix4<float> GetModelMatrix() const;
		
		void SetRotation(float XRotation, float YRotation, float ZRotation);
		void SetRotation(MBMath::MBVector3<float> const& RotationToSet);
		MBMath::MBVector3<float> GetRotation() const;

		void SetPosition(MBMath::MBVector3<float> const& PositionToSet);
		void SetPosition(float x,float y,float z);
		MBMath::MBVector3<float> GetPosition() const;

		void SetScaling(MBMath::MBVector3<float> const& PositionToSet);
		void SetScaling(float x, float y, float z);
		MBMath::MBVector3<float> GetScaling() const;
	};

	class Model
	{
	private:
		//eftersom position och eventuellt vinklar �r s�dant som vi inte n�dv�ndigtvis vill kunna �ndra utan vidare
		//MBMath::MBVector3<float> WorldPosition = MBMath::MBVector3<float>(0, 0, 0);
		
		std::vector<std::unique_ptr<Mesh>> ModelMeshes = {};
		std::vector<size_t> m_MeshMaterialIndexes = {};
		std::vector<Material> ModelMaterials = {};
		//ShaderProgram< ModelShaderProgram = nullptr;
		Node TopNode;
		size_t m_NumberOfNodes = 0;

		std::shared_ptr<ShaderProgram> m_ModelShader = nullptr;

		std::vector<MaterialAttribute> ModelShaderAttributes = { MaterialAttribute::DiffuseTexture,MaterialAttribute::NormalTexture,MaterialAttribute::SpecularTexture };
		std::unordered_map<std::string, Bone> m_BoneMap = {};
		std::vector<ModelAnimation> Animations = {};


		bool AnimationIsPlaying = false;
		double AnimationTime = 0;
		void p_DrawAnimation(ModelAnimation const& ModelAnimationToDraw,ShaderProgram* ShaderToUse, double AnimationTimeInSec);
		void p_DrawDefault();
		void p_DrawDefault_Node(const Node* NodeToProcess, MBMath::MBMatrix4<float> ParentTransformation);
		
		//Mesh* GetMesh(unsigned int MeshIndex);
		MBMath::MBMatrix4<float> m_InverseGlobalMatrix = MBMath::MBMatrix4<float>();
	public:

		//std::vector<std::string> NodeNames = {};
		Transform ModelTransform;
		Model(std::string const& ModelPath, MBGraphicsEngine* AttachedEngine);
		Model(std::string const& ModelPath, std::vector<MaterialAttribute> MaterialAttributes, MBGraphicsEngine* AttachedEngine);
		
		//std::string ModelShader = "";
		MBGraphicsEngine* AssociatedEngine = nullptr;



		ModelAnimation* GetCurrentAnimation();
		void SetShader(std::shared_ptr<ShaderProgram> ProgramToUse);
		
		bool IsPlayingAnimation() { return(AnimationIsPlaying); };
		double GetAnimationTimeInSec() { return(AnimationTime); };
		void Rotate(float DegressToRotate, MBMath::MBVector3<float> const& AxisToRotate);
		void Draw();
		//Material* GetMaterial(unsigned int MaterialIndex);
		//void SetRotation(float XRotation, float YRotation, float ZRotation);
		//MBMath::MBVector3<float> GetRotation();
	};
	class SpriteModel
	{
	private:
		Mesh m_SpriteMesh;
		std::shared_ptr<Texture> m_SpriteTexture = nullptr;
		std::shared_ptr<ShaderProgram> m_SpriteShader = nullptr;
		//MBGraphicsEngine* m_AssociatedEngine = nullptr;
	public:
		Transform ModelTransform;
		SpriteModel(std::shared_ptr<Texture> Texture);

		void SetShader(std::shared_ptr<ShaderProgram> ShaderToUse);
		void SetTexture(std::shared_ptr<Texture> TextureToUse);
		std::shared_ptr<ShaderProgram> GetShader()
		{
			return(m_SpriteShader);
		}
		std::shared_ptr<Texture> GetTexture()
		{
			return(m_SpriteTexture);
		}
		void Draw();
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
		//MBGraphicsEngine* AssociatedGraphicsEngine = nullptr;
		UniformValue m_Uniforms;
	public:
		MBMath::MBVector3<float> WorldSpaceCoordinates = MBMath::MBVector3<float>(0, 0, -4);

		MBMath::MBVector3<float> GetRotation();
		void SetRotation(float XaxisRotation, float YaxisRotation, float ZAxisRotation);
		void SetRotation(MBMath::MBVector3<float> const& NewRotation);
		void SetModelMatrix(MBMath::MBMatrix4<float> const& ModelMatrix);

		MBMath::MBMatrix4<float> GetViewMatrix();
		MBMath::MBMatrix4<float> GetModelMatrix();
		MBMath::MBMatrix4<float> GetProjectionMatrix();

		void Update(ShaderProgram* ShaderToUpdate);
		void Update(UniformValue& ValuesToUpdate);
		MBMath::MBVector3<float> GetDirection();
		MBMath::MBVector3<float> GetRightAxis();
		MBMath::MBVector3<float> GetUpAxis();
		bool IsOrtoGraphic = false;
		void SetFrustum(float NearPlane, float FarPlane, float XMin, float XMax, float YMin, float YMax);
		void SetOrtographicProjection(float Width,float Height);
		Camera();
		//Camera(MBGraphicsEngine* EngineToAttach);
		MBMath::MBMatrix4<float> GetTransformationMatrix();
	};
	class LightSource
	{
	private:
		MBGraphicsEngine* AssociatedGraphicsEngine;

		UniformValue m_Uniforms;
	public:
		MBMath::MBVector3<float> WorldPosition = MBMath::MBVector3<float>(0, 0, 0);
		MBMath::MBVector3<float> LightColor = MBMath::MBVector3<float>(1, 1, 1);
		float AmbienceStrength = 0.1;
		float SpecularExp = 32;
		float SpecularStrength = 0.3;
		void SetLightning(int Position,ShaderProgram* ProgramToUpdate);
		void SetLightning(int Position,UniformValue& ValuesToUpdate);
		LightSource();
	};
	enum class TextureOptions
	{
		NoFilter = 1,
		LinearFilter = 2,
	};
	enum class TextureType
	{
		RED,
		RGBA8,
		RGB8,
	};
	class Texture
	{
	private:
		unsigned int TextureHandle = 0;
		int m_PixelWidth = 0;
		int m_PixelHeight = 0;
		uint8_t* p_LoadFileData(std::string const& FilePath,uint64_t* OutGLType,int* OutWidth,int* OutHeight);
		void p_LoadWithOptions(void* Data,int Width,int Height,uint64_t GL_TextureType,uint64_t Options);

		uint64_t p_TextureTypeToGLType(TextureType);
	public:
		Texture(std::string FilePath);
		Texture(std::string const& FilePath,uint64_t Options);
		Texture(void* Data,int Width,int Height, TextureType DataType, uint64_t Options);
		void FillTexture(void* Data, int Width, int Height, TextureType PixelFormat,uint64_t Options);
		void Bind(int TextureLocation);
		void UnBind(int TextureLocation);
		int GetWidth()
		{
			return(m_PixelWidth);
		}
		int GetHeight()
		{
			return(m_PixelHeight);
		}
		~Texture();
	};
	enum class KeyCode
	{
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		LeftShift, Space, Left, Right, Up, Down, Esc, Enter,_END
	};
	enum class KeyInputType
	{
		Down,
		Pressed,
		Released,
		Null,
	};
	class MBGraphicsEngine
	{
	private:
		void* Window = nullptr;
		std::string ResourceFolder = "";
		std::vector<Model*> LoadedModels = {};
		//std::string CurrentShaderID = "";
		
		std::shared_ptr<ShaderProgram> m_CurrentShader = nullptr;

		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> LoadedShaders = {};
		std::unordered_map<std::string, std::shared_ptr<Texture>> LoadedTextures = {};
		std::vector<LightSource*> LightSources = {};
		
		
		std::vector<std::shared_ptr<ShaderProgram>> m_PBS_Shaders = {};
		std::unique_ptr<VertexArrayObject> PBS_VertexArray;
		std::unique_ptr<VertexLayout> PBS_Layout;
		std::unique_ptr<VertexBuffer> PBS_VertexBuffer;
		std::unique_ptr<ElementBufferObject> PBS_ElementBuffer;
		std::unique_ptr<FrameBuffer> PostProcessingFrameBuffer;
		void DrawPostProcessing();
		clock_t DeltaTimeTimer = clock();
		double DeltaTime = 0;
		//std::thread PollEventsThread;
		bool _DefaultRemoved = false;

		//Agnostic user input grejer
		std::mutex m_InputMutex;
		KeyInputType m_KeyboardInputs[uint32_t(KeyCode::_END)];

		void p_ResetInput();

		void p_LoadDefaultShaders();

		//Underliggande implementation specifika funktioner
		static void GFLW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	public:
		bool GetKeyDown(KeyCode KeyToCheck);
		bool GetKeyPressed(KeyCode KeyToCheck);
		bool GetKeyReleased(KeyCode KeyToCheck);

		void AddPostProcessingShader(std::shared_ptr<ShaderProgram> NewPostProcessingShader);
		bool FrameByFrame = false;
		double GetDeltaTimeInSec() { return(DeltaTime); };
		//std::string PostProcessingShaderID = "Default_PPS";
		Camera CameraObject = Camera();
		void UpdateUniforms(ShaderProgram* ProgramToUpdate);
		std::shared_ptr<ShaderProgram> GetCurrentShader();
		LightSource* AddLightSource();
		//void SetCurrentShader(std::string ShaderID);
		void SetCurrentShader(std::shared_ptr<ShaderProgram> ShaderToUse);
		MBGraphicsEngine();
		//bool GetKeyUp(unsigned int KeyCode);
		//bool GetKey(unsigned int KeyCode);
		Model* LoadModel(std::string ModelPath,std::vector<MaterialAttribute> MaterialAttributes);
		std::shared_ptr<ShaderProgram> LoadShader(std::string ShaderID,std::string VertexShaderPath,std::string FragmentShaderPath);
		std::shared_ptr<ShaderProgram> LoadShader(std::string ShaderID,std::string VertexShaderPath,std::string GeometryShaderPath,std::string FragmentShaderPath);
		std::shared_ptr<ShaderProgram> GetShader(std::string ShaderID);
		std::shared_ptr<Texture> GetTexture(std::string const& TextureFilePath);
		void PollEvents();
		void Update();
		void WindowCreate(size_t Width,size_t Height,std::string const& MonitorName,bool FullScreen);
		void GetWindowSize(int* Width, int* Height);
	};
};