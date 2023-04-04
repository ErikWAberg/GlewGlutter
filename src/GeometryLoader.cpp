#include <GeometryLoader.h>
#include <vr/FileSystem.h>
#include <string>
#include <vr/AC3DLoader.h>
#include <unistd.h>
size_t GeometryLoader::loadObjFile(const std::string &filename, GeometrySharedPtrVector &geometries) {
    std::string filepath = filename;
     /*
    if(filepath.substr(filepath.find_last_of(".") + 1) == "3ds" || filepath.substr(filepath.find_last_of(".") + 1) == "3DS") {
        std::cout << "Reading AC3D " << filepath << std::endl;
        vr::AC3DLoader loader; 

        bool success = loader.load(filepath.c_str());
        if(success) {
            std::cout << "hej"<< std::endl;
        }
        std::cout << "Successfully loaded AC3D model: " << success << std::endl;
        return 0;
    } */


    bool exist = vr::FileSystem::exists(filepath);

    if (!exist) {
        std::cerr << "The file " << filename << " does not exist" << std::endl;
        return 0;
    }

    // We will try to find the material file in the same directory as the obj file
    std::string path = vr::FileSystem::getDirectory(filepath);
    if (path.size() > 0)
        path = path + "/";

    vr::shape_t_vector shapes;
    vr::material_t_vector materials;

    // Now load the obj file and collect shapes and materials
    std::string err = vr::LoadObj(shapes, materials, filepath.c_str(), path.c_str());

    // Nothing found, something went wrong
    if (!err.empty()) {
        std::cerr << "Error: " << err << std::endl;
        return 0;
    }
   // std::cout << "Nr of Materials: " << materials.size() << std::endl;
    size_t numRead = 0;
    // Go through each shape and create a Geometry that we can render
    for (vr::shape_t_vector::iterator it = shapes.begin(); it != shapes.end(); ++it) {
        vr::shape_t &shape = *it;
        std::shared_ptr<Geometry> geom(new Geometry);
        geom->setVertices(shape.mesh.positions);
        geom->setIndices(shape.mesh.indices);
        geom->setNormals(shape.mesh.normals);
        
        geom->setTexCoords(shape.mesh.texcoords);


        geometries.push_back(geom);
       // std::cout << "Shape name: " << shape.name << std::endl;

        // Do we have a material for this mesh?
        if (!materials.empty() && !shape.mesh.material_ids.empty()) {
            vr::material_t material = materials[shape.mesh.material_ids[0]];
            //   std::cout << "'per-mesh material id' " << shape.mesh.material_ids[0] << std::endl;

            // Collect all the textures if available
            if (!material.ambient_texname.empty()) {
              //  material.ambient_texname = path + material.ambient_texname;
              //  std::cout << "Ambient name: " << material.ambient_texname << std::endl;
                geom->State::addTexture((path + material.ambient_texname).c_str());
            }
            if (!material.diffuse_texname.empty()) {
               // material.diffuse_texname = path + material.diffuse_texname;
              //  std::cout << "Diffuse name: " << material.diffuse_texname << std::endl;
              //  geom->State::addTexture(("../models/Sandtrooper/" + material.diffuse_texname).c_str());
                geom->State::addTexture((path + material.diffuse_texname).c_str());    
                
            }

            if (!material.specular_texname.empty()) {
              //  material.specular_texname = path+ material.specular_texname;
             //   std::cout << "Specular name: " << material.specular_texname << std::endl;
                geom->State::addTexture((path + material.specular_texname).c_str());
            }

            if (!material.bump_texname.empty()) {
                //material.bump_texname = path + material.bump_texname;
               // std::cout << "Bump name: " << material.bump_texname << std::endl;
                geom->State::addTexture((path + material.bump_texname).c_str());
            }

            geom->setMaterial(material);
        }

        numRead++;

    }
  //  std::cout << "NUm read " << numRead << std::endl;

    return numRead;
}


size_t GeometryLoader::loadGeometry(std::string geometry_path, GeometrySharedPtrVector &loadedGeometries) {
    size_t numRead = 0;

    std::cout << "--- Loading geometry: " << " ---" << std::endl;
    std::cout << geometry_path << std::endl;

    numRead = GeometryLoader::loadObjFile(geometry_path, loadedGeometries);

    if (!numRead)
        std::cerr << "Error loading model: " << geometry_path << std::endl;
    else
        //std::cout << "Geometries loaded from object: " << numRead << std::endl;

    return numRead;
}

size_t GeometryLoader::loadGeometries(StringVector geometry_paths, GeometrySharedPtrVector &loadedGeometries) {
    //size_t num = loadedGeometries.size();
    size_t numRead = 0;
    for (StringVector::const_iterator it = geometry_paths.begin(); it != geometry_paths.end(); ++it) {
        numRead += GeometryLoader::loadGeometry(*it, loadedGeometries);

    }

    // g_scene.ballController.setTranslateScale(g_scene.getMaxDistance()*0.3);
    return numRead;
}