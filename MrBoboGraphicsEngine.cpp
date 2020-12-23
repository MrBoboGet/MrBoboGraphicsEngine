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
		-0.5,-0.5,0,0,0,
		-0.5,0.5,0,	0,1,
		0.5,0.5,0,	1,1,
		0.5,-0.5,0,	1,0
	};
	MBGE::VertexArrayObject TestVAO;
	TestVAO.Bind();
	MBGE::VertexBuffer TestBuffer(MBGE::VBTypes::DynamicDraw, sizeof(Positions), (void*)Positions);
	TestBuffer.Bind();
	MBGE::VertexLayout TestLayout;
	TestLayout.AddAttribute(sizeof(float), 3, MBGE::DataTypes::Float);
	TestLayout.AddAttribute(sizeof(float), 2, MBGE::DataTypes::Float);
	TestLayout.Bind();
	MBGE::Texture TestTexture("./Resources/Textures/Test.png");
	unsigned int ElementBufferData[] =
	{
		0,1,2,
		0,2,3
	};
	MBGE::ElementBufferObject TestElementBuffer(6, ElementBufferData);
	TestElementBuffer.Bind();
	while (true)
	{
		TestElementBuffer.DrawTriangles();
		GraphicEngine.Update();
	}
	return 0;
}
