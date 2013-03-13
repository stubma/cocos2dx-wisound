#ifndef _SoundTest_H_
#define _SoundTest_H_

#include "../testBasic.h"

using namespace std;
USING_NS_CC;


enum
{
    SOUND_PLAY_MP3 = 0,
    SOUND_PLAY_OGG = 1,
    SOUND_PLAY_WAV = 2,
    SOUND_LAYER_COUNT,
};


// the class inherit from TestScene
// every Scene each test used must inherit from TestScene,
// make sure the test have the menu item for back to main menu
class SoundTestScene : public TestScene
{
public:
    virtual void runThisTest();
};

class SoundDemo : public CCLayer
{
protected:
	float m_volume;
    
public:
    SoundDemo();
    virtual void onEnter();
    virtual void onExit();
	
    virtual string title();
    virtual string subtitle();

    void restartCallback(CCObject* pSender);
    void nextCallback(CCObject* pSender);
    void backCallback(CCObject* pSender);
};

class SoundPlayMP3 : public SoundDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
    
    void onPlayClicked();
    void onStopClicked();
    void onEffectMonoClicked();
    void onEffectStereoClicked();
    void onVolumeDownClicked();
    void onVolumeUpClicked();
};

class SoundPlayOGG : public SoundDemo
{
public:
    virtual void onEnter();
    virtual string subtitle();
    
    void onPlayClicked();
    void onStopClicked();
    void onEffectMonoClicked();
    void onEffectStereoClicked();
    void onVolumeDownClicked();
    void onVolumeUpClicked();
};

class SoundPlayWAV : public SoundDemo
{	
public:
	SoundPlayWAV();
	virtual ~SoundPlayWAV();
    virtual void onEnter();
    virtual string subtitle();
    
    void onPlayClicked();
    void onStopClicked();
    void onEffectMonoClicked();
    void onEffectStereoClicked();
    void onVolumeDownClicked();
    void onVolumeUpClicked();
};

#endif
