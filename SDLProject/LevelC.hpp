#include "Scene.h"

class LevelC : public Scene {
public:
    // ————— DESTRUCTOR ————— //
    ~LevelC();
    
    // ————— METHODS ————— //
    void Initialize(int numLives) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
