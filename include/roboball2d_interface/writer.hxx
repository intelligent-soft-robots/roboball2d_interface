
template <class Action,int NB_ROBOTS, int NB_BALLS,int TYPE>
Writer<Action,NB_ROBOTS,NB_BALLS,TYPE>::Writer(std::string interface_id) : interface_id_(interface_id)
{
}

template <class Action,int NB_ROBOTS, int NB_BALLS, int TYPE>
void Writer<Action,NB_ROBOTS,NB_BALLS,TYPE>::clear(std::string interface_id)
{
    shared_memory::clear_shared_memory(interface_id);
}

template <class Action,int NB_ROBOTS, int NB_BALLS, int TYPE>
void Writer<Action,NB_ROBOTS,NB_BALLS,TYPE>::write_world_state(const WorldState<NB_ROBOTS,NB_BALLS,TYPE>& world_state) const
{
    shared_memory::serialize(interface_id_, "world_state", world_state);
}

template <class Action,int NB_ROBOTS, int NB_BALLS, int TYPE>
void Writer<Action,NB_ROBOTS,NB_BALLS,TYPE>::write_action(const Action& action) const
{
    shared_memory::serialize(interface_id_, "action", action);
}



