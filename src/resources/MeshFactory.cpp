#include "resources/MeshFactory.hpp"

#include <numbers>

namespace MeshFactory{

    Mesh eqTriangle(float side){
        auto halfSide = side/2;
        auto halfHeight = 0.866f/2.0f * side;

        std::vector<Vertex> vertices = {
            Vertex{{-halfSide, -halfHeight, 0.0f}, {0, 0, 1}, {0, 0}},
            Vertex{{0.0f,       halfHeight, 0.0f}, {0, 0, 1}, {0.5f, 1}},
            Vertex{{halfSide,  -halfHeight, 0.0f}, {0, 0, 1}, {1, 0}},
        };
        std::vector<std::uint32_t> indices = { 2, 1, 0 };
        
        Mesh mesh("builtin:eqTriangle", vertices, indices);
        return mesh;
    }

    Mesh quad(float side){
        auto halfSide = side/2;

        std::vector<Vertex> vertices = {
            Vertex{{-halfSide, -halfSide, 0}, {0, 0, 1}, {0, 0}},
            Vertex{{-halfSide,  halfSide, 0}, {0, 0, 1}, {0, 1}},
            Vertex{{ halfSide,  halfSide, 0}, {0, 0, 1}, {1, 1}},
            Vertex{{ halfSide, -halfSide, 0}, {0, 0, 1}, {1, 0}},
        };
        std::vector<std::uint32_t> indices = { 2, 1, 0, 3, 2, 0 };

        Mesh mesh("builtin:quad", vertices, indices);        
        return mesh;
    }

    Mesh cube(float side){
        auto halfSide = side/2;

        // Duplicate vertices to allow for flat shading
        std::vector<Vertex> vertices = {
            // Front
            Vertex{{-halfSide, -halfSide,  halfSide}, {0, 0, 1}, {0, 0}},
            Vertex{{-halfSide,  halfSide,  halfSide}, {0, 0, 1}, {0, 1}},
            Vertex{{ halfSide,  halfSide,  halfSide}, {0, 0, 1}, {1, 1}},
            Vertex{{ halfSide, -halfSide,  halfSide}, {0, 0, 1}, {1, 0}},

            // Back
            Vertex{{-halfSide, -halfSide, -halfSide},  {0, 0, -1}, {0, 0}},
            Vertex{{-halfSide,  halfSide, -halfSide},  {0, 0, -1}, {0, 1}},
            Vertex{{ halfSide,  halfSide, -halfSide},  {0, 0, -1}, {1, 1}},
            Vertex{{ halfSide, -halfSide, -halfSide},  {0, 0, -1}, {1, 0}},

            // Top
            Vertex{{-halfSide, halfSide,   halfSide}, {0, 1, 0}, {0, 0}},
            Vertex{{-halfSide, halfSide,  -halfSide}, {0, 1, 0}, {0, 1}},
            Vertex{{ halfSide, halfSide,  -halfSide}, {0, 1, 0}, {1, 1}},
            Vertex{{ halfSide, halfSide,   halfSide}, {0, 1, 0}, {1, 0}},

            // Bottom
            Vertex{{-halfSide, -halfSide,  halfSide}, {0, -1, 0}, {0, 0}},
            Vertex{{-halfSide, -halfSide, -halfSide}, {0, -1, 0}, {0, 1}},
            Vertex{{ halfSide, -halfSide, -halfSide}, {0, -1, 0}, {1, 1}},
            Vertex{{ halfSide, -halfSide,  halfSide}, {0, -1, 0}, {1, 0}},

            // Left
            Vertex{{-halfSide, -halfSide, -halfSide}, {-1, 0, 0}, {0, 0}},
            Vertex{{-halfSide,  halfSide, -halfSide}, {-1, 0, 0}, {0, 1}},
            Vertex{{-halfSide,  halfSide,  halfSide}, {-1, 0, 0}, {1, 1}},
            Vertex{{-halfSide, -halfSide,  halfSide}, {-1, 0, 0}, {1, 0}},

            // Right
            Vertex{{ halfSide, -halfSide, -halfSide}, {1, 0, 0}, {0, 0}},
            Vertex{{ halfSide,  halfSide, -halfSide}, {1, 0, 0}, {0, 1}},
            Vertex{{ halfSide,  halfSide,  halfSide}, {1, 0, 0}, {1, 1}},
            Vertex{{ halfSide, -halfSide,  halfSide}, {1, 0, 0}, {1, 0}},
        };

        std::vector<std::uint32_t> indices = { 
            2, 1, 0, 3, 2, 0,       // Front face
            4, 5, 6, 4, 6, 7,       // Back face
            10, 9, 8, 11, 10, 8,    // Top face
            12, 13, 14, 12, 14, 15, // Bottom face
            18, 17, 16, 19, 18, 16, // Left face
            20, 21, 22, 20, 22, 23, // Right face
        };

        Mesh mesh("builtin:cube", vertices, indices);        
        return mesh;
    }

