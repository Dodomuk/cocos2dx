/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "HelloMenu.h"

USING_NS_CC;
using namespace CocosDenshion;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}
Size winSize;
Sprite* Dragon;
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	MusicStart();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	initData();
	initBG();

	initPlayer();
	initScore();
	initHP();
	this->scheduleUpdate();
	this->schedule(schedule_selector(HelloWorld::setMissile), 0.2);
	this->schedule(schedule_selector(HelloWorld::setItem), 5.0+rand()%4);
	this->schedule(schedule_selector(HelloWorld::setEnemy), 1);
    return true;
}


//ó�� ����� ����Ʈ�� ���� �⺻���·� ����//
void HelloWorld::initData()
{
	winSize = Director::getInstance()->getWinSize();
	items.clear();
	enemies.clear();
	enemiesnamed.clear();
	missiles.clear();
	isGetItem = false;
	PlayerHp = 3;

	nScore = 0;
	nScoreHigh = UserDefault::getInstance()->getIntegerForKey("HIGH_SCORE", 0);
}


//���� ���//
void HelloWorld::initBG()
{
	auto bgLayer = Layer::create();
	this->addChild(bgLayer);

	auto spr_1_0 = Sprite::create("res/spaces.png");
	spr_1_0->setAnchorPoint(Point::ZERO);
	spr_1_0->setPosition(Point::ZERO);
	bgLayer->addChild(spr_1_0);

	auto spr_1_1 = Sprite::create("res/spaces.png", Rect(0, 0, 550, 780));
	spr_1_1->setAnchorPoint(Point::ZERO);
	spr_1_1->setPosition(Point(0, spr_1_0->getContentSize().height));
	bgLayer->addChild(spr_1_1);

	auto action_0 = MoveBy::create(5.0, Point(0, -780));
	auto action_1 = Place::create(Point::ZERO);
	auto action_2 = Sequence::create(action_0, action_1, NULL);
	auto action_3 = RepeatForever::create(action_2);

	bgLayer->runAction(action_3);
}

//�÷��̾� �巡�� ����//
void HelloWorld::initPlayer()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dragon.plist");
	Dragon = Sprite::createWithSpriteFrameName("dragon_1.png");
	Dragon->setPosition(Vec2(240, 50));
	Dragon->setTag(TAG_SPRITE_PLAYER);
	this->addChild(Dragon);
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.2);
	for (int i = 1; i < 4; i++) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("dragon_%d.png", i));
		animation->addSpriteFrame(frame);
	}
	auto animate = Animate::create(animation);
	Dragon->runAction(RepeatForever::create(animate));
}

//�̻��� ����//
void HelloWorld::setMissile(float delta)
{
	auto sprPlayer = Dragon;// (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);
	Sprite *sprMissile;
	auto animation_shot = Animation::create();
	if (isGetItem) {// �������� ������ �Ѿ��� ����//
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("powershot.plist");
		sprMissile = Sprite::createWithSpriteFrameName("powershot_1.png");
		sprMissile->setPosition(Vec2(Dragon->getPositionX(), Dragon->getPositionY() + 30));
		sprMissile->setTag(5);

		animation_shot->setDelayPerUnit(0.5);
		for (int i = 1; i < 4; i++) {
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("powershot_%d.png", i));
			animation_shot->addSpriteFrame(frame);

		}

	}

	else {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shot.plist");
		sprMissile = Sprite::createWithSpriteFrameName("shot_1.png");
		sprMissile->setPosition(Vec2(Dragon->getPositionX(), Dragon->getPositionY() + 30));
		sprMissile->setTag(1);


		animation_shot->setDelayPerUnit(0.5);
		for (int i = 1; i < 4; i++) {
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("shot_%d.png", i));
			animation_shot->addSpriteFrame(frame);

		}

	}

	sprMissile->setPosition(sprPlayer->getPosition() + Vec2(-1, 20));
	this->addChild(sprMissile);
	missiles.pushBack(sprMissile);

	auto animate_shot = Animate::create(animation_shot);
	auto action = Sequence::create(
		MoveBy::create(1.0, Vec2(0, winSize.height)),
		CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetMissile, this)),
		NULL);
	sprMissile->runAction(animate_shot);
	sprMissile->runAction(action);
}

