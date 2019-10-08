#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <memory>
#include <switch.h>

class SCREEN
{
public:
	virtual ~SCREEN() {}
	virtual void Logic(u64 hDown) = 0;
	virtual void Draw() const = 0;
private:
};

#endif