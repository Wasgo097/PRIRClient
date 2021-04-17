#ifndef RETURNBOOK_H
#define RETURNBOOK_H

#include "mytask.h"

class ReturnBook : public MyTask
{
    Q_OBJECT
public:
    ReturnBook(MainWindow * Window,
               std::shared_ptr<ThreadingResources<QTcpSocket>> socket,
               std::shared_ptr<ThreadingResourcesLight<UserState>> state,
               std::shared_ptr<ThreadingResourcesLight<UserData>> user);
};

#endif // RETURNBOOK_H
