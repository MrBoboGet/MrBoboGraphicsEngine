// MrBoboGraphicsEngine.cpp : Defines the entry point for the application.
//

#include "MrBoboGraphicsEngine.h"
#include <MBGE.h>
#include <iostream>
#include <string>
#include <filesystem>
int main()
{
	//quaternion tests
	/*
	MBMath::Quaternion<float> TestQuaternion1(4.0, 2.0, -3.0, 14.0);
	MBMath::Quaternion<float> TestQuaternion2(-4.0, 3.0, 2.0, -1.0);
	std::cout << TestQuaternion1 * TestQuaternion2 << std::endl;
	MBMath::Quaternion<float> TestRotation(45, MBMath::MBVector3<float>(0, 1, 0));
	MBMath::Quaternion<float> TestRotation2(45, MBMath::MBVector3<float>(0, 0, 1));
	std::cout << TestRotation << std::endl;
	std::cout << TestRotation.RotateVector(MBMath::MBVector3<float>(1, 0, 0))<<std::endl;
	std::cout << TestRotation.GetRotationMatrix() << std::endl;
	std::cout << TestRotation2 << std::endl;
	std::cout << TestRotation.Slerp(TestRotation2, 0.5) << std::endl;
	exit(0);
	*/
	//std::cout << std::filesystem::current_path() << std::endl;
	
	
	std::filesystem::current_path(std::filesystem::current_path().parent_path());
	
	
	//std::cout << std::filesystem::current_path() << std::endl;
	MBGE::MBGraphicsEngine GraphicEngine;
	GraphicEngine.WindowCreate(800, 600, "MBGraphicsEngine Test", false);
	std::shared_ptr<MBGE::ShaderProgram> PPShader= GraphicEngine.LoadShader("Default_PPS", "./Resources/Shaders/PostProcessing/PostProcessingVert.vert", "./Resources/Shaders/PostProcessing/PostProcessingFrag.frag");
	PPShader->Bind();
	PPShader->SetUniform1i("RenderedImage", 0);
	PPShader->SetUniform1i("DepthStencilTexture", 1);
	GraphicEngine.AddPostProcessingShader(PPShader);
	std::cout << "MBGETest 3" << std::endl;
	//GraphicEngine.CameraObject.SetFrustum(0.1, 100000, -0.1, 0.1, -0.1, 0.1);
	GraphicEngine.CameraObject.SetOrtographicProjection(500, 375);
	GraphicEngine.CameraObject.WorldSpaceCoordinates = MBMath::MBVector3<float>(0,0,-10);

	clock_t Timer = clock();
	std::vector<MBGE::MaterialAttribute> TestAttributes = { MBGE::MaterialAttribute::DiffuseTexture,MBGE::MaterialAttribute::NormalTexture,MBGE::MaterialAttribute::SpecularTexture};
	//MBGE::Model* TestModel = GraphicEngine.LoadModel("./Resources/Models/source/Survival_BackPack_2.fbx",TestAttributes);

	auto TigerShader = GraphicEngine.LoadShader("TextureShader", "./Resources/Shaders/LightningVertex.vert", "./Resources/Shaders/LightningFrag.frag");
	
	GraphicEngine.SetCurrentShader(TigerShader);
	
	//std::cout << sizeof(MBGE::UniformValue) << std::endl;
	//std::cout << sizeof(MBGE::UniformValue_Float) << std::endl;
	//std::cout << sizeof(MBGE::UniformValue_Int) << std::endl;
	//std::cout << sizeof(MBGE::UniformValue_Vec3) << std::endl;
	//std::cout << sizeof(MBGE::UniformValue_Mat4) << std::endl;

	//GraphicEngine.GetCurrentShader()->Bind();
	
	MBGE::LightSource* TestLightning = GraphicEngine.AddLightSource();

	TestLightning->AmbienceStrength = 0.1;
	TestLightning->LightColor = MBMath::MBVector3<float>(1, 1, 1);
	TestLightning->SpecularStrength = 1;
	TestLightning->SpecularExp = 16;
	//TestModel->ModelShader = "TextureShader";

	std::vector<MBGE::MaterialAttribute> CheshireAttributes = { MBGE::MaterialAttribute::NormalTexture, MBGE::MaterialAttribute::DiffuseTexture };
	//MBGE::Model* ChesireModel = GraphicEngine.LoadModel("./Resources/Models/Chesire/source/cheshire.fbx", CheshireAttributes);
	//ChesireModel->ModelShader = "TextureShader";
	int PrintDeltaIncrement = 0;
	std::vector<MBGE::MaterialAttribute> TigerAttributes = { MBGE::MaterialAttribute::DiffuseTexture };
	MBGE::Model* TigerModel = GraphicEngine.LoadModel("./Resources/Models/Tiger/source/Tiger.fbx", TigerAttributes);
	MBGE::Model* TigerModel2 = GraphicEngine.LoadModel("./Resources/Models/Tiger/source/Tiger.fbx", TigerAttributes);
	MBGE::Model* TigerModel3 = GraphicEngine.LoadModel("./Resources/Models/Tiger/source/Tiger.fbx", TigerAttributes);
	TigerModel->SetShader(TigerShader);

	TigerModel->ModelTransform.SetScaling(1, 1, 1);
	TigerModel->ModelTransform.SetPosition(0, 0, 0);
	TigerModel->ModelTransform.SetRotation(90, 0, 0);

	TigerModel2->SetShader(TigerShader);
	TigerModel2->ModelTransform.SetScaling(1, 1, 1);
	TigerModel2->ModelTransform.SetPosition(150, 0, 0);
	TigerModel2->ModelTransform.SetRotation(180, 0, 0);

	TigerModel3->SetShader(TigerShader);
	TigerModel3->ModelTransform.SetScaling(1, 1, 1);
	TigerModel3->ModelTransform.SetPosition(-150, 0, 0);
	TigerModel3->ModelTransform.SetRotation(270, 0, 0);

	GraphicEngine.CameraObject.WorldSpaceCoordinates = MBMath::MBVector3<float>(0, 0, 150);
	GraphicEngine.CameraObject.SetRotation(0, 180, 0);

	MBGE::SpriteModel TestSprite = MBGE::SpriteModel(GraphicEngine.GetTexture("./Resources/Textures/Test.png"));
	std::shared_ptr<MBGE::ShaderProgram> SpriteShader = GraphicEngine.LoadShader("SpriteShader", "./Resources/Shaders/VertexTextureTest.vert","./Resources/Shaders/FragmentTextureTest.frag");
	TestSprite.SetShader(SpriteShader);
	TestSprite.ModelTransform.SetScaling(100, 100, 1);

	while (true)
	{
		GraphicEngine.PollEvents();
		if (GraphicEngine.GetKeyPressed(MBGE::KeyCode::Z))
		{
			GraphicEngine.FrameByFrame = !GraphicEngine.FrameByFrame;
			std::cout << "Z pressed" << std::endl;
		}
		//if ((clock() - Timer) / (float)CLOCKS_PER_SEC < 0.0166666 && GraphicEngine.FrameByFrame == false)
		if (false && GraphicEngine.FrameByFrame == false)
		{
			continue;
		}
		if (GraphicEngine.FrameByFrame)
		{
			//if (GraphicEngine.GetKeyPressed(MBGE::KeyCode::X))
			//{
			//	GraphicEngine.
			//}
		}
		PrintDeltaIncrement += 1;
		if (PrintDeltaIncrement% 60 == 0)
		{
			double DeltaTime = (clock() - Timer) / (double)CLOCKS_PER_SEC;
			std::cout << "Delta time: "<<DeltaTime<<"="<<1/DeltaTime<<"fps" << std::endl;
		}
		Timer = clock();
		//GraphicEngine.GetCurrentShader()->PrintActiveAttributesAndUniforms();
		//gör lite grundläggande åka runt grejer
		///*
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::W))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetDirection() * 2;
		}
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::A))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetRightAxis() * -2;
		}
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::S))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetDirection() * -2;
		}
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::D))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetRightAxis() * 2;
		}
		//åker upp/ner
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::Space))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetUpAxis() * 2;
		}
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::LeftShift))
		{
			GraphicEngine.CameraObject.WorldSpaceCoordinates = GraphicEngine.CameraObject.WorldSpaceCoordinates + GraphicEngine.CameraObject.GetUpAxis() * -2;
		}
		//piltangenterna
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::Right)) //right
		{
			GraphicEngine.CameraObject.SetRotation(GraphicEngine.CameraObject.GetRotation() + MBMath::MBVector3<float>(0, 2, 0));
		}
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::Left)) //left
		{
			GraphicEngine.CameraObject.SetRotation(GraphicEngine.CameraObject.GetRotation() + MBMath::MBVector3<float>(0, -2, 0));
		}
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::Up)) //up
		{
			GraphicEngine.CameraObject.SetRotation(GraphicEngine.CameraObject.GetRotation() + MBMath::MBVector3<float>(2, 0, 0));
		}
		if (GraphicEngine.GetKeyDown(MBGE::KeyCode::Down)) //down
		{
			GraphicEngine.CameraObject.SetRotation(GraphicEngine.CameraObject.GetRotation() + MBMath::MBVector3<float>(-2, 0, 0));
		}
		TestLightning->WorldPosition = GraphicEngine.CameraObject.WorldSpaceCoordinates;
		//*/
		GraphicEngine.UpdateUniforms(TigerShader.get());
		TigerModel->Draw();
		TigerModel2->Draw();
		TigerModel3->Draw();
		GraphicEngine.CameraObject.Update(TestSprite.GetShader().get());
		TestSprite.GetShader()->Bind();
		TestSprite.GetShader()->SetUniformMat4f("Model",TestSprite.ModelTransform.GetModelMatrix().GetContinousData());
		TestSprite.Draw();
		//TestModel->Draw();
		
		//ChesireModel->Draw();
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
