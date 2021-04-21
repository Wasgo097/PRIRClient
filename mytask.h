#ifndef MYTASK_H
#define MYTASK_H
#include <memory>
#include <string>
#include <QThread>
#include <QTcpSocket>
#include "Utilities.h"
#include "mainwindow.h"
class QTcpSocket;
class MyTask : public QThread{
    Q_OBJECT
public:
    MyTask(MainWindow * Window,
           std::shared_ptr<ThreadingResources<QTcpSocket>> socket,
           std::shared_ptr<ThreadingResourcesLight<UserState>> state,
           std::shared_ptr<ThreadingResourcesLight<UserData>> user);
    virtual ~MyTask(){}
protected:
    MainWindow * Window;
    std::shared_ptr<ThreadingResources<QTcpSocket>> _socket;
    std::shared_ptr<ThreadingResourcesLight<UserState>> _state;
    std::shared_ptr<ThreadingResourcesLight<UserData>> _user;
};

#endif // MYTASK_H
