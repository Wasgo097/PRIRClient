#ifndef UTILITIES_H
#define UTILITIES_H
#include <mutex>
#include <memory>
#include <string>
enum class UserState{
    Unconnected,
    Connected,
    Logged
};
struct UserData{
    std::string login;
    std::string password;
    void clear(){
        login="";
        password="";
    }
};
template <class T>
struct ThreadingResources{
    std::shared_ptr<T> Resource;
    std::mutex Resource_mtx;
};
template <class T>
struct ThreadingResourcesLight{
    T Resource;
    std::mutex Resourc_mtx;
};
#endif // UTILITIES_H
