#ifndef _GEOMETRY_LOADER_H
#define _GEOMETRY_LOADER_H

#include <Geometry.h>

typedef std::vector<std::string> StringVector;


class GeometryLoader {
public:
    static size_t loadGeometry(std::string geometry_path, GeometrySharedPtrVector &loadedGeometries);

    static size_t loadGeometries(StringVector geometry_paths, GeometrySharedPtrVector &loadedGeometries);

private:
    static size_t loadObjFile(const std::string &filename, GeometrySharedPtrVector &geometries);
};

#endif