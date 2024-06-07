#pragma once

#define WIN32_LEAN_AND_MEAN


////////////////////////////////////////////
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <windows.h>
#include <SDL_keycode.h>
#include <SDL_events.h>
#include <SDL_mixer.h>

////////////////////////////////////////////
#include <stdexcept>

////////////////////////////////////////////
#include <glm/glm.hpp>

////////////////////////////////////////////
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <mutex>
#include <utility>

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <condition_variable>

////////////////////////////////////////////
#include "Minigin.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Component.h"
#include "GameObject.h"
#include "TextObject.h"
#include "TextureComponent.h"
#include "FPSCounter.h"
#include "RotationComponent.h"
#include "Font.h"
#include "Timer.h"
#include "Transform.h"
#include "InputManager.h"
#include "Texture2D.h"
#include "GamePad.h"
#include "Input.h"
#include "Command.h"
#include "Event.h"
#include "HealthComponent.h"
#include "Keyboard.h"
#include "ScoreComponent.h"
#include "SoundSystem.h"
#include "BoxCollider.h"

////////////////////////////////////////////
#define CREATE_GAMEOBJECT(...) std::make_shared<dae::GameObject>(__VA_ARGS__)


////////////////////////////////////////////
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_WHITE "\033[37m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_ORANGE "\033[33m"


#define LOG_TRACE(message) std::cout << ANSI_COLOR_GREEN << "[TRACE] " << message << ANSI_COLOR_RESET << std::endl
#define LOG_INFO(message) std::cout << ANSI_COLOR_WHITE << "[INFO] " << message << ANSI_COLOR_RESET << std::endl
#define LOG_CRITICAL(message) std::cerr << ANSI_COLOR_RED << "[CRITICAL] " << message << ANSI_COLOR_RESET << std::endl
#define LOG_WARNING(message) std::cout << ANSI_COLOR_ORANGE << "[WARNING] " << message << ANSI_COLOR_RESET << std::endl