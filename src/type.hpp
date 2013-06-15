//
//  type.hpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-14.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#ifndef geocached_type_hpp
#define geocached_type_hpp

#include <cfloat>
#include <string>

namespace geocached{

    //geospatial distance in meters
    typedef double GCDistance; 
    const GCDistance GCDistanceMAX = DBL_MAX;
    
    //numerical earth degree and its default valid range
    typedef double GCDegree;
    const GCDegree GCDegreeLatitudeMin = -90.0f;
    const GCDegree GCDegreeLatitudeMax = 90.0f;
    const GCDegree GCDegreeLongitudeMin = -180.0f;
    const GCDegree GCDegreeLongitudeMax = 180.0f;
    
    //a minimum degree diference before considering two degree values being equal
    const GCDegree GCDegreeResolution = 0.000001f;
    
    
    typedef struct _GCLocation{
        GCDegree latitude;
        GCDegree longitude;
    }GCLocation;
    
    /**
      describe a rectangle region using center coordinate
      and lat/long degree span 
     **/
    typedef struct _GCRect{
        GCLocation center;
        GCDegree    latSpan;
        GCDegree    longSpan;
    }GCRect;
    
    //geohash datatype
    typedef std::string GCGeoHash;
    
}
#endif
