﻿#include "ArgonGui.h"
#include <algorithm>

int GetGamepadEventIndex(const ArInputEvent& event)
{
	if (const auto& data = std::get_if<ArGamepadButtonEventData>(&event.eventData))
	{
		return data->index;
	}
	else if (const auto& data = std::get_if<ArGamepadLeftStickEventData>(&event.eventData))
	{
		return data->index;
	}
	else if (const auto& data = std::get_if<ArGamepadRightStickEventData>(&event.eventData))
	{
		return data->index;
	}
	else if (const auto& data = std::get_if<ArGamepadLeftTriggerEventData>(&event.eventData))
	{
		return data->index;
	}
	else if (const auto& data = std::get_if<ArGamepadRightTriggerEventData>(&event.eventData))
	{
		return data->index;
	}
	return -1;
}

void ArMouseState::HandleEvent(const ArInputEvent& event)
{
	if (const auto& data = std::get_if<ArMouseMoveEventData>(&event.eventData))
	{
		lastPosition = position;
		position = data->position;
		delta = lastPosition - position;
	}
	else if (const auto& data = std::get_if<ArMouseButtonEventData>(&event.eventData))
	{
		ArMouseButton button = data->button;
		ArMouseButtonState& buttonState = buttonStates[button];

		buttonState.pressing = data->pressing;

		if (data->pressing)
		{
			buttonState.pressStates.emplace_back(ArHelp::GetTimePoint(), position, ArInputAction::Down);
			buttonState.frameAction = ArInputAction::Down;
		}
		else
		{
			buttonState.pressStates.emplace_back(ArHelp::GetTimePoint(), position, ArInputAction::Up);
			buttonState.frameAction = ArInputAction::Up;
		}
	}
	else if (const auto& data = std::get_if<ArMouseWheelEventData>(&event.eventData))
	{
		wheelDelta = data->wheelDelta;
	}
}

void ArMouseState::EndFrame()
{
	for (auto& [_, state] : buttonStates)
	{
		state.frameAction = ArInputAction::None;
		int pairCount = 0;
		int popCount = 0;
		ArTimePoint currentTime = ArHelp::GetTimePoint();
		state.pressStates.erase(std::remove_if(state.pressStates.begin(), state.pressStates.end(),
			[currentTime, &pairCount, &popCount](const ArMouseButtonState::PressedState& state) {
				if (state.frameAction == ArInputAction::Down)
				{
					pairCount++;
					return false;
				}
				else if (state.frameAction == ArInputAction::Up)
				{
					if (pairCount > 0)
					{
						pairCount--;
						return false;
					}
					else
					{
						popCount++;
						return true;
					}
				}
				return pairCount == popCount && (currentTime - state.pressTime) > ArgonInputManager::MaxClickInterval;
			}), state.pressStates.end());
	}

	delta = ArVec2();
	wheelDelta = ArVec2();
}

void ArKeyboardState::HandleEvent(const ArInputEvent& event)
{
	if (const auto& data = std::get_if<ArKeyboardEventData>(&event.eventData))
	{
		ArKeyCode keyCode = data->keyCode;
		ArButtonState& keyState = keyStates[keyCode];

		keyState.pressing = data->pressing;

		if (data->pressing)
		{
			keyState.pressTimes.push_back(ArHelp::GetTimePoint());
			keyState.frameAction = ArInputAction::Down;
		}
		else
		{
			keyState.frameAction = ArInputAction::Up;
		}
	}
}

void ArKeyboardState::EndFrame()
{
	for (auto& [_, keyState] : keyStates)
	{
		keyState.frameAction = ArInputAction::None;
		ArTimePoint currentTime = ArHelp::GetTimePoint();
		keyState.pressTimes.erase(std::remove_if(keyState.pressTimes.begin(), keyState.pressTimes.end(),
			[currentTime](const ArTimePoint& time) {
				return (currentTime - time) > ArgonInputManager::MaxClickInterval;
			}), keyState.pressTimes.end());
	}
}