    /// @brief 
    /// @param height 
    /// @param radius 
    /// @param segments Min value = 3 
    /// @return 
    Mesh cylinder(const char* id, float height, float radius, std::uint16_t segments){
        auto halfHeight = height/2;
        int nVertices = segments * 4 + 4; // Duplicated vertices for flat shading

        std::vector<Vertex> vertices;
        vertices.reserve(nVertices);

        vertices.push_back(Vertex{{0, -halfHeight, 0}, {0, -1, 0}, {0.5f, 0.5f}});
        // Add vertices for the 2 caps
        for(int i=0; i< segments; i++){
            auto segmentProportion = static_cast<float>(i)/static_cast<float>(segments);
            auto theta = (2.0f*std::numbers::pi)*segmentProportion;
            vertices.push_back(Vertex{{glm::cos(theta)*radius, -halfHeight, glm::sin(theta)*radius}, {0, -1, 0}, {0, 0}}); // TODO fix UVs
        }
        vertices.push_back(Vertex{{0,  halfHeight, 0}, {0,  1, 0}, {0.5f, 0.5f}});
        for(int i=0; i< segments; i++){
            auto segmentProportion = static_cast<float>(i)/static_cast<float>(segments);
            auto theta = (2.0f*std::numbers::pi)*segmentProportion;
            vertices.push_back(Vertex{{glm::cos(theta)*radius,  halfHeight, glm::sin(theta)*radius}, {0,  1, 0}, {0, 0}}); // TODO fix UVs
        }

        // Add vertices for the side rings
        // (segments+1 bc UVs need to go 0->1: first and last vertex overlap but have different uvs)
        for(int i=0; i < (segments + 1); i++){
            auto segmentProportion = static_cast<float>(i)/static_cast<float>(segments);
            auto theta = (2.0f*std::numbers::pi)*segmentProportion;

            // normalize(h·cosθ, r, h·sinθ)
            glm::vec3 normalVector = glm::normalize(glm::vec3{height*glm::cos(theta), radius, height*glm::sin(theta)}); 

            // Ring vertices
            vertices.push_back(Vertex{{glm::cos(theta)*radius, -halfHeight, glm::sin(theta)*radius}, 
                normalVector, {segmentProportion, 0}});
            vertices.push_back(Vertex{{glm::cos(theta)*radius,  halfHeight, glm::sin(theta)*radius}, 
                normalVector, {segmentProportion, 0}});
        }

        auto nIndices = 3 * (4 * segments); // n triangles * 3
        std::vector<std::uint32_t> indices;
        indices.reserve(nIndices);

        auto topCapBaseIdx = segments + 1;
        // Indices for the caps
        for(int i=0; i<segments; i++){
            // Bottom cap
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back((i+1)%segments + 1);

            // Top cap
            indices.push_back(topCapBaseIdx + (i+1)%segments + 1);
            indices.push_back(topCapBaseIdx + i + 1);
            indices.push_back(topCapBaseIdx);
        }

        auto startingSideIndices = segments * 2 + 2;
        // Indices for the side
        for(int nSegment=0; nSegment<segments; nSegment++){
            indices.push_back(startingSideIndices + nSegment * 2);
            indices.push_back(startingSideIndices + nSegment * 2 + 1);
            indices.push_back(startingSideIndices + nSegment * 2 + 2);

            indices.push_back(startingSideIndices + nSegment * 2 + 3);
            indices.push_back(startingSideIndices + nSegment * 2 + 2);
            indices.push_back(startingSideIndices + nSegment * 2 + 1);
        }

        Mesh mesh(id, vertices, indices);        
        return mesh;
    }

