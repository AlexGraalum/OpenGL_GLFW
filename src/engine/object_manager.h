#pragma once

#include <map>
#include <memory>
#include <string>
#include "object.h"

class ObjectManager {
private:
     std::map<std::string, std::shared_ptr<Object>> objects;
public:
     ObjectManager() {}

     static std::shared_ptr<ObjectManager> Create() {
          return std::shared_ptr<ObjectManager>(new ObjectManager());
     }

     ~ObjectManager() { UnloadAll(); }

     void LoadObject(std::string name, std::shared_ptr<Object> object) {
          if (objects.find(name) != objects.end()) return;
          objects.emplace(name, object);
     }

     void UnloadObject(std::string name) {
          objects.erase(objects.find(name));
     }

     void UnloadAll() {
          objects.erase(objects.begin(), objects.end());
     }

     void Render3DObjects(std::shared_ptr<Camera> camera, glm::vec3 lightPos) {
          for (auto const& o : objects) {
               o.second->Draw3D(camera, lightPos);
          }
     }

     void Render2DObjects() {
          for (auto const& o : objects) {
               o.second->Draw2D();
          }
     }

     std::shared_ptr<Object> GetObject(std::string objectName) {
          return objects[objectName];
     }
};