#include "Lose.hpp"
#include "Utility.h"
constexpr char SPRITESHEET_FILEPATH[] = "Spritesheet.png",
               PLATFORM_FILEPATH[]    = "tilesheet_complete.png",
               ENEMY_FILEPATH[]       = "enemy_spritesheet.png";

int player_walking_animationL[4][4] =
{
    { 5, 6, 7, 8 },  // for Player to move to the left,
    { 9, 10, 11, 12 }, // for player to move to the right,
    { 13, 14, 15, 16 }, // for player to move upwards,
    { 1, 2, 3, 4 }   // for Player to move downwards
};

void Lose::Initialize(int numLives){
    m_game_state.next_scene_id = -1;
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    glm::vec3 acceleration = glm::vec3(0, -9.81f, 0);


    m_game_state.player = new Entity(
                                     player_texture_id,         // texture id
                                     3.0f,                      // speed
                                     acceleration,              // acceleration
                                     7.0f,                      // jumping power
                                     player_walking_animationL,  // animation index sets
                                     0.0f,                      // animation time
                                     4,                         // animation frame amount
                                     1,                         // current animation index
                                     4,                         // animation column amount
                                     4,                         // animation row amount
                                     0.8f,                      // width
                                     0.8f,                       // height
                                     PLAYER
                                     );
    
    m_game_state.player->set_position(glm::vec3(5, 0, 0));
    m_game_state.player->set_movement(glm::vec3(0));
    m_game_state.player->set_numLives(numLives);
    m_game_state.player->set_height(0.8f);
    m_game_state.player->set_width(0.8f);
        
}
void Lose::update(float delta_time) {
    
}
void Lose::render(ShaderProgram *program) {
    Mix_HaltChannel(-1);  // Stop all sounds including die_sfx
    Utility::draw_text(program, Utility::load_texture("font1.png"), "You Lose!", 1, -0.5, glm::vec3(2.25f, -2.0f, 0));
}


