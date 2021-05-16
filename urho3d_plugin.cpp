//
// Created by scaled
//

#include <obs-module.h>
#include "Urho3DPlugin.h"

OBS_DECLARE_MODULE()
MODULE_EXPORT const char *obs_module_description(void)
{
	return "Urho3D integration into OBS Studio";
}

bool obs_module_load(void)
{
	obs_source_info Urho3D_video{
	};
	Urho3D_video.id = "Urho3D";
	Urho3D_video.type = OBS_SOURCE_TYPE_INPUT;
	Urho3D_video.output_flags = OBS_SOURCE_VIDEO;
	Urho3D_video.get_name =
		Urho3DPlugin::Urho3DPlugin::Urho3DGetName;
	Urho3D_video.create =
		Urho3DPlugin::Urho3DPlugin::Urho3DCreate;
	Urho3D_video.destroy =
		Urho3DPlugin::Urho3DPlugin::Urho3DDestroy;
	Urho3D_video.video_render =
		Urho3DPlugin::Urho3DPlugin::Urho3DRender;
	Urho3D_video.get_width =
		Urho3DPlugin::Urho3DPlugin::Urho3DWidth;
	Urho3D_video.get_height =
		Urho3DPlugin::Urho3DPlugin::Urho3DHeight;

	obs_register_source(&Urho3D_video);

	return true;
}
