//
// Created by scaled
//

#include "Urho3DPlugin.h"
#include "Urho3DModule.h"
#include <obs-module.h>
#include <obs/obs-frontend-api.h>
#include <Urho3D/Core/Context.h>

namespace{
	struct Urho3D_data {
		obs_source_t *source;
		uint16_t modelId;
		Urho3DModule* myApp;
		Urho3D::Context* context;
		bool not_first = false;
	};

	static uint64_t modelCount = 0;

	}

const char * Urho3DPlugin::Urho3DPlugin::Urho3DGetName(void *unused)
{
	UNUSED_PARAMETER(unused);
	return "Urho3DModule";
}

void * Urho3DPlugin::Urho3DPlugin::Urho3DCreate(obs_data_t *settings,
							   obs_source_t *source)
{
	Urho3D_data *vtb = (Urho3D_data *)malloc(sizeof(Urho3D_data));
	vtb->source = source;
	
	//obs_enter_graphics();
	vtb->context = (new Urho3D::Context());
	vtb->myApp = new Urho3DModule(vtb->context, NULL);
	//vtb->myApp = new Urho3DModule(vtb->context, obs_get_video());
	//vtb->myApp = new Urho3DModule(vtb->context, obs_frontend_get_main_window());
	
	vtb->myApp->Setup();
	vtb->myApp->Start();
	//vtb->myApp->UrhoFrame();
	
	/*for (int i = 1; i < 100; i++)
		vtb->myApp->UrhoFrame();*/
	
	//obs_leave_graphics();
	
	modelCount++;

	return vtb;
}

void Urho3DPlugin::Urho3DPlugin::Urho3DDestroy(void *data)
{
	
	Urho3D_data *spv = (Urho3D_data *)data;

	delete spv;
}

void Urho3DPlugin::Urho3DPlugin::Urho3DRender(
	void *data,
	gs_effect_t *effect)
{
	Urho3D_data *spv = (Urho3D_data *)data;

	int width, height;
	width = 1024;
	height = 768;

	obs_enter_graphics();

	/*gs_texture_t *tex =gs_texture_create(width, height, GS_RGBA, 1, NULL, GS_DYNAMIC);

	uint8_t *ptr;
	uint32_t linesize;
	
	if (gs_texture_map(tex, &ptr, &linesize))
		spv->myApp->UrhoFrame();
	gs_texture_unmap(tex);
	
	obs_source_draw(tex, 0, 0, 0, 0, true);

	gs_texture_destroy(tex);*/
	
	if (spv->not_first == false) {
		spv->not_first = true;
		//spv->context = (new Urho3D::Context());
		//spv->myApp = new Urho3DModule(spv->context, NULL);
		//spv->myApp = new Urho3DModule(spv->context, obs_get_video());
		//spv->myApp = new Urho3DModule(spv->context, obs_frontend_get_main_window());
		//spv->myApp->Setup();
		//spv->myApp->Start();
	}
	else {
		spv->myApp->UrhoFrame();
	}
	
	obs_leave_graphics();
}

uint32_t Urho3DPlugin::Urho3DPlugin::Urho3DWidth(void *data)
{
	return 1024;
}

uint32_t Urho3DPlugin::Urho3DPlugin::Urho3DHeight(void *data)
{
	return 768;
}
