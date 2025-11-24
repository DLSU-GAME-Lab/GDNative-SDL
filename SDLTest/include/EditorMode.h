#pragma once
// Detect EditorModule.h
#if defined(__has_include)
#if __has_include("EditorModule.h")
#define EDITOR_MODULE_AVAILABLE 1
#else
#define EDITOR_MODULE_AVAILABLE 0
#endif
#else
#define EDITOR_MODULE_AVAILABLE 0
#endif

// Toggle editor mode (set to 1 to enable, 0 to disable)
#define EDITOR_MODE (EDITOR_MODULE_AVAILABLE && 0)   // <- put 1 or 0 here