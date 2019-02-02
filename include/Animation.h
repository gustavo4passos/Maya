#ifndef ANIMATION_H
#define ANIMATION_H

#include "Spritesheet.h"

class Animation {
public:
    Animation(const Spritesheet& spritesheet, int nFrames, int speed = 20);
	Animation(const Animation& a);

    // Start playing the animation
    void Play(bool loop = false);
    // Pauses the animation at the current frame
    void Pause();

    // Pauses the animation at a certain frame
    // the animation can be resumed by calling "Play()"
    void PauseAtFrame(int frame);

    // Stops playing the animation and resets to the first frame
    void Stop();
    // Resets animation to the first frame and continue playing
    void Reset();
    
    // The update assumes it is called at constant intervals
    // It MUST be called every frame
    void Update();
    
    void Render(Renderer* renderer, const Vector2D& pos, bool flip = false);
    void RenderFrame(Renderer* renderer, int frame, const Vector2D& pos, bool flip = false);

    inline bool IsPlaying() const { return _playing; }
    inline bool IsLooping() const { return _looping; }

    // Marks that the animation has finished playing
    // If looping, this is never set
    inline bool IsDone() const { return _done; }

private:
    Spritesheet _spritesheet;
    int _nFrames;
    int _currentFrame;
    int _speed;
    bool _playing;
    bool _looping;
    bool _done;
    int _time;
};

#endif
