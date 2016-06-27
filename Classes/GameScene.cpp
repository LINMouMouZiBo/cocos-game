#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics( );
    //scene->getPhysicsWorld( )->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -WORLD_GRAVITY));
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());

	//��ӹؽ�
	Sprite * last_sp = NULL;
	PhysicsBody * last_bd = NULL;
	last_sp = Sprite::create();
	last_sp->setTextureRect(Rect(0, 0, 10, 10));
	last_bd = PhysicsBody::createCircle(last_sp->getContentSize().width / 2, PhysicsMaterial(1.0f, 0.0f, 0.0f));
	last_bd->setRotationEnable(false);
	last_bd->setMass(1);
	last_sp->setPhysicsBody(last_bd);
	last_sp->setPosition(layer->player->getPositionX(), layer->player->getPositionY() - 20);
	layer->addChild(last_sp);

	auto first_joint = PhysicsJointDistance::construct(layer->player->getPhysicsBody(), last_sp->getPhysicsBody(), Point::ZERO, Point::ZERO);
	layer->sceneWorld->addJoint(first_joint);

	// ѭ����ӹؽ�
	Sprite * new_sp = NULL;
	PhysicsBody * new_bd = NULL;
	for (int i = 0; i < 6; i++)
	{
		new_sp = Sprite::create();
		new_sp->setTextureRect(Rect(0, 0, 10, 10));
		new_bd = PhysicsBody::createCircle(new_sp->getContentSize().width / 2, PhysicsMaterial(1.0f, 0.0f, 0.0f));
		new_bd->setRotationEnable(false);
		new_bd->setMass(1);
		new_sp->setPhysicsBody(new_bd);
		new_sp->setPosition(last_sp->getPositionX(), last_sp->getPositionY() - 10);
		layer->addChild(new_sp);

		auto joint = PhysicsJointDistance::construct(last_sp->getPhysicsBody(), new_sp->getPhysicsBody(), Point::ZERO, Point::ZERO);
		layer->sceneWorld->addJoint(joint);
		last_sp = new_sp;
	}

	auto last_joint = PhysicsJointDistance::construct(last_sp->getPhysicsBody(), layer->package->getPhysicsBody(), Point::ZERO, Point::ZERO);
	layer->sceneWorld->addJoint(last_joint);

	// ͨ��PhysicsJointGroove����С���˶������ǵ���Ϸ�ԣ����
	/*auto origin = Sprite::create("package.png");
	origin->setPosition(layer->visibleSize.width/2, layer->visibleSize.height/2);
	origin->setPhysicsBody(PhysicsBody::createCircle(10));
	origin->getPhysicsBody()->setDynamic(false);
	origin->getPhysicsBody()->setCollisionBitmask(0x00);
	origin->getPhysicsBody()->setContactTestBitmask(0x00);
	layer->addChild(origin);
	auto joint = PhysicsJointGroove::construct(origin->getPhysicsBody(), layer->player->getPhysicsBody(), Vec2(0, -layer->visibleSize.height / 2), Vec2(0, layer->visibleSize.height / 2), Vec2::ZERO);
	joint->setCollisionEnable(false);
	layer->sceneWorld->addJoint(joint);*/

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
	
	//����
	preloadMusic();
	playBgm();

    auto backgroundSprite = Sprite::create( "bg.jpg" );
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size size = Director::getInstance()->getWinSize();
	backgroundSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	float winw = size.width; //��ȡ��Ļ���
	float winh = size.height;//��ȡ��Ļ�߶�

	float spx = backgroundSprite->getTextureRect().getMaxX();
	float spy = backgroundSprite->getTextureRect().getMaxY();

	backgroundSprite->setScaleX(winw / spx); //���þ��������ű���
	backgroundSprite->setScaleY(winh / spy);

    this->addChild( backgroundSprite );
    
    auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
    edgeBody->setCollisionBitmask(ENEMY_SPAWN_FREQUENCY);
    edgeBody->setContactTestBitmask( true );
    
	// ��Ļ�߽�
    auto edgeNode = Node::create();
    edgeNode->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
    
    edgeNode->setPhysicsBody( edgeBody );
    
    this->addChild( edgeNode );
    
	// ׼��
	scheduleOnce(schedule_selector(GameScene::ready), 5.0f);
	readyLabel = Label::createWithTTF("Ready", "fonts/Marker Felt.ttf", 50);
	readyLabel->setPosition(-100, visibleSize.height - 100);
	addChild(readyLabel);
	readyLabel->runAction(MoveTo::create(1.0f, Vec2(visibleSize.width / 2, visibleSize.height - 100)));

	//��һֻ����
    player = Player::create();
	addChild(player);

	//����Ҫ��ݵİ���
	package = Sprite::create("package.png");
	package->setPosition(player->getPosition()-Vec2(15, 112));
	package->setPhysicsBody(PhysicsBody::createBox(package->getContentSize()));
	package->getPhysicsBody()->setMass(PACKAGE_MASS);
	package->getPhysicsBody()->setRotationEnable(false);
	package->setScale(0.5);
	// bitmask
	package->getPhysicsBody()->setCategoryBitmask(0x05);
	package->getPhysicsBody()->setCollisionBitmask(0x06);
	package->getPhysicsBody()->setContactTestBitmask(0x00);

	// ��ʼ������
	player->getPhysicsBody()->applyForce(Vec2(INTERACTION_FORCE, (PLAYER_MASS + PACKAGE_MASS)*WORLD_GRAVITY + 5000));
	package->getPhysicsBody()->applyForce(Vec2(-INTERACTION_FORCE, 0));

	addChild(package);

	//����ĳ־�ʱ��
    score = 0;
	scoreLock = 10;
    
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/Marker Felt.ttf";
	ttfConfig.fontSize = 36;
    
    scoreLabel = Label::createWithTTF(ttfConfig, "0");
    scoreLabel->setColor( Color3B::WHITE );
    scoreLabel->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height * 0.9 + origin.y ) );
    
    this->addChild( scoreLabel, 10000 );
    
	//����񵽵�Ǯ��
	moneyCount = 0;
	MoneyLabel = Label::createWithTTF(ttfConfig, "0");
	MoneyLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100, visibleSize.height * 0.9 + origin.y));
	addChild(MoneyLabel, 10000);

    this->scheduleUpdate( );
    
    return true;
}

