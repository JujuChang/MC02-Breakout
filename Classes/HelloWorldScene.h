#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "proj.win32\Tail.h"

class HelloWorld : public cocos2d::Layer
{
public:
	Vec2 origin;
	Size visibleSize;
	Sprite* ball;
	Sprite* paddle;
	Sprite* edgeSp;
	PhysicsWorld* m_world;


public:
	
	void setPhyWorld(PhysicsWorld* world){ m_world = world; };
	// Touch event
	void onTouchMoved(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);

	bool onTouchBegan(Touch *touch, Event *event);
	bool onContactBegin(PhysicsContact& contact);
	void onContactSeperate(PhysicsContact& contact);
	void tick(float dt);

    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