    /// @brief  
    /// @param height 
    /// @param radius 
    /// @param segments Minimum value = 3 
    /// @return 
    Mesh cone(const char* id, float height, float radius, std::uint16_t segments){

        auto halfHeight = height/2;
        int nVertices = segments * 3 + 3; // Duplicated vertices for flat shading + apex vertex duplicated for UVs 

        std::vector<Vertex> vertices;
        vertices.reserve(nVertices);

        vertices.push_back(Vertex{{0, -halfHeight, 0}, {0, -1, 0}, {0.5f, 0.5f}});

        // Add vertices for the cap ring
        for(int i=0; i< segments; i++){
            auto theta = (2.0f*std::numbers::pi)*i/segments;
            vertices.push_back(Vertex{{glm::cos(theta)*radius, -halfHeight, glm::sin(theta)*radius}, {0, -1, 0}, {0, 0}}); // TODO fix UVs
        }

        // Add vertices for the side ring + apex 
        // (segments+1 bc UVs need to go 0->1: first and last vertex overlap but have different uvs)
        for(int i=0; i < segments + 1; i++){
            auto segmentProportion = static_cast<float>(i)/static_cast<float>(segments);
            auto theta = (2.0f*std::numbers::pi)*segmentProportion;

            // normalize(h·cosθ, r, h·sinθ)
            glm::vec3 normalVector = glm::normalize(glm::vec3{height*glm::cos(theta), radius, height*glm::sin(theta)}); 

            // Cap ring vertex
            vertices.push_back(Vertex{{glm::cos(theta)*radius, -halfHeight, glm::sin(theta)*radius}, 
                normalVector, {segmentProportion, 0}});

            // Apex vertex
            vertices.push_back(Vertex{{0, halfHeight, 0}, 
                normalVector, {segmentProportion, 1}});
        }

        auto nIndices = 3 * (2 * segments); // n triangles * 3
        std::vector<std::uint32_t> indices;
        indices.reserve(nIndices);

        // Indices for the cap
        for(int i=0; i<segments; i++){
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back((i+1)%segments + 1);
        }

        auto startingSideIndices = segments + 1;
        // Indices for the side
        for(int nSegment=0; nSegment<segments; nSegment++){
            indices.push_back(startingSideIndices + nSegment * 2);
            indices.push_back(startingSideIndices + nSegment * 2 + 1);
            indices.push_back(startingSideIndices + nSegment * 2 + 2);
        }

        Mesh mesh(id, vertices, indices);        
        return mesh;
    }

    Mesh torus(const char* id, float cirlceRadius, float girthRadius, int segments, int girthsegments){
        // return cylinder(id, girthRadius, cirlceRadius, segments);

        int nVertices = (segments + 1) * (girthsegments + 1);
        cirlceRadius *= 0.5f;
        girthRadius *= 0.5f;

        std::vector<Vertex> vertices;
        vertices.reserve(nVertices);

        for(int circleSegment = 0; circleSegment <= segments; circleSegment++){            
            auto circleSegmentProportion = static_cast<float>(circleSegment)/static_cast<float>(segments);
            auto theta = (2.0f*std::numbers::pi)*circleSegmentProportion;
            auto tubeCenter = cirlceRadius * glm::vec3{glm::cos(theta), 0, glm::sin(theta)} ;

            for(int girthSegment = 0; girthSegment <= girthsegments; girthSegment++){
                auto girthSegmentProportion = static_cast<float>(girthSegment)/static_cast<float>(girthsegments);
                auto gamma = (2.0f*std::numbers::pi)*girthSegmentProportion;

                glm::vec3 position = {
                    glm::cos(theta) * (cirlceRadius + girthRadius * glm::cos(gamma)),
                    glm::sin(gamma) * girthRadius,
                    glm::sin(theta) * (cirlceRadius + girthRadius * glm::cos(gamma))
                };

                glm::vec3 normalVector = glm::normalize(position - tubeCenter); 

                // Cap ring vertex
                vertices.push_back(Vertex{ position, normalVector, {circleSegmentProportion, girthSegmentProportion}}); // Fix UVs
            }
        }

        auto nIndices = 6 * segments * girthsegments;
        std::vector<std::uint32_t> indices;
        indices.reserve(nIndices);

        const int stride = girthsegments + 1;

        for (int i = 0; i < segments; ++i) {
            for (int j = 0; j < girthsegments; ++j) {
                auto v1 = i * stride + j;
                auto v2 = v1 + 1;
                auto v3 = (i+1) * stride + j;
                auto v4 = v3 + 1;

                indices.push_back(v1);
                indices.push_back(v2);
                indices.push_back(v3);

                indices.push_back(v3);
                indices.push_back(v2);
                indices.push_back(v4);
            }
        }

        Mesh mesh(id, vertices, indices);        
        return mesh;
    }

}
