#pragma once

#include <map>
#include <memory>
#include <string>

template <class T>
class FileManager {
private:
     std::map<std::string, T*> files;
public:
     FileManager() {}

     static std::shared_ptr<FileManager> Create() {
          return std::shared_ptr<FileManager>(new FileManager());
     }

     ~FileManager() { UnloadAll(); }

     void LoadFile(std::string fileName) {
          std::string keyName = GetKeyName(fileName);

          if (files.find(keyName) != files.end()) return;
          files.emplace(keyName, new T(fileName));
     }

     void LoadFile(std::string fileOne, std::string fileTwo) {
          std::string keyName = GetKeyName(fileOne);

          if (files.find(keyName) != files.end()) return;
          files.emplace(keyName, new T(fileOne, fileTwo));
     }

     void UnloadFile(std::string name) {
          files.erase(files.find(name));
     }

     void UnloadAll() {
          files.erase(files.begin(), files.end());
     }

     T* GetValue(std::string keyName) {
          return files[keyName];
     }

     static std::string GetKeyName(std::string fileName) {
          std::string keyName = "";

          size_t path = fileName.find_last_of("/\\") + 1;
          size_t ext = fileName.find_last_of(".");
          keyName = fileName.substr(path, ext - path);

          return keyName;
     }
};