//Ԥ������Ϸ��Ҫ������
void GameScene::preloadMusic() {
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/Point.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/Wing.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/Hit.mp3");
}

//���ű�������
void GameScene::playBgm() {
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
}

// ׼���׶�
void GameScene::ready(float dt) {
	player->getPhysicsBody()->applyForce(Vec2(0, -(PLAYER_MASS + PACKAGE_MASS)*WORLD_GRAVITY + 5000));
	readyLabel->setString(Value("Go").asString());
	readyLabel->setPosition(Vec2(visibleSize.width / 2, readyLabel->getPositionY()+50));
	readyLabel->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, - 50)), FadeOut::create(2.0f), NULL));

	//������ע��
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	//������ָ����谭��
	this->schedule(schedule_selector(GameScene::newEnemy), ENEMY_SPAWN_FREQUENCY * visibleSize.width);

	//ÿ��3��������ֽ��
	this->schedule(schedule_selector(GameScene::newMoney), MONEY_SPAWN_FREQUENCY);

}

//ÿ��3��������ֽ��
void GameScene::newMoney(float dt) {
	Vector<Node*> moneys = MoneyGenerator::getInstance()->GenerateMoney();
	for (int i = 0; i < moneys.size(); i++) {
		this->addChild(moneys.at(i));
	}
}

//��������ϰ���
void GameScene::newEnemy( float dt )
{
	this->addChild(EnemyGenerator::getInstance()->GenerateEnemy());
}

