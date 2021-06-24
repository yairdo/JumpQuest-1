#pragma once
#include <BaseButton.h>

template<class StateType>
class Button : public BaseButton {
public:
	using BaseButton::BaseButton;

	//----------Function---------
	std::unique_ptr<State> ButtonState(StateManager&, sf::RenderWindow&, bool,
		std::shared_ptr<NetworkObject>) const override;
private:
};
//-----------------------------------------------------------------------------
/*
	Function: button state
	This function is turn on the wanted state.
*/
template<class StateType>
std::unique_ptr<State> Button<StateType>::ButtonState(StateManager& manager,
	sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> netObj) const {
	return StateManager::build<StateType>(manager, window, replace, netObj);
}