#pragma once

#include "roboball2d_interface/world_state.hpp"
#include "shared_memory/shared_memory.hpp"

namespace roboball2d_interface
{
    template <class Action,int NB_ROBOTS,int NB_BALLS>
class Reader
{
public:
    Reader(std::string interface_id);
    WorldState<NB_ROBOTS,NB_BALLS> read_world_state() const;
    Action read_action() const;

private:
    std::string interface_id_;
};

#include "reader.hxx"
};
