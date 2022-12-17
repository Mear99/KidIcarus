#include "pch.h"
#include "SoundManager.h"
#include "SoundEffect.h"
#include "SoundStream.h"

SoundManager* SoundManager::m_pManager{ nullptr };

SoundManager* SoundManager::GetInstance() {
	if (!m_pManager) {
		m_pManager = new SoundManager();
	}
	return m_pManager;
}

SoundManager::SoundManager()
	: m_pCurrentMusic{ nullptr }
{
	m_Music["Underworld"] = new SoundStream{ "Resources/Sounds/02 The Underworld.mp3" };
	m_Music["Boss"] = new SoundStream{ "Resources/Sounds/06 Boss.mp3" };
	m_Music["Reaper"] = new SoundStream{ "Resources/Sounds/08 Reaper Theme.mp3" };
	m_Music["Menu"] = new SoundStream{ "Resources/Sounds/01 Title BGM.mp3" };
	m_Music["GameOver"] = new SoundStream{ "Resources/Sounds/GameOver.mp3" };
	m_Music["Win"] = new SoundStream{ "Resources/Sounds/Win.wav" };

	m_SoudEffects["Jump"] = new SoundEffect{ "Resources/Sounds/Jump.wav" };
	m_SoudEffects["Shoot"] = new SoundEffect{ "Resources/Sounds/Shoot.wav" };
	m_SoudEffects["Damage"] = new SoundEffect{ "Resources/Sounds/Damage.wav" };
	m_SoudEffects["Collected"] = new SoundEffect{ "Resources/Sounds/Collected.wav" };
	m_SoudEffects["EnemyKilled"] = new SoundEffect{ "Resources/Sounds/EnemyKilled.wav" };
	m_SoudEffects["Timer"] = new SoundEffect{ "Resources/Sounds/Timer.wav" };

}

void SoundManager::CleanUp() {

	for (std::pair<std::string, SoundEffect*> effect : m_SoudEffects) {
		delete effect.second;
	}
	for (std::pair<std::string, SoundStream*> music : m_Music) {
		delete music.second;
	}

	delete m_pManager;
}

void SoundManager::PlayMusic(std::string name, bool loop) {

	if (m_Music.count(name) && m_Music[name] != m_pCurrentMusic) {
		if (SoundStream::IsPlaying()) {
			SoundStream::Stop();
		}
		m_Music[name]->Play(loop);
		m_pCurrentMusic = m_Music[name];
	}
	else if (name == "") {
		if (SoundStream::IsPlaying()) {
			SoundStream::Stop(); 
				m_pCurrentMusic = nullptr;
		}
	}
}

void SoundManager::PlayEffect(std::string name) {
	m_SoudEffects[name]->Play(0);
}
