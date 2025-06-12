#pragma once

#include "cApplication.h"
#include "engine/core/math/cVector.h"
#include "engine/core/math/math.h"
#include "engine/graphics/api/iGraphicsDevice.h"
#include "engine/graphics/assets/cFont.h"
#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/cameras/cFreeFlightCamera.h"
#include "engine/graphics/cameras/cRenderTextureCamera2D.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/gui/cWidget_gui.h"
#include "engine/graphics/vulkan/pipeline/cPipeline_vulkan.h"
#include "engine/graphics/window/iWindow.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cFontManager.h"
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
	void renderGui();
	void imgui();
	void input( const df::input::sInputs& _input );

	df::cFreeFlightCamera*        camera;
	df::cRenderTextureCamera2D*   camera2;
	df::vulkan::cPipeline_vulkan* pipeline;
	df::cTexture2D*               texture;
};

inline cTesting::cTesting()
	: texture( nullptr )
{
	auto quad = df::cQuadManager::load( "quad", df::cVector3f( 300, 200, 0 ), df::cVector2f( 600, 400 ), df::color::blue );
	quad->loadTexture( "data/resources/window.png" );
	df::cModelManager::load( "model", "data/glTF-Sample-Assets/Models/Sponza/glTF/Sponza.gltf" );

	camera = new df::cFreeFlightCamera( df::cCamera::sDescription(), 1, .1f );
	camera->setActive( true );
	camera->m_flip_y = true;

	camera2 = df::cRenderTextureCamera2D::create( df::cCamera::sDescription() );
	df::cRenderTexture2D::sDescription description{
		.name       = "test",
		.size       = df::cRenderer::getGraphicsDevice()->getWindow()->getSize(),
		.mip_levels = 1,
		.format     = df::sTextureFormat::kRGBA,
		.usage      = df::sTextureUsage::kTransferSource | df::sTextureUsage::kTransferDestination | df::sTextureUsage::kStorage | df::sTextureUsage::kSampled
		       | df::sTextureUsage::kColorAttachment,
	};
	camera2->createTexture( description );
	camera2->m_flip_y = true;

	const df::cTexture2D::sImageInfo image_info = df::cTexture2D::getInfoFromFile( "window.png" );
	df::cTexture2D::sDescription     description2{
			.name       = "test2",
			.size       = image_info.size,
			.mip_levels = 1,
			.format     = image_info.format,
			.usage      = df::sTextureUsage::kSampled | df::sTextureUsage::kTransferDestination,
	};
	texture = df::cTexture2D::create( description2 );
	texture->uploadDataFromFile( "window.png", texture->getFormat(), 0, false, false );

	df::cEventManager::subscribe( df::event::update, camera, &df::cFreeFlightCamera::update );
	df::cEventManager::subscribe( df::event::render_3d, this, &cTesting::render3d );
	df::cEventManager::subscribe( df::event::render_gui, this, &cTesting::renderGui );
	// df::cEventManager::subscribe( df::event::imgui, this, &cTesting::imgui );
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
	df::cEventManager::unsubscribe( df::event::render_gui, this );
	df::cEventManager::unsubscribe( df::event::render_3d, this );
	df::cEventManager::unsubscribe( df::event::update, camera );

	delete texture;
	delete camera2;
	delete camera;
}

inline void cTesting::render3d()
{
	camera2->bind();
	camera2->m_transform = camera->m_transform;
	camera2->update();
	camera2->beginRender( df::cCamera::kColor | df::cCamera::kDepth );

	df::cModelManager::render();
	df::cQuadManager::render();

	camera2->endRender();
	camera2->unbind();
}

inline void cTesting::renderGui()
{
	std::vector< df::gui::cWidget_gui > elements;
	elements.reserve( 5 );
	for( int i = 0; i < 5; i++ )
		elements.push_back( df::gui::cWidget_gui().layout( df::gui::cLayout_gui().widthGrow( 0 ).heightFixed( 50 ) ).color( df::cColor( .88f, .55f, .19f, 1 ) ) );

	df::gui::cWidget_gui( "OuterContainer" )
		.layout( df::gui::cLayout_gui().widthGrow( 0 ).heightGrow( 0 ).padding( 16 ).margin( 16 ) )
		.color( df::cColor( .98f, .98f, 1, 1 ) )

		.addChild(
			df::gui::cWidget_gui( "SideBar" )
				.layout( df::gui::cLayout_gui().widthFixed( 300 ).heightGrow( 0 ).padding( 16 ).margin( 16 ).direction( df::gui::cLayout_gui::kTopToBottom ) )
				.color( df::cColor( .87f, .84f, .82f, 1 ) )
				.cornerRadius( .5f, .1f )

				.addChild(
					df::gui::cWidget_gui( "ProfilePictureOuter" )
						.layout( df::gui::cLayout_gui().widthGrow( 0 ).padding( 16 ).margin( 16 ).verticalAlignment( df::gui::cLayout_gui::kCenterV ) )
						.color( df::cColor( .65f, .25f, .1f, 1 ) )
						.cornerRadius( .5f, .1f )
						.border( df::gui::cBorder_gui().color( df::cColor( 0, 1, 0, 1 ) ).width( 1, 0 ) )

						.addChild( df::gui::cWidget_gui( "ProfilePicture" ).layout( df::gui::cLayout_gui().widthFixed( 60 ).heightFixed( 60 ) ).image( texture ) )

						.addChild( df::gui::cWidget_gui( "Text" ).addChild( df::gui::cText_gui( "Clay - UI Library" ).size( 24 ).font( df::cFontManager::get( "roboto" ) ) ) ) )

				.addChildren( elements )

				.addChild( df::gui::cWidget_gui( "MainContent" ).layout( df::gui::cLayout_gui().widthGrow( 0 ).heightGrow( 0 ) ).color( df::cColor( .87f, .84f, .82f, 1 ) ) ) )

		.addChild( df::gui::cWidget_gui( "Viewport" )
	                   .layout( df::gui::cLayout_gui().widthPercent( .5f ).heightPercent( .5f ) )
	                   .border( df::gui::cBorder_gui().color( df::color::red ).width( 2, 0 ) )
	                   .image( camera2->getTextures()[ 0 ] ) )
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
