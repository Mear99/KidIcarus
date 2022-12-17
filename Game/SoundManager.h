#pragma once
class SoundStream;
class SoundEffect;
#include <unordered_map>

class SoundManager final
{
public:
	static SoundManager* GetInstance();

	SoundManager(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;

	~SoundManager() = default;

	void CleanUp();

	void PlayMusic(std::string name, bool loop = true);
	void PlayEffect(std::string name);

private:
	SoundManager();

	static SoundManager* m_pManager;

	std::unordered_map<std::string, SoundEffect*> m_SoudEffects;
	std::unordered_map<std::string, SoundStream*> m_Music;
	SoundStream* m_pCurrentMusic;
};

