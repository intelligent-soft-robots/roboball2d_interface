#pragma once

#include <array>
#include "shared_memory/serializer.hpp"
#include "shared_memory/shared_memory.hpp"

#include "roboball2d_interface/item.hpp"

namespace roboball2d_interface
{
class BallGunAction
{
public:
    BallGunAction();
    BallGunAction(bool shoot);

    void set_ball_gun(bool shoot);
    bool should_shoot() const;
    std::string to_string() const;
    bool is_valid() const;
    
public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(id, shoot_, valid_);
    }

public:
    static int get_id();
    int id;

private:
    friend shared_memory::private_serialization;
    bool shoot_;
    bool valid_;
};
}
