

template<class Action>
Driver<Action>::Driver(std::string interface_id)
  : interface_id_(interface_id),
    writer_(interface_id),
    reader_(interface_id){}

template<class Action>
void Driver<Action>::initialize(){}

template<class Action>
Action Driver<Action>::apply_action(const Action &desired_action)
{
  std::lock_guard<std::mutex> guard(mutex_);
  writer_.write_action(desired_action);
  return desired_action;
}

template<class Action>
WorldState Driver<Action>::get_latest_observation()
{
  WorldState ws = reader_.read_world_state();
  return ws;
}

template<class Action>
void Driver<Action>::shutdown(){}

template<class Action>
std::string Driver<Action>::get_error()
{
  return "";
}
