#include "../include/Camera.h"

#include "../include/ErrorHandler.h"
#include "../include/EventDispatcher.h"
#include "../include/GameObject.h"
#include "../include/LevelChangedEvent.h"

Camera::Camera(float fovw, float fovh, int fovleft, int fovright, int fovtop, int fovbottom, GameObject* subject) 
:    _subject(subject),
	_pos(0.f, 0.f),
	_center(0.f, 0.f),
	_velocity(0.f, 0.f),
    _fovw(fovw),
    _fovh(fovh),
	_fovleft(fovleft),
	_fovright(fovright),
	_fovtop(fovtop),
	_fovbottom(fovbottom)
{
	EventDispatcher::AddListener(this, EventType::LEVEL_CHANGED);
}

Camera::~Camera() {
	EventDispatcher::RemoveListener(this, EventType::LEVEL_CHANGED);
}

bool Camera::OnNotify(Event* event) {
	const Level* newLevel = dynamic_cast<LevelChangedEvent*>(event)->newLevel();
	_fovright = newLevel->width() * newLevel->tileWidth();
	_fovbottom = newLevel->height() * newLevel->tileHeight();
	std::cout << _fovright << " ";
	std::cout << _fovbottom;
	std::cin.get();
	return false;
}

void Camera::FocusAt(const GameObject* subject){
	_subject = subject;
}

void Camera::Update(){
	FindCenter();

	_velocity = Vector2D(_center.x() - _fovw / 2, _center.y() - _fovh / 2) - _pos;
	_pos += _velocity;
	
	if(_pos.x() < _fovleft) _pos.setX(_fovleft);
	if(_pos.x() > _fovright - _fovw) _pos.setX(_fovright - _fovw);
	if(_pos.y() < _fovtop) _pos.setY(_fovtop);
	if(_pos.y() > _fovbottom - _fovh) _pos.setY(_fovbottom - _fovh);
}

void Camera::SetFieldOfView(int fovleft, int fovright, int fovtop, int fovbottom){
	_fovleft = fovleft;
	_fovright = fovright;
	_fovtop = fovtop;
	_fovbottom = fovbottom;
}

void Camera::FindCenter(){
	if(_subject){
  		_center.setX(_subject->x() + (_subject->w() / 2.f));
		_center.setY(_subject->y() + (_subject->h() / 2.f));
	}
	else {
		_center.setX(_fovw / 2);
		_center.setY(_fovh / 2);
	}
}
