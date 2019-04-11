

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

// 3D offsets.
typedef struct _Offset3D {
	float topbg;
} Offset3D;
extern Offset3D offset3D[2];	// 0 == Left; 1 == Right

#endif /* SETTINGS_H */
