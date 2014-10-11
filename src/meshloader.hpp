#ifndef MESHLOADER_HPP_INCLUDED
#define MESHLOADER_HPP_INCLUDED

#include <memory>
#include <string>

class WorldObject;

class MeshLoader {
public:
    static std::unique_ptr<WorldObject> load(std::string path);
    static bool load(std::string path, WorldObject & object);
};

#endif
