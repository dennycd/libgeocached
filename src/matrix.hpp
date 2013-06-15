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

namespace geocached{

    
    /**
        A rectangular global matrix 
        Object - indexed data object type
     **/
    template<class Object>
    class Matrix {
    public:
        Matrix();
        virtual ~Matrix();
    
    private:
        
    };
    
    
    
    
    
    template<class Object>
    Matrix<Object>::Matrix(){
        
    }
    
    template<class Object>
    Matrix<Object>::~Matrix(){
        
    }
    
    
    
}

#endif
