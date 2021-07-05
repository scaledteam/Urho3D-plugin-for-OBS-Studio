//
// Created by scaled
//

#include "Urho3DPlugin.h"
#include "Urho3DModule.h"
#include <obs-module.h>
#include <obs/obs-frontend-api.h>
#include <Urho3D/Core/Context.h>

//#define MUTEX_ENABLE
//#define INIT_INSIDE_CONTEXT
//#define DRAW_CUBE
//#define RENDER_ONLY_IN_BEGIN
#define WRAP_INTO_TEXRENDER
#define WRAP_INTO_TEXRENDER_AND_RENDER
//#define RENDER_PIXELS_FROM_PATCH

#ifdef MUTEX_ENABLE
#include <mutex>
static std::mutex mut;
#endif


#ifdef DRAW_CUBE
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>

/*6----7
 /|   /|
3----2 |
| 5--|-4
|/   |/
0----1

*/

GLfloat cube_vertices[][8] =  {
 /*  X     Y     Z   Nx   Ny   Nz    S    T */
 {-1.0, -1.0,  1.0, 0.0, 0.0, 1.0, 0.0, 0.0}, // 0
 { 1.0, -1.0,  1.0, 0.0, 0.0, 1.0, 1.0, 0.0}, // 1
 { 1.0,  1.0,  1.0, 0.0, 0.0, 1.0, 1.0, 1.0}, // 2
 {-1.0,  1.0,  1.0, 0.0, 0.0, 1.0, 0.0, 1.0}, // 3

 { 1.0, -1.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0}, // 4
 {-1.0, -1.0, -1.0, 0.0, 0.0, -1.0, 1.0, 0.0}, // 5
 {-1.0,  1.0, -1.0, 0.0, 0.0, -1.0, 1.0, 1.0}, // 6
 { 1.0,  1.0, -1.0, 0.0, 0.0, -1.0, 0.0, 1.0}, // 7

 {-1.0, -1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0}, // 5
 {-1.0, -1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 0.0}, // 0
 {-1.0,  1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0}, // 3
 {-1.0,  1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0}, // 6

 { 1.0, -1.0,  1.0,  1.0, 0.0, 0.0, 0.0, 0.0}, // 1
 { 1.0, -1.0, -1.0,  1.0, 0.0, 0.0, 1.0, 0.0}, // 4
 { 1.0,  1.0, -1.0,  1.0, 0.0, 0.0, 1.0, 1.0}, // 7
 { 1.0,  1.0,  1.0,  1.0, 0.0, 0.0, 0.0, 1.0}, // 2

 {-1.0, -1.0, -1.0,  0.0, -1.0, 0.0, 0.0, 0.0}, // 5
 { 1.0, -1.0, -1.0,  0.0, -1.0, 0.0, 1.0, 0.0}, // 4
 { 1.0, -1.0,  1.0,  0.0, -1.0, 0.0, 1.0, 1.0}, // 1
 {-1.0, -1.0,  1.0,  0.0, -1.0, 0.0, 0.0, 1.0}, // 0

 {-1.0, 1.0,  1.0,  0.0,  1.0, 0.0, 0.0, 0.0}, // 3
 { 1.0, 1.0,  1.0,  0.0,  1.0, 0.0, 1.0, 0.0}, // 2
 { 1.0, 1.0, -1.0,  0.0,  1.0, 0.0, 1.0, 1.0}, // 7
 {-1.0, 1.0, -1.0,  0.0,  1.0, 0.0, 0.0, 1.0}, // 6
};

static void draw_cube(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 8, &cube_vertices[0][0]);
	glNormalPointer(GL_FLOAT, sizeof(GLfloat) * 8, &cube_vertices[0][3]);
	glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, &cube_vertices[0][6]);

	glDrawArrays(GL_QUADS, 0, 24);
}

float const light0_dir[]={0,1,0,0};
float const light0_color[]={78./255., 80./255., 184./255.,1};

float const light1_dir[]={-1,1,1,0};
float const light1_color[]={255./255., 220./255., 97./255.,1};

float const light2_dir[]={0,-1,0,0};
float const light2_color[]={31./255., 75./255., 16./255.,1};

static void redrawTheWindow()
{
	int width, height;
	width = 1024;
	height = 768;
	
	float const aspect = (float)width / (float)height;

	static float a=0;
	static float b=0;
	static float c=0;

	glDrawBuffer(GL_BACK);

	glViewport(0, 0, width, height);

	// Clear with alpha = 0.0, i.e. full transparency
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-aspect, aspect, -1, 1, 2.5, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLightfv(GL_LIGHT0, GL_POSITION, light0_dir);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);

	glLightfv(GL_LIGHT1, GL_POSITION, light1_dir);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);

	glLightfv(GL_LIGHT2, GL_POSITION, light2_dir);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_color);

	glTranslatef(0., 0., -5.);

	glRotatef(a, 1, 0, 0);
	glRotatef(b, 0, 1, 0);
	glRotatef(c, 0, 0, 1);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glColor4f(1., 1., 1., 0.9);

	glCullFace(GL_FRONT);
	draw_cube();
	glCullFace(GL_BACK);
	draw_cube();

	a = fmod(a+0.1, 360.);
	b = fmod(b+0.5, 360.);
	c = fmod(c+0.25, 360.);

	//glXSwapBuffers(Xdisplay, glX_window_handle);
}
#endif

namespace{
	struct Urho3D_data {
		obs_source_t *source;
		uint16_t modelId;
		Urho3DModule* myApp;
		Urho3D::Context* context;
		bool not_first = false;
		gs_texrender_t *render;
	};

	static uint64_t modelCount = 0;

	}

