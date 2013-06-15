//
//  main.cpp
//  geocached
//
//  Created by Denny C. Dai on 2013-06-14.
//  Copyright (c) 2013 Denny C. Dai. All rights reserved.
//

#include <iostream>
#include <gtest/gtest.h>
#include <cell.hpp>
#include <matrix.hpp>
#include <geohash.h>

#include <uuid/uuid.h>

using namespace std;
using namespace geocached;

TEST(UUIDTest, test){
    uuid_t myid;
    uuid_generate(myid);
    uuid_string_t myidStr;
    uuid_unparse(myid, myidStr);
    cout << "uuid is " << myidStr << endl;
}

TEST(Cell, test){
    
//    Cell<int> mycell("testid", 0,0, );
    
    
}


TEST(GeoHash, test){
    char *hash = GEOHASH_encode(36.232, -123.324, 22);
    cout << "geohash: " << hash << endl;
    free(hash);
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

