/*
 * AnimationMgr.h
 *
 *  Created on: Apr 24, 2018
 *      Author: zachcooper
 */

#ifndef INC_ANIMATIONMGR_H_
#define INC_ANIMATIONMGR_H_

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <Aspect.h>
#include <Engine.h>
#include <EntityMgr.h>
#include <UiMgr.h>
#include <GfxMgr.h>
#include <InputMgr.h>
#include <deque>

class AnimationMgr : Mgr
{
public:

	AnimationMgr(Engine *engine);
	virtual ~AnimationMgr();

	void Init();
	void LoadLevel();
	void Tick(float dt);
	void Stop();

    virtual void createFrameListener(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    virtual bool keyPressed(const OIS::KeyEvent& ke);
    virtual bool keyReleased(const OIS::KeyEvent& ke);
    virtual bool nextLocation(void);

    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);

    std::vector<Ogre::Vector3*> mDirections;
    //std::vector<Ogre::AnimationState*> mAnimations;
    std::deque<Ogre::Vector3> mWalkList;


    Ogre::AnimationState* mAnimation;
    Ogre::Vector3 dirVec;
};




#endif /* INC_ANIMATIONMGR_H_ */
