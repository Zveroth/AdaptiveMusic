#pragma once

#include "stdio.h"
#include "conio.h"
#include "irrKlang.h"




int main()
{
	unsigned int Health = 100;
	unsigned int Sanity = 100;

	irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

	irrklang::ISound* MainTrack = SoundEngine->play2D("music.wav", true, false, true);
	irrklang::ISound* Heartbeat = nullptr;
	float HeartbeatVol = 0.0f;
	irrklang::ISound* Nightmare = nullptr;
	float NightmareVol = 0.0f;

	char Key = 0;

	printf("Controls:\nq - quit\ne - decrease health by 5\nr - increase health by 5\ny - decrease sanity by 5\nu - increase sanity by 5\nMusic starts to adapt after at least one stat falls bellow 50\n\n");

	while (Key != 'q')
	{
		Key = _getch();

		switch (Key)
		{
		default:
			break;

		case 'e':
			if (Health > 5)
			{
				Health -= 5;

				if (Health < 50)
				{
					HeartbeatVol = 1.0f - (Health / 100.0f);
					if (!Heartbeat)
					{
						Heartbeat = SoundEngine->play2D("heartbeat.wav", true, true, true);
						Heartbeat->setPlayPosition(Heartbeat->getPlayLength() * ((float)MainTrack->getPlayPosition() / (float)MainTrack->getPlayLength()));
						Heartbeat->setVolume(2.0f * HeartbeatVol);
						Heartbeat->setIsPaused(false);
					}
					else
						Heartbeat->setVolume(2.0f * HeartbeatVol);

					if (HeartbeatVol > NightmareVol)
						MainTrack->setVolume(1.0f - (0.7f * (HeartbeatVol - 0.5f) * 2.0f));
					else
						MainTrack->setVolume(1.0f - (0.7f * (NightmareVol - 0.5f) * 2.0f));
				}
			}
			break;
		case 'r':
			if (Health < 100)
			{
				Health += 5;
				HeartbeatVol = 1.0f - (Health / 100.0f);

				if (Heartbeat)
				{
					if (Health >= 50)
					{
						Heartbeat->stop();
						Heartbeat->drop();
						Heartbeat = nullptr;
						if (NightmareVol <= 0.5f)
							MainTrack->setVolume(1.0f);
					}
					else
					{
						Heartbeat->setVolume(2.0f * HeartbeatVol);

						if (HeartbeatVol > NightmareVol)
							MainTrack->setVolume(1.0f - (0.7f * (HeartbeatVol - 0.5f) * 2.0f));
						else
							MainTrack->setVolume(1.0f - (0.7f * (NightmareVol - 0.5f) * 2.0f));
					}
				}
			}
			break;

		case 'y':
			if (Sanity > 5)
			{
				Sanity -= 5;

				if (Sanity < 50)
				{
					NightmareVol = 1.0f - (Sanity / 100.0f);
					if (!Nightmare)
					{
						Nightmare = SoundEngine->play2D("nightmare.wav", true, true, true);
						Nightmare->setPlayPosition(Nightmare->getPlayLength() * ((float)MainTrack->getPlayPosition() / (float)MainTrack->getPlayLength()));
						Nightmare->setVolume(0.35f * NightmareVol);
						Nightmare->setIsPaused(false);
					}
					else
						Nightmare->setVolume(0.35f * NightmareVol);

					if (NightmareVol > HeartbeatVol)
						MainTrack->setVolume(1.0f - (0.7f * (NightmareVol - 0.5f) * 2.0f));
					else
						MainTrack->setVolume(1.0f - (0.7f * (HeartbeatVol - 0.5f) * 2.0f));
				}
			}
			break;
		case 'u':
			if (Sanity < 100)
			{
				Sanity += 5;
				NightmareVol = 1.0f - (Sanity / 100.0f);

				if (Nightmare)
				{
					if (Sanity >= 50)
					{
						Nightmare->stop();
						Nightmare->drop();
						Nightmare = nullptr;
						if (HeartbeatVol <= 0.5f)
							MainTrack->setVolume(1.0f);
					}
					else
					{
						Nightmare->setVolume(0.35f * NightmareVol);

						if (NightmareVol > HeartbeatVol)
							MainTrack->setVolume(1.0f - (0.7f * (NightmareVol - 0.5f) * 2.0f));
						else
							MainTrack->setVolume(1.0f - (0.7f * (HeartbeatVol - 0.5f) * 2.0f));
					}
				}
			}
			break;
		}
		printf("Health: .%d.	Sanity: .%d.\r", Health, Sanity);
	}

	MainTrack->drop();
	if (Heartbeat)
		Heartbeat->drop();
	if (Nightmare)
		Nightmare->drop();

	SoundEngine->drop();

	return 1;
}