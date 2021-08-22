#ifndef KVASIR_ENGINE_INCLUDE_H
#define KVASIR_ENGINE_INCLUDE_H 1

#define NO_USE_INCLUDES 1

// Define KVASIR_DLL if using a shared library

#include "./src/config.h"
#include "./src/utils/data.h"
#include <stddef.h>
#include "./lib/galg/galg.h"
#include "./lib/galg/galg_ops.h"
#include "./src/utils/galg-dllexport.h"
#include "./src/utils/memory-aid.h"
#include "./src/utils/rel-paths.h"

using namespace galg;

#include "./src/world/position.h"
#include "./src/utils/fps-manager.h"
#include "./src/render-base/buffer.h"
#include "./src/render-base/render-buffer.h"
#include "./src/render-base/texture.h"
#include "./src/world/material.h"
#include "./src/world/mesh.h"
#include "./src/physics/physics.h"
#include "./src/world/camera.h"
#include "./src/utils/input.h"
#include "./src/render-base/shader.h"
#include "./src/renderers/renderer.h"
#include "./src/utils/common-ops.h"
#include "./src/kvasir-engine.h"

#endif