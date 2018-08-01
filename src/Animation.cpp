#include "../include/Animation.h"

#include "../include/Renderer.h"

Animation::Animation(const Spritesheet& spritesheet, int nFrames) 
:   _spritesheet(spritesheet),
    _nFrames(nFrames),
    _currentFrame(0),
    _speed(20),
    _time(0)
{ }

void Animation::Update() {
    if(_playing) {
        _time++;
        _currentFrame = (_time / _speed);

        // Check if animation has finished playing
        if(_currentFrame > (_nFrames - 1)) {
            // If looping, start again
            if(_looping) {
                _currentFrame = 0;
                _time = 0;
            } // Otherwise, stop it
            else {
                Pause();
            }
        }
    }
}

void Animation::Play(bool loop) {
    _playing = true;
    _looping = loop;

    if(_currentFrame >= _nFrames) _currentFrame = 0;
}

void Animation::Pause() {
    _playing = false;
}

void Animation::PauseAtFrame(int frame) {
    Pause();
    if(frame > (_nFrames - 1)) {
        LOG_WARNING("Can't pause at frame: " + std::to_string(frame) + ". Spritesheet: " + _spritesheet.textureName() + " has only " + std::to_string(_nFrames) + " frames.");
    }
    _currentFrame = frame;
    _time = frame * _speed;
}

void Animation::Stop() {
    _currentFrame = 0;
    _playing = false;
    _time = 0;
}

void Animation::Render(Renderer* renderer, const Vector2D& pos, bool flip) {
    _spritesheet.RenderFrame(renderer, _currentFrame, pos, flip);
}

void Animation::RenderFrame(Renderer* renderer, int frame, const Vector2D& pos, bool flip) {
    _spritesheet.RenderFrame(renderer, frame, pos, flip);
}