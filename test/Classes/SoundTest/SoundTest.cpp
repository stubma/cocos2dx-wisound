#include "SoundTest.h"
#include "../testResource.h"
#include "cocos2d.h"
#include "CCUtils.h"
#include "SimpleAudioEngine.h"

using namespace WiSound;

TESTLAYER_CREATE_FUNC(SoundPlayMP3);
TESTLAYER_CREATE_FUNC(SoundPlayOGG);
TESTLAYER_CREATE_FUNC(SoundPlayWAV);

static NEWTESTFUNC createFunctions[] = {
    CF(SoundPlayMP3),
	CF(SoundPlayOGG),
	CF(SoundPlayWAV)
};

static int sceneIdx=-1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

static CCLayer* nextAction()
{
    sceneIdx++;
    sceneIdx = sceneIdx % MAX_LAYER;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* backAction()
{
    sceneIdx--;
    int total = MAX_LAYER;
    if( sceneIdx < 0 )
        sceneIdx += total;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* restartAction()
{
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

void SoundTestScene::runThisTest()
{
    sceneIdx = -1;
    addChild(nextAction());

    CCDirector::sharedDirector()->replaceScene(this);
}


std::string SoundDemo::title()
{
    return "SoundTest";
}

std::string SoundDemo::subtitle()
{
    return "";
}

SoundDemo::SoundDemo() : m_volume(1) {
    
}

void SoundDemo::onEnter()
{
    CCLayer::onEnter();

    // add title and subtitle
    std::string str = title();
    const char * pTitle = str.c_str();
    CCLabelTTF* label = CCLabelTTF::create(pTitle, "Arial", 32);
    addChild(label, 1);
    label->setPosition( ccp(VisibleRect::center().x, VisibleRect::top().y - 30) );

    std::string strSubtitle = subtitle();
    if( ! strSubtitle.empty() ) 
    {
        CCLabelTTF* l = CCLabelTTF::create(strSubtitle.c_str(), "Thonburi", 16);
        addChild(l, 1);
        l->setPosition( ccp(VisibleRect::center().x, VisibleRect::top().y - 60) );
    }    

    // add menu
    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(SoundDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(SoundDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(SoundDemo::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

    addChild(menu, 1);
}

void SoundDemo::onExit()
{
    CCLayer::onExit();
}

void SoundDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new SoundTestScene();
    s->addChild( restartAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void SoundDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new SoundTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void SoundDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new SoundTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

//------------------------------------------------------------------
//
// Play MP3
//
//------------------------------------------------------------------
void SoundPlayMP3::onEnter()
{
    SoundDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCLabelTTF* label1 = CCLabelTTF::create("Play", "Helvetica", 24);
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(label1, this, menu_selector(SoundPlayMP3::onPlayClicked));
	CCLabelTTF* label2 = CCLabelTTF::create("Stop", "Helvetica", 24);
	CCMenuItemLabel* item2 = CCMenuItemLabel::create(label2, this, menu_selector(SoundPlayMP3::onStopClicked));
    CCLabelTTF* label3 = CCLabelTTF::create("Effect(mono)", "Helvetica", 24);
	CCMenuItemLabel* item3 = CCMenuItemLabel::create(label3, this, menu_selector(SoundPlayMP3::onEffectMonoClicked));
    CCLabelTTF* label4 = CCLabelTTF::create("Effect(stereo)", "Helvetica", 24);
	CCMenuItemLabel* item4 = CCMenuItemLabel::create(label4, this, menu_selector(SoundPlayMP3::onEffectStereoClicked));
    CCLabelTTF* label5 = CCLabelTTF::create("Volume Down", "Helvetica", 24);
	CCMenuItemLabel* item5 = CCMenuItemLabel::create(label5, this, menu_selector(SoundPlayMP3::onVolumeDownClicked));
    CCLabelTTF* label6 = CCLabelTTF::create("Volume Up", "Helvetica", 24);
	CCMenuItemLabel* item6 = CCMenuItemLabel::create(label6, this, menu_selector(SoundPlayMP3::onVolumeUpClicked));
	CCMenu* menu = CCMenu::create(item1, item2, item3, item4, item5, item6, NULL);
	menu->alignItemsVertically();
	menu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(menu);
    
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Audios/bg_mp3.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Audios/effect_mp3_mono.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Audios/effect_mp3_stereo.mp3");
}

std::string SoundPlayMP3::subtitle()
{
    return "Play MP3";
}

void SoundPlayMP3::onPlayClicked() {
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audios/bg_mp3.mp3", true);
}

void SoundPlayMP3::onStopClicked() {
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void SoundPlayMP3::onEffectMonoClicked() {
    SimpleAudioEngine::sharedEngine()->playEffect("Audios/effect_mp3_mono.mp3");
}

void SoundPlayMP3::onEffectStereoClicked() {
    SimpleAudioEngine::sharedEngine()->playEffect("Audios/effect_mp3_stereo.mp3");
}

void SoundPlayMP3::onVolumeDownClicked() {
    m_volume -= 0.1f;
    m_volume = MAX(0, m_volume);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_volume);
}

void SoundPlayMP3::onVolumeUpClicked() {
    m_volume += 0.1f;
    m_volume = MIN(1.0f, m_volume);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_volume);
}

//------------------------------------------------------------------
//
// Play OGG
//
//------------------------------------------------------------------
void SoundPlayOGG::onEnter()
{
    SoundDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCLabelTTF* label1 = CCLabelTTF::create("Play", "Helvetica", 24);
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(label1, this, menu_selector(SoundPlayOGG::onPlayClicked));
	CCLabelTTF* label2 = CCLabelTTF::create("Stop", "Helvetica", 24);
	CCMenuItemLabel* item2 = CCMenuItemLabel::create(label2, this, menu_selector(SoundPlayOGG::onStopClicked));
    CCLabelTTF* label3 = CCLabelTTF::create("Effect(mono)", "Helvetica", 24);
	CCMenuItemLabel* item3 = CCMenuItemLabel::create(label3, this, menu_selector(SoundPlayOGG::onEffectMonoClicked));
    CCLabelTTF* label4 = CCLabelTTF::create("Effect(stereo)", "Helvetica", 24);
	CCMenuItemLabel* item4 = CCMenuItemLabel::create(label4, this, menu_selector(SoundPlayOGG::onEffectStereoClicked));
    CCLabelTTF* label5 = CCLabelTTF::create("Volume Down", "Helvetica", 24);
	CCMenuItemLabel* item5 = CCMenuItemLabel::create(label5, this, menu_selector(SoundPlayOGG::onVolumeDownClicked));
    CCLabelTTF* label6 = CCLabelTTF::create("Volume Up", "Helvetica", 24);
	CCMenuItemLabel* item6 = CCMenuItemLabel::create(label6, this, menu_selector(SoundPlayOGG::onVolumeUpClicked));
	CCMenu* menu = CCMenu::create(item1, item2, item3, item4, item5, item6, NULL);
	menu->alignItemsVertically();
	menu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(menu);
    
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Audios/bg_ogg.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Audios/effect_ogg_mono.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Audios/effect_ogg_stereo.ogg");
}

string SoundPlayOGG::subtitle()
{
    return "Play OGG";
}

void SoundPlayOGG::onPlayClicked() {
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audios/bg_ogg.ogg", true);
}

void SoundPlayOGG::onStopClicked() {
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void SoundPlayOGG::onEffectMonoClicked() {
    SimpleAudioEngine::sharedEngine()->playEffect("Audios/effect_ogg_mono.ogg");
}

void SoundPlayOGG::onEffectStereoClicked() {
    SimpleAudioEngine::sharedEngine()->playEffect("Audios/effect_ogg_stereo.ogg");
}

void SoundPlayOGG::onVolumeDownClicked() {
    m_volume -= 0.1f;
    m_volume = MAX(0, m_volume);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_volume);
}

void SoundPlayOGG::onVolumeUpClicked() {
    m_volume += 0.1f;
    m_volume = MIN(1.0f, m_volume);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_volume);
}

//------------------------------------------------------------------
//
// Play WAV
//
//------------------------------------------------------------------
SoundPlayWAV::SoundPlayWAV() {
}

SoundPlayWAV::~SoundPlayWAV() {
}

void SoundPlayWAV::onEnter()
{
    SoundDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCLabelTTF* label1 = CCLabelTTF::create("Play", "Helvetica", 24);
	CCMenuItemLabel* item1 = CCMenuItemLabel::create(label1, this, menu_selector(SoundPlayWAV::onPlayClicked));
	CCLabelTTF* label2 = CCLabelTTF::create("Stop", "Helvetica", 24);
	CCMenuItemLabel* item2 = CCMenuItemLabel::create(label2, this, menu_selector(SoundPlayWAV::onStopClicked));
    CCLabelTTF* label3 = CCLabelTTF::create("Effect(mono)", "Helvetica", 24);
	CCMenuItemLabel* item3 = CCMenuItemLabel::create(label3, this, menu_selector(SoundPlayWAV::onEffectMonoClicked));
    CCLabelTTF* label4 = CCLabelTTF::create("Effect(stereo)", "Helvetica", 24);
	CCMenuItemLabel* item4 = CCMenuItemLabel::create(label4, this, menu_selector(SoundPlayWAV::onEffectStereoClicked));
    CCLabelTTF* label5 = CCLabelTTF::create("Volume Down", "Helvetica", 24);
	CCMenuItemLabel* item5 = CCMenuItemLabel::create(label5, this, menu_selector(SoundPlayWAV::onVolumeDownClicked));
    CCLabelTTF* label6 = CCLabelTTF::create("Volume Up", "Helvetica", 24);
	CCMenuItemLabel* item6 = CCMenuItemLabel::create(label6, this, menu_selector(SoundPlayWAV::onVolumeUpClicked));
	CCMenu* menu = CCMenu::create(item1, item2, item3, item4, item5, item6, NULL);
	menu->alignItemsVertically();
	menu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(menu);
    
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Audios/bg_wav_mono_16.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Audios/effect_wav_mono_16.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("Audios/effect_wav_stereo_16.wav");
}

string SoundPlayWAV::subtitle()
{
    return "Play WAV";
}

void SoundPlayWAV::onPlayClicked() {
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audios/bg_wav_mono_16.wav", true);
}

void SoundPlayWAV::onStopClicked() {
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void SoundPlayWAV::onEffectMonoClicked() {
    SimpleAudioEngine::sharedEngine()->playEffect("Audios/effect_wav_mono_16.wav");
}

void SoundPlayWAV::onEffectStereoClicked() {
    SimpleAudioEngine::sharedEngine()->playEffect("Audios/effect_wav_stereo_16.wav");
}

void SoundPlayWAV::onVolumeDownClicked() {
    m_volume -= 0.1f;
    m_volume = MAX(0, m_volume);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_volume);
}

void SoundPlayWAV::onVolumeUpClicked() {
    m_volume += 0.1f;
    m_volume = MIN(1.0f, m_volume);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_volume);
}