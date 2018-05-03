/*
 * InputMgr.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */

#include <Engine.h>
#include <GfxMgr.h>
#include <InputMgr.h>
#include <EntityMgr.h>
#include <GameMgr.h>
#include <UiMgr.h>

#include <OgreSceneQuery.h>

#include <Utils.h>

InputMgr::InputMgr(Engine *engine) : Mgr(engine) {

	this->mInputMgr = 0;
	this->mKeyboard = 0;
	this->mMouse = 0;
	this->keyboardTimer = keyTime;
	deltaDesiredSpeed = 10.0f;
	deltaDesiredHeading = 10.0f;
	mMovableFound = false;
	mRaySceneQuery = 0;
	isPlus = false;
}

InputMgr::~InputMgr() {

}

void InputMgr::Init(){

	  OIS::ParamList pl;
	  size_t windowHandle = 0;
	  std::ostringstream windowHandleStr;

	  engine->gfxMgr->mWindow->getCustomAttribute("WINDOW", &windowHandle);
	  windowHandleStr << windowHandle;
	  pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

	  #if defined OIS_WIN32_PLATFORM
	  	  pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	  	  pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	  	  pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	  	  pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	  #elif defined OIS_LINUX_PLATFORM
	  	  pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	  	  pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	  	  //pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	  	  pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
	  #endif
	  mInputMgr = OIS::InputManager::createInputSystem(pl);

	  mKeyboard = static_cast<OIS::Keyboard*>(
	    mInputMgr->createInputObject(OIS::OISKeyboard, true));
	  mMouse = static_cast<OIS::Mouse*>(
	    mInputMgr->createInputObject(OIS::OISMouse, true));

	  int left, top;
	  unsigned int width, height, depth;

	  engine->gfxMgr->mWindow->getMetrics(width, height, depth, left, top);
	  const OIS::MouseState &ms = mMouse->getMouseState();
	  ms.width = width;
	  ms.height = height;

	  mMouse->setEventCallback(this);
	  mKeyboard->setEventCallback(this);

}



void InputMgr::Stop(){
	if(mInputMgr){
		mInputMgr->destroyInputObject(mMouse);
		mInputMgr->destroyInputObject(mKeyboard);

		OIS::InputManager::destroyInputSystem(mInputMgr);
		mInputMgr = 0;
   	}
}

void InputMgr::Tick(float dt){
	//Must capture both every tick for buffered input to work
	mMouse->capture();
	mKeyboard->capture();
	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE)){
		engine->keepRunning = false;
	}

//	mTrayMgr->frameRenderingQueued(fe);

	UpdateCamera(dt);
	UpdateVelocityAndSelection(dt);
}

void InputMgr::UpdateCamera(float dt){
	float move = 400.0f;
	float rotate = 0.1f;

	 Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;

	  if (mKeyboard->isKeyDown(OIS::KC_W))
	    dirVec.z -= move;

	  if (mKeyboard->isKeyDown(OIS::KC_S))
	    dirVec.z += move;

	  if (mKeyboard->isKeyDown(OIS::KC_Q))
		  engine->gameMgr->cameraNode->pitch(Ogre::Degree(5 * rotate));

	  if (mKeyboard->isKeyDown(OIS::KC_E))
		  engine->gameMgr->cameraNode->pitch(Ogre::Degree(-5 * rotate));

	  if (mKeyboard->isKeyDown(OIS::KC_A))
	  {
	    if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		      engine->gameMgr->cameraNode->yaw(Ogre::Degree(5 * rotate));
	    else
	      dirVec.x -= move;
	  }

	  if (mKeyboard->isKeyDown(OIS::KC_D))
	  {
	    if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
	      engine->gameMgr->cameraNode->yaw(Ogre::Degree(-5 * rotate));
	    else
	      dirVec.x += move;
	  }

	  if (mKeyboard->isKeyDown(OIS::KC_Z))
	  {
		  engine->gameMgr->cameraNode->yaw(Ogre::Degree(5 * rotate));
	  }

	  if (mKeyboard->isKeyDown(OIS::KC_C))
	  {
		  engine->gameMgr->cameraNode->yaw(Ogre::Degree(-5 * rotate));
	  }

	  engine->gameMgr->cameraNode->translate(dirVec * dt, Ogre::Node::TS_LOCAL);
}

