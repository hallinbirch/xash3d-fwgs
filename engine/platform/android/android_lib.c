/*
android_lib.c - dynamic library code for Android OS
Copyright (C) 2018 Flying With Gauss

This program is free software: you can redistribute it and/sor modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#ifdef __ANDROID__

#include "common.h"
#include "library.h"
#include "filesystem.h"
#include "server.h"
#include "platform/android/android_lib.h"
#include "platform/android/dlsym_weak.h" // Android < 5.0

void *ANDROID_LoadLibrary( const char *dllname )
{
	char path[MAX_SYSPATH];
	const char *libdir[2];
	int i;
	void *pHandle;

	libdir[0] = getenv("XASH3D_GAMELIBDIR");
	libdir[1] = getenv("XASH3D_ENGLIBDIR");

	for( i = 0; i < 2; i++ )
	{
		Q_snprintf( path, MAX_SYSPATH, "%s/lib%s"POSTFIX"."OS_LIB_EXT, libdir[i], dllname );
		pHandle = dlopen( path, RTLD_LAZY );
		if( pHandle )
			return pHandle;

		COM_PushLibraryError( dlerror() );
	}

	// HACKHACK: keep old behaviour for compability
	pHandle = dlopen( dllname, RTLD_LAZY );
	if( pHandle )
		return pHandle;

	COM_PushLibraryError( dlerror() );
}

void *ANDROID_GetProcAddress( void *hInstance, const char *name )
{
	void *p = dlsym_weak( hInstance, name );
	
	if( p ) return p;
	
	return dlsym( hInstance, name );
}


#endif // __ANDROID__
