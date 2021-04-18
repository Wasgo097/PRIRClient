#ifndef UTILITIES_H
#define UTILITIES_H
#include <mutex>
#include <memory>
#include <string>
#include <QString>
enum class UserState{
    Unconnected,
    Connected,
    Logged
};
enum class BookState{
    Available,
    Unavailable,
    Default
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
struct Book{
    QString Id;
    QString Name;
    QString Author;
    QString Date;
    BookState State=BookState::Default;
    QString ToQStr(){
        if(State==BookState::Available)
            return Id+" "+Name+" "+Author+" "+Date+" Available";
        else if(State==BookState::Unavailable)
            return Id+" "+Name+" "+Author+" "+Date+" Unavailable";
        else
            return Id+" "+Name+" "+Author+" "+Date;
    }
};

#endif // UTILITIES_H
