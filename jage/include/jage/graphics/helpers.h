#pragma once

#include "log.h"

#include <string>

#include <glad/glad.h>

namespace jage::graphics
{
	void CheckGLError();
}

#ifndef JAGE_CONFIG_RELEASE
#define JAGE_CHECK_GL_ERROR jage::graphics::CheckGLError();
#else
#define JAGE_CHECK_GL_ERROR (void)0
#endif