void InputMgr::UpdateVelocityAndSelection(float dt){
	keyboardTimer -= dt;

	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD8)){
		keyboardTimer = keyTime;
		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
			if (engine->entityMgr->entities[i]->isSelected){
				engine->entityMgr->entities[i]->desiredSpeed += deltaDesiredSpeed;
			}
		}
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD2)){
		keyboardTimer = keyTime;
		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
			if (engine->entityMgr->entities[i]->isSelected){
				engine->entityMgr->entities[i]->desiredSpeed -= deltaDesiredSpeed;
			}
		}
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD4)){
		keyboardTimer = keyTime;
		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
			if (engine->entityMgr->entities[i]->isSelected){
				engine->entityMgr->entities[i]->desiredHeading -= deltaDesiredHeading;
			}
		}
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD6)){
		keyboardTimer = keyTime;
		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
			if (engine->entityMgr->entities[i]->isSelected){
				engine->entityMgr->entities[i]->desiredHeading += deltaDesiredHeading;
			}
		}
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_PGUP)){
		keyboardTimer = keyTime;
		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
			if (engine->entityMgr->entities[i]->isSelected){
				engine->entityMgr->entities[i]->desiredAltitude += engine->entityMgr->entities[i]->climbRate;
			}
		}
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_PGDOWN)){
		keyboardTimer = keyTime;
		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
			if (engine->entityMgr->entities[i]->isSelected){
				engine->entityMgr->entities[i]->desiredAltitude -= engine->entityMgr->entities[i]->climbRate;
			}
		}
	}

	for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
		if (engine->entityMgr->entities[i]->isSelected){
			engine->entityMgr->entities[i]->desiredHeading = FixAngle(engine->entityMgr->entities[i]->desiredHeading);
		}
	}

	//Set velocity to zero....
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_SPACE)){
		keyboardTimer = keyTime;
		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
			if (engine->entityMgr->entities[i]->isSelected){
				engine->entityMgr->entities[i]->velocity = Ogre::Vector3::ZERO;
				engine->entityMgr->entities[i]->desiredSpeed = engine->entityMgr->entities[i]->speed = 0;
				engine->entityMgr->entities[i]->desiredHeading = engine->entityMgr->entities[i]->heading;
				engine->entityMgr->entities[i]->desiredAltitude = 0;
			}
		}
	}

	//tab handling
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_N)){
		if(mKeyboard->isKeyDown(OIS::KC_TAB)){
			keyboardTimer = keyTime;
			isPlus = true;
			engine->entityMgr->SelectNextEntity(isPlus);
		}
	}

	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_M)){
		if(mKeyboard->isKeyDown(OIS::KC_TAB)){
			keyboardTimer = keyTime;
			isPlus = false;
			engine->entityMgr->SelectNextEntity(isPlus);
		}
	}
}

void InputMgr::LoadLevel(){

}

bool InputMgr::keyPressed(const OIS::KeyEvent& ke){
	return true;
}

bool InputMgr::keyReleased(const OIS::KeyEvent& ke){
	return true;
}

bool InputMgr::mouseMoved(const OIS::MouseEvent& me){
	return true;
}

bool InputMgr::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID mid){
	float diffx;
	float diffy;
	float diffz;

	if (engine->uiMgr->mTrayMgr->injectMouseDown(me, mid)) return true;
    if (mid == OIS::MB_Left){ //mMouse->getMouseState().buttonDown(OIS::MB_Left)){
		// get window height and width
    	Ogre::Real screenWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    	Ogre::Real screenHeight = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    	OIS::MouseState arg = mMouse->getMouseState();

    	// convert to 0-1 offsest
    	Ogre::Real offsetX = arg.X.abs / screenWidth;
    	Ogre::Real offsetY = arg.Y.abs / screenHeight;

    	//  set up the ray
    	Ogre::Ray mouseRay = engine->gfxMgr->mCamera->getCameraToViewportRay(offsetX, offsetY);

    	std::pair <bool, float> result = mouseRay.intersects(engine->gameMgr->myPlane);

    	if(result.first){
    		Ogre::Vector3 point = mouseRay.getPoint(result.second);
    		std::cout << point << std::endl;
    		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
        		diffx = (point.x) - (engine->entityMgr->entities[i]->position.x);
        		diffy = (point.y) - (engine->entityMgr->entities[i]->position.y);
        		diffz = (point.z) - (engine->entityMgr->entities[i]->position.z);
        		engine->gfxMgr->debugDrawer->getSingleton().drawLine(engine->gameMgr->cameraNode->getPosition(), point, Ogre::ColourValue::Red );

    			if(diffx <= 150 && diffx >= -150 && diffy <= 150 && diffy >= -150 && diffz <= 150 && diffz >= -150){
    				engine->uiMgr->score += 10;
    				engine->uiMgr->enemyHealth = engine->uiMgr->enemyHealth - 0.01;
					if(engine->entityMgr->entities[i]->isSelected == true){
						engine->entityMgr->entities[i]->isSelected = false;
					}
					else {
						engine->entityMgr->entities[i]->isSelected = true;
					}
    			}
    		}
    	}
    }
    /*
    if (mid == OIS::MB_Right)
    {
    	engine->gameMgr->cameraNode->yaw(Ogre::Degree(-0.1 * me.state.X.rel), Ogre::Node::TS_WORLD);
    	engine->gameMgr->cameraNode->pitch(Ogre::Degree(-0.1 * me.state.Y.rel), Ogre::Node::TS_LOCAL);
    }
    */

return true;
}


bool InputMgr::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID mid){
	if (engine->uiMgr->mTrayMgr->injectMouseUp(me, mid)) return true;
		return true;
}
