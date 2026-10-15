#include "resources/ResourceManager.hpp"

#include "resources/MeshFactory.hpp"
#include "resources/Shader.hpp"
#include "resources/Material.hpp"

#include <filesystem>

void ResourceManager::initializeBuiltinResources(){

    // Shaders
    auto whiteShader = std::make_shared<Shader>("builtin:white",
        std::filesystem::path::path("builtin_resources/shaders/unlit.vert").string(),
        std::filesystem::path::path("builtin_resources/shaders/white.frag").string());
    this->add(whiteShader);

    auto unlitShader = std::make_shared<Shader>("builtin:unlit",
        std::filesystem::path::path("builtin_resources/shaders/unlit.vert").string(),
        std::filesystem::path::path("builtin_resources/shaders/unlit.frag").string());
    this->add(unlitShader);
            
    auto litShader = std::make_shared<Shader>("builtin:lit",
        std::filesystem::path::path("builtin_resources/shaders/lit.vert").string(),
        std::filesystem::path::path("builtin_resources/shaders/lit.frag").string());
    this->add(litShader);

    // Materials
    auto defaultMat = std::make_shared<Material>("builtin:defaultMat", litShader);
    this->add(defaultMat);
    defaultMat->set("baseColor", MaterialPropertyValue{glm::vec3{1,1,1}});
    defaultMat->set("ambient", MaterialPropertyValue{0.25f});
    defaultMat->set("lightDirection", MaterialPropertyValue{glm::vec3{-0.25f,-1,-0.65f}});
    auto unlitMat = std::make_shared<Material>("builtin:unlitMat", unlitShader);
    this->add(unlitMat);

    // Meshes
    Mesh eqTriangle = MeshFactory::eqTriangle(1);
    Mesh quad = MeshFactory::quad(1);
    Mesh cube = MeshFactory::cube(1);
    Mesh cylinder = MeshFactory::cylinder("builtin:cylinder", 1, 0.5f, 16);
    Mesh cone = MeshFactory::cone("builtin:cone", 1, 0.5f, 24);
    Mesh pyramid3 = MeshFactory::cone("builtin:3pyramid", 1, 0.5f, 3);
    Mesh pyramid4 = MeshFactory::cone("builtin:4pyramid", 1, 0.5f, 4);
    Mesh torus = MeshFactory::torus("builtin:torus", 0.8f, 0.2f, 32, 16);

    this->add(std::make_shared<Mesh>(std::move(eqTriangle)));
    this->add(std::make_shared<Mesh>(std::move(quad)));
    this->add(std::make_shared<Mesh>(std::move(cube)));
    this->add(std::make_shared<Mesh>(std::move(cylinder)));
    this->add(std::make_shared<Mesh>(std::move(cone)));
    this->add(std::make_shared<Mesh>(std::move(pyramid3)));
    this->add(std::make_shared<Mesh>(std::move(pyramid4)));
    this->add(std::make_shared<Mesh>(std::move(torus)));

}