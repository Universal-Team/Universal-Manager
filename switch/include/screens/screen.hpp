#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <switch.h>
#include <memory>

class SCREEN
{
public:
    virtual ~SCREEN() {}
    virtual void Logic(u64 hDown) = 0;
    virtual void Draw() const = 0;
private:
};

#endif