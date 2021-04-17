#include "mytask.h"
MyTask::MyTask(MainWindow *Window,
               std::shared_ptr<ThreadingResources<QTcpSocket>> socket,
               std::shared_ptr<ThreadingResourcesLight<UserState>> state,
               std::shared_ptr<ThreadingResourcesLight<UserData>> user):QThread(Window),Window{Window}{
    _socket=socket;
    _state=state;
    _user=user;
}
