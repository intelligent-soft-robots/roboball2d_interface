#pragma once
  
#include "shared_memory/shared_memory.hpp"
#include "roboball2d_interface/world_state.hpp"

namespace roboball2d_interface
{

  template<class Action>
  class Reader
  {
  public:
    Reader(std::string interface_id);
    WorldState read_world_state() const;
    Action read_action() const;
  private:
    std::string interface_id_;
  };

#include "reader.hxx"
  
};


