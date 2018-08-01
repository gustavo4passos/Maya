#ifndef ANIMATION_H
#define ANIMATION_H

#include "Spritesheet.h"

class Animation {
public:
    Animation(const Spritesheet& spritesheet, int nFrames);

    void Play(bool loop = false);
    void Pause();
    void PauseAtFrame(int frame);
    void Stop();
    void Reset();
    
    // The update assumes it is called at constant intervals
    void Update();
    
    void Render(Renderer* renderer, const Vector2D& pos, bool flip = false);
    void RenderFrame(Renderer* renderer, int frame, const Vector2D& pos, bool flip = false);

    inline bool IsPlaying() const { return _playing; }
    inline bool IsLooping() const { return _looping; }

private:
    Spritesheet _spritesheet;
    int _nFrames;
    int _currentFrame;
    int _speed;
    bool _playing;
    bool _looping;
    int _time;
};

#endif