bool GameScene::onContactBegin( cocos2d::PhysicsContact &contact )
{
	Node * a = contact.getShapeA()->getBody()->getNode();
    Node * b = contact.getShapeB()->getBody()->getNode();
    
    if (a->getTag() == ENEMY_TAG && b->getTag() == PLAYER_TAG || b->getTag() == ENEMY_TAG && a->getTag() == PLAYER_TAG)
    {         //�����ϰ���ײ��
		SimpleAudioEngine::getInstance()->playEffect("music/Hit.mp3");
		this->unscheduleAllSelectors();
		delete MoneyGenerator::getInstance();
		delete EnemyGenerator::getInstance();
		auto scene = GameOverScene::createScene( score, moneyCount );
        Director::getInstance( )->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
	} else if (a->getTag() == MONEY_TAG && b->getTag() == PLAYER_TAG) { //�����Ǯ
		SimpleAudioEngine::getInstance()->playEffect("music/Point.mp3");
		auto ps = ParticleSystemQuad::create("explode.plist");
		ps->setPosition(b->getPosition());
		this->addChild(ps);
		MoneyGenerator::getInstance()->obtainMoney(a);
		moneyCount++;
		MoneyLabel->setString(Value(moneyCount).asString());
	} else if (b->getTag() == MONEY_TAG && a->getTag() == PLAYER_TAG) { //�����Ǯ
		SimpleAudioEngine::getInstance()->playEffect("music/Point.mp3");
		auto ps = ParticleSystemQuad::create("explode.plist");
		ps->setPosition(b->getPosition());
		this->addChild(ps);
		MoneyGenerator::getInstance()->obtainMoney(b);
		moneyCount++;
		MoneyLabel->setString(Value(moneyCount).asString());
	}
    
    return true;
}

bool GameScene::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event )
{
	SimpleAudioEngine::getInstance()->playEffect("music/Wing.mp3");
	player->Fly();
    return true;
}

//�ո񣬻س����ϼ�ͷ�������Կ��ƴ������Ϸ�
void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW
		|| keyCode == EventKeyboard::KeyCode::KEY_ENTER
		|| keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
		SimpleAudioEngine::getInstance()->playEffect("music/Wing.mp3");
		player->Fly();
	}
}

//��ʱ����ÿ֡���п���
void GameScene::update( float dt )
{
	//��ʱ����ÿ֡���м�ʱ ����dt��֤��ʱ����Ϊ��������֡��
	if (scoreLock <= 0) {
		scoreLock = 10;
		score += 1;
		scoreLabel->setString(String::createWithFormat("%i", score)->_string);
	}
	else {
		scoreLock -= dt*100;
	}
	CCLOG("time %d", dt);
	//ÿ֡�ж��ϰ���ͽ�� �Ƿ��ƶ�����Ļ֮�⣬�Ǿ��Ƴ�
	EnemyGenerator::getInstance()->removeEnemys();
	MoneyGenerator::getInstance()->removeMoney();

	// �Զ��������λ��
	if (player->getPositionX() < visibleSize.width / 2 - 10) {
		player->setPositionX(player->getPositionX()+0.5);
	}
	else if (player->getPositionX() > visibleSize.width / 2 + 10) {
		player->setPositionX(player->getPositionX() - 0.5);
	}

	// ��ݰ���λ���޸�
	if (package->getPositionY() > player->getPositionY()) {
		package->setPositionX(package->getPositionX() - 1);
	}

	//�������λ������Ļ��Ե֮�⣬Ҳ���ǵ��أ���Ҫ��
	if (player->getPosition().y < -10 || player->getPosition().y > visibleSize.height + 50) {
		this->unscheduleAllSelectors();
		delete MoneyGenerator::getInstance();
		delete EnemyGenerator::getInstance();
		auto scene = GameOverScene::createScene(score, moneyCount);
		Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
	}

}
