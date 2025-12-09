#include "LevelB.hpp"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

using namespace std;

constexpr char SPRITESHEET_FILEPATH[] = "Spritesheet.png",
               PLATFORM_FILEPATH[]    = "tilesheet_complete.png",
               ENEMY_FILEPATH[]       = "enemy_spritesheet.png";

unsigned int LEVELB_DATA[] =
{
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 3, 3, 3, 3, 3, 3, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1
};

LevelB::~LevelB()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.g_jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
    Mix_FreeChunk(m_game_state.g_die_sfx);
    Mix_FreeChunk(m_game_state.g_win_sfx);
}

void LevelB::Initialize(int numLives)
{
    m_game_state.next_scene_id = -1;
    set_number_of_enemies(1);
    GLuint map_texture_id = Utility::load_texture(PLATFORM_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 3, 1);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    
    int player_walking_animation[4][4] =
    {
        { 5, 6, 7, 8 },    // Left animation frames
        { 9, 10, 11, 12 }, // Right animation frames
        { 13, 14, 15, 16 },// Up animation frames
        { 1, 2, 3, 4 }     // Down animation frames
    };
    
    glm::vec3 acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    
    m_game_state.player = new Entity(
        player_texture_id,
        3.0f,                      // Speed
        acceleration,              // Acceleration
        6.0f,                      // Jumping power
        player_walking_animation,  // Animation
        0.0f,                      // Animation time
        4,                         // Animation frames
        0,                         // Current index
        4,                         // Columns
        4,                         // Rows
        0.8f,                      // Width
        0.8f,                      // Height
        PLAYER
    );
    m_game_state.player->set_position(glm::vec3(1, -2, 0));
    m_game_state.player->set_movement(glm::vec3(0));
    m_game_state.player->set_numLives(numLives);
    
    // Enemies
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);
    glm::vec3 enemy_acceleration = glm::vec3(0, -9.81, 0);

    m_game_state.enemies = new Entity[get_number_of_enemies()];
    for(int i=0; i<get_number_of_enemies(); i++){
        m_game_state.enemies[i] = Entity(
            enemy_texture_id,
            1.0f,                     // Speed
            0.8f,                     // Width
            0.8f,                     // Height
            ENEMY,                    // Type
            WALKER,               // AI Type
            IDLE                     // AI State
        );
        m_game_state.enemies[i].set_acceleration(enemy_acceleration);
        m_game_state.enemies[i].activate();
        m_game_state.enemies[i].set_jumping_power(5.0f);
    }
    m_game_state.enemies[0].set_position(glm::vec3(12, -2.0f, 0));
    
    // Audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.bgm = Mix_LoadMUS("background.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(100.0f);
    
    m_game_state.g_jump_sfx = Mix_LoadWAV("jump.mp3");
    m_game_state.g_die_sfx = Mix_LoadWAV("die.mp3");
    m_game_state.g_win_sfx = Mix_LoadWAV("win.mp3");
}

void LevelB::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, get_number_of_enemies(), m_game_state.map);
    
    for (int i = 0; i < get_number_of_enemies(); i++) {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
    
    // Check for level transition
    if (m_game_state.player->get_position().x > LEVEL_WIDTH &&
        m_game_state.player->get_position().y <= -4.0f &&
        m_game_state.next_scene_id < 0)
    {
        m_game_state.next_scene_id = 3; // Go to LevelC
        return;
    }
    
    // Only check for death when within level bounds
    if (m_game_state.player->get_position().x <= LEVEL_WIDTH &&
        (m_game_state.player->get_position().y <= -10.0f || !m_game_state.player->get_activity_status()))
    {
        m_game_state.player->activate();
        m_game_state.player->set_numLives(m_game_state.player->get_numLives()-1);
        Mix_PlayChannel(-1, m_game_state.g_die_sfx, 0);
        if (m_game_state.player->get_numLives() <= 0) {
            m_game_state.next_scene_id = 4; // Lose screen
        }
        m_game_state.player->set_position(glm::vec3(1, 0, 0));
    }
}

void LevelB::render(ShaderProgram *g_shader_program)
{
    // Get camera-relative position for text
    float text_x = m_game_state.player->get_position().x > 5.0f ?
                  m_game_state.player->get_position().x - 5.0f : 0.0f;
    
    // Text that scrolls with camera (stays in top-left)
    Utility::draw_text(g_shader_program, Utility::load_texture("font1.png"),
                      "Level 2", 0.5f, -0.1f, glm::vec3(text_x + 0.75f, -0.25f, 0));
    Utility::draw_text(g_shader_program, Utility::load_texture("font1.png"),
                      "Lives: " + std::to_string(m_game_state.player->get_numLives()),
                      0.5f, -0.1f, glm::vec3(text_x + 0.75f, -1.0f, 0));
    Utility::draw_text(g_shader_program, Utility::load_texture("font1.png"),
                      "Exit ->", 0.5f, -0.1f, glm::vec3(text_x + 0.75f, -1.75f, 0));
    
    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i < get_number_of_enemies(); i++) {
        m_game_state.enemies[i].render(g_shader_program);
    }
}
