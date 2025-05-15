#pragma once

#define DF_DeleteCopy( T )             \
	T( const T& )            = delete; \
	T& operator=( const T& ) = delete;

#define DF_DeleteMove( T )        \
	T( T&& )            = delete; \
	T& operator=( T&& ) = delete;

#define DF_DeleteCopyAndMove( T ) \
	DF_DeleteCopy( T );           \
	DF_DeleteMove( T );

#define DF_DefaultCopy( T )             \
	T( const T& )            = default; \
	T& operator=( const T& ) = default;

#define DF_DefaultMove( T )        \
	T( T&& )            = default; \
	T& operator=( T&& ) = default;

#define DF_DefaultCopyAndMove( T ) \
	DF_DefaultCopy( T );           \
	DF_DefaultMove( T );
