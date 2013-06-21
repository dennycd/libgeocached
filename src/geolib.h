//
//  geolib.h
//  geocached
//
//  Created by Denny C. Dai on 2013-06-21.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#ifndef __geocached__geolib__
#define __geocached__geolib__

#include <type.hpp>

/**
  A collection of geographic utility functions
 **/
namespace libgeocached {
    
    
//true if a given location lies inside a rect
bool GCPointInRect(GCLocation point, GCRect rect);

//check if a circle corssed a given latitude line
bool GCCircleCrossLatitude(GCCircle circle, GCDegree latitude);

//check if a circle crossed a given longitude line
bool GCCircleCrossLongitude(GCCircle circle, GCDegree longitude);

//check if a circle overlap with a rect / geohash rect
bool GCCircleRectOverlap(GCCircle circle, GCRect rect);
bool GCCircleRectOverlap(GCCircle circle, GCGeoHash geohash);
    
}

#endif /* defined(__geocached__geolib__) */
