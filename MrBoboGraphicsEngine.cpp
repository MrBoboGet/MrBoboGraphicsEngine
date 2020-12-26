// MrBoboGraphicsEngine.cpp : Defines the entry point for the application.
//

#include "MrBoboGraphicsEngine.h"
#include <MBGE.h>
#include <iostream>
#include <string>
#include <filesystem>
int main()
{
	std::cout << std::filesystem::current_path() << std::endl;
	std::filesystem::current_path(std::filesystem::current_path().parent_path().parent_path().parent_path());
	std::cout << std::filesystem::current_path() << std::endl;
	MBGE::MBGraphicsEngine GraphicEngine;
	MBGE::VertexShader TestVertex("./Resources/Shaders/VertexTextureTest");
	MBGE::FragmentShader TestFragment("./Resources/Shaders/FragmentTextureTest");
	MBGE::ShaderProgram ProgramTest(TestVertex, TestFragment);
	ProgramTest.Bind();
	float Positions[] =
	{
		-5,	-5,	-5,	0,0,
		 5,	-5,	-5,	1,0,
		-5,	 5,	-5,	0,1,
		 5,	 5,	-5,	1,1,
		 	 
		-5,	-5,	5,	0,0,
		 5,	-5,	5,	1,0,
		-5,	 5,	5,	0,1,
		 5,	 5,	5,	1,1
	};
	MBGE::VertexArrayObject TestVAO;
	TestVAO.Bind();
	MBGE::VertexBuffer TestBuffer(MBGE::VBTypes::DynamicDraw, sizeof(Positions), (void*)Positions);
	TestBuffer.Bind();
	MBGE::VertexLayout TestLayout;
	TestLayout.AddAttribute(sizeof(float), 3, MBGE::DataTypes::Float);
	TestLayout.AddAttribute(sizeof(float), 2, MBGE::DataTypes::Float);
	TestLayout.Bind();
	MBGE::Texture TestTexture("./Resources/Textures/Johan.png");
	unsigned int ElementBufferData[] =
	{
		0,1,2,
		1,2,3,

		4,5,6,
		5,6,7,

		1,5,3,
		5,3,7,

		0,4,2,
		4,2,6,

		2,3,7,	
		2,7,6,

		0,1,5,
		0,5,4
	};
	float TestPositions2[] =
	{
		-10,	-10,	-1,	0,0,
		 10,	-10,	-1,	1,0,
		-10,	 10,	18,	0,1,
		 10,	 10,	18,	1,1,
	};
	unsigned int TestElement2[] =
	{
		0,1,2,
		1,2,3
	};
	MBGE::VertexBuffer BasicBuffer(MBGE::VBTypes::DynamicDraw, sizeof(TestPositions2), TestPositions2);
	MBGE::ElementBufferObject BasicArrayObject(6, TestElement2);
	BasicBuffer.Bind();
	BasicArrayObject.Bind();
	//TestTexture.Bind();
	//MBGE::ElementBufferObject TestElementBuffer(6, ElementBufferData);
	//TestElementBuffer.Bind();
	MBGE::Mesh TestMesh(8, TestLayout.VertexSize(), Positions, 36, ElementBufferData);
	BasicBuffer.Bind();
	BasicArrayObject.Bind();
	//skjut mig i huvudet vad ointuitivt, måste binda layout för varje facking buffer, bufferns layout är relativt till den nuvarande storade buffern
	TestLayout.Bind();
	//ny kamera för test grejer
	MBGE::Camera TestCamera = MBGE::Camera();
	TestCamera.SetFrustum(1, 1000, -10, 10, -10, 10);
	TestCamera.WorldSpaceCoordinates = MBMath::MBVector3<float>(0, 0,-10);
	MBMath::MBMatrix4<float> TransormationMatrixTest = TestCamera.GetTransformationMatrix();
	std::cout << TransormationMatrixTest << std::endl;
	ProgramTest.SetUniformMat4f("TransformationMatrix",TransormationMatrixTest.GetContinousDataPointer());
	while (true)
	{
		//gör lite grundläggande åka runt grejer
		///*
		if (GraphicEngine.GetKey('W'))
		{
			TestCamera.WorldSpaceCoordinates = TestCamera.WorldSpaceCoordinates + TestCamera.GetDirection() * 0.2;
		}
		if (GraphicEngine.GetKey('A'))
		{
			TestCamera.WorldSpaceCoordinates = TestCamera.WorldSpaceCoordinates + TestCamera.GetRightAxis() * -0.2;
		}
		if (GraphicEngine.GetKey('S'))
		{
			TestCamera.WorldSpaceCoordinates = TestCamera.WorldSpaceCoordinates + TestCamera.GetDirection() * -0.2;
		}
		if (GraphicEngine.GetKey('D'))
		{
			TestCamera.WorldSpaceCoordinates = TestCamera.WorldSpaceCoordinates + TestCamera.GetRightAxis() * 0.2;
		}
		//*/
		TransormationMatrixTest = TestCamera.GetTransformationMatrix();
		ProgramTest.SetUniformMat4f("TransformationMatrix", TransormationMatrixTest.GetContinousDataPointer());
		TestMesh.Rotate(0.03, MBMath::MBVector3<float>(1, 1, 1));
		TestMesh.Draw();
		GraphicEngine.Update();
	}
	return 0;
}
