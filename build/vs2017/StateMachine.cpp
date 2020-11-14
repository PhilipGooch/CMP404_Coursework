#include "StateMachine.h"
#include <audio/audio_manager.h>
#include "MenuState.h"
#include "GameState.h"
#include "OptionsState.h"

StateMachine::StateMachine()
{
	game_state_ = new GameState();
	menu_state_ = new MenuState();
	options_state_ = new OptionsState();

	SetState(State::TYPE::MENU);
}

StateMachine::~StateMachine()
{
}

void StateMachine::SetState(State::TYPE type)
{
	
	switch (type)
	{
	case State::TYPE::GAME:
		state_ = game_state_;
		break;
	case State::TYPE::MENU:
		state_ = menu_state_;
		break;
	case State::TYPE::OPTIONS:
		state_ = options_state_;
		break;
	}

	
}
