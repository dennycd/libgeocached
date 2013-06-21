//
//  main.cpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-14.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#include <iostream>
#include <gtest/gtest.h>
#include <geohash.h>
#include <uuid/uuid.h>

#include <cell.hpp>
#include <matrix.hpp>

#include <gctree.hpp>

using namespace std;
using namespace libgeocached;


class DataObject{
public:
    
    DataObject(){
    }
    
    DataObject(string d){
        data = d;
    }
    string data;
};


TEST(GCTree, test){
    
    GCTree tree;
    EXPECT_TRUE(tree.insert("ezs4"));
    EXPECT_TRUE(tree.insert("cbcb"));

    EXPECT_TRUE(tree.exists("ezs4"));
    EXPECT_TRUE(tree.exists("cbcb"));
    
    tree.traverse(10, [](GCGeoHash hash){
        cout << "HASH " << hash << endl;
    });
    
    EXPECT_TRUE( tree.remove("ezs4"));
    EXPECT_TRUE(!tree.remove("zzzz"));

    EXPECT_TRUE( tree.remove("cbcb"));
    EXPECT_TRUE(!tree.remove("cbcb"));
    
    EXPECT_TRUE(tree.empty());
}


TEST(UUIDTest, test){
    uuid_t myid;
    uuid_generate(myid);
    uuid_string_t myidStr;
    uuid_unparse(myid, myidStr);
    cout << "uuid is " << myidStr << endl;
}

TEST(Cell, test){
    
    Matrix<DataObject> matrix;
    
    
    //data insertion
    ObjectID id1 = ObjectIDNew();
    EXPECT_TRUE(matrix.insert(id1, DataObject("hello"), GCLocationMake(23.23234, -123.34324)));
    
    ObjectID id2 = ObjectIDNew();
    EXPECT_TRUE(matrix.insert(id2, DataObject("world"), GCLocationMake(34.1232, -23.34324)));
    
    ObjectID id3 = ObjectIDNew();
    EXPECT_TRUE(matrix.insert(id3, DataObject("denny"), GCLocationMake(77.2323, 123.34324)));

    //retrieval
    DataObject obj;
    EXPECT_TRUE(matrix.retrieve(id1, obj));
    EXPECT_TRUE(matrix.retrieve(id2, obj));
    EXPECT_TRUE(matrix.retrieve(id3, obj));

    //trversal
    matrix.traverse([](string key, DataObject& data){
       cout << "key - " << key <<  ",  data - " << data.data << endl;
    });


    //location upate
    EXPECT_TRUE(matrix.update_location(id1, {11.232323, 90.1312})); 
    EXPECT_TRUE(matrix.update_location(id2, {11.232323, 90.1312}));
    EXPECT_TRUE(matrix.update_location(id3, {11.232323, 90.1312}));

    matrix.traverse([](string key, DataObject& data){
        cout << "key - " << key <<  ",  data - " << data.data << endl;
    });
    
    
    //removal
    EXPECT_TRUE(matrix.remove(id1)); EXPECT_TRUE(matrix.size()==2);
    EXPECT_TRUE(matrix.remove(id2)); EXPECT_TRUE(matrix.size()==1);
    EXPECT_TRUE(matrix.remove(id3)); EXPECT_TRUE(matrix.size()==0);

    matrix.traverse([](string key, DataObject& data){
        cout << "key - " << key <<  ",  data - " << data.data << endl;
    });
    
    
}


TEST(GeoHash, test){
    char *hash = GEOHASH_encode(36.232, -123.324, 8);
    cout << "geohash: " << hash << endl;
    free(hash);
    
    {
        char *hash = GEOHASH_encode(36.232, -123.324, 7);
        cout << "geohash: " << hash << endl;
        free(hash);
        
    }
    
    
}

TEST(MatrixTest, test){
    EXPECT_TRUE(true);
}


int main(int argc, const char * argv[])
{
    #ifdef __cplusplus
    cout << "cplusplus defined!" << endl;
    #endif
    
    ::testing::InitGoogleTest(&argc, (char**)argv);
    return RUN_ALL_TESTS();
}