//�̻����� MoveBy�� �̵��� �ٵǸ� �̻����� �����ϴ� �Լ�//
void HelloWorld::resetMissile(Ref *sender)
{
	auto sprMissile = (Sprite*)sender;
	missiles.eraseObject(sprMissile);
	this->removeChild(sprMissile);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	posStartTouch = touch->getLocation();
	auto sprPlayer = Dragon;//(Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);
	posStartPlayer = sprPlayer->getPosition();
	return true;
}
void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{

	auto location = touch->getLocation();
	Dragon->setPosition(location.x, 50);
}

//�Ѿ��� ���ϰ� ����� ������ �Լ�//
void HelloWorld::setItem(float delta)
{
	//PADDING_SCREEN = �������� ���κп� �ȳ����� �ϱ����� 10���� ������ ��
	int x = PADDING_SCREEN + rand() % ((int)winSize.width - PADDING_SCREEN * 2);
	auto sprItem = Sprite::create("game/item.png");
	sprItem->setPosition(Vec2(x, winSize.height));
	this->addChild(sprItem);

	items.pushBack(sprItem);
	auto action = Sequence::create(
		MoveBy::create(3.0, Vec2(0, -winSize.height)),
		CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetItem, this)),
		NULL);
	sprItem->runAction(action);
}

//�������� ȭ��� ������� �������̶� ����Ʈ�Ѵ� ����//
void HelloWorld::resetItem(Ref *sender)
{
	auto sprItem = (Sprite*)sender;

	items.eraseObject(sprItem);

	this->removeChild(sprItem);
}

