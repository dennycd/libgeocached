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
namespace libgeocached{
    //Object - indexed data object type
    template<class Object, class Key = std::string>
    class Cell {
    public:
        typedef std::function<void(Key key, Object& obj)> TraversalFunc;
        typedef std::unordered_map<Key, Object> ObjectMap;
        
    public:
        Cell(GCGeoHash hash);
        virtual ~Cell();
        
        size_t size() const{ return _space.size(); }
        void traverse(TraversalFunc func);
        bool insert(const Key& key, const Object& obj);
        bool remove(const Key& key);
        Object& obj(const Key& key); //access obj via key
        
        const GCGeoHash geohash() const{ return _geohash; }
        
    private:
        GCGeoHash   _geohash; //geo hash tag spec for the cell
        ObjectMap _space; //associative container maping the object via their unique id key
    };
    
    template<class O, class K>
    O& Cell<O,K>::obj(const K& key){
        if(_space.count(key)==0) throw std::exception();
        return _space[key];
    }

    template<class O, class K>
    bool Cell<O,K>::remove(const K& key)
    {
        if(_space.count(key)==0) return false;
        _space.erase(key);
        return true;
    }
    
    template<class O, class K>
    bool Cell<O,K>::insert(const K& key, const O& obj)
    {
        if(_space.count(key)!=0) return false;//key duplication
        _space[key] = obj; //maintaining a self copy of the stuff
        return true;
    }

    //iterate through all objects
    template<class O, class K>
    void Cell<O,K>::traverse(TraversalFunc func){
        for(auto it = _space.begin(); it!=_space.end(); it++){
            func(it->first, it->second);
        }
    }


    template<class O, class K>
    Cell<O,K>::Cell(GCGeoHash hash){
        _geohash = hash;
    }
    
    //clean up: make sure we get rid of all heap allocation if any
    template<class O, class K>
    Cell<O,K>::~Cell(){
    }
}


#endif
