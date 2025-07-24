#pragma once

namespace df
{
	class cColor
	{
	public:
		cColor();
		cColor( float _r, float _g, float _b, float _a );

		cColor operator+( const cColor& _other ) const { return { r + _other.r, g + _other.g, b + _other.b, a + _other.a }; }
		cColor operator-( const cColor& _other ) const { return { r - _other.r, g - _other.g, b - _other.b, a - _other.a }; }
		cColor operator*( const cColor& _other ) const { return { r * _other.r, g * _other.g, b * _other.b, a * _other.a }; }
		cColor operator/( const cColor& _other ) const { return { r / _other.r, g / _other.g, b / _other.b, a / _other.a }; }
		cColor operator+( const float _scalar ) const { return { r + _scalar, g + _scalar, b + _scalar, a + _scalar }; }
		cColor operator-( const float _scalar ) const { return { r - _scalar, g - _scalar, b - _scalar, a - _scalar }; }
		cColor operator*( const float _scalar ) const { return { r * _scalar, g * _scalar, b * _scalar, a * _scalar }; }
		cColor operator/( const float _scalar ) const { return { r / _scalar, g / _scalar, b / _scalar, a / _scalar }; }

		cColor& operator+=( const cColor& _other ) { return *this = { r + _other.r, g + _other.g, b + _other.b, a + _other.a }; }
		cColor& operator-=( const cColor& _other ) { return *this = { r - _other.r, g - _other.g, b - _other.b, a - _other.a }; }
		cColor& operator*=( const cColor& _other ) { return *this = { r * _other.r, g * _other.g, b * _other.b, a * _other.a }; }
		cColor& operator/=( const cColor& _other ) { return *this = { r / _other.r, g / _other.g, b / _other.b, a / _other.a }; }
		cColor& operator+=( const float _scalar ) { return *this = { r + _scalar, g + _scalar, b + _scalar, a + _scalar }; }
		cColor& operator-=( const float _scalar ) { return *this = { r - _scalar, g - _scalar, b - _scalar, a - _scalar }; }
		cColor& operator*=( const float _scalar ) { return *this = { r * _scalar, g * _scalar, b * _scalar, a * _scalar }; }
		cColor& operator/=( const float _scalar ) { return *this = { r / _scalar, g / _scalar, b / _scalar, a / _scalar }; }

		float r;
		float g;
		float b;
		float a;
	};

	namespace color
	{
		inline const cColor black       = { 0.0f, 0.0f, 0.0f, 1.0f };
		inline const cColor dark_gray   = { 0.25f, 0.25f, 0.25f, 1.0f };
		inline const cColor gray        = { 0.5f, 0.5f, 0.5f, 1.0f };
		inline const cColor light_gray  = { 0.75f, 0.75f, 0.75f, 1.0f };
		inline const cColor white       = { 1.0f, 1.0f, 1.0f, 1.0f };
		inline const cColor transparent = { 0.0f, 0.0f, 0.0f, 0.0f };

		inline const cColor dark_red  = { 0.5f, 0.0f, 0.0f, 1.0f };
		inline const cColor red       = { 1.0f, 0.0f, 0.0f, 1.0f };
		inline const cColor light_red = { 1.0f, 0.5f, 0.5f, 1.0f };

		inline const cColor dark_pink  = { 0.8f, 0.6f, 0.6f, 1.0f };
		inline const cColor pink       = { 1.0f, 0.8f, 0.8f, 1.0f };
		inline const cColor light_pink = { 1.0f, 0.9f, 0.9f, 1.0f };

		inline const cColor dark_brown  = { 0.4f, 0.15f, 0.15f, 1.0f };
		inline const cColor brown       = { 0.6f, 0.2f, 0.2f, 1.0f };
		inline const cColor light_brown = { 0.7f, 0.3f, 0.3f, 1.0f };

		inline const cColor dark_orange  = { 0.7f, 0.4f, 0.0f, 1.0f };
		inline const cColor orange       = { 1.0f, 0.6f, 0.0f, 1.0f };
		inline const cColor light_orange = { 1.0f, 0.75f, 0.25f, 1.0f };

		inline const cColor dark_yellow  = { 0.7f, 0.7f, 0.0f, 1.0f };
		inline const cColor yellow       = { 1.0f, 1.0f, 0.0f, 1.0f };
		inline const cColor light_yellow = { 1.0f, 1.0f, 0.5f, 1.0f };

		inline const cColor dark_green  = { 0.0f, 0.5f, 0.0f, 1.0f };
		inline const cColor green       = { 0.0f, 1.0f, 0.0f, 1.0f };
		inline const cColor light_green = { 0.5f, 1.0f, 0.5f, 1.0f };

		inline const cColor dark_teal  = { 0.0f, 0.3f, 0.3f, 1.0f };
		inline const cColor teal       = { 0.0f, 0.5f, 0.5f, 1.0f };
		inline const cColor light_teal = { 0.5f, 0.75f, 0.75f, 1.0f };

		inline const cColor dark_cyan  = { 0.0f, 0.7f, 0.7f, 1.0f };
		inline const cColor cyan       = { 0.0f, 1.0f, 1.0f, 1.0f };
		inline const cColor light_cyan = { 0.5f, 1.0f, 1.0f, 1.0f };

		inline const cColor dark_blue      = { 0.0f, 0.0f, 0.5f, 1.0f };
		inline const cColor blue           = { 0.0f, 0.0f, 1.0f, 1.0f };
		inline const cColor light_blue     = { 0.5f, 0.5f, 1.0f, 1.0f };
		inline const cColor sky_blue       = { 0.5f, 0.75f, 1.0f, 1.0f };
		inline const cColor dark_sky_blue  = { 0.3f, 0.5f, 0.75f, 1.0f };
		inline const cColor light_sky_blue = { 0.7f, 0.85f, 1.0f, 1.0f };

		inline const cColor dark_purple  = { 0.3f, 0.0f, 0.3f, 1.0f };
		inline const cColor purple       = { 0.5f, 0.0f, 0.5f, 1.0f };
		inline const cColor light_purple = { 0.7f, 0.3f, 0.7f, 1.0f };

		inline const cColor dark_magenta  = { 0.7f, 0.0f, 0.7f, 1.0f };
		inline const cColor magenta       = { 1.0f, 0.0f, 1.0f, 1.0f };
		inline const cColor light_magenta = { 1.0f, 0.5f, 1.0f, 1.0f };
	}
}
