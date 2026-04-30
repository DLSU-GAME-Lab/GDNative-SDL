#include "LobbyPlayer.h"
#include "TextureManager.h"
#include "CameraController.h"
#include "AudioManager.h"

LobbyPlayer::LobbyPlayer() : AGameObject("LobbyPlayer")
{
    this->pRenderer = NULL;
    this->pSpriteAnimator = NULL;
    this->pTweenAnimator = NULL;
    this->pGUI = NULL;
    this->pLeft = NULL;
    this->pRight = NULL;
}

void LobbyPlayer::initialize()
{
    this->pRenderer = new SpriteRenderer("player_idle");
    this->pSpriteAnimator = new SpriteAnimator(pRenderer);
    
    auto vecIdle = TextureManager::getInstance()->getTexture("player_idle");
    auto vecRun = TextureManager::getInstance()->getTexture("player_run");

    Animation* pIdle = new Animation("idle", vecIdle, 12, AnimationType::LOOP);
    Animation* pRun = new Animation("run", vecRun, 16, AnimationType::LOOP);
    
    this->pSpriteAnimator->addAnimation(pIdle);
    this->pSpriteAnimator->addAnimation(pRun);
    this->pSpriteAnimator->play("idle");

    this->pTweenAnimator = new TweenAnimator();
    this->pTweenAnimator->setAnimationType(AnimationType::ONCE);
    this->pTweenAnimator->addListener(this);

    CameraController* pController = new CameraController();
    pController->setOffset(-this->fVecTranslate);

    this->attachComponent(pRenderer);
    this->attachComponent(pSpriteAnimator);
    this->attachComponent(pTweenAnimator);
    this->attachComponent(pController);
    this->initialPosX = this->fVecTranslate.x;
}

void LobbyPlayer::onAnimationFinished()
{
    if (this->pGUI) this->pGUI->setEnabled(true);
    if (this->fVecTranslate.x < this->initialPosX)
    {
        this->pLeft->setEnabled(false);
        this->pRight->setEnabled(true);

        AudioManager::getInstance()->stopAll();
        AudioManager::getInstance()->play(new AudioPlayer("SideRooms_Music", "BGM", AudioGroupTag::MUSIC, OnAudioFinished::LOOP));
    }
    else if (this->fVecTranslate.x > this->initialPosX)
    {
        this->pLeft->setEnabled(true);
        this->pRight->setEnabled(false);

        AudioManager::getInstance()->stopAll();
        AudioManager::getInstance()->play(new AudioPlayer("SideRooms_Music", "BGM", AudioGroupTag::MUSIC, OnAudioFinished::LOOP));
    }
    else
    {
        this->pLeft->setEnabled(true);
        this->pRight->setEnabled(true);

        AudioManager::getInstance()->stopAll();
        AudioManager::getInstance()->play(new AudioPlayer("Lobby_Music", "BGM", AudioGroupTag::MUSIC, OnAudioFinished::LOOP));
    }

    this->pSpriteAnimator->play("idle");
    
}

void LobbyPlayer::goLeft()
{
    std::cout << "[Lobby Player]: going left\n";
    Vector2D pos = this->fVecTranslate;
    float newPosX = pos.x - 2420.0f;
    Tween2D tween = Tween2D::from(pos.x, pos.y)
        .to(newPosX, pos.y)
        .during(2000)
        .via(tweeny::easing::linear);

    this->pRenderer->setFlipX(true);
    this->pTweenAnimator->setTweenPos(tween);
    this->pTweenAnimator->play();
    this->pSpriteAnimator->play("run");
    if (this->pGUI) this->pGUI->setEnabled(false);
}

void LobbyPlayer::goRight()
{
    std::cout << "[Lobby Player]: going right\n";
    Vector2D pos = this->fVecTranslate;
    float newPosX = pos.x + 2420.0f;
    Tween2D tween = Tween2D::from(pos.x, pos.y)
        .to(newPosX, pos.y)
        .during(2000)
        .via(tweeny::easing::linear);

    this->pRenderer->setFlipX(false);
    this->pTweenAnimator->setTweenPos(tween);
    this->pTweenAnimator->play();
    this->pSpriteAnimator->play("run");
    if (this->pGUI) this->pGUI->setEnabled(false);
}

void LobbyPlayer::setGUI(AGameObject* pGUI)
{
    this->pGUI = pGUI;
}

void LobbyPlayer::setLeftArrow(AGameObject* pLeft)
{
    this->pLeft = pLeft;
}

void LobbyPlayer::setRightArrow(AGameObject * pRight)
{
    this->pRight = pRight;
}
