
#if !defined(__HORIZON_HPP__)
#define __HORIZON_HPP__
#include <HorizonLine.hpp>
#include <Obstacle.hpp>
#include <SDL3/SDL_rect.h>
#include <Sprite.hpp>
#include <algorithm>
#include <random>
#include <vector>

class Cloud {
};

class Runner;
class Horizon {
    friend Runner;

public:
    Horizon() = default;

    void update(float deltaTime, float SPEED)
    {
        deltaTime = deltaTime / 20;

        if (this->obstacles.size() < 1) {
            int random = this->dis(this->gen);
            this->obstacles.emplace_back(static_cast<Obstacle::Type>(random));
        }

        this->obstacles.erase(std::remove_if(this->obstacles.begin(), this->obstacles.end(), [](const Obstacle& o) { return !o.isVisible(); }), this->obstacles.end());

        this->horizonLine.update(deltaTime, SPEED);

        for (auto& o : this->obstacles) {
            o.update(deltaTime, SPEED);
        }
    }
    void clear()
    {
        this->obstacles.clear();
    }
    void draw(Sprite& s)
    {
        this->horizonLine.Draw(s);
        for (auto& o : this->obstacles) {
            s.draw(o);
        }
    }

    bool checkCollision(const SDL_FRect& rect)
    {
        if (obstacles.size() > 0) {
            auto o = obstacles.front();
            if (auto r = o.getCollisionBox(); SDL_HasRectIntersectionFloat(&rect, &r)) {
                return true;
            }
        }

        return false;
    }

private:
    std::vector<Obstacle> obstacles;
    HorizonLine horizonLine;

    std::random_device rd;
    std::mt19937 gen { rd() };
    std::uniform_int_distribution<> dis { 0, 5 };
};

#endif