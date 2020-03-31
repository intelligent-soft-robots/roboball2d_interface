#include "roboball2d_interface/mirror_action.hpp"

namespace roboball2d_interface
{

  MirrorAction::MirrorAction()
    : id(MirrorAction::get_id())
  {
    std::array<double,2> zeros = {0,0};
    set_mirroring_information(zeros,zeros,zeros);
  }
      
  MirrorAction::MirrorAction(const std::array<double,2>& mi1,
			     const std::array<double,2>& mi2,
			     const std::array<double,2>& mi3)
  {
    set_mirroring_information(mi1,mi2,mi3);
  }
      
  int MirrorAction::get_id()
  {
    static int id;
    id++;
    return id;
  }
    
  std::string MirrorAction::to_string() const
  {
    std::string s = "mirroring action ("+std::to_string(id)+")\n";
    auto ad2_str = [&s](const std::array<double,2>&mi)
      {
	s+= "\tangle: " +std::to_string(mi[0])+
	", velocity: "+std::to_string(mi[1])+"\n";
      };
    std::for_each(mirroring_information_.begin(),
		  mirroring_information_.end(),ad2_str);
    return s;

  }
    
  void MirrorAction::set_mirroring_information(const std::array<double,2>& mi1,
					       const std::array<double,2>& mi2,
					       const std::array<double,2>& mi3)
  {
    mirroring_information_[0] = mi1;
    mirroring_information_[1] = mi2;
    mirroring_information_[2] = mi3;
  }
    
  std::array<std::array<double,2>,3>
  MirrorAction::get_mirroring_information() const
  {
    return mirroring_information_;
  }
    
    
}


