//
// Created by scaled
//

#include "Urho3DModule.h"

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Graphics/RenderPath.h>

#define UI_ENABLE
#ifdef UI_ENABLE
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#endif

#define EXTERNAL_WINDOW

//#define SDL_INVISIBLE_WINDOW
#ifdef SDL_INVISIBLE_WINDOW
#include <SDL/SDL.h>
#endif

//#define GLFW_INVISIBLE_WINDOW
#ifdef GLFW_INVISIBLE_WINDOW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

Urho3DModule::Urho3DModule(Context* context, void *window) :
	Urho3D::Object(context), window_(window)
{
}

void Urho3DModule::Setup()
{
	Urho3D::VariantMap engineParameters_;
	//engineParameters_[EP_WINDOW_TITLE] = "Urho3DModule";
	engineParameters_[EP_FULL_SCREEN]  = false;
	engineParameters_[EP_SOUND]		= false;
	engineParameters_[EP_WINDOW_WIDTH] = 1024;
	engineParameters_[EP_WINDOW_HEIGHT] = 768;
	engineParameters_[EP_RESOURCE_PATHS] = "/home/scaled/projects/Urho3D/Urho3DModule/Data;/home/scaled/projects/Urho3D/Urho3DModule/CoreData";
	engineParameters_[EP_WORKER_THREADS] = false;
	
	#ifdef URHO_PATCHED
	engineParameters_[EP_NOT_USE_SDL] = true;
	#endif
	
	#ifdef EXTERNAL_WINDOW
	#ifndef GLFW_INVISIBLE_WINDOW
	#ifndef SDL_INVISIBLE_WINDOW
	engineParameters_["ExternalWindow"] = window_;
	#endif
	#endif
	#endif


	#ifdef GLFW_INVISIBLE_WINDOW
	// GLFW
	if (glfwInit() == GL_FALSE)
	{
		return;
	}

	// Window
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	GLFWwindow* _window = glfwCreateWindow(1024, 768, "vtuber", NULL,NULL);
	if (_window == NULL)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return;
	}

	engineParameters_["ExternalWindow"] = _window;
	#endif

	#ifdef SDL_INVISIBLE_WINDOW
	//SDL_Window * sdl_window = SDL_CreateWindow("VTuber", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
	SDL_Window * sdl_window = SDL_CreateWindowFrom(window_, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
	SDL_HideWindow(sdl_window);
	//SDL_GLContext glcontext = SDL_GL_CreateContext(sdl_window);
	
	engineParameters_["ExternalWindow"] = sdl_window;
	#endif

	engine_ = new Urho3D::Engine(context_);
	engine_->Initialize(engineParameters_);
	printf("HUI!\n");
	
	Input* input = GetSubsystem<Input>();
        input->SetMouseMode(MM_FREE);
	input->SetMouseVisible(true);
	
	
	// Set custom window Title & Icon
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	//// test code	
	scene_ = new Scene(context_);
	scene_->CreateComponent<Octree>();
	
	Urho3D::Node* cameraNode_ = scene_->CreateChild("Camera");
	Urho3D::Camera* camera_ = cameraNode_->CreateComponent<Camera>();
	cameraNode_->SetPosition(Vector3(-1.5f, 0.0f, -1.5f));
	cameraNode_->SetRotation(Quaternion(0.0f, 45.0f, 0.0f));
	
	Renderer* renderer = GetSubsystem<Renderer>();
	viewport_ = (new Viewport(context_, scene_, camera_));
	renderer->SetViewport(0, viewport_);
	
	// Set background color
	renderer->GetDefaultZone()->SetFogColor(Color::GREEN);
	//renderer->GetDefaultZone()->SetFogColor(Color::TRANSPARENT);
	//renderer->GetDefaultZone()->SetFogColor(Color::MAGENTA);
	renderer->GetDefaultZone()->SetAmbientColor(Color::WHITE);
	
	/*SharedPtr<RenderPath> effectRenderPath = viewport_->GetRenderPath()->Clone();
	effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/FXAA3.xml"));
	viewport_->SetRenderPath(effectRenderPath);*/
	
	// Create cube Cube
	Node* cube = scene_->CreateChild("Box");
	
	StaticModel* cubeModel = cube->CreateComponent<StaticModel>();
	cubeModel->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	
	for (int i = 1; i < 1000; i++) {
		Node* myCube = scene_->CreateChild("Box" + 1);
		myCube->SetPosition( (Vector3(i%10, (i/10)%10, i/100) - Vector3(0.5, 0.5, 0.5)) * 100 );
		
		StaticModel* myCubeModel = myCube->CreateComponent<StaticModel>();
		myCubeModel->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	}
	
	// Construct new Text object
	#ifdef UI_ENABLE
	Urho3D::Text* helloText = new Text(context_);
	helloText->SetText("Hello World!");
	helloText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 20);
	helloText->SetColor(Color(0.0f, 1.0f, 0.0f));
	helloText->SetHorizontalAlignment(HA_CENTER);
	helloText->SetVerticalAlignment(VA_CENTER);
	GetSubsystem<UI>()->GetRoot()->AddChild(helloText);
	#endif
}

#define TURN_LENGTH 25
int counter = 0;

void Urho3DModule::UrhoFrame()
{
	Renderer* renderer = GetSubsystem<Renderer>();
	renderer->SetViewport(0, viewport_);
	
	counter++;
	float counterFloat = (counter%TURN_LENGTH) / (float)TURN_LENGTH;
	renderer->GetDefaultZone()->SetFogColor(Color(counterFloat, 0, 0, counterFloat));
	
	Node* cube = scene_->GetChild("Box", true);
	cube->SetRotation(Quaternion(0, counterFloat * 360, 0));
		
	
	engine_->RunFrame();
	/*renderer->Render();
	#ifdef UI_ENABLE
	GetSubsystem<UI>()->Render();
	#endif*/
}

#ifdef URHO_PATCHED
void Urho3DModule::ReadPixels(unsigned char* data, unsigned int data_length)
{
	engine_->GetRenderPixels(data, data_length);
}
#endif

