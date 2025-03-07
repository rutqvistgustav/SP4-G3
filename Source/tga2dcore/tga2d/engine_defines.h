﻿#pragma once

#define USE_VIDEO // If this is defined, you can play a video by using CVideo

// #define USE_NOISE // If this is defined, a texture with perlin noise will be avalible in all shaders to play with
// #define USE_LIGHTS // If this is defined, the engine will calculate per pixel lightning using CLight

#define CAN_USE_DDS_NOT_POWER_OF_TWO true
#define CAN_USE_OTHER_FORMATS_THAN_DDS true 

#define FONT_BUFF_SIZE 1024
#define MAX_TEXT_SPRITES 1024

#ifndef MAX_POINTS_IN_CUSTOM_SHAPE
#define MAX_POINTS_IN_CUSTOM_SHAPE  69984 
#endif

#define NUMBER_OF_LIGHTS_ALLOWED 8 

#define SPINE_MESH_VERTEX_COUNT_MAX 1000

/* If these are changed, your new vector class need the same operators and functions as below classes*/
#define VECTOR2F Tga2D::Vector2<float>
#define VECTOR2UI Tga2D::Vector2<unsigned int>
#define VECTOR2I Tga2D::Vector2<int>
#define VECTORFILE <tga2d/math/vector2imp.h>

#define MAX_ANIMATION_BONES 64 //Want to increase this numer? Talk to Stefan Ek first please!

// 1 is the baked framerate from maya, if you want to slow down the animation, you might need more precision. Be aware, loadingtimes and memory will suffer if high
// Increase small numbers, try with 3 if you are not happy with "chugging" animations
#define ANIMATION_PRECISION 2