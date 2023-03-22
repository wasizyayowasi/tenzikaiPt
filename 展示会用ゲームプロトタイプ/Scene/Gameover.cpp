#include "Gameover.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"
#include "GameMain.h"
#include "BossBattleScene.h"
#include "../Object/PlayerMotion.h"
#include "../DrawFunctions.h"

namespace {
	constexpr int ultimateFrames = 120;
}

Gameover::Gameover(SceneManager& manager, const InputState& input, int num):SceneBase(manager),updateFunc_(&Gameover::fadeInUpdate), inputState(input),sceneNum(num)
{
	handle = my::myLoadGraph("data/objectGraph/player.png");
	lightHandle = my::myLoadGraph("data/objectGraph/light.png");
	uiSound = LoadSoundMem("data/soundEffect/ui3.mp3");
	uiSound2 = LoadSoundMem("data/soundEffect/ui4.mp3");

	gameOverChoiceTable[GameOverChoice::continueMain] = "continue";
	gameOverChoiceTable[GameOverChoice::end] = "end";

	motion = new PlayerMotion;

	LPCSTR UIfontPath = "data/other/Silver.ttf";
	if (AddFontResourceEx(UIfontPath, FR_PRIVATE, NULL) > 0)
	{
	}
	
	fontHandle = CreateFontToHandle("Silver", 48, 9, -1);
	fontHandle2 = CreateFontToHandle("Silver", 64, 9, -1);
	fontHandle3 = CreateFontToHandle("Silver", 128, 9, -1);
}

Gameover::~Gameover()
{
	DeleteGraph(handle);
	DeleteGraph(lightHandle);
	DeleteFontToHandle(fontHandle);
	DeleteFontToHandle(fontHandle2);
	DeleteFontToHandle(fontHandle3);
	DeleteSoundMem(uiSound);
	DeleteSoundMem(uiSound2);
}

void Gameover::update(const InputState& input)
{
	ultimateTimer = (std::max)(ultimateTimer - 1, 0);
	(this->*updateFunc_)(input);
}

void Gameover::draw()
{
	
	constexpr int pw_width = Game::kScreenWidth / 2 - 200;	
	constexpr int pw_height = Game::kScreenHeight / 2 - 200;
	constexpr int pw_start_x = Game::kScreenWidth / 2 + 200;
	constexpr int pw_start_y = Game::kScreenHeight / 2 + 200;

	int titleFontSize = GetDrawStringWidthToHandle("GAME OVER", strlen("GAME OVER"), fontHandle3);
	DrawStringToHandle(Game::kScreenWidth / 2 - titleFontSize / 2, 200, "GAME OVER", 0x740A00, fontHandle3);

	auto y = Game::kScreenHeight / 2 + 300;
	int x = Game::kScreenWidth / 2 + 10;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : gameOverChoiceTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		int font = strlen(name.second.c_str());
		fontSize = GetDrawStringWidthToHandle(name.second.c_str(), font, fontHandle);

		//ëIëÇ≥ÇÍÇΩéûÇÃèàóù
		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			color = 0xffff00;
			fontSize = GetDrawStringWidthToHandle(name.second.c_str(), font, fontHandle2);

			if (ultimateTimer > 0) {
				if ((ultimateTimer / 4) % 2 == 0) {

				}
				else {
					DrawStringToHandle(x - fontSize / 2, y, name.second.c_str(), color, fontHandle2);
				}
			}
			else {
				DrawStringToHandle(x - fontSize / 2, y, name.second.c_str(), color, fontHandle2);
			}

		}
		else {
			DrawStringToHandle(x - fontSize / 2, y, name.second.c_str(), color, fontHandle);
		}

		y += 50;
		++idx;
	}

	DrawRotaGraph(Game::kScreenWidth / 2 - 5, Game::kScreenHeight / 2, 5.0f, 0.0f, lightHandle, true);
	if (updateFunc_ == &Gameover::normalUpdate) {
		motion->deadDraw({ Game::kScreenWidth / 2 - 10,Game::kScreenHeight / 2 + 150 }, handle, false);
	}
	else if(updateFunc_ == &Gameover::continueFadeOutUpdate) {
		motion->resuscitationDraw({ Game::kScreenWidth / 2 - 10,Game::kScreenHeight / 2 + 150 }, handle, false);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//âÊñ ëSëÃÇê^Ç¡çïÇ…ìhÇËÇ¬Ç‘Ç∑
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Gameover::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast < int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &Gameover::normalUpdate;
		fadeValue_ = 0;
		PlayMusic("data/music/orehamou.mp3", DX_PLAYTYPE_LOOP);
	}
}

void Gameover::normalUpdate(const InputState& input)
{
	if (musicVolume < 120) {
		musicVolume++;
		SetVolumeMusic(musicVolume);
	}

	const int nameCount = static_cast <int>(gameOverChoiceTable.size());

	if (input.isTriggered(InputType::up)) {
		ChangeVolumeSoundMem(160, uiSound);
		PlaySoundMem(uiSound, DX_PLAYTYPE_BACK);
		currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
	}
	else if (input.isTriggered(InputType::down)) {
		ChangeVolumeSoundMem(160, uiSound);
		PlaySoundMem(uiSound, DX_PLAYTYPE_BACK);
		currentInputIndex = (currentInputIndex + 1) % nameCount;
	}


	if (currentInputIndex == 0) {
		if (input.isTriggered(InputType::next))
		{
			ultimateTimer = ultimateFrames;
			ChangeVolumeSoundMem(180, uiSound2);
			PlaySoundMem(uiSound2, DX_PLAYTYPE_BACK);
			updateFunc_ = &Gameover::continueFadeOutUpdate;
		}
	}
	if (currentInputIndex == 1) {
		if (input.isTriggered(InputType::next))
		{
			ultimateTimer = ultimateFrames;
			updateFunc_ = &Gameover::fadeOutUpdate;
		}
	}
}

void Gameover::fadeOutUpdate(const InputState& input)
{
	if (musicVolume > 1) {
		musicVolume--;
		SetVolumeMusic(musicVolume);
	}
	fadeValue_ = static_cast<int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval)));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new TitleScene(manager_));
		StopMusic();
		return;
	}
}

void Gameover::continueFadeOutUpdate(const InputState& input)
{
	if (musicVolume > 1) {
		musicVolume--;
		SetVolumeMusic(musicVolume);
	}

	if (motion->returnContinue()) {
		fadeValue_ = static_cast<int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval)));
	}

	if (motion->returnContinue()) {
		if (++fadeTimer_ == fade_interval) {
			if (sceneNum == 1) {
				manager_.changeScene(new GameMain(manager_));
				StopMusic();
			}
			else if (sceneNum == 2) {
				manager_.changeScene(new BossBattleScene(manager_));
				StopMusic();
			}
			return;
		}
	}
}
