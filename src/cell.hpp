//
//  cell.hpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-14.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#ifndef geocached_cell_hpp
#define geocached_cell_hpp

#include <share.hpp>
#include <type.hpp>
#include <geohash.h>

/**
 
 A rectangular geospatial cell indexes all objects fall within it 

 **/
namespace geocached{
    //Object - indexed data object type
    template<class Object, class Key = std::string>
    class Cell {
    public:
        Cell(std::string id, long latIdx, long longIdx, GCGeoHash hash);
        virtual ~Cell();
        
        
    private:
        std::string _id; //unique cell id
        long        _latIdx; //row index in the global matrix
        long        _longIdx; //column index in the global matrix
        
        GCGeoHash   _geohash; //geo hash tag spec for the cell
        GCRect      _georect; //numerical spec for the cell
        
        
        //associative container maping the object via their unique id key
        std::unordered_map<Key, Object> _space;
    };



    template<class O, class K>
    Cell<O,K>::Cell(std::string id, long latIdx, long longIdx, GCGeoHash hash){
        _id = id;
        _latIdx = latIdx;
        _longIdx = longIdx;
        _geohash = hash;
    }
    
    template<class O, class K>
    Cell<O,K>::~Cell(){
    }
}


#endif
