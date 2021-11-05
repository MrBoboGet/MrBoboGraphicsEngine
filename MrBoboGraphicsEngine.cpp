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
	std::filesystem::current_path(std::filesystem::current_path().parent_path().parent_path().parent_path());
	//std::cout << std::filesystem::current_path() << std::endl;
	MBGE::MBGraphicsEngine GraphicEngine;

	GraphicEngine.CameraObject.SetFrustum(0.1, 1000, -0.1, 0.1, -0.1, 0.1);
	GraphicEngine.CameraObject.WorldSpaceCoordinates = MBMath::MBVector3<float>(0,0,-10);

	clock_t Timer = clock();
	std::vector<MBGE::MaterialAttribute> TestAttributes = { MBGE::MaterialAttribute::DiffuseTexture,MBGE::MaterialAttribute::NormalTexture,MBGE::MaterialAttribute::SpecularTexture};
	//MBGE::Model* TestModel = GraphicEngine.LoadModel("./Resources/Models/source/Survival_BackPack_2.fbx",TestAttributes);

	GraphicEngine.LoadShader("TextureShader", "./Resources/Shaders/LightningVertex.vert", "./Resources/Shaders/LightningFrag.frag");
	GraphicEngine.SetCurrentShader("TextureShader");
	GraphicEngine.GetCurrentShader()->Bind();
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
	TigerModel->ModelShader = "TextureShader";
	bool ZWasPressed = false;
	bool XWasPressed = false;
	while (true)
	{
		GraphicEngine.PollEvents();
		if (GraphicEngine.GetKey('Z'))
		{
			if (ZWasPressed == false)
			{
				GraphicEngine.FrameByFrame = !GraphicEngine.FrameByFrame;
			}
			ZWasPressed = true;
		}
		else
		{
			ZWasPressed = false;
		}
		if ((clock() - Timer) / (float)CLOCKS_PER_SEC < 0.0166666 && GraphicEngine.FrameByFrame == false)
		{
			continue;
		}
		if (GraphicEngine.FrameByFrame)
		{
			if (GraphicEngine.GetKey('X'))
			{
				if (XWasPressed == false)
				{
					XWasPressed = true;
				}
				else
				{
					XWasPressed = true;
					continue;
				}
				XWasPressed = true;
			}
			else
			{
				XWasPressed = false;
				continue;
			}
		}
		PrintDeltaIncrement += 1;
		if (PrintDeltaIncrement% 60 == 0)
		{
			std::cout << (clock() - Timer) / (float)CLOCKS_PER_SEC << std::endl;
		}
		Timer = clock();
		//GraphicEngine.GetCurrentShader()->PrintActiveAttributesAndUniforms();
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
		TigerModel->Draw();
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
