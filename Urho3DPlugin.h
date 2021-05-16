//
// Created by scaled
//

#pragma once
#include <obs.h>

namespace Urho3DPlugin {

	class Urho3DPlugin
	{
	public:
		static const char *Urho3DGetName(void *unused);

		static void *Urho3DCreate(obs_data_t *settings,obs_source_t *source);

		static void Urho3DDestroy(void *data);

		static void Urho3DRender(void *data,gs_effect_t *effect);

		static uint32_t Urho3DWidth(void *data);

		static uint32_t Urho3DHeight(void *data);
	};
}
