#pragma once
#include "cocos2d.h"
#include "EnemyInfo.h"
#include "EnemySprite.h"

USING_NS_CC;

class EnemyLayer : public Layer
{
public:
	EnemyLayer();
	~EnemyLayer();
	virtual bool init();
	CREATE_FUNC(EnemyLayer);

public:
	void addEnemy1(float dt); // 敌机1添加及飞行速度
	void addEnemy2(float dt); // 敌机2添加及飞行速度
	void addEnemy3(float dt); // 敌机2添加及飞行速度

	void blowupEnemy(EnemySprite* pEnemySprite);	// 敌机爆炸
	void removeEnemy(Node *pNode);					// 移除敌机

	void setScore(int nScore);						// 设置分数
	int  getScore();								// 获取分数
	void updateScore(int score);					// 更新分数

public:
	Vector<EnemySprite*> vecEnemy;	// 敌机容器
	static int m_score;				// 分数
	//LabelBMFont* scoreItem;			// 分数显示器

	void f_removemyAnimation(Node *pNode);

};
