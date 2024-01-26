#include <algorithm>
#include <iostream>
#if !defined(__HORIZON_HPP__)
#define __HORIZON_HPP__
#include <HorizonLine.hpp>
#include <Obstacle.hpp>
#include <SDL3/SDL_rect.h>
#include <Sprite.hpp>
#include <vector>

class Cloud {
};
class Horizon {

public:
    Horizon()
        : horizonLine()
    {
    }

    void update(float deltaTime, float SPEED)
    {
        this->horizonLine.update(deltaTime, SPEED);
        if (this->obstacles.size() < 1) {
            this->obstacles.emplace_back(Obstacle::Type::kSmallCactus1);
        }
        std::clog << "Obstacles count: " << this->obstacles.size() << "\n";

        this->obstacles.erase(std::remove_if(this->obstacles.begin(), this->obstacles.end(), [](const Obstacle& o) { return !o.isVisible(); }), this->obstacles.end());
        for (auto& o : this->obstacles) {
            o.update(deltaTime, SPEED);
        }
    }

    void draw(Sprite& s)
    {
        s.draw(this->horizonLine);
        for (auto& o : this->obstacles) {
            s.draw(o);
        }
    }

    bool checkCollision(const SDL_FRect& rect)
    {
        for (auto& o : this->obstacles) {
            if (auto r = o.getDestRect(); SDL_HasRectIntersectionFloat(&rect, &r)) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<Obstacle> obstacles;
    HorizonLine horizonLine;
};

#endif