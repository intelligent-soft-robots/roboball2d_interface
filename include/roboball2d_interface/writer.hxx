
template <class Action>
Writer<Action>::Writer(std::string interface_id) : interface_id_(interface_id)
{
}

template <class Action>
void Writer<Action>::clear(std::string interface_id)
{
    shared_memory::clear_shared_memory(interface_id);
}

template <class Action>
void Writer<Action>::write_world_state(const WorldState& world_state) const
{
    shared_memory::serialize(interface_id_, "world_state", world_state);
}

template <class Action>
void Writer<Action>::write_action(const Action& action) const
{
    shared_memory::serialize(interface_id_, "action", action);
}