const char * Urho3DPlugin::Urho3DPlugin::Urho3DGetName(void *unused)
{
	UNUSED_PARAMETER(unused);
	return "Urho3DModule";
}

void * Urho3DPlugin::Urho3DPlugin::Urho3DCreate(obs_data_t *settings, obs_source_t *source)
{
	Urho3D_data *vtb = (Urho3D_data *)malloc(sizeof(Urho3D_data));
	vtb->source = source;
	
	#ifndef INIT_INSIDE_CONTEXT
	obs_enter_graphics();
	
	#ifdef WRAP_INTO_TEXRENDER
	vtb->render = gs_texrender_create(GS_RGBA, GS_Z16);
	
	gs_texrender_reset(vtb->render);
	gs_texrender_begin(vtb->render, 1024, 768);
	#endif
	
	vtb->context = (new Urho3D::Context());
	vtb->myApp = new Urho3DModule(vtb->context, obs_frontend_get_main_window_handle());
	
	vtb->myApp->Setup();
	//vtb->myApp->UrhoFrame();
	
	#ifdef RENDER_ONLY_IN_BEGIN
	for (int i = 1; i < 100; i++)
		//vtb->myApp->UrhoFrame();
	#endif
	
	#ifdef WRAP_INTO_TEXRENDER
	gs_texrender_end(vtb->render);
	#endif
	
	obs_leave_graphics();
	#endif
	
	modelCount++;

	return vtb;
}

void Urho3DPlugin::Urho3DPlugin::Urho3DDestroy(void *data)
{
	
	Urho3D_data *spv = (Urho3D_data *)data;
	
	delete spv->myApp;

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
	//gs_blend_state_push();
	//gs_matrix_push();
	//gs_viewport_push();
	//gs_blend_function(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);
	
	/*
	gs_resize(1024, 768);*/

	/*gs_texture_t *tex =gs_texture_create(width, height, GS_RGBA, 1, NULL, GS_DYNAMIC);

	uint8_t *ptr;
	uint32_t linesize;
	
	if (gs_texture_map(tex, &ptr, &linesize))
		spv->myApp->UrhoFrame();
	gs_texture_unmap(tex);
	
	obs_source_draw(tex, 0, 0, 0, 0, true);

	gs_texture_destroy(tex);*/
	
	//gs_set_3d_mode(90.0, 0.1, 110);
	//gs_perspective(90.0, 1, 0.1, 10);
	#ifdef MUTEX_ENABLE
	mut.lock();
	#endif
	
	if (spv->not_first == false) {
		spv->not_first = true;
		
		#ifdef INIT_INSIDE_CONTEXT
		#ifdef WRAP_INTO_TEXRENDER
		//spv->render = gs_texrender_create(GS_RGBA, GS_ZS_NONE);
		spv->render = gs_texrender_create(GS_RGBA, GS_Z16);
		
		gs_texrender_reset(spv->render);
		gs_texrender_begin(spv->render, 1024, 768);
		#endif
		
		spv->context = (new Urho3D::Context());
		spv->myApp = new Urho3DModule(spv->context, obs_frontend_get_main_window_handle());
		spv->myApp->Setup();
		
		#ifdef WRAP_INTO_TEXRENDER
		gs_texrender_end(spv->render);
		#endif
		#endif
	}
	else {
		#ifdef WRAP_INTO_TEXRENDER
		//gs_blend_state_push();
		//gs_blend_function(GS_BLEND_ONE, GS_BLEND_ZERO);
		gs_texrender_reset(spv->render);
		gs_texrender_begin(spv->render, 1024, 768);
		//gs_perspective(90.0, 1, 0.1, 10);
		//gs_set_3d_mode(90.0, 0.1, 110);
		#endif
		
		#ifndef RENDER_ONLY_IN_BEGIN
		spv->myApp->UrhoFrame();
		#endif
		
		//draw_cube();
		//redrawTheWindow();
		//gs_blend_state_pop();
		
		
		#ifdef WRAP_INTO_TEXRENDER
		gs_texrender_end(spv->render);
		#endif
		
		#ifdef WRAP_INTO_TEXRENDER_AND_RENDER
		gs_effect_t *effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
		gs_texture_t *tex = gs_texrender_get_texture(spv->render);
		if (tex) {
			gs_eparam_t *image = gs_effect_get_param_by_name(effect, "image");
			gs_effect_set_texture(image, tex);

			while (gs_effect_loop(effect, "Draw"))
				gs_draw_sprite(tex, 0, 1024, 768);
		}
		#endif
		
		#ifdef RENDER_PIXELS_FROM_PATCH
		gs_texture_t *tex1 =gs_texture_create(width, height, GS_RGBA, 1, NULL, GS_DYNAMIC);

		uint8_t *ptr;
		uint32_t linesize;
		unsigned int data_length = 1024*768*4;
		gs_texture_map(tex1, &ptr, &linesize);
		spv->myApp->ReadPixels((unsigned char*) ptr, data_length);
		gs_texture_unmap(tex1);

		//obs_source_draw(tex1, 0, 0, 0, 0, true);
		//gs_draw_sprite(tex1, 0, 1024, 768);
		
		gs_effect_t *effect1 = obs_get_base_effect(OBS_EFFECT_DEFAULT);
		gs_eparam_t *image1 = gs_effect_get_param_by_name(effect1, "image");
		gs_effect_set_texture(image1, tex1);
		while (gs_effect_loop(effect1, "Draw"))
			gs_draw_sprite(tex1, 0, 1024, 768);

		gs_texture_destroy(tex1);
		#endif
	}
	#ifdef MUTEX_ENABLE
	mut.unlock();
	#endif
	
	//gs_viewport_pop();
	//gs_matrix_pop();
	//gs_blend_state_pop();
	
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
