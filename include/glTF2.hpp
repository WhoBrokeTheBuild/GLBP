#pragma once

#include <string>
#include <vector>

namespace glTF2 {

std::vector<Mesh::Primitive> LoadPrimitivesFromFile(const std::string& filename);

}