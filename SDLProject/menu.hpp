

#ifndef menu_hpp
#define menu_hpp

#include <stdio.h>

#include "Scene.h"

class Menu : public Scene {
public:
    void Initialize(int numLives) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};

#endif /* menu_hpp */
