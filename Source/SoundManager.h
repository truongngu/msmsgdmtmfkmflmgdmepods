#ifndef __SOUNDMANAGER__
#define __SOUNDMANAGER__

#	include <cstdio>
#	include <string>
#	include "Utilities/utilities.h"
#	include "DebugDefine.h"
#if !defined WindowPhone
#	include "al.h"
#	include "alc.h"
#endif

#ifdef Win32
#	include "vorbis/vorbisfile.h"
#endif
#ifdef Android
#	include "Tremolo/ivorbisfile.h"
#endif
#if defined WindowPhone
#include "mmreg.h"
#include <vector>
#include <memory>
#include <xaudio2.h>
#include "SoundFileReader.h"
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

#if defined WindowPhone
namespace
{
	//
	// Handler for XAudio source voice callbacks to maintain playing state
	//
	class SoundCallbackHander : public IXAudio2VoiceCallback
	{
	public:
		SoundCallbackHander(bool* isPlayingHolder) :
			m_isPlayingHolder(isPlayingHolder)
		{
		}

		~SoundCallbackHander()
		{
			m_isPlayingHolder = nullptr;
		}

		//
		// Voice callbacks from IXAudio2VoiceCallback
		//
		STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 bytesRequired);
		STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS);
		STDMETHOD_(void, OnStreamEnd) (THIS);
		STDMETHOD_(void, OnBufferStart) (THIS_ void* bufferContext);
		STDMETHOD_(void, OnBufferEnd) (THIS_ void* bufferContext);
		STDMETHOD_(void, OnLoopEnd) (THIS_ void* bufferContext);
		STDMETHOD_(void, OnVoiceError) (THIS_ void* bufferContext, HRESULT error);

	private:
		bool* m_isPlayingHolder;
	};

	//
	// Callback handlers, only implement the buffer events for maintaining play state
	//
	void SoundCallbackHander::OnVoiceProcessingPassStart(UINT32 /*bytesRequired*/)
	{
	}
	void SoundCallbackHander::OnVoiceProcessingPassEnd()
	{
	}
	void SoundCallbackHander::OnStreamEnd()
	{
	}
	void SoundCallbackHander::OnBufferStart(void* /*bufferContext*/)
	{
		*m_isPlayingHolder = true;
	}
	void SoundCallbackHander::OnBufferEnd(void* /*bufferContext*/)
	{
		*m_isPlayingHolder = false;
	}
	void SoundCallbackHander::OnLoopEnd(void* /*bufferContext*/)
	{
	}
	void SoundCallbackHander::OnVoiceError(void* /*bufferContext*/, HRESULT /*error*/)
	{
	}
}
#endif

struct SoundData
{
#if !defined WindowPhone
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
#endif
#if defined WindowPhone
	SoundData::SoundData(Platform::Array<byte>^ data) :
		source(nullptr),
		buffer(data),
		isPlaying(false),
		callbackHander(&isPlaying)
	{
	}

	~SoundData()
	{

	}

	void Destroy(){
		if (source)
		{
			source->DestroyVoice();
			source = nullptr;
		}
	}

	IXAudio2SourceVoice*    source;
	Platform::Array<byte>^  buffer;
	bool                    isPlaying;
	SoundCallbackHander     callbackHander;
	float pitch;
	float gain;
	float volume;
#endif
};

class SoundManager
{
private:
	static SoundManager* m_pThis;
#if !defined WindowPhone
	ALCdevice* m_pDevice;
	ALCcontext* m_pContext;
#endif
#if defined WindowPhone
	interface IXAudio2*                     m_audioEngine;
	interface IXAudio2MasteringVoice*       m_masteringVoice;
#endif
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
#if !defined WindowPhone
	void SetSoundAttribute(string sound_name, ALfloat pitch, ALfloat gain);
#endif
#if defined WindowPhone
	void SetSoundAttribute(string sound_name, float pitch, float gain);
#endif
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
private:
	void PlayEffect(SoundData sound, bool loop);
	void PlayMusic(SoundData sound, bool loop);
	void SetPositionAttribute(SoundData sound, Vector3 sound_position, Vector3 sound_velocity,
		Vector3 listener_position, Vector3 listener_velocity,
		Vector3 listener_orientation, Vector3 listener_up_vector
		);
#if !defined WindowPhone
	void SetSoundAttribute(SoundData sound, ALfloat pitch, ALfloat gain);
#endif
#if defined WindowPhone
	void SetSoundAttribute(SoundData sound, float pitch, float gain);
#endif
	void releaseSource(SoundData sound);
	void Pause(SoundData sound);
	void Stop(SoundData sound);
	bool isPlaying(SoundData sound);
	void SetVolumeForSound(SoundData sound,float volume);
#if !defined WindowPhone
	bool loadWavFile(char* filename, unsigned char* &data, ALsizei* size, ALsizei* frequency, ALenum* format);
	bool loadOGG(char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq);
#endif
#if defined WindowPhone
	Platform::Array<byte>^  loadWavFile(char* filename, WAVEFORMATEX*);
	bool loadOGG(char *fileName, Platform::Array<byte>^ buffer);
#endif
	SoundManager();
};

#endif