#pragma once

#include "cApplication.h"
#include "engine/core/math/cVector.h"
#include "engine/core/math/math.h"
#include "engine/graphics/api/iGraphicsDevice.h"
#include "engine/graphics/cameras/cFreeFlightCamera.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/gui/cElement_gui.h"
#include "engine/graphics/vulkan/pipeline/cPipeline_vulkan.h"
#include "engine/graphics/window/iWindow.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/managers/cLightManager.h"
#include "imgui.h"

class cTesting
{
public:
	cTesting();
	~cTesting();

	void update( float _delta_time );
	void render3d();
	void render2d();
	void imgui();
	void input( const df::input::sInputs& _input );

	df::cFreeFlightCamera*        camera;
	df::vulkan::cPipeline_vulkan* pipeline;
};

inline cTesting::cTesting()
{
	// auto quad = df::cQuadManager::load( "quad", df::cVector3f( 300, 200, 0 ), df::cVector2f( 600, 400 ), df::color::blue );
	// quad->loadTexture( "data/resources/window.png" );
	// df::cModelManager::load( "model", "data/glTF-Sample-Assets/Models/Sponza/glTF/Sponza.gltf" );

	camera = new df::cFreeFlightCamera( "freeflight", 1, .1f );
	camera->setActive( true );

	df::cEventManager::subscribe( df::event::update, camera, &df::cFreeFlightCamera::update );
	// df::cEventManager::subscribe( df::event::render_3d, this, &cTesting::render3d );
	df::cEventManager::subscribe( df::event::render_2d, this, &cTesting::render2d );
	df::cEventManager::subscribe( df::event::imgui, this, &cTesting::imgui );
	df::cEventManager::subscribe( df::event::input, this, &cTesting::input );

	df::cRenderer::getGraphicsDevice()->getWindow()->setRelativeMouseMode( true );
	df::iWindow::setCaptureMouse( true );

	df::sLight light{};
	light.type      = df::sLight::kAmbient;
	light.intensity = 0.1f;
	df::cLightManager::create( "ambient", light );

	light           = {};
	light.type      = df::sLight::kDirectional;
	light.direction = df::cVector3f( -.2f, -1, -.3f ).normalized();
	light.intensity = 1;
	df::cLightManager::create( "directional", light );

	light           = {};
	light.type      = df::sLight::kPoint;
	light.position  = df::cVector3f( 0, 100, 0 );
	light.radius    = 1000;
	light.intensity = 1;
	df::cLightManager::create( "point", light );

	light                  = {};
	light.type             = df::sLight::kSpot;
	light.position         = df::cVector3f( 700, 100, 0 );
	light.direction        = df::cVector3f( 0, 0, -1 ).normalized();
	light.radius           = 1000;
	light.intensity        = 1;
	light.spot_inner_angle = df::math::cos( df::math::radians( 12.5f ) );
	light.spot_outer_angle = df::math::cos( df::math::radians( 17.5f ) );
	df::cLightManager::create( "spot", light );
}

inline cTesting::~cTesting()
{
	df::cEventManager::unsubscribe( df::event::input, this );
	df::cEventManager::unsubscribe( df::event::imgui, this );
	df::cEventManager::unsubscribe( df::event::render_2d, this );
	df::cEventManager::unsubscribe( df::event::render_3d, this );
	df::cEventManager::unsubscribe( df::event::update, camera );
}

inline void cTesting::render3d()
{
	camera->beginRender( df::cCamera::kColor | df::cCamera::kDepth );

	df::cModelManager::render();
	df::cQuadManager::render();

	camera->endRender();
}

inline void cTesting::render2d()
{
	std::vector< df::gui::cElement_gui > elements;
	for( int i = 0; i < 5; i++ )
	{
		elements.push_back( df::gui::cElement_gui()
		                        .layout( df::gui::cLayout_gui().width( 0, df::gui::cLayout_gui::kGrow ).height( 50, df::gui::cLayout_gui::kFixed ) )
		                        .color( df::cColor( .88f, .55f, .19f, 1 ) ) );
	}

	df::gui::cElement_gui( "OuterContainer" )
		.layout( df::gui::cLayout_gui().width( 0, df::gui::cLayout_gui::kGrow ).height( 0, df::gui::cLayout_gui::kGrow ).padding( 16 ).margin( 16 ) )
		.color( df::cColor( .98f, .98f, 1, 1 ) )

		.addChild(
			df::gui::cElement_gui( "SideBar" )
				.layout( df::gui::cLayout_gui()
	                         .width( 300, df::gui::cLayout_gui::kFixed )
	                         .height( 0, df::gui::cLayout_gui::kGrow )
	                         .padding( 16 )
	                         .margin( 16 )
	                         .direction( df::gui::cLayout_gui::kTopToBottom ) )
				.color( df::cColor( .87f, .84f, .82f, 1 ) )
				.cornerRadius( .5f, .1f )

				.addChild(
					df::gui::cElement_gui( "ProfilePictureOuter" )
						.layout( df::gui::cLayout_gui().width( 0, df::gui::cLayout_gui::kGrow ).padding( 16 ).margin( 16 ).verticalAlignment( df::gui::cLayout_gui::kCenterV ) )
						.color( df::cColor( .65f, .25f, .1f, 1 ) )
						.cornerRadius( .5f, .1f )
						.border( df::gui::cBorder_gui().color( df::cColor( 0, 1, 0, 1 ) ).width( 1, 0 ) )

						.addChild( df::gui::cElement_gui( "ProfilePicture" )
	                                   .layout( df::gui::cLayout_gui().width( 60, df::gui::cLayout_gui::kFixed ).height( 60, df::gui::cLayout_gui::kFixed ) )
	                                   .color( df::cColor( .65f, .25f, .1f, 1 ) ) ) )

				.addChildren( elements )

				.addChild( df::gui::cElement_gui( "MainContent" )
	                           .layout( df::gui::cLayout_gui().width( 0, df::gui::cLayout_gui::kGrow ).height( 00, df::gui::cLayout_gui::kGrow ) )
	                           .color( df::cColor( .87f, .84f, .82f, 1 ) ) ) )
		.paint();
}

inline void cTesting::imgui()
{
	if( ImGui::Begin( "Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		ImGui::Text( "FPS: %i", cApplication::getFps() );
		ImGui::End();
	}
}

inline void cTesting::input( const df::input::sInputs& /*_input*/ )
{
	if( df::cInputManager::checkKey( df::input::sInput< df::input::kKeyboard >::kEscape ) == df::input::sInput< df::input::kAction >::kPress )
		cApplication::quit();
}
