#include "Player.h"
#include "Definitions.h"

USING_NS_CC;

bool Player::init() {
	if (!Sprite::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	setTag(PLAYER_TAG);

	setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// ��ҵĸ���
	auto flappyBody = PhysicsBody::createCircle(18);
	flappyBody->setPositionOffset(Vec2(15, 12));
	flappyBody->setRotationEnable(false);
	flappyBody->setAngularVelocityLimit(0);
	flappyBody->setMass(PLAYER_MASS);
	// bitmask
	flappyBody->setCategoryBitmask(0x05);
	flappyBody->setCollisionBitmask(0x06);
	flappyBody->setContactTestBitmask(0x02);

	setAnchorPoint(Vec2(1, 1));
	setPhysicsBody(flappyBody);

	runAction(Animate::create(AnimationCache::getInstance()->getAnimation("flying_bird")));

	// ������������ʵ�������󣬴˱�������Ҫ
	isFalling = true;
	speed = 0;
	accelerate = -0.0002;

	return true;
}
//�������µ����µ����̿������������� ������������ʵ�������󣬴˺�������Ҫ
void Player::Fall()
{
	speed += accelerate;

	setPositionX(visibleSize.width / 2 + origin.x);
	setPositionY(getPositionY() + (speed * visibleSize.height));
}
//�������Ϸ�
void Player::Fly()
{
	speed = 0.005;  // ������������ʵ�������󣬴˱�������Ҫ
	getPhysicsBody()->setVelocity(Vec2(0, 400));
	//getPhysicsBody()->applyImpulse(Vec2(0, 100000));
}
