#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include <iostream>

#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"

#include "LevelA.h"
#include "LevelB.hpp"
#include "LevelC.hpp"

#include "menu.hpp"
#include "Win.hpp"
#include "Lose.hpp"

using namespace std;

// ————— CONSTANTS ————— //
constexpr int WINDOW_WIDTH  = 640 * 2,
              WINDOW_HEIGHT = 480 * 2;

constexpr float BG_RED     = 0.1922f,
            BG_BLUE    = 0.549f,
            BG_GREEN   = 0.9059f,
            BG_OPACITY = 1.0f;

int numLives = 3;
float missionPass = 0; // 0 = ongoing, 1 = pass, -1 = fail

constexpr int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

enum AppStatus { RUNNING, TERMINATED };

// ————— GLOBAL VARIABLES ————— //

Scene *sceneList[6];
Scene *g_current_scene;

// BGM
constexpr int CD_QUAL_FREQ = 44100,  // CD quality
          AUDIO_CHAN_AMT  = 2,      // Stereo
          AUDIO_BUFF_SIZE = 4096;

// SFX
constexpr int PLAY_ONCE   =  0,
          NEXT_CHNL   = -1,  // next available channel
          MUTE_VOL    =  0,
          MILS_IN_SEC =  1000,
          ALL_SFX_CHN = -1;



constexpr int  LOOP_FOREVER   = -1;  // -1 means loop forever in Mix_PlayMusic; 0 means play once and loop zero times

Mix_Music *g_music;
Mix_Chunk *g_bounce;
Mix_Chunk *g_die_sfx;
Mix_Chunk *g_win_sfx;


SDL_Window* g_display_window;

AppStatus g_app_status = RUNNING;
ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

void switch_to_scene(Scene *scene)
{
    g_current_scene = scene;
    g_current_scene->Initialize(numLives);
    
    // Keep music playing across scenes
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(g_music, LOOP_FOREVER);
    } else if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

void Initialize();
void process_input();
void Update();
void Render();
void shutdown();




void Initialize()
{
    // ————— VIDEO ————— //
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("Hello, Scenes!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    if (context == nullptr)
    {
        shutdown();
    }
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    // ————— GENERAL ————— //
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    


        sceneList[0] = new Menu();
        sceneList[1] = new LevelA();
        sceneList[2] = new LevelB();
        sceneList[3] = new LevelC();
        sceneList[4] = new Lose();
        sceneList[5] = new Win();
        switch_to_scene((sceneList[0]));
    
    
    // ————— BLENDING ————— //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    g_current_scene->get_state().player->set_movement(glm::vec3(0.0f));
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_app_status = TERMINATED;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (g_current_scene->get_state().player->get_collided_bottom()) {
                            g_current_scene->get_state().player->jump();
                            Mix_PlayChannel(-1, g_bounce, 0);
//                            Mix_VolumeChunk(g_bounce, 100);
                        }
                        break;
                    case SDLK_RETURN:
                        if (g_current_scene == sceneList[0]) {
                            g_current_scene->update(-1);
                        }
                        break;
                }
                break;
        }
    }
    
    // ————— KEY HOLD ————— //
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    if (key_state[SDL_SCANCODE_LEFT]) {
        g_current_scene->get_state().player->move_left();
    }
    else if (key_state[SDL_SCANCODE_RIGHT]) {
        g_current_scene->get_state().player->move_right();
    }
    
    if (glm::length(g_current_scene->get_state().player->get_movement()) > 1.0f) {
        g_current_scene->get_state().player->normalise_movement();
    }
}

void Update()
{
    // ————— DELTA TIME / FIXED TIME STEP CALCULATION ————— //
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    
    delta_time += g_accumulator;
    
    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }
    
    while (delta_time >= FIXED_TIMESTEP) {
        // ————— UPDATING THE SCENE (i.e. map, character, enemies...) ————— //
        g_current_scene->update(FIXED_TIMESTEP);
        
        delta_time -= FIXED_TIMESTEP;
    }
    
    g_accumulator = delta_time;
    
    
    // ————— PLAYER CAMERA ————— //
    g_view_matrix = glm::mat4(1.0f);
    
    if (g_current_scene->get_state().player->get_position().x > 5) {
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->get_state().player->get_position().x, 3.75, 0));
    } else {
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5, 3.75, 0));
    }
}

void Render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    g_current_scene->render(&g_shader_program);
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

}

// ————— GAME LOOP ————— //
int main(int argc, char* argv[])
{
    Initialize();
    
    while (g_app_status == RUNNING)
    {
        process_input();
        Update();
        
        // Check for scene transition immediately after update
        if (g_current_scene->get_state().next_scene_id >= 0)
        {
            int next_scene = g_current_scene->get_state().next_scene_id;
            int current_lives = g_current_scene->get_state().player->get_numLives();
            
            // Special case for menu transition
            if (g_current_scene == sceneList[0] && next_scene == 1) {
                current_lives = 3; // Reset lives when starting from menu
            }
            
            switch_to_scene(sceneList[next_scene]);
            g_current_scene->get_state().player->set_numLives(current_lives);
            numLives = current_lives;
            

        }
        
        Render();
    }
    
    shutdown();
    return 0;
}
