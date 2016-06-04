#ifndef __BIRD_H__
#define __BIRD_H__

#include "cocos2d.h"

USING_NS_CC;

class Player : public Sprite
{
public:
	CREATE_FUNC(Player);
	virtual bool init();

    void Fall( );
    void Fly( ) { isFalling = false; };
    void StopFlying( ) { isFalling = true; }
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    bool isFalling;
    
};

#endif // __BIRD_H__
