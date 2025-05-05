#pragma once

#include <stdio.h>

#include <stb_image.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "core.h"

GLuint LoadTexture(const char* texture_file_path);
