## libgeocached 
A fast in-memory geospatial index and query library in C++. The library supports the following

* indexing of arbitray object types with geolocation tag via C++ template
* efficient [geohash]<https://en.wikipedia.org/wiki/Geohash>-based spatial partitioning and search tree impplementation for geospatial query
* optional persistency support for NoSQL library including [Redis]<http://redis.io/> and [MongoDB]<http://www.mongodb.org/>



### Usage
To create a new geocache matrix, do 
```cpp
#include <matrix.hpp>

libgeocached::Matrix<DataObject> matrix; //DataObject can be any type
```

To add an object into the matrix and associate it with an id and geolocaiton tag:

```cpp
//data insertion
ObjectID id1 = ObjectIDNew();
DataObject obj("dummy");
GCLocation location =  GCLocationMake(23.23234, -123.34324);
matrix.insert(id1, obj, location);
```

To query for all objects within a circular geographical area 
```cpp
#include <vector>
...
//define a circle area with center lat/lng and 1000 meter radius
GCCircle circle = GCCircleMake(GCLocationMake(23.23234, -123.34324), 1000);
std::vector<DataObject> objs;

//circular query
matrix.objs_in_circle(circle, objs);

for(DataObject& obj : objs)
    cout << obj << endl;
```


### GCTree 
GCTree is a geohash-based search tree implementation for efficient indexing of rectanglar geospatial regions. 



## Performance
[TBD]

## Road Map
[TBD]

## Dependencies

* Geographiclib <http://geographiclib.sourceforge.net/>
* libgeohash <https://github.com/lyokato/libgeohash>
* libspatialindex <http://libspatialindex.github.io/>
* RTree <http://superliminal.com/sources/sources.htm#C%20&%20C++%20Code>


## Author 
Denny C. Dai <dennycd@me.com> or visit <http://dennycd.me>

## License 
The MIT License (MIT) 
<http://opensource.org/licenses/MIT>
