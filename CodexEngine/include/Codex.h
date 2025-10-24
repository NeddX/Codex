#ifndef CODEX_H
#define CODEX_H

// Core
#include <Engine/Core/Application.h>
#include <Engine/Core/CommonDef.h>
#include <Engine/Core/Exception.h>
#include <Engine/Core/Geomtryd.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/ResourceHandler.h>
#include <Engine/Core/Serializer.h>
#include <Engine/Core/Window.h>

// Debug
#include <Engine/Debug/Profiler.h>
#include <Engine/Debug/TimeScope.h>

// Concurrency
#include <Engine/Concurrency/Mutex.h>

// Events
#include <Engine/Events/Event.h>
#include <Engine/Events/KeyEvent.h>
#include <Engine/Events/MouseEvent.h>

// File system
#include <Engine/FileSystem/FileSystem.h>

// Math
#include <Engine/Math/Math.h>

// Memory
#include <Engine/Memory/Memory.h>

// Scene
#include <Engine/Scene/Camera.h>
#include <Engine/Scene/Components.h>
#include <Engine/Scene/ECS.h>
#include <Engine/Scene/EditorCamera.h>
#include <Engine/Scene/Scene.h>

// Reflection
#include <Engine/Reflection/Lexer.h>
#include <Engine/Reflection/Reflector.h>

// Graphics
#include <Engine/Graphics/BatchRenderer2D.h>
#include <Engine/Graphics/DebugDraw.h>
#include <Engine/Graphics/Image2D.h>
#include <Engine/Graphics/RenderBatch.h>
#include <Engine/Graphics/Renderer.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Graphics/Texture2D.h>

// System
#include <Engine/System/DynamicLibrary.h>
#include <Engine/System/Process.h>

#endif // CODEX_H
