
template <class Action,int NB_ROBOTS, int NB_BALLS, int TYPE>
Reader<Action,NB_ROBOTS,NB_BALLS,TYPE>::Reader(std::string interface_id)
    : interface_id_(interface_id)
{
}

template <class Action,int NB_ROBOTS, int NB_BALLS, int TYPE>
WorldState<NB_ROBOTS,NB_BALLS,TYPE> Reader<Action,NB_ROBOTS,NB_BALLS,TYPE>::read_world_state() const
{
    WorldState<NB_ROBOTS,NB_BALLS,TYPE> ws;
    try
    {
        shared_memory::deserialize(interface_id_, "world_state", ws);
    }
    catch (...)
    {
    }
    return ws;
}

template <class Action,int NB_ROBOTS, int NB_BALLS, int TYPE>
Action Reader<Action,NB_ROBOTS,NB_BALLS,TYPE>::read_action() const
{
    Action action;
    try
    {
        shared_memory::deserialize(interface_id_, "action", action);
    }
    catch (...)
    {
    }
    return action;
}

