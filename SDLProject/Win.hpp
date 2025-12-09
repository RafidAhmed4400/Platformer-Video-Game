#ifndef Win_hpp
#define Win_hpp

#include <stdio.h>

#include "Scene.h"

class Win : public Scene {
public:
    void Initialize(int numLives) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};

#endif 