void HelloWorld::update(float delta)
{
	Rect rectPlayer = Rect(Dragon->getPositionX() - 10, Dragon->getPositionY() - 10, 20, 20); // �÷��̾��� �ǰ� ������ ����
	auto removeSpr = Sprite::create();
	for (Sprite* sprItem : items) {
		Rect rectItem = sprItem->getBoundingBox();
		if (rectPlayer.intersectsRect(rectItem)) { // �����۰� �÷��̾ �ε����� Ȯ��
			removeSpr = sprItem; // �ε����� removeSpr�� sprItem ����
			//������ ������ �Ҹ�
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance(); 
			audio->preloadEffect("Powerup.wav");
			audio->playEffect("Powerup.wav");
		}
	}
	if (items.contains(removeSpr)) { //������ Vector�� �ش� Sprite�� �ִ��� Ȯ�� ������  resetItem�� ���� ������ ����
		resetItem(removeSpr);
		isGetItem = true; // �̻��� �Ŀ��� ��
		this->scheduleOnce(schedule_selector(HelloWorld::resetGetItem), 5.0); // 5�ʵ� ����//
	}

	auto removeMissile = Sprite::create();
	auto removeEnemy = Sprite::create();
		for(int i=0; i<missiles.size(); i++){
			Sprite* sprMissile = missiles.at(i);
		Rect rectMissile = sprMissile->getBoundingBox();
		for (Sprite* sprEnemys : enemies) {
			Rect rectEnemy = Rect(sprEnemys->getPositionX() - 10, sprEnemys->getPositionY() - 10, 20, 20);

			//�Ϲ� ���Ͱ� ������ �ڵ�//
			if (rectMissile.intersectsRect(rectEnemy)) {
				int attack = sprMissile->getTag();
				int hp = sprEnemys->getTag();
				removeMissile = sprMissile;
				if (hp - attack > 0) {
					sprEnemys->setTag(hp - attack);
				}
				else {
					removeEnemy = sprEnemys;
					addScore(1); // ���Ͱ� ������ addScore�� 1�� �߰�//
					//���� �״� �Ҹ�//
					auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					audio->preloadEffect("Hit_Hurt.wav");
					audio->playEffect("Hit_Hurt.wav");
				}
			}

		}
		//���ӵ� ���Ͱ� ������ �ڵ�//
		for (Sprite* sprEnemyNamed : enemiesnamed)
		{
			Rect rectEnemyNamed = Rect(sprEnemyNamed->getPositionX() - 10, sprEnemyNamed->getPositionY() - 10, 20, 20);

			if (rectMissile.intersectsRect(rectEnemyNamed))
			{
				int attack = sprMissile->getTag();
				int hp = sprEnemyNamed->getTag();
				removeMissile = sprMissile;
				if (hp - attack > 0)
				{
					sprEnemyNamed->setTag(hp - attack); //���ӵ� ���ʹ� hp�� 5�̱� ������ hp�� 0�� �ɶ����� �ǰ� ��

				}
				else {
					removeEnemy = sprEnemyNamed;
					addScore(10); // ���ӵ���� �ǰ� 0�̵Ǹ� ���ھ� 10�� ȹ��
					auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					audio->preloadEffect("Hit_Hurt.wav");
					audio->playEffect("Hit_Hurt.wav");
				}
			}
		}

		for (int i = 0; i < enemies.size(); i++)
		{
			Rect Mobs = enemies.at(i)->getBoundingBox();

			if (rectPlayer.intersectsRect(Mobs))
			{
				int Hit = 1;
				 
				if (PlayerHp - Hit >= 0)
				{
					PlayerATK(); // �÷��̾ ���Ͷ� �ε����� ��ƼŬ�� �Ҹ� ������ �Լ�
					resetMonster();//�÷��̾�� ���� �ε����� ȭ����� ����, �̻������� ������ �� ����
					addPlayerHP(1);// �÷��̾��� Life�� 1���ҽ�Ŵ
					
				
				}
				if (PlayerHp == 0)
				{
					PlayerATK();
					resetMonster();
					GameOver(); // ȭ�� �߾ӿ� Game Over �� ����
					doStop(); //�÷��̾��� Life�� ����,�̻���,������ �� ������ �ߴ���
				}
				
			}
			
		}
		for (int i = 0; i < enemiesnamed.size(); i++)
		{
			Rect MobsNamed = enemiesnamed.at(i)->getBoundingBox();
			if (rectPlayer.intersectsRect(MobsNamed))
			{
				int Hit = 1;
				if (PlayerHp - Hit >= 0)
				{
					PlayerATK();
					resetMonster();
					addPlayerHP(1);
					

				}
				if (PlayerHp == 0)
				{
					PlayerATK();
					resetMonster();
					GameOver();
					doStop();
				}
			}
		}

	}

	//Vector �ȿ� ���Ͷ� �̻����� �ε��� Sprite�� �ִ��� Ȯ��
	if (missiles.contains(removeMissile)) {
		resetMissile(removeMissile);
	}
	//Vector �ȿ� �ǰ� 0�̵� ���Ͱ� �ִ��� Ȯ��
	//������ �� �������� ��ƼŬ ���� �� ����
	if (enemies.contains(removeEnemy)) {
		auto particle = ParticleSystemQuad::create("game/explosion.plist");
		particle->setPosition(removeEnemy->getPosition());
		particle->setScale(0.5);
		this->addChild(particle);
		auto action = Sequence::create(
			DelayTime::create(1.0),
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetBoom, this)),
			NULL);
		particle->runAction(action);
		resetEnemy(removeEnemy);
	}
	if (enemiesnamed.contains(removeEnemy)) {
		auto particle = ParticleSystemQuad::create("game/explosion.plist");
		particle->setPosition(removeEnemy->getPosition());
		particle->setScale(0.5);
		this->addChild(particle);
		auto action = Sequence::create(
			DelayTime::create(1.0),
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetBoom, this)),
			NULL);
		particle->runAction(action);
		resetEnemyNamed(removeEnemy);
	}


}


