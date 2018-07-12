#ifndef EVENT_H
#define EVENT_H



class Event{

public:
    Event(){}
    virtual ~Event(){}
    virtual void Activate() = 0;
    inline bool active(){ return _active; }
    inline bool _onTouch(){ return _onTouch; }

private:
    bool _active;
    bool _onTouch;
};

#endif