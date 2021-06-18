#pragma once
#include <BaseButton.h>
template<class StateType>
class Button : public BaseButton {
public:
	using BaseButton::BaseButton;
	std::unique_ptr<State> ButtonState(StateManager&, sf::RenderWindow&, bool, std::shared_ptr<NetworkObject>) const override;
private:
};

template<class StateType>
std::unique_ptr<State> Button<StateType>::ButtonState(StateManager& manager, sf::RenderWindow& window, bool replace, std::shared_ptr<NetworkObject> netObj) const {
	return StateManager::build<StateType>(manager, window, replace, netObj);
}