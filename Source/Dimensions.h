/****************************//**
 *	@file Dimensions.h
 *	@author Isaac Bries
 *	@date Created:	8/19/2018
 *	@date Edited:	8/19/2018
 *******************************/

#pragma once

/****************************************************************//**
 *	@class Dimensions
 *
 *	@brief Holds (mostly graphics) constants needed by
 *		   the application
 *
 *	Contains constants that define the sizes
 *	of graphic elements. Each source file that needs access to
 *	these constants should have the line "#include "Dimensions.h"
 *	in its respective header file.
 *******************************************************************/
class Dimensions
{
public:
	static const int BORDER = 10;				///< Width of border surrounding all subcomponents, in pixels
	static const int VOLUME_SLIDER_WIDTH = 40;	///< Width of local volume slider, in pixels
	static const int MASTER_FADER_WIDTH = 40;	///< Width of master volume slider, in pixels
};
