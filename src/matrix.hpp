//
//  matrix.hpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-14.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#ifndef geocached_matrix_hpp
#define geocached_matrix_hpp

#include <cell.hpp>
#include <geohash.h>
#include <gctree.hpp>

namespace libgeocached{

    
    //REFERENCE https://en.wikipedia.org/wiki/Geohash
    typedef enum {
        GCMatrixResolutionSparse = 4,  // +/- 20 km
        GCMatrixResolutionLow = 5, // +/- 2.4 km
        GCMatrixResolutionNormal = 6, // +/- 610 meters
        GCMatrixResolutionHigh = 7, // +/- 76 meters
        GCMatrixResolutionMax = 8 // +/- 19 meters  => roughly 38 meter span for a single cell
    }GCMatrixResolution;
    
    
    /**
        A rectangular global matrix 
        Object - indexed data object type
        Key - object key 
     **/
    template<class Object, class Key = std::string>
    class Matrix {
        
    public:
        
        
        
    public:
        typedef Cell<Object, Key>* CellPtr;
        typedef GCHASHMAP<GCGeoHash, CellPtr> CellMap;
        typedef std::function<void(Key key, Object& obj)> TraversalFunc;
        
    public:
        Matrix(GCMatrixResolution resol = GCMatrixResolutionNormal);
        virtual ~Matrix();
        
        //add a new obj with location tag
        bool insert(const Key& key, const Object& obj, const GCLocation& location);
        
        //check object existence with object key
        bool exists(const Key& key) const;
        
        //remove an object from matrix
        bool remove(const Key& key);
        
        //retrieve via id
        bool retrieve(const Key& key, Object& obj);
        
        //total number of holding objects in matrix
        size_t size() const;
        
        //inform object's location update
        bool update_location(const Key& key, const GCLocation& location);
        
        /**
          traversal through all contained objects
         **/
        void traverse(TraversalFunc func); //traverse all objects - debug only - 

    
    private:
        CellMap _map; //sparse map holding global cells: mapping from geohash to cell
        GCMatrixResolution _resol; //current resolution level
        GCTree _gctree; //geospatial tree indexing the cells
        
        //internal auxiliary indexing 
        GCHASHMAP<Key, CellPtr>  _indexObjCell; //object key to cell index
        
    protected:
        
        //query for all active cells (containing objects) within a circle area
        std::vector<CellPtr> _cellsInCircule(GCLocation center, GCDistance radius){
            
        }
        

        //destroy cell if cell contains no object
        void _cleanupCell(CellPtr cell){
            if(!cell) return;
            if(cell->size()==0){
                GCGeoHash geohash = cell->geohash();
                std::cout << "CELL REMOVED ==> " << geohash << std::endl;

                //remove from map
                _map.erase(geohash);
            
                //erate record from gctree
                _gctree.remove(geohash);

                delete cell;
                cell = NULL;
            }
        }
        
        //given a coordinate, return the cell encompassing it. create if not exist
        CellPtr _cellForLocation(GCDegree lat, GCDegree lng){
            char *chash = GEOHASH_encode(lat, lng, _resol);
            GCGeoHash hashkey = chash;
            free(chash);

            CellPtr pcell = NULL;
            
            auto it = _map.find(hashkey); //O(1) find
            if(it != _map.end())
                pcell = it->second;
            else{
                pcell = new Cell<Object,Key>(hashkey);
                if(!pcell) throw std::exception();
                
                //add to hash index
                _map[hashkey] = pcell;   // O(1) insertion
                
                //add to gctree index
                _gctree.insert(hashkey);
                
                std::cout << "CELL CREATED ==> " << hashkey << std::endl;
            }
                
            return pcell;
        }
        
    };
    
    template<class Object, class Key>
    bool Matrix<Object, Key>::retrieve(const Key& key, Object& obj){
        if(_indexObjCell.count(key)==0) return false;
        CellPtr cell = _indexObjCell[key];
        obj = cell->obj(key);
        return true;
    }
    
    template<class Object, class Key>
    bool Matrix<Object, Key>::update_location(const Key& key, const GCLocation& location)
    {
        if(_indexObjCell.count(key)==0) return false;
        CellPtr preCell = _indexObjCell[key];
        CellPtr curCell = _cellForLocation(location.latitude, location.longitude);
        
        if(curCell != preCell){
            Object obj = preCell->obj(key); //copy obj to a stack
            this->remove(key);
            this->insert(key, obj, location); //re-add with new location
        }
        
        return true;
    }

    
    template<class Object, class Key>
    size_t Matrix<Object, Key>::size() const{
        size_t total = 0;
        for(auto it = _map.begin(); it!= _map.end(); it++){
            CellPtr pcell = it->second;
            total += pcell->size();
        }
        return total;
    }
    
    template<class Object, class Key>
    bool Matrix<Object, Key>::remove(const Key& key){
        if(_indexObjCell.count(key)==0) return false;
        CellPtr cell = _indexObjCell[key];
        if(cell->remove(key)){
            
            //erase object key index
            _indexObjCell.erase(key);
            
            //destroy cell if cell contains no object
            _cleanupCell(cell);
            
            return true;
        }
        else return false;
    }

    //existence via key
    template<class Object, class Key>
    bool Matrix<Object, Key>::exists(const Key& key) const{
        return _indexObjCell.count(key) > 0;
    }

    //insert
    template<class Object, class Key>
    bool Matrix<Object, Key>::insert(const Key& key, const Object& obj, const GCLocation& location){
        CellPtr cell = _cellForLocation(location.latitude, location.longitude);
        if(cell->insert(key, obj)){
            _indexObjCell[key] = cell;
            return true;
        }
        else
            return false;
    }
    
    
    template<class Object, class Key>
    void Matrix<Object, Key>::traverse(TraversalFunc func){
        for(auto it = _map.begin(); it!= _map.end(); it++){
            CellPtr pcell = it->second;
            pcell->traverse(func);
        }
    }

    template<class Object, class Key>
    Matrix<Object, Key>::Matrix(GCMatrixResolution resol){
        _resol = resol;
    }
    
    template<class Object, class Key>
    Matrix<Object, Key>::~Matrix(){
        
    }
    
}

#endif
