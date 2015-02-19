#ifndef included_GameCommon
#define included_GameCommon
#pragma once

//-----------------------------------------------------------------------------------------------
const float pi = 3.1415926535897932384626433832795f;
const float TWO_PI = 2.f * pi;
const float ONE_OVER_PI = 1.f / pi;
const float MARGIN_OF_ERROR = 0.001f;
const float ONE_OVER_SIX = 1.f / 6.f;
const float TWO_OVER_SIX = 2.f / 6.f;
const float FOUR_OVER_SIX = 4.f / 6.f;
const float FIVE_OVER_SIX = 5.f / 6.f;
const float SIXTEEN_BY_NINE = 16.f / 9.f;
const float ONE_OVER_FIFTEEN = 1.f / 15.f;
const float ONE_OVER_SIXTEEN = 1.f / 16.f;
const float ONE_OVER_TWENTYFOUR = ( 1.f / 24.f );
const float ONE_OVER_ONE_HUNDRED_EIGHTY = 1.f / 180.f;
const double FRAME_TIME_SECONDS = ( 1.0 / 60.0 );


//-----------------------------------------------------------------------------------------------
class World;
extern World g_world;


#endif // included_GameCommon