//�÷��̾� �ǰ� �Լ�
void HelloWorld::PlayerATK()
{
	auto particle = ParticleSystemQuad::create("game/explosion.plist");
	particle->setPosition(Dragon->getPosition());
	particle->setScale(0.5);
	this->addChild(particle);
	auto action = Sequence::create(
		DelayTime::create(1.0),
		CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetBoom, this)),
		NULL);

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect("Hit_Hurt.wav");
	audio->playEffect("Hit_Hurt.wav");
}

//������ Sprite�� ���͸���Ʈ ���� ����
void HelloWorld::resetMonster()
{
	for (int i = 0; i < missiles.size(); i++)
	{
		removeChild(missiles.at(i));
		
	}
	missiles.clear();
	for (int i = 0; i < enemies.size(); i++)
	{
		removeChild(enemies.at(i));
	}
	enemies.clear();
	for (int i = 0; i < enemiesnamed.size(); i++)
	{
		removeChild(enemiesnamed.at(i));

	}

	enemiesnamed.clear();

}
//�������� �԰� �� 5���� �ٽ� ���� �̻��Ϸ� �ٲܶ� ���
void HelloWorld::resetGetItem(float delta)
{
	isGetItem = false;
}

//���� ���� �Լ�
void HelloWorld::setEnemy(float detla)
{
	int x = PADDING_SCREEN + rand() % ((int)winSize.width - PADDING_SCREEN * 2);

	auto animation_mob = Animation::create();

	if (rand() % 100 < 20) { // Ȯ�������� ���ӵ� ���Ͱ� ��������


		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("named.plist");
		sprEnemyNamed = Sprite::createWithSpriteFrameName("named_1.png");
		sprEnemyNamed->setTag(5); // ���ӵ� ������ HP = 5

		animation_mob->setDelayPerUnit(0.2);
		for (int i = 1; i < 4; i++) {
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("named_%d.png", i));
			animation_mob->addSpriteFrame(frame);
		}

		sprEnemyNamed->setPosition(Vec2(x, winSize.height));
		this->addChild(sprEnemyNamed);
		enemiesnamed.pushBack(sprEnemyNamed);

		auto animate_mob = Animate::create(animation_mob);
		auto RepeateF = RepeatForever::create(animate_mob);
		auto action = Sequence::create(
			MoveBy::create(4.0, Vec2(0, -winSize.height)),
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetEnemy, this)),
			NULL);
		sprEnemyNamed->runAction(action);
		sprEnemyNamed->runAction(RepeateF);
	}

	else {

		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mob.plist");
		sprEnemy = Sprite::createWithSpriteFrameName("mob_1.png");
		sprEnemy->setTag(1);

		animation_mob->setDelayPerUnit(0.2);
		for (int i = 1; i < 4; i++) {
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("mob_%d.png", i));
			animation_mob->addSpriteFrame(frame);
		}

		sprEnemy->setPosition(Vec2(x, winSize.height));
		this->addChild(sprEnemy);
		enemies.pushBack(sprEnemy);

		auto animate_mob = Animate::create(animation_mob);
		auto RepeateF = RepeatForever::create(animate_mob);
		auto action = Sequence::create(
			MoveBy::create(5.0, Vec2(0, -winSize.height)),
			CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetEnemy, this)),
			NULL);
		sprEnemy->runAction(action);
		sprEnemy->runAction(RepeateF);
	}
	

}
//MoveBy�� ���� ���͸� �����ϴ� �Լ�
void HelloWorld::resetEnemy(Ref *sender)
{
	auto sprEnemys = (Sprite*)sender;
	enemies.eraseObject(sprEnemys);
	this->removeChild(sprEnemys);
}
//MoveBy�� ���� ���ӵ� ���͸� �����ϴ� �Լ�
void HelloWorld::resetEnemyNamed(Ref *sender)
{
	auto sprEnemyss = (Sprite*)sender;
	enemiesnamed.eraseObject(sprEnemyss);
	this->removeChild(sprEnemyss);
}

