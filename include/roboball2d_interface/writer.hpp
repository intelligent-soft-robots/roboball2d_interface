#pragma once
#include "shared_memory/shared_memory.hpp"
#include "roboball2d_interface/world_state.hpp"

namespace roboball2d_interface
{

  template<class Action>
  class Writer
  {
  public:
    Writer(std::string interface_id);
    static void clear(std::string interface_id);
    void write_world_state(const WorldState& world_state) const;
    void write_action(const Action& action) const;

  private:
    std::string interface_id_;
  };

  #include "writer.hxx"
  
}


