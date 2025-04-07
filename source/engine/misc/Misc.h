#pragma once

#define DF_DisableCopy( T )           \
	T( const T& )            = delete; \
	T& operator=( const T& ) = delete;

#define DF_DisableMove( T )      \
	T( T&& )            = delete; \
	T& operator=( T&& ) = delete;

#define DF_DisableCopyAndMove( T ) \
	DF_DisableCopy( T );             \
	DF_DisableMove( T );
