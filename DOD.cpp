#include "Game.h"

int main() {
    Game game;
    if (!game.Init()) {
        return 0;
    }
    game.Run();

    return 0;
}
