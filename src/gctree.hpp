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
    
    
    
    
    /**
      A geohash tree structure
     **/
    class GCTree {
    public:
        
        
        /**
          A single geohash node representing a geo rect area
          it has maximu 4 childs representing spliting its rect into four parts
          by adding 1 additional bit to its lat and long
         
         **/
        struct GCNode {
            
        };
        
        
    
    };
}

#endif
