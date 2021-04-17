#ifndef RESERVE_H
#define RESERVE_H

#include "mytask.h"

class Reserve : public MyTask
{
    Q_OBJECT
public:
    Reserve(MainWindow * Window,
            std::shared_ptr<ThreadingResources<QTcpSocket>> socket,
            std::shared_ptr<ThreadingResourcesLight<UserState>> state,
            std::shared_ptr<ThreadingResourcesLight<UserData>> user);
protected:
    virtual void run()override;
};

#endif // RESERVE_H
