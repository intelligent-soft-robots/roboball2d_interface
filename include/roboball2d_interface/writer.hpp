#pragma once
#include "roboball2d_interface/world_state.hpp"
#include "shared_memory/shared_memory.hpp"

namespace roboball2d_interface
{
    template <class Action,int NB_ROBOTS, int NB_BALLS,int TYPE>
class Writer
{
public:
    Writer(std::string interface_id);
    static void clear(std::string interface_id);
    void write_world_state(const WorldState<NB_ROBOTS,NB_BALLS,TYPE>& world_state) const;
    void write_action(const Action& action) const;

private:
    std::string interface_id_;
};

#include "writer.hxx"
}
