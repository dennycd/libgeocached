//
//  type.hpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-14.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#ifndef geocached_type_hpp
#define geocached_type_hpp

#include <cassert>
#include <cfloat>
#include <string>
#include <unordered_map>
#include <uuid/uuid.h>
#include <vector>


namespace libgeocached{

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
    
    inline GCLocation GCLocationMake(const GCDegree& lat, const GCDegree& lon){
        return {lat, lon};
    }
    
    /**
      describe a rectangle region using four boundary lat/lng lines
     **/
    typedef struct _GCRect{
        GCDegree lat_south;
        GCDegree lat_north;
        GCDegree lng_west;
        GCDegree lng_east;
    }GCRect;
    
    inline GCRect GCRectMake(GCDegree latSouth, GCDegree latNorth, GCDegree lngWest, GCDegree lngEast){
        assert(latSouth <= latNorth && lngWest <= lngEast);
        return {latSouth, latNorth, lngWest, lngEast};
    }
    
    typedef struct _GCCircle{
        GCLocation center;
        GCDistance radius;
    }GCCircle;
    
    inline GCCircle GCCircleMake(const GCLocation& loc, const GCDistance& dis){
        return {loc,dis};
    }
    
    //geohash datatype
    typedef std::string GCGeoHash;
    
    //binary data for a geohash
    typedef long GCGeoHashBinary;
    
    
    typedef std::string ObjectID;
    inline ObjectID ObjectIDNew(){
        uuid_t newid;
        uuid_generate(newid);
        uuid_string_t newidStr;
        uuid_unparse(newid, newidStr);
        return newidStr;
    }
    
    /**
      A line segment with two end points 
     **/
    typedef struct _GCSegment {
        GCLocation p1;
        GCLocation p2;
    }GCSegment;
    
    /**
      A convex polygon consists a set of coordinates ordered in clock wise direction
     **/
    typedef struct _GCPolygon{
        std::vector<GCLocation> points;
    }GCPolygon;
    
    
    
    #define GCHASHMAP std::unordered_map
}
#endif
