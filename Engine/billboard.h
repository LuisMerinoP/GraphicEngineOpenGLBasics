#pragma once
#include "object.h"


object_t* createBillboard(const char* texture, GLuint programID);
void updateBillboard(object_t* bboard, camera_t* cam);
