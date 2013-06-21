//
//  geolib.cpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-21.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#include <geolib.h>
#include <cstdlib>
#include <GeographicLib/Geodesic.hpp>
#include <geohash.h>

using namespace GeographicLib;
using namespace std;

namespace libgeocached {
    
    
    bool GCCircleRectOverlap(GCCircle circle, GCGeoHash geohash)
    {
        GEOHASH_area* area = GEOHASH_decode(geohash.c_str());
        GCRect rect = GCRectMake(area->latitude.min, area->latitude.max, area->longitude.min, area->longitude.max);
        free(area);
        return GCCircleRectOverlap(circle, rect);
    }
    
    
    /**
       overlap is any of the following true 
     
       - circle centre within rect
       - circle cross any of the rect edge lines 
     
     **/
    bool GCCircleRectOverlap(GCCircle circle, GCRect rect)
    {
        return GCPointInRect(circle.center, rect) ||
                GCCircleCrossLatitude(circle, rect.lat_north) ||
                GCCircleCrossLatitude(circle, rect.lat_south) ||
                GCCircleCrossLongitude(circle, rect.lng_west) ||
                GCCircleCrossLongitude(circle, rect.lng_east);
    }
    
    bool GCCircleCrossLongitude(GCCircle circle, GCDegree longitude)
    {
        double heading = circle.center.longitude < longitude ? 90 : 270;
        double proj_lat, proj_lon;
        Geodesic::WGS84.Direct(circle.center.latitude, circle.center.longitude, heading, circle.radius, proj_lat, proj_lon);
        return (circle.center.longitude - longitude)*(proj_lon - longitude) < 0;
    }
    

    //follow a geodesic from circle center by radius distance, then
    //see if the end point passed the latitude 
    bool GCCircleCrossLatitude(GCCircle circle, GCDegree latitude)
    {
        double heading =  circle.center.latitude < latitude ? 0 : 180;
        double proj_lat, proj_lon;
        Geodesic::WGS84.Direct(circle.center.latitude, circle.center.longitude, heading, circle.radius, proj_lat, proj_lon);
        return (circle.center.latitude - latitude) * (proj_lat - latitude) < 0;
    }
    
    bool GCPointInRect(GCLocation point, GCRect rect){
        return (point.latitude <= rect.lat_north) &&
                (point.latitude >= rect.lat_south) &&
                (point.longitude >= rect.lng_west) &&
                (point.longitude <= rect.lng_east);
    }
    
    
    
}