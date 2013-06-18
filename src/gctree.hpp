//
//  gctree.hpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-18.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#ifndef geocached_gctree_hpp
#define geocached_gctree_hpp

#include "type.hpp"

namespace geocached {
    
    #define GCNODE_MAX_GEOHASH_BIT 20  //max number of bits used for lat/lng binary value

    /**
     A single geohash node representing a geo rect area
     it has maximum 4 childs representing spliting its rect into four parts
     by adding 1 additional bit to its lat and long
     
     **/
    struct GCNode {
        
        
        
        int     bits; //number of bits 
        long    latitude; //latitude geohash
        long    longitude;//longitude geohash
    };
    
    
    
    /**
      A geohash tree structure
     **/
    class GCTree {
    public:
        
        

        
    
    };
}

#endif
