#include "GamepadEventListener.h"
#include <iostream>

GamepadEventListener::GamepadEventListener(void): _jumpRelased(false), _jumpConfirmCounter(0){

}

GamepadEventListener::~GamepadEventListener(void){

}

void GamepadEventListener::Move(float xRatio, float yRatio){

}

void GamepadEventListener::UpdateCamera(float xRatio, float yRatio){

}

void GamepadEventListener::Stop(){

}

void GamepadEventListener::Jump(){
	std::cout << "jump pressed\n";
	if(++_jumpConfirmCounter > 5){
		std::cout << "jump pressed confirm\n";
		_jumpRelased = false;
	}
}

void GamepadEventListener::JumpRelease(){
	if(!_jumpRelased){
		_jumpConfirmCounter = 0;
		std::cout << "jump released\n";
		_jumpRelased = true;
	}
}

// testing functions
void GamepadEventListener::LockOn(){

}