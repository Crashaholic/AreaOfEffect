#ifndef SPRITE_ANIMATION
#define SPRITE_ANIMATION
#include "Mesh.h"
struct Animation 
{
	Animation() {}

	void Set(int startFrame, int endFrame, float time, bool active, bool doRepeat)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->animTime = time;
		this->animActive = active;
		this->doRepeat = doRepeat;
	}

	int startFrame;
	int endFrame;
	float animTime;
	bool doRepeat;
	bool ended;
	bool animActive;

};

class SpriteAnimation : public Mesh
{
public:
	SpriteAnimation(const std::string &meshName, int row, int col);
	~SpriteAnimation();
	void Update(double dt);
	virtual void Render() override; // Polymorphism

	int m_row;
	int m_col;
	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
	Animation *m_anim;

};

#endif
