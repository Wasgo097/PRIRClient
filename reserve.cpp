#include "reserve.h"


Reserve::Reserve(MainWindow *Window,
                 std::shared_ptr<ThreadingResources<QTcpSocket> > socket,
                 std::shared_ptr<ThreadingResourcesLight<UserState> > state,
                 std::shared_ptr<ThreadingResourcesLight<UserData> > user):
MyTask{Window,socket,state,user}{
}