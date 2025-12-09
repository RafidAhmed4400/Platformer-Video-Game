#ifndef Lose_hpp
#define Lose_hpp

#include <stdio.h>

#include "Scene.h"

class Lose : public Scene {
public:
    void Initialize(int numLives) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};

#endif
