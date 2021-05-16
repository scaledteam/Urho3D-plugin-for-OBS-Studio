//
// Created by scaled
//

#pragma once

#include <Urho3D/Core/Object.h>

namespace Urho3D
{
	class Engine;
	class Scene;
}

// All Urho3D classes reside in namespace Urho3D
using namespace Urho3D;

class Urho3DModule : public Urho3D::Object
{
	// Enable type information.
	URHO3D_OBJECT(Urho3DModule, Urho3D::Object);

public:
	/// Construct.
	Urho3DModule(Context* context, void* window);

	virtual void Setup();
	virtual void Start();
    	virtual void UrhoFrame();
	virtual void Stop();

private:
	SharedPtr<Scene> scene_;
  	Urho3D::SharedPtr<Urho3D::Engine> engine_;
	void* window_ = NULL;
};

