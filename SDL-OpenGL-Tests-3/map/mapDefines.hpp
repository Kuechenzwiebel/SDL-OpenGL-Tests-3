//
//  mapDefines.h
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 17.04.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef mapDefines_hpp
#define mapDefines_hpp

#include <GL/glew.h>

#define CHUNK_WIDTH 32
#define TRIANGLE_WIDTH 0.25f
#define INVERSE_TRIANGLE_WIDTH 4
#define INVERSE_CHUNK_TEXTURE_WIDTH 0.25f

#define VIEW_RANGE CHUNK_WIDTH * 3

#define CHUNK_SIDE_LENGHT (CHUNK_WIDTH * INVERSE_TRIANGLE_WIDTH)
#define CHUNK_SIDE_LENGHT_FLOAT (float)(CHUNK_SIDE_LENGHT)
#define CHUNK_ARRAY_SIZE (CHUNK_SIDE_LENGHT + 1) * (CHUNK_SIDE_LENGHT + 1)
#define MAP_INDICES_ARRAY_SIZE ((CHUNK_ARRAY_SIZE - 1 - CHUNK_SIDE_LENGHT) * 6)


#if CHUNK_ARRAY_SIZE >= 65535
#define MAP_INDICES_ARRAY_TYPE unsigned int
#define MAP_INDICES_ARRAY_GL_TYPE GL_UNSIGNED_INT
#else
#define MAP_INDICES_ARRAY_TYPE unsigned short
#define MAP_INDICES_ARRAY_GL_TYPE GL_UNSIGNED_SHORT
#endif

#endif /* mapDefines_hpp */