void ArGamepadState::HandleEvent(const ArInputEvent& event)
{
	if (const auto& data = std::get_if<ArGamepadButtonEventData>(&event.eventData))
	{
		ArGamepadButton button = data->button;
		ArButtonState& buttonState = buttonStates[button];
		buttonState.pressing = data->pressing;
		if (data->pressing)
		{
			buttonState.pressTimes.push_back(ArHelp::GetTimePoint());
			buttonState.frameAction = ArInputAction::Down;
		}
		else
		{
			buttonState.frameAction = ArInputAction::Up;
		}
	}
	else if (const auto& data = std::get_if<ArGamepadLeftStickEventData>(&event.eventData))
	{
		leftStick = data->leftStick;
	}
	else if (const auto& data = std::get_if<ArGamepadRightStickEventData>(&event.eventData))
	{
		rightStick = data->rightStick;
	}
	else if (const auto& data = std::get_if<ArGamepadLeftTriggerEventData>(&event.eventData))
	{
		leftTrigger = data->leftTrigger;
	}
	else if (const auto& data = std::get_if<ArGamepadRightTriggerEventData>(&event.eventData))
	{
		rightTrigger = data->rightTrigger;
	}
	else if (const auto& data = std::get_if<ArGamepadConnectionEventData>(&event.eventData))
	{
		enabled = data->connected;
	}
}

void ArGamepadState::EndFrame()
{
	for (auto& [_, state] : buttonStates)
	{
		state.frameAction = ArInputAction::None;
		ArTimePoint currentTime = ArHelp::GetTimePoint();
		state.pressTimes.erase(std::remove_if(state.pressTimes.begin(), state.pressTimes.end(),
			[currentTime](const ArTimePoint& time) {
				return (currentTime - time) > ArgonInputManager::MaxClickInterval;
			}), state.pressTimes.end());
	}
}

void ArDisplayState::HandleEvent(const ArInputEvent& event)
{
	const auto& data = std::get<ArDisplayEventData>(event.eventData);
	position = data.position;
	size = data.size;
}

void ArgonInputManager::AddEvent(const ArInputEvent& event)
{
	std::lock_guard<std::mutex> lock(eventMutex);
	eventBuffer.push(event);
}

void ArgonInputManager::StartFrame()
{
	{
		std::lock_guard<std::mutex> lock(eventMutex);
		while (!eventBuffer.empty())
		{
			eventQueue.push(eventBuffer.front());
			eventBuffer.pop();
		}
	}

	while (!eventQueue.empty())
	{
		ArInputEvent event = eventQueue.front();
		eventQueue.pop();
		ParseEvent(event);
	}
}

void ArgonInputManager::EndFrame()
{
	mouseState.EndFrame();
	keyboardState.EndFrame();
	for (auto& gamepadState : gamepadStates)
	{
		if (!gamepadState.enabled)
			continue;
		gamepadState.EndFrame();
	}
}

void ArgonInputManager::ParseEvent(const ArInputEvent& event)
{
	int gamepadIndex = 0;

	switch (event.deviceType)
	{
	case ArInputDevice::Mouse:
		mouseState.enabled = true;
		mouseState.HandleEvent(event);
		return;
	case ArInputDevice::Keyboard:
		keyboardState.enabled = true;
		keyboardState.HandleEvent(event);
		return;
	case ArInputDevice::Gamepad:
		gamepadIndex = GetGamepadEventIndex(event); 
		if (gamepadIndex < 0 || gamepadIndex >= gamepadStates.size())
			return;
		gamepadStates[gamepadIndex].enabled = true;
		gamepadStates[gamepadIndex].HandleEvent(event);
		return;
	case ArInputDevice::Display:
		displayState.HandleEvent(event);
		return;
	default:
		return;
	}

	if (event.deviceType != ArInputDevice::Display)
		lastInputDevice = event.deviceType;
}

bool ArgonInputManager::IsMouseButtonDown(ArMouseButton button) const
{
	auto it = mouseState.buttonStates.find(button);
	return it != mouseState.buttonStates.end() && it->second.frameAction == ArInputAction::Down;
}

bool ArgonInputManager::IsMouseButtonHeld(ArMouseButton button) const
{
	auto it = mouseState.buttonStates.find(button);
	return it != mouseState.buttonStates.end() && it->second.pressing;
}

bool ArgonInputManager::IsMouseButtonUp(ArMouseButton button) const
{
	auto it = mouseState.buttonStates.find(button);
	return it != mouseState.buttonStates.end() && it->second.frameAction == ArInputAction::Up;
}

bool ArgonInputManager::IsMouseButtonClicked(ArMouseButton button, int clickCount) const
{
	auto it = mouseState.buttonStates.find(button);
	if (it == mouseState.buttonStates.end())
		return false;

	clickCount += 1;

	auto& times = it->second.pressStates;
	size_t count = times.size();
	if (count < static_cast<size_t>(clickCount))
		return false;

	ArMouseButtonState::PressedState lastPressedState = times[count - 1];
	ArMouseButtonState::PressedState firstPressedState = times[count - clickCount];
	ArDuration interval = lastPressedState.pressTime - firstPressedState.pressTime;
	float distance = (lastPressedState.pressPosition - firstPressedState.pressPosition).Length();

	if (it->second.frameAction != ArInputAction::Up)
		return false;

	return (clickCount > 2 ? interval < MaxClickInterval : true) && distance < MouseClickedDeltaAllowance && firstPressedState.frameAction == ArInputAction::Down;
}

