//
//  gctree.hpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-18.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#ifndef geocached_gctree_hpp
#define geocached_gctree_hpp

#include "share.hpp"
#include "type.hpp"

namespace libgeocached {
    
    #define GCNODE_SUB_REGION_COUNT 4
    
    /**
      default using 20 bits for lat and lng
      corresponding to 8 character geohash length using base-32 representation
     **/
    #define GCNODE_MAX_GEOHASH_BIT 20
    #define GCNODE_DEFAULT_GEOHASH_LENGTH 8 

    typedef std::function<void(GCGeoHash geohash)> TraverseFunc;
    
    /**
     A single geohash node representing a geo rect area
     it has maximum 4 childs representing spliting its rect into four parts
     by adding 1 additional bit to its lat and long
     
     **/
    struct GCNode {
        GCNode* sub[GCNODE_SUB_REGION_COUNT]; //four sub region arranged as 00 (south west), 01 (south east), 10 (north west), 11 (north east) 
        int     bits; //number of bits for both lat and lng at this level
        long    latitude; //binary latitude geohash in decimal form (max 64 bit)
        long    longitude;//binary longitude geohash in decimal form (max 64 bit)
        
        GCNode(int b, long lat, long lng) : bits(b), latitude(lat), longitude(lng){
            for(int i=0;i<GCNODE_SUB_REGION_COUNT;i++) sub[i] = NULL;
        }
        
        bool empty(){
            for(int i=0;i<GCNODE_SUB_REGION_COUNT;i++)
                if(sub[i]!=NULL) return false;
            return true;
        }
    };
    
    
    
    /**
      A geohash tree structure
     **/
    class GCTree {
    
    public:
        //query for all geohash rects fall within the circle
        std::vector<GCGeoHash> nodes_in_circle(GCDegree center, GCDistance radius);
        
    public:
        //given a geohash region in base-32 string representation, insert it into the gc tree
        bool insert(const GCGeoHash& geohash);
        //query if a a given geohash entry exists in tree
        bool exists(const GCGeoHash& geohash);
        //remove a geohash entry
        bool remove(const GCGeoHash& geohash);
        //iterate over all present geohash at a specific level
        void traverse(int target_bits, TraverseFunc func);
        //check emptynes
        bool empty()const{ return m_root->empty();}
        //cleanup
        void cleanup(){ _recursive_cleanup(m_root); }
    public:
        GCTree();
        virtual ~GCTree();
        
    private:
        
        //encode & decode
        void _decode_geohash(const GCGeoHash& geohash, long& lat, long& lng, int& bits) const;
        GCGeoHash _encode_geohash(long lat, long lng, int bits) const;
        
        //tree operations 
        bool _recursive_remove(GCNode*& present, const long& lat, const long& lng, const int& bits);
        void _recursive_traverse(GCNode* present, TraverseFunc& func, int target_bits);
        GCNode* _recursive_search(GCNode*&present, const long& lat, const long& lng, const int& bits);
        bool _recursive_insert(GCNode*&present, const long& lat, const long& lng, const int& bits);
        void _recursive_cleanup(GCNode*& parent);
        
        //root node represent the entire geo range from [-90, 90) and [-180, 180)
        GCNode* m_root; 
    };
    
    
}

#endif
