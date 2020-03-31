
template<class Action>
Reader<Action>::Reader(std::string interface_id)
  : interface_id_(interface_id)
{}

template<class Action>
WorldState Reader<Action>::read_world_state() const
{
  WorldState ws;
  try
    {
      shared_memory::deserialize(interface_id_,
				 "world_state",
				 ws);
    }
  catch(...)
    {}
  return ws;
}

template<class Action>
Action Reader<Action>::read_action() const
{
  Action action;
  try
    {
      shared_memory::deserialize(interface_id_,
				 "action",
				 action);
    }
  catch(...)
    {}
  return action;
}
    

