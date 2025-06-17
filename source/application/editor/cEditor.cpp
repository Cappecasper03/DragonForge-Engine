#include "cEditor.h"

#include "engine/graphics/api/iGraphicsApi.h"
#include "engine/graphics/assets/textures/cRenderTexture2D.h"
#include "engine/graphics/cameras/cFreeFlightCamera.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/gui/cVerticalList_gui.h"
#include "engine/graphics/gui/cWindow_gui.h"
#include "engine/graphics/window/iWindow.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cEventManager.h"

namespace df
{
	cEditor::cEditor()
	{
		DF_ProfilingScopeCpu;

		m_camera = MakeUnique< cFreeFlightCamera >( cCamera::sDescription(), 1.f, .1f );
		m_camera->setActive( true );

		cRenderTexture2D::sDescription description{
			.name       = "test",
			.size       = cRenderer::getApi()->getWindow()->getSize(),
			.mip_levels = 1,
			.format     = sTextureFormat::kRGBA,
			.usage = sTextureUsage::kTransferSource | sTextureUsage::kTransferDestination | sTextureUsage::kStorage | sTextureUsage::kSampled | sTextureUsage::kColorAttachment,
		};

		if( cRenderer::isDeferred() )
		{
			cCameraManager::getInstance()->m_camera_main = cRenderTextureCamera2D::create(
				cCamera::sDescription{ .name = "", .type = cCamera::kOrthographic, .clear_color = color::black, .fov = 90, .near_clip = -1, .far_clip = 100 } );
			cCameraManager::getInstance()->m_camera_main->m_flip_y = cRenderer::getApiType() != cRenderer::kVulkan;
			reinterpret_cast< cRenderTextureCamera2D* >( cCameraManager::getInstance()->m_camera_main.get() )->createTexture( description );

			const cCamera::sDescription camera_description{
				.name        = "deferred",
				.type        = cCamera::kPerspective,
				.clear_color = cColor( .5f, .75f, 1, 1 ),
				.fov         = 90,
				.near_clip   = .1f,
				.far_clip    = 10000,
			};
			cCameraManager::getInstance()->m_deferred_camera = cRenderTextureCamera2D::create( camera_description );

			const cRenderTexture2D::sDescription texture_description{
				.name       = "render_texture",
				.size       = cRenderer::getApi()->getWindow()->getSize(),
				.mip_levels = 1,
				.format     = sTextureFormat::kRGBA,
				.usage = sTextureUsage::kTransferSource | sTextureUsage::kTransferDestination | sTextureUsage::kStorage | sTextureUsage::kSampled | sTextureUsage::kColorAttachment,
			};
			cCameraManager::getInstance()->m_deferred_camera->createTexture( texture_description );
			cCameraManager::getInstance()->m_deferred_camera->createTexture( texture_description );
			cCameraManager::getInstance()->m_deferred_camera->createTexture( texture_description );
		}
		else
		{
			cCameraManager::getInstance()->m_camera_main = cRenderTextureCamera2D::create( cCamera::sDescription() );

			cRenderTextureCamera2D* camera2 = reinterpret_cast< cRenderTextureCamera2D* >( cCameraManager::getInstance()->m_camera_main.get() );
			camera2->createTexture( description );
			camera2->m_flip_y = cRenderer::getApiType() != cRenderer::kVulkan;
		}

		const cCamera::sDescription camera_description{
			.name        = "clay",
			.type        = cCamera::eType::kOrthographic,
			.clear_color = color::transparent,
			.fov         = 90,
			.near_clip   = -1,
			.far_clip    = 100,
		};
		cCameraManager::getInstance()->m_camera_gui           = MakeUnique< cCamera >( camera_description );
		cCameraManager::getInstance()->m_camera_gui->m_flip_y = true;

		cEventManager::subscribe( event::update, this, &cEditor::update );
		cEventManager::subscribe( event::render_3d, this, &cEditor::render3D );
		cEventManager::subscribe( event::render_gui, this, &cEditor::renderGui );

		m_widget = gui::cHorizontalList_gui::create()
		               ->color( color::gray )

		               ->addSlot( gui::cVerticalList_gui::create()
		                              ->color( color::red )

		                              ->addSlot( gui::cVerticalList_gui::create()->color( color::red ) )

		                              ->addSlot( gui::cVerticalList_gui::create()->color( color::green ) ) )

		               ->addSlot( gui::cHorizontalList_gui::create()->color( color::blue ) );
	}

	void cEditor::update( const float _delta_time )
	{
		DF_ProfilingScopeCpu;

		m_camera->update( _delta_time );
	}

	void cEditor::render3D() { DF_ProfilingScopeCpu; }

	void cEditor::renderGui()
	{
		DF_ProfilingScopeCpu;

		m_widget->paint();
	}
}
