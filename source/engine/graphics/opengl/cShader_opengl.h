#pragma once

#include <string>

#include "engine/core/math/cMatrix.h"
#include "engine/core/math/cVector.h"
#include "engine/core/utils/cColor.h"
#include "engine/core/utils/Misc.h"

namespace df::opengl
{
	class cShader_opengl
	{
	public:
		DF_DeleteCopy( cShader_opengl );
		DF_DefaultMove( cShader_opengl );

		cShader_opengl();
		cShader_opengl( const std::string& _name, uint32_t _program );
		cShader_opengl( const std::string& _name );
		~cShader_opengl();

		void use() const;

		void setBool( const std::string& _name, bool _value ) const;
		void setUnsignedInt( const std::string& _name, unsigned _value ) const;
		void setInt( const std::string& _name, int _value ) const;
		void setFloat( const std::string& _name, float _value ) const;

		void setFloatVector4( const std::string& _name, const cVector4f& _vector ) const;
		void setFloatVector4( const std::string& _name, int _size, const float* _value ) const;

		void setFloatColor( const std::string& _name, const cColor& _color ) const;

		void setFloatMatrix4( const std::string& _name, const cMatrix4f& _matrix, int _amount = 1, bool _transpose = false ) const;
		void setFloatMatrix4( const std::string& _name, int _size, const float* _value, bool _transpose = false ) const;

		void setUniformSampler( const std::string& _name, const int _sampler ) const { setInt( _name, _sampler ); }

	private:
		void createProgram( unsigned _vertex_shader, unsigned _fragment_shader );

		static unsigned compileShader( const std::string& _name, int _type );

		unsigned    m_program;
		std::string m_name;
	};
}
