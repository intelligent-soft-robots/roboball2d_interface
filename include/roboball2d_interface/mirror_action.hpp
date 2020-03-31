#pragma once

#include <algorithm>
#include <array>
#include "shared_memory/shared_memory.hpp"
#include "shared_memory/serializer.hpp"
#include "roboball2d_interface/item.hpp"

namespace roboball2d_interface
{

  class MirrorAction
  {

  public:
      
    MirrorAction();
    MirrorAction(const std::array<double,2>& joint1,
		 const std::array<double,2>& joint2,
		 const std::array<double,2>& joint3);
    void set_mirroring_information(const std::array<double,2>& joint1,
				   const std::array<double,2>& joint2,
				   const std::array<double,2>& joint3);
    std::array<std::array<double,2>,3> get_mirroring_information() const;
    std::string to_string() const;
      
  public:

    template<class Archive>
    void serialize(Archive& archive)
    {
      archive(id,
	      mirroring_information_);
    }
      
  public:

    static int get_id();
    int id;
      
  private:

    friend shared_memory::private_serialization;
    std::array<std::array<double,2>,3> mirroring_information_;

  };

}


