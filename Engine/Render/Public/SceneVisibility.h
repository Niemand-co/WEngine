#pragma once

bool IsBoxInFrustum(WEngine::WArray<glm::vec3> Frustum, glm::vec3 BoxMin, glm::vec3 BoxMax);

bool IsSphereInFrustum(WEngine::WArray<glm::vec3> Frustum, glm::vec3 Center, float Radius)