ArVec2 ArgonInputManager::GetMousePosition() const
{
	return mouseState.position;
}

ArVec2 ArgonInputManager::GetMouseDelta() const
{
	return mouseState.delta;
}

ArVec2 ArgonInputManager::GetMouseWheelDelta() const
{
	return mouseState.wheelDelta;
}

bool ArgonInputManager::IsKeyDown(ArKeyCode keyCode) const
{
	auto it = keyboardState.keyStates.find(keyCode);
	return it != keyboardState.keyStates.end() && it->second.frameAction == ArInputAction::Down;
}

bool ArgonInputManager::IsKeyHeld(ArKeyCode keyCode) const
{
	auto it = keyboardState.keyStates.find(keyCode);
	return it != keyboardState.keyStates.end() && it->second.pressing;
}

bool ArgonInputManager::IsKeyUp(ArKeyCode keyCode) const
{
	auto it = keyboardState.keyStates.find(keyCode);
	return it != keyboardState.keyStates.end() && it->second.frameAction == ArInputAction::Up;
}

bool ArgonInputManager::IsKeyCombinationPressed(const std::vector<ArKeyCode>& keys) const
{
	for (size_t i = 0; i < keys.size(); i++)
	{
		ArKeyCode keyCode = keys[i];
		if (i == keys.size() - 1 && !IsKeyDown(keyCode))
			return false;
		else if (!IsKeyHeld(keyCode))
			return false;
	}
	return true;
}

bool ArgonInputManager::IsGamepadButtonDown(ArGamepadButton button, int index) const
{
	if (index < 0 || index >= gamepadStates.size())
		return false;
	if (!gamepadStates[index].enabled)
		return false;
	auto it = gamepadStates[index].buttonStates.find(button);
	return it != gamepadStates[index].buttonStates.end() && it->second.frameAction == ArInputAction::Down;
}

bool ArgonInputManager::IsGamepadButtonHeld(ArGamepadButton button, int index) const
{
	if (index < 0 || index >= gamepadStates.size())
		return false;
	if (!gamepadStates[index].enabled)
		return false;
	auto it = gamepadStates[index].buttonStates.find(button);
	return it != gamepadStates[index].buttonStates.end() && it->second.pressing;
}

bool ArgonInputManager::IsGamepadButtonUp(ArGamepadButton button, int index) const
{
	auto it = gamepadStates[index].buttonStates.find(button);
	return it != gamepadStates[index].buttonStates.end() && it->second.frameAction == ArInputAction::Up;
}

bool ArgonInputManager::IsGamepadConnected(int index) const
{
	if (index < 0 || index >= gamepadStates.size())
		return false;
	return gamepadStates[index].enabled;
}

ArVec2 ArgonInputManager::GetGamepadLeftStick(int index) const
{
	if (index < 0 || index >= gamepadStates.size())
		return ArVec2();
	if (!gamepadStates[index].enabled)
		return ArVec2();
	return gamepadStates[index].leftStick;
}

ArVec2 ArgonInputManager::GetGamepadRightStick(int index) const
{
	if (index < 0 || index >= gamepadStates.size())
		return ArVec2();
	if (!gamepadStates[index].enabled)
		return ArVec2();
	return gamepadStates[index].rightStick;
}

float ArgonInputManager::GetGamepadLeftTrigger(int index) const
{
	if (index < 0 || index >= gamepadStates.size())
		return 0.0f;
	if (!gamepadStates[index].enabled)
		return 0.0f;
	return gamepadStates[index].leftTrigger;
}

float ArgonInputManager::GetGamepadRightTrigger(int index) const
{
	if (index < 0 || index >= gamepadStates.size())
		return 0.0f;
	if (!gamepadStates[index].enabled)
		return 0.0f;
	return gamepadStates[index].rightTrigger;
}

size_t ArgonInputManager::GetGamepadConnectedCount() const
{
	size_t count = 0;
	for (const auto& gamepadState : gamepadStates)
	{
		if (gamepadState.enabled)
			count++;
	}
	return count;
}

ArDisplayState ArgonInputManager::GetDisplayState() const
{
	return displayState;
}
