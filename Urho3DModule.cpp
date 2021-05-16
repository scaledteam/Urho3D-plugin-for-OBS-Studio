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

Urho3DModule::Urho3DModule(Context* context, void *window) :
	Urho3D::Object(context)
{
}

void Urho3DModule::Setup()
{
	Urho3D::VariantMap engineParameters_;
	engineParameters_[EP_WINDOW_TITLE] = "Urho3DModule";
	engineParameters_[EP_FULL_SCREEN]  = false;
	engineParameters_[EP_SOUND]		= false;
	engineParameters_[EP_WINDOW_WIDTH] = 1024;
	engineParameters_[EP_WINDOW_HEIGHT] = 768;
	//engineParameters_[EP_HEADLESS]	 = true;
	
	/*if (window_ != NULL)
		engineParameters_["ExternalWindow"] = window_;
	else
		printf("\n\nExternal window not initialized.\n\n\n");*/
	
	engineParameters_[EP_RESOURCE_PATHS] = "/home/scaled/projects/Urho3D/Urho3DModule/Data;/home/scaled/projects/Urho3D/Urho3DModule/CoreData";

	engine_ = new Urho3D::Engine(context_);
	engine_->Initialize(engineParameters_);
}


void Urho3DModule::Start()
{
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
	Urho3D::Viewport* viewport_ = (new Viewport(context_, scene_, camera_));
	renderer->SetViewport(0, viewport_);
	
	// Set background color
	renderer->GetDefaultZone()->SetFogColor(Color::GREEN);
	//renderer->GetDefaultZone()->SetFogColor(Color::TRANSPARENT);
	//renderer->GetDefaultZone()->SetFogColor(Color::MAGENTA);
	renderer->GetDefaultZone()->SetAmbientColor(Color::WHITE);
	
	// Create cube Cube
	Node* cube = scene_->CreateChild("Box");
	
	StaticModel* cubeModel = cube->CreateComponent<StaticModel>();
	cubeModel->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	
	Input* input = GetSubsystem<Input>();
        input->SetMouseMode(MM_FREE);
	input->SetMouseVisible(true);
}

#define TURN_LENGTH 25
int counter = 0;
void Urho3DModule::UrhoFrame()
{
	Renderer* renderer = GetSubsystem<Renderer>();
	
	counter++;
	renderer->GetDefaultZone()->SetFogColor(Color((counter%TURN_LENGTH)*.1, 0, 0));
	
	Node* cube = scene_->GetChild("Box", true);
	cube->SetRotation(Quaternion(0, counter * 360 / TURN_LENGTH, 0));
	
	engine_->RunFrame();
}

void Urho3DModule::Stop()
{
}
