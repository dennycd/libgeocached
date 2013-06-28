//
//  gctree.cpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-21.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#include <gctree.hpp>
#include <GeographicLib/Geodesic.hpp>
#include <geohash.h>
#include <stack>
#include <geolib.h>

using namespace std;
using namespace GeographicLib;

namespace libgeocached{

static const char BASE32_ENCODE_TABLE[33] = "0123456789bcdefghjkmnpqrstuvwxyz";
static const char BASE32_DECODE_TABLE[44] = {
    /* 0 */   0, /* 1 */   1, /* 2 */   2, /* 3 */   3, /* 4 */   4,
    /* 5 */   5, /* 6 */   6, /* 7 */   7, /* 8 */   8, /* 9 */   9,
    /* : */  -1, /* ; */  -1, /* < */  -1, /* = */  -1, /* > */  -1,
    /* ? */  -1, /* @ */  -1, /* A */  -1, /* B */  10, /* C */  11,
    /* D */  12, /* E */  13, /* F */  14, /* G */  15, /* H */  16,
    /* I */  -1, /* J */  17, /* K */  18, /* L */  -1, /* M */  19,
    /* N */  20, /* O */  -1, /* P */  21, /* Q */  22, /* R */  23,
    /* S */  24, /* T */  25, /* U */  26, /* V */  27, /* W */  28,
    /* X */  29, /* Y */  30, /* Z */  31
};

/**
  collect all leafs fall within the circle
 **/
void GCTree::nodes_in_circle(GCCircle circle, std::vector<GCGeoHash>& nodes){
    
    stack<GCNode*> stack; //for recursive descend
    stack.push(m_root);
    
    while (!stack.empty()) {
        
        //the node must have been overlapping with the circle 
        GCNode* node = stack.top();
        stack.pop();
        
        //leaf node
        if(node != m_root && node->empty())
            nodes.push_back(_encode_geohash(node->latitude, node->longitude, node->bits));
        //internal node
        else{
            for(int i=0;i<GCNODE_SUB_REGION_COUNT;i++){
                //ignore if not exist
                if(!node->sub[i]) continue;
                
                //only descend if the sub region overlaps with the circel
                if(GCCircleRectOverlap(circle, _encode_geohash(node->sub[i]->latitude, node->sub[i]->longitude, node->sub[i]->bits)))
                    stack.push(node->sub[i]);
            }
        }
    }
}


bool GCTree::_recursive_remove(GCNode*& present, const long& lat, const long& lng, const int& bits){
    if(!present) return false; 
    
    //reaching target 
    if(present->bits == bits){
        assert(present->latitude == lat && present->longitude == lng);
        cout << "hitting target at bits " << bits << ":  lat = 0b" << std::bitset<16>(lat) << " lng = 0b" << std::bitset<16>(lng) << endl;
        
        //remove node, including any offspring
        _recursive_cleanup(present);
    
        return true;
    }
    else{
        
        int next_bit = present->bits + 1;
        char lat_bit = (lat >> (bits - next_bit)) & 0x01;
        char lng_bit = (lng >> (bits - next_bit)) & 0x01;
        
        //the next sub region it falles into
        int idx = (lat_bit << 1) + lng_bit;
        assert(idx >=0 && idx < GCNODE_SUB_REGION_COUNT);
        
        bool ret = _recursive_remove(present->sub[idx], lat, lng, bits);
        
        //if node itself becomes empty clean it up
        if(present->empty() && present!=m_root){
            delete present;
            present = NULL;
        }
        
        return ret;
    }
}

    
GCNode* GCTree::_recursive_search(GCNode*&present, const long& lat, const long& lng, const int& bits){
    if(!present) return NULL;
    
    if(present->bits == bits){
        assert(present->latitude == lat && present->longitude == lng);
        cout << "hitting leaf at bits " << bits << ":  lat = 0b" << std::bitset<16>(lat) << " lng = 0b" << std::bitset<16>(lng) << endl;
        return present;
    }
    else{
        int next_bit = present->bits + 1;
        char lat_bit = (lat >> (bits - next_bit)) & 0x01;
        char lng_bit = (lng >> (bits - next_bit)) & 0x01;
        
        //the next sub region it falles into
        int idx = (lat_bit << 1) + lng_bit;
        assert(idx >=0 && idx < GCNODE_SUB_REGION_COUNT);
        
        //descend further 
        return _recursive_search(present->sub[idx], lat, lng, bits);
    }
}

//given binary geohash encoding of lat / long if bits length, return a geohash code
GCGeoHash GCTree::_encode_geohash(long lat, long lng, int bits) const{
    
    long geohash = 0;
    int geohash_bits = 0;
    
    for(int i=0; i<bits; i++){
        geohash = ((geohash << 1) | ( (lng >> (bits -1 -i)) & 0x01 )); //lng into even
        geohash = ((geohash << 1) | ( (lat >> (bits -1 -i)) & 0x01 )); //lat into odd
        geohash_bits += 2;
    }
    
    //it shall be a valid base-32
    assert(geohash_bits % 5 == 0);
    
    GCGeoHash hash;
    for(int i=0;i<geohash_bits; i+=5){
        //map every 5 bit into base-32 str
        char val = ((geohash >> (geohash_bits-5-i)) & 0x1F);
        assert(val >=0 && val < 32);
        hash.push_back(BASE32_ENCODE_TABLE[val]);
    }
    return hash;
}


void GCTree::_decode_geohash(const GCGeoHash& geohash, long& lat, long& lng, int& bits) const
{
    size_t n = geohash.length();
    int lat_bit = 0, lng_bit = 0, total_bits = 0;
    
    //assert(n==GCNODE_DEFAULT_GEOHASH_LENGTH);
    
    //extract binary lat lng value from geohash
    lat = 0;
    lng = 0;
    
    //from high big to low
    for(int i=0;i<n;i++){
        
        //the base-32 char in 8-bit, clean up high bits
        char c = BASE32_DECODE_TABLE[ toupper(geohash[i]) - 0x30 ];
        assert(c!=-1);
        c = c & 0x1F;
        
        
        for(int j=0;j<5;j++)  //extract each bit on the char
        {
            //odd bit for latitude
            if(total_bits%2){
                lat = (lat << 1) | ((c >> (4-j)) & 0x01);  //take the jth bit and move into lat's lowest bit
                lat_bit++;
            }
            //even bit for longitude
            else{
                lng = (lng << 1) | ((c >> (4-j)) & 0x01);  //take the jth bit and move into lat's lowest bit
                lng_bit++;
            }
            
            total_bits++;
        }
    }
    //assert(lat_bit == GCNODE_MAX_GEOHASH_BIT && lng_bit == GCNODE_MAX_GEOHASH_BIT);
    assert(lat_bit == lng_bit);
    
    bits = lat_bit;
}
    
    
/**
 
  @bits - total bits in lat/lng
 **/
bool GCTree::_recursive_insert(GCNode*&present, const long& lat, const long& lng, const int& bits){
    assert(present);
    
    //hitting leaf
    if(present->bits == bits){
        assert(present->latitude == lat && present->longitude == lng);
        cout << "hitting leaf at bits " << bits << ":  lat = 0b" << std::bitset<16>(lat) << " lng = 0b" << std::bitset<16>(lng) << endl;
        return true;
    }
    
    int next_bit = present->bits + 1;
    char lat_bit = (lat >> (bits - next_bit)) & 0x01;
    char lng_bit = (lng >> (bits - next_bit)) & 0x01;

    //the next sub region it falles into
    int idx = (lat_bit << 1) + lng_bit;
    assert(idx >=0 && idx < GCNODE_SUB_REGION_COUNT);
    
    //if not yet exist, create it and assign proper value
    if(!present->sub[idx]){
        present->sub[idx] = new GCNode(next_bit, (lat >> (bits - next_bit)), (lng >> (bits-next_bit)));
    }
    return _recursive_insert(present->sub[idx], lat, lng, bits);
}

bool GCTree::remove(const GCGeoHash& geohash){
    long lat = 0, lng = 0;
    int bits = 0;
    _decode_geohash(geohash, lat, lng, bits);    
    return _recursive_remove(m_root, lat, lng, bits);
}
    
bool GCTree::exists(const GCGeoHash& geohash){
    long lat = 0, lng = 0;
    int bits = 0;
    _decode_geohash(geohash, lat, lng, bits);
    return _recursive_search(m_root, lat, lng, bits) != NULL;
}
    
/**
 
 **/
bool GCTree::insert(const GCGeoHash& geohash){

    long lat = 0, lng = 0;
    int bits = 0;
    _decode_geohash(geohash, lat, lng, bits);
    
    //now add to tree
    return _recursive_insert(m_root, lat, lng, bits);
}
    
void GCTree::traverse(int target_bits, TraverseFunc func){
    _recursive_traverse(m_root, func, target_bits);
}

void GCTree::_recursive_traverse(GCNode* present, TraverseFunc& func, int target_bits){
    if(!present) return;
    assert(target_bits >= present->bits);
    
    if(target_bits == present->bits){
        func(_encode_geohash(present->latitude, present->longitude, present->bits));
    }
    else{
        for(int i=0; i< GCNODE_SUB_REGION_COUNT; i++)
            _recursive_traverse(present->sub[i], func, target_bits);
    }
}


GCTree::GCTree(){
    m_root = new GCNode(0, -1, -1); //root node is empty
}

GCTree::~GCTree(){
    _recursive_cleanup(m_root);
    delete m_root;
}
    
void GCTree::_recursive_cleanup(GCNode*& parent){
    if(!parent) return;
    for(int i=0;i<GCNODE_SUB_REGION_COUNT;i++)
        _recursive_cleanup(parent->sub[i]);
    
    if(parent!=m_root){
        delete parent;
        parent = NULL;
    }
}
    
}
