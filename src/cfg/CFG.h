/**
 * Handles system wide non-volatile configuration using a wrapped file system
 */

#pragma once

#include "sys/core.h"

Status CFG_Init (const char* root);
Status CFG_Open (void* file, const char* path);
Status CFG_Read (void* file, void* dst, size_t offset, size_t length);
Status CFG_Write(void* file, void* src, size_t offset, size_t length);
Status CFG_Sync (void* file);
Status CFG_Close(void* file, size_t );