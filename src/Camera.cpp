#include "../include/Camera.h"

Camera::Camera(Rect focusAt, float fovw, float fovh, int fovleft, int fovright, int fovtop, int fovbottom) : 
    _focusPoint(focusAt),
    _fovw(fovw),
    _fovh(fovh),
	_fovleft(fovleft),
	_fovright(fovright),
	_fovtop(fovtop),
	_fovbottom(fovbottom)
{ 
}

void Camera::FocusAt(const Rect& center){
	_focusPoint = center;	
}

void Camera::Update(){
	FindCenter();
	_pos.setX(_center.x() - _fovw / 2);
	_pos.setY(_center.y() - _fovh / 2);
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
  	_center.setX(_focusPoint.x() + (_focusPoint.w() / 2.f));
	_center.setY(_focusPoint.y() - (_focusPoint.h() / 2.f));
}

