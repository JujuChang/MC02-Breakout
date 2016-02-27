#include "HelloWorldScene.h"
#include "proj.win32\GameOverScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
	Vect gravity(0.0f, 0.0f); // Vector gia tốc =0
	
	scene->getPhysicsWorld()->setGravity(gravity);
	auto layer = HelloWorld::create();
	layer->setPhyWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);// Create physics body
	boundBody->getShape(0)->setRestitution(1.0f);
	boundBody->getShape(0)->setFriction(0.0f);
	boundBody->getShape(0)->setDensity(1.0f);
	edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2)); // Set the position and the centre of Box in the center of the screen
	edgeSp->setPhysicsBody(boundBody); // Set physics Body
	boundBody->setContactTestBitmask(0x000001); // This is the important command, if not available, there is nothing happening when colliding
	this->addChild(edgeSp); // Add into Layer
	edgeSp->setTag(0); // Tag==0, to check object when colliding belongs to some kind

	ball = Sprite::create("Ball.png", Rect(0, 0, 52, 52));
	ball->setPosition(100, 100);
	auto ballBody = PhysicsBody::createCircle(ball->getContentSize().width / 2.); // The physics body circle shape
	ballBody->getShape(0)->setRestitution(1.0f);
	ballBody->getShape(0)->setFriction(0.0f);
	ballBody->getShape(0)->setDensity(1.0f);
	ballBody->setGravityEnable(false); // Not set acceleration
	Vect force = Vect(1010000.0f, 1010000.0f); // Create a force Vector to act with the direction of 45 degree, because x = y 
	ballBody->applyImpulse(force); // Push a force into the ball edge
	ball->setPhysicsBody(ballBody); // Set Physics body
	ballBody->setContactTestBitmask(0x000001); //
	ball->setTag(1);
	this->addChild(ball);

	// Similar with the ball
	paddle = Sprite::create("Paddle.png");
	auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	paddleBody->getShape(0)->setRestitution(1.0f);
	paddleBody->getShape(0)->setFriction(0.0f);
	paddleBody->getShape(0)->setDensity(10.0f);
	paddleBody->setGravityEnable(false);
	paddleBody->setDynamic(false); // Set static when reacting, no restitution, no changing position
	paddle->setPosition(visibleSize.width / 2, 50);
	paddle->setPhysicsBody(paddleBody);
	paddleBody->setContactTestBitmask(0x000001); // With reaction 
	ball->setTag(2);
	this->addChild(paddle);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	for (int j = 1; j <= 4; j++){
		for (int i = 0; i < 18; i++) {
			static int padding = 5;
			auto block = Sprite::create("brick.png");
			auto brickBody = PhysicsBody::createBox(block->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			brickBody->getShape(0)->setDensity(10.0f);
			brickBody->getShape(0)->setFriction(0.0f);
			brickBody->getShape(0)->setRestitution(1.f);
			brickBody->setDynamic(false);
			// Create the distance even among the bricks
			int xOffset = padding + block->getContentSize().width / 2 +
				((block->getContentSize().width + padding)*i);
			block->setPosition(xOffset, visibleSize.height - (50 * j));
			brickBody->setContactTestBitmask(0x000001);
			block->setPhysicsBody(brickBody);
			block->setTag(3);
			this->addChild(block);
		}
	}
	
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(HelloWorld::onContactSeperate, this);
	dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->schedule(schedule_selector(HelloWorld::tick), 0);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
		"bg.wav", true);
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	return true; // Not use but must return True
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	// Not use
}

// Use to move the paddle simplest
void HelloWorld::onTouchMoved(Touch* touch, Event* event){
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
	// To be simple, use this command: Point touchLocation = touch->getLocation();
	paddle->setPositionX(touchLocation.x); // Set the position horizontal of the paddle follow the Touch point
}

bool HelloWorld::onContactBegin(PhysicsContact& contact)
{

	auto a = contact.getShapeA()->getBody();
	auto b = contact.getShapeB()->getBody();

	float* vel = new float[2];
	vel[0] = a->getVelocity().length();
	vel[1] = b->getVelocity().length();

	contact.setData(vel);

	
	return true;
}

void HelloWorld::onContactSeperate(PhysicsContact& contact) {
	PhysicsBody* bodyA = contact.getShapeA()->getBody();
	PhysicsBody* bodyB = contact.getShapeB()->getBody();

	// restore the velocity, keep the direction of the velocity.
	float* v = (float*)contact.getData();
	Vec2 velA = bodyA->getVelocity();
	Vec2 velB = bodyB->getVelocity();
	velA.normalize();
	velB.normalize();
	bodyA->setVelocity(velA * v[0]);
	bodyB->setVelocity(velB * v[1]);

	delete v;

	// Get two collided object
	auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	// Check kinds of objects
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();

	if (tagA == 3) // is brick
	{
		this->removeChild(spriteA, true);
	}

	if (tagB == 3)  // is brick
	{
		this->removeChild(spriteB, true);
	}

	if ((tagA == 0 || tagB == 0)& (ball->getPositionY() <= paddle->getPositionY()))
	{
		auto gameOverScene = GameOverScene::create();
		gameOverScene->showMessage("Thou Hast Failed!", false);
		Director::getInstance()->replaceScene(gameOverScene);
	}

}

void HelloWorld::tick(float dt)
{
	// a bool variable confirm Win game and the initial value is true;
	bool isWin = true;

	// Vector bodies get all bodies of world ( ball, edge, paddle body)
	Vector<PhysicsBody*> bodies = m_world->getAllBodies();

	// Navigate the items in the above vector, check the kinds of objects by Tag, you should study again the "for" command, it has many variants for each special kind of class, you can read advanced C++ about list, vector, queue,etc.

	for each(PhysicsBody* body in bodies) // This command will generate error when building android, you should edit as follow:  for (auto body : bodies) , this is the new standard for C++ v.11
	{
		if (body->getNode()->getTag() == 3) // If there is still  body of bricks, it means you haven't destroyed all yet.
		{
			isWin = false; // Not Win yet
		}
	}
	// If navigate all isWin but not change, process Win game
	if (isWin == true)
	{
		auto gameOverScene = GameOverScene::create();
		gameOverScene->showMessage("Thou Hast Succeeded!", true);


		//gameOverScene->getLayer()->getLabel()->setString("Thou Hast Succeeded!");
		Director::getInstance()->replaceScene(gameOverScene);
	}
}