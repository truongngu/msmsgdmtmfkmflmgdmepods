#ifndef __SOUNDMANAGER__
#define __SOUNDMANAGER__

#	include <cstdio>
#	include <string>
#	include "../Utilities/utilities.h"
#	include "DebugDefine.h"
#	include "al.h"
#	include "alc.h"

#ifdef Win32
#	include "vorbis/vorbisfile.h"
#endif
#ifdef Android
#	include "Tremolo/ivorbisfile.h"
#endif
#if defined WindowPhone
//#include <xaudio2.h>
//#pragma comment(lib,"xaudio2.lib")
//#	include "vorbis/vorbisfile.h"
#endif
#	include <map>
#	include <vector>
#	define BUFFER_SIZE   32768
#	define SOUND_SPEED 343.0f


using namespace std;

struct RIFF_Header {
	char chunkID[4];
	long chunkSize;//size not including chunkSize or chunkID
	char format[4];
};

struct WAVE_Format {
	char subChunkID[4];
	long subChunkSize;
	short audioFormat;
	short numChannels;
	long sampleRate;
	long byteRate;
	short blockAlign;
	short bitsPerSample;
};

struct WAVE_Data {
	char subChunkID[4]; //should contain the word data
	long subChunk2Size; //Stores the size of the data block
};



struct SoundData
{
	ALuint buffer;
	ALuint source;
	SoundData(ALint tbuffer, ALuint tsource)
	{
		buffer = tbuffer;
		source = tsource;
	}

	~SoundData()
	{
	}
};

class SoundManager
{
private:
	static SoundManager* m_pThis;
	ALCdevice* m_pDevice;
	ALCcontext* m_pContext;
#ifdef WindowPhone
	/*Microsoft::WRL::ComPtr<IXAudio2> XAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	static const unsigned int DEFAULT_PLAYBACK_DEVICE = 0xffffffff;*/
#endif
	//Sound* listSound[10];
	map<string, SoundData*> _soundList;
	int count;
	bool isSoundOn;
public:
	static SoundManager* getInstance()
	{
		if (!m_pThis)
			m_pThis = new SoundManager;
		return m_pThis;
	}
	SoundData* LoadWAV(char* file_name);
	SoundData* LoadOGG(char* file_name);
	int Load();
	void PlayEffect(string sound_name, bool loop);
	void PlayMusic(string sound_name, bool loop);
	void SetPositionAttribute(string sound_name, Vector3 sound_position, Vector3 sound_velocity,
		Vector3 listener_position, Vector3 listener_velocity,
		Vector3 listener_orientation, Vector3 listener_up_vector
		);
	void SetSoundAttribute(string sound_name, ALfloat pitch, ALfloat gain);
	void Pause(string sound_name);
	void Stop(string sound_name);
	void StopAll();
	bool isPlaying(string sound_name);
	void TurnOffSound();
	void TurnOnSound(float volume);
	bool IsSoundOn(){return isSoundOn;}
	void SetVolumeForSound(string sound_name,float volume);
	//void delete
	void DeleteSound(string sound_name);
	~SoundManager();
#if defined WindowPhone
	void getDevicesList(std::vector<std::string> &devicesListOut);
#endif
private:
	void PlayEffect(SoundData sound, bool loop);
	void PlayMusic(SoundData sound, bool loop);
	void SetPositionAttribute(SoundData sound, Vector3 sound_position, Vector3 sound_velocity,
		Vector3 listener_position, Vector3 listener_velocity,
		Vector3 listener_orientation, Vector3 listener_up_vector
		);
	void SetSoundAttribute(SoundData sound, ALfloat pitch, ALfloat gain);
	void releaseSource(SoundData sound);
	void Pause(SoundData sound);
	void Stop(SoundData sound);
	bool isPlaying(SoundData sound);
	void SetVolumeForSound(SoundData sound,float volume);
	bool loadWavFile(char* filename, unsigned char* &data, ALsizei* size, ALsizei* frequency, ALenum* format);
	bool loadOGG(char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq);
	SoundManager();
};

#endif