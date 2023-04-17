#pragma once

//#include <vector>
#include <map>
#include <memory>
#include <string>

template <class T>
class FileManager {
private:
     //std::vector<std::pair<std::string, std::shared_ptr<T>>> files;
     std::map<std::string, T*> files;
     //std::map<std::string, std::shared_ptr<T>> files;
public:
     FileManager() {}

     ~FileManager() { UnloadAll(); }

     void LoadFile(std::string fileName) {
          std::string keyName = GetKeyName(fileName);

          if (files.find(keyName) != files.end()) return;
          //files.emplace(keyName, T::Create(fileName));
          files.emplace(keyName, new T(fileName));
     }

     void LoadFile(std::string fileOne, std::string fileTwo) {
          std::string keyName = GetKeyName(fileOne);

          if (files.find(keyName) != files.end()) return;
          //files.emplace(keyName, T::Create(fileOne, fileTwo));
          files.emplace(keyName, new T(fileOne, fileTwo));
     }

     void UnloadFile(std::string name) {
          files.erase(files.find(name));
     }

     void UnloadAll() {
          files.erase(files.begin(), files.end());
     }

     //std::shared_ptr<T> GetValue(std::string keyName) {
     T* GetValue(std::string keyName) {
          //if (files.empty) return;
          return files[keyName];
          //return files.at(keyName);
     }

     static std::string GetKeyName(std::string fileName) {
          std::string keyName = "";

          keyName = fileName.substr(fileName.find_last_of("/\\") + 1);
          keyName = keyName.substr(0, keyName.find_last_of("."));

          return keyName;
     }
};