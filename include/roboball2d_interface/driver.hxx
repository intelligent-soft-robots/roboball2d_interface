

template <class Action,int NB_ROBOTS, int NB_BALLS,int TYPE>
Driver<Action,NB_ROBOTS,NB_BALLS,TYPE>::Driver(std::string interface_id)
    : interface_id_(interface_id), writer_(interface_id), reader_(interface_id)
{
}

template <class Action,int NB_ROBOTS, int NB_BALLS,int TYPE>
void Driver<Action,NB_ROBOTS,NB_BALLS,TYPE>::initialize()
{
}

template <class Action,int NB_ROBOTS, int NB_BALLS,int TYPE>
Action Driver<Action,NB_ROBOTS,NB_BALLS,TYPE>::apply_action(const Action &desired_action)
{
    std::lock_guard<std::mutex> guard(mutex_);
    if (desired_action.is_valid())
	{
	    writer_.write_action(desired_action);
	}
    return desired_action;
}

template <class Action,int NB_ROBOTS, int NB_BALLS,int TYPE>
WorldState<NB_ROBOTS,NB_BALLS,TYPE> Driver<Action,NB_ROBOTS,NB_BALLS,TYPE>::get_latest_observation()
{
    WorldState<NB_ROBOTS,NB_BALLS,TYPE> ws = reader_.read_world_state();
    return ws;
}

template <class Action,int NB_ROBOTS, int NB_BALLS,int TYPE>
void Driver<Action,NB_ROBOTS,NB_BALLS,TYPE>::shutdown()
{
}

template <class Action,int NB_ROBOTS, int NB_BALLS,int TYPE>
std::string Driver<Action,NB_ROBOTS,NB_BALLS,TYPE>::get_error()
{
    return "";
}
