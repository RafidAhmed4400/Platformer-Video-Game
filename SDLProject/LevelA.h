#include "Scene.h"

class LevelA : public Scene {
public:

    // ————— DESTRUCTOR ————— //
    ~LevelA();
    
    // ————— METHODS ————— //
    void Initialize(int numLives) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
