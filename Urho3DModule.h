//
// Created by scaled
//

#pragma once

//#define URHO_PATCHED

#include <Urho3D/Core/Object.h>

namespace Urho3D
{
	class Engine;
	class Scene;
	class Viewport;
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
    	virtual void UrhoFrame();
    	#ifdef URHO_PATCHED
    	virtual void ReadPixels(unsigned char* data, unsigned int data_length);
    	#endif

private:
	Urho3D::Scene* scene_;
  	Urho3D::Engine* engine_;
  	Urho3D::Viewport* viewport_;
	void* window_ = NULL;
};

