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
	//MBGE::ShaderProgram ProgramTest(TestVertex, TestFragment);
	//ProgramTest.Bind();
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
	//MBGE::Texture TestTexture("./Resources/Models/VikingRoom/source/texture.png");
	MBGE::Texture TestTexture("./Resources/Models/diffuse.jpg");
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
	//MBGE::Camera TestCamera = MBGE::Camera();
	//TestCamera.SetFrustum(0.1, 1000, -0.1, 0.1, -0.1, 0.1);
	//TestCamera.WorldSpaceCoordinates = MBMath::MBVector3<float>(0, 0,-10);
	//MBMath::MBMatrix4<float> TransormationMatrixTest = TestCamera.GetTransformationMatrix();
	//std::cout << TransormationMatrixTest << std::endl;
	GraphicEngine.CameraObject.SetFrustum(0.1, 1000, -0.1, 0.1, -0.1, 0.1);
	GraphicEngine.CameraObject.WorldSpaceCoordinates = MBMath::MBVector3<float>(0,0,-10);
	//ProgramTest.SetUniformMat4f("TransformationMatrix",TransormationMatrixTest.GetContinousDataPointer());
	//frameratge timer
	clock_t Timer = clock();
	std::vector<MBGE::MaterialAttribute> TestAttributes = { MBGE::MaterialAttribute::DiffuseTexture,MBGE::MaterialAttribute::NormalTexture,MBGE::MaterialAttribute::SpecularTexture};
	//MBGE::Model* TestModel = GraphicEngine.LoadModel("./Resources/Models/Chesire/source/cheshire.fbx",TestAttributes);
	MBGE::Model* TestModel = GraphicEngine.LoadModel("./Resources/Models/source/Survival_BackPack_2.fbx",TestAttributes);
	//MBGE::Model* TestModel = GraphicEngine.LoadModel("./Resources/Models/backpack.obj",TestAttributes);
	
	//MBGE::Model* TestModel = GraphicEngine.LoadModel("./Resources/Models/VikingRoom/source/vikingroom.fbx");
	//GraphicEngine.LoadShader("TextureShader", "./Resources/Shaders/FlatColorVertex", "./Resources/Shaders/FlatColorFragment");
	//GraphicEngine.LoadShader("TextureShader", "./Resources/Shaders/VertexTextureTest", "./Resources/Shaders/FragmentTextureTest");
	GraphicEngine.LoadShader("TextureShader", "./Resources/Shaders/LightningVertex.vert", "./Resources/Shaders/LightningFrag.frag");
	GraphicEngine.SetCurrentShader("TextureShader");
	GraphicEngine.GetCurrentShader()->Bind();
	//GraphicEngine.GetCurrentShader()->SetUniformMat4f("TransformationMatrix", TransormationMatrixTest.GetContinousDataPointer());
	MBGE::LightSource* TestLightning = GraphicEngine.AddLightSource();
	//MBGE::Texture TestNormalTexture = MBGE::Texture("./Resources/Models/normal.png");
	//MBGE::Texture TestSpecularTexture = MBGE::Texture("./Resources/Models/specular.jpg");
	//TestTexture.Bind(0);
	//TestNormalTexture.Bind(1);
	//TestSpecularTexture.Bind(2);
	MBGE::VertexShader DebugVetex("./Resources/Shaders/DebugShaders/NormalVertex.vert");
	MBGE::FragmentShader DebugFragment("./Resources/Shaders/DebugShaders/NormalFragment.frag");
	MBGE::GeometryShader DebugGeom("./Resources/Shaders/DebugShaders/NormalGeometry.geom");
	MBGE::ShaderProgram* DebugShader = GraphicEngine.LoadShader("Debug", "./Resources/Shaders/DebugShaders/NormalVertex.vert", "./Resources/Shaders/DebugShaders/NormalGeometry.geom","./Resources/Shaders/DebugShaders/NormalFragment.frag");
	MBGE::ShaderProgram* StandardShader = GraphicEngine.GetCurrentShader();
	StandardShader->SetUniform1i("DiffuseTex", 0);
	StandardShader->SetUniform1i("NormalTex", 1);
	StandardShader->SetUniform1i("SpecTex", 2);
	TestLightning->AmbienceStrength = 0.1;
	TestLightning->LightColor = MBMath::MBVector3<float>(1, 1, 1);
	TestLightning->SpecularStrength = 1;
	TestLightning->SpecularExp = 16;
	TestModel->ModelShader = "TextureShader";

	std::vector<MBGE::MaterialAttribute> CheshireAttributes = { MBGE::MaterialAttribute::NormalTexture, MBGE::MaterialAttribute::DiffuseTexture };
	MBGE::Model* ChesireModel = GraphicEngine.LoadModel("./Resources/Models/Chesire/source/cheshire.fbx", CheshireAttributes);
	ChesireModel->ModelShader = "TextureShader";
	int PrintDeltaIncrement = 0;
	while (true)
	{
		if ((clock() - Timer) / (float)CLOCKS_PER_SEC < 0.0166666)
		{
			continue;
		}
		PrintDeltaIncrement += 1;
		if (PrintDeltaIncrement% 60 == 0)
		{
			std::cout << (clock() - Timer) / (float)CLOCKS_PER_SEC << std::endl;
		}
		Timer = clock();
		//gör lite grundläggande åka runt grejer
		///*
		if (GraphicEngine.GetKey('W'))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetDirection() * 2;
		}
		if (GraphicEngine.GetKey('A'))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetRightAxis() * -2;
		}
		if (GraphicEngine.GetKey('S'))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetDirection() * -2;
		}
		if (GraphicEngine.GetKey('D'))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetRightAxis() * 2;
		}
		//åker upp/ner
		if (GraphicEngine.GetKey(' '))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetUpAxis() * 2;
		}
		if (GraphicEngine.GetKey(340))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetUpAxis() * -2;
		}
		//piltangenterna
		if (GraphicEngine.GetKey(262)) //right
		{
			GraphicEngine.CameraObject.SetRotation(GraphicEngine.CameraObject.GetRotation() + MBMath::MBVector3<float>(0, 2, 0));
		}
		if (GraphicEngine.GetKey(263)) //left
		{
			GraphicEngine.CameraObject.SetRotation(GraphicEngine.CameraObject.GetRotation() + MBMath::MBVector3<float>(0, -2, 0));
		}
		if (GraphicEngine.GetKey(265)) //up
		{
			GraphicEngine.CameraObject.SetRotation(GraphicEngine.CameraObject.GetRotation() + MBMath::MBVector3<float>(2, 0, 0));
		}
		if (GraphicEngine.GetKey(264)) //down
		{
			GraphicEngine.CameraObject.SetRotation(GraphicEngine.CameraObject.GetRotation() + MBMath::MBVector3<float>(-2, 0, 0));
		}
		TestLightning->WorldPosition = GraphicEngine.CameraObject.WorldSpaceCoordinates;
		//*/
		GraphicEngine.UpdateUniforms();
		TestModel->Draw();
		ChesireModel->Draw();
		//GraphicEngine.SetCurrentShader("Debug");
		//TestModel->Draw();
		//std::cout << GraphicEngine.CameraObject.WorldSpaceCoordinates << std::endl;
		//std::cout << TestLightning->WorldPosition << std::endl;
		//TestMesh.Rotate(0.03, MBMath::MBVector3<float>(1, 1, 1));
		//GraphicEngine.GetCurrentShader()->SetUniformMat4f("TransformationMatrix", GraphicEngine.CameraObject.GetTransformationMatrix().GetContinousDataPointer());
		//TestMesh.Draw();
		GraphicEngine.Update();
	}
	return 0;
}
