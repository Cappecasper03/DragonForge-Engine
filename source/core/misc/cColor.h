#pragma once

namespace df
{
    class cColor
    {
    public:
        cColor( const float& _r, const float& _g, const float& _b, const float& _a );

        cColor operator+( const cColor& _other ) const { return { r + _other.r, g + _other.g, b + _other.b, a + _other.a }; }
        cColor operator-( const cColor& _other ) const { return { r - _other.r, g - _other.g, b - _other.b, a - _other.a }; }
        cColor operator*( const cColor& _other ) const { return { r * _other.r, g * _other.g, b * _other.b, a * _other.a }; }
        cColor operator/( const cColor& _other ) const { return { r / _other.r, g / _other.g, b / _other.b, a / _other.a }; }
        cColor operator+( const float& _scalar ) const { return { r + _scalar, g + _scalar, b + _scalar, a + _scalar }; }
        cColor operator-( const float& _scalar ) const { return { r - _scalar, g - _scalar, b - _scalar, a - _scalar }; }
        cColor operator*( const float& _scalar ) const { return { r * _scalar, g * _scalar, b * _scalar, a * _scalar }; }
        cColor operator/( const float& _scalar ) const { return { r / _scalar, g / _scalar, b / _scalar, a / _scalar }; }

        cColor& operator+=( const cColor& _other ) { return *this = { r + _other.r, g + _other.g, b + _other.b, a + _other.a }; }
        cColor& operator-=( const cColor& _other ) { return *this = { r - _other.r, g - _other.g, b - _other.b, a - _other.a }; }
        cColor& operator*=( const cColor& _other ) { return *this = { r * _other.r, g * _other.g, b * _other.b, a * _other.a }; }
        cColor& operator/=( const cColor& _other ) { return *this = { r / _other.r, g / _other.g, b / _other.b, a / _other.a }; }
        cColor& operator+=( const float& _scalar ) { return *this = { r + _scalar, g + _scalar, b + _scalar, a + _scalar }; }
        cColor& operator-=( const float& _scalar ) { return *this = { r - _scalar, g - _scalar, b - _scalar, a - _scalar }; }
        cColor& operator*=( const float& _scalar ) { return *this = { r * _scalar, g * _scalar, b * _scalar, a * _scalar }; }
        cColor& operator/=( const float& _scalar ) { return *this = { r / _scalar, g / _scalar, b / _scalar, a / _scalar }; }

        float r;
        float g;
        float b;
        float a;
    };

    namespace color
    {
        constexpr cColor red     = { 1.0f, 0.0f, 0.0f, 1.0f };
        constexpr cColor green   = { 0.0f, 1.0f, 0.0f, 1.0f };
        constexpr cColor blue    = { 0.0f, 0.0f, 1.0f, 1.0f };
        constexpr cColor black   = { 0.0f, 0.0f, 0.0f, 1.0f };
        constexpr cColor white   = { 1.0f, 1.0f, 1.0f, 1.0f };
        constexpr cColor yellow  = { 1.0f, 1.0f, 0.0f, 1.0f };
        constexpr cColor magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
        constexpr cColor cyan    = { 0.0f, 1.0f, 1.0f, 1.0f };
        constexpr cColor orange  = { 1.0f, 0.6f, 0.0f, 1.0f };
        constexpr cColor purple  = { 0.5f, 0.0f, 0.5f, 1.0f };
        constexpr cColor pink    = { 1.0f, 0.8f, 0.8f, 1.0f };
        constexpr cColor brown   = { 0.6f, 0.2f, 0.2f, 1.0f };
        constexpr cColor teal    = { 0.0f, 0.5f, 0.5f, 1.0f };
        constexpr cColor gray    = { 0.5f, 0.5f, 0.5f, 1.0f };
    }
}
