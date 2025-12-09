#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Util.h"
#include "Entity.h"
#include "Map.h"


struct GameState
{
    // ————— GAME OBJECTS ————— //
    Map *map;
    Entity *player;
    Entity *enemies;
    
    // ————— AUDIO ————— //
    Mix_Music *bgm;
    Mix_Chunk *g_jump_sfx;
    Mix_Chunk *g_die_sfx;
    Mix_Chunk *g_win_sfx;
    
    
    // ————— POINTERS TO OTHER SCENES ————— //
    int next_scene_id;
};

class Scene {
protected:
    GameState m_game_state;
    
public:
    // ————— ATTRIBUTES ————— //
    int m_number_of_enemies;
    
    // ————— METHODS ————— //
    virtual void Initialize(int numLives) = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program) = 0;
    
    
    
    // ————— GETTERS ————— //
    GameState const get_state() const { return m_game_state; }
    int const get_number_of_enemies() const { return m_number_of_enemies; }
    
    // ————— SETTERS ————— //
    void set_number_of_enemies(int num) { m_number_of_enemies = num; }

};