//��ƼŬ ������ �����ϴ� �Լ�
void HelloWorld::resetBoom(Ref *sender)
{
	auto particle = (ParticleSystemQuad*)sender;
	this->removeChild(particle);
}

//���ھ��� �������� ��Ÿ���� �Լ�
void HelloWorld::initScore()
{
	auto labelHigh = Label::createWithSystemFont("", "", 14);
	labelHigh->setAnchorPoint(Vec2(1, 1));
	labelHigh->setPosition(Vec2(winSize.width - 10, winSize.height - 10));
	labelHigh->setColor(Color3B::WHITE);
	labelHigh->setTag(TAG_LABEL_HIGHSCORE);
	this->addChild(labelHigh, 100);
	addScore(0);
}


void HelloWorld::addScore(int add)
{
	nScore += add;

	if (nScore > nScoreHigh) // ���� ������ �ְ� �������� ���� ��� �ְ� �������� �����ϴ� ����
	{
		nScoreHigh = nScore;
		UserDefault::getInstance()->setIntegerForKey("HIGH_SCORE", nScoreHigh);
		UserDefault::getInstance()->flush(); // UserDefault�� ����� ������ xml ���Ϸ� ����, flush ���ϸ� ���� ����� ���� ����
	}
	auto labelHigh = (Label*)this->getChildByTag(TAG_LABEL_HIGHSCORE);
	labelHigh->setString(StringUtils::format("SCORE : %d / %d", nScore, nScoreHigh));
}

//�÷��̾��� Life�� ��Ÿ���� �Լ�
void HelloWorld::initHP()
{
	auto labelScore = Label::createWithSystemFont("", "", 14);
	labelScore->setAnchorPoint(Vec2(0, 1));
	labelScore->setPosition(Vec2(10, winSize.height - 10));
	labelScore->setColor(Color3B::WHITE);
	labelScore->setTag(TAG_LABEL_PLAYERHP);
	this->addChild(labelScore, 100);
	addPlayerHP(0);
}

//�Է¹��� Life ���Ҹ� �޴� �Լ�
void HelloWorld::addPlayerHP(int add)
{
	PlayerHp -= add;
	UserDefault::getInstance()->setIntegerForKey("Life", PlayerHp);
	UserDefault::getInstance()->flush();
	auto labelScore = (Label*)this->getChildByTag(TAG_LABEL_PLAYERHP);
	labelScore->setString(StringUtils::format("Life : %d", PlayerHp));
}

//////////////////////////���ӿ��� ����////////////////////////////
void HelloWorld::GameOver()
{


	auto label1 = Label::createWithSystemFont("GAME OVER", "Algerian", 20);
	auto GameOver = MenuItemLabel::create(label1, CC_CALLBACK_1(HelloWorld::changeScenes, this));
	GameOver->setColor(Color3B::RED); // ���� ����

	auto menu = Menu::create(GameOver, NULL);
	menu->alignItemsVertically();
	this->addChild(menu);

}

//�̻���,���� ���� �Լ� �ߴ�
void HelloWorld::doStop()
{
	
	this->unschedule(schedule_selector(HelloWorld::setMissile));
	this->unschedule(schedule_selector(HelloWorld::setItem));
	this->unschedule(schedule_selector(HelloWorld::setEnemy));
}

//���ӿ��� Ŭ���� ����ȭ�� �����Լ�
void HelloWorld::changeScenes(Ref *sender)
{
	MusicStop();//�������ִ� �뷡 �ߴ�//
	auto scene = TransitionPageTurn::create(1.0, HelloMenu::createScene(), true);
	Director::getInstance()->replaceScene(scene);
}
//ó�� �������� ����
void HelloWorld::MusicStart()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("gameBGM.mp3");
}
//�뷡 �ߴ�
void HelloWorld::MusicStop()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
