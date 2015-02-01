#include <algorithm>
#include "SoundManager.h"
#include <string>
#include "Utils.h"
#include "DebugDefine.h"
using namespace std;

SoundManager* SoundManager::m_pThis = 0;

bool SoundManager::loadOGG(char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq)
{
#if defined Win32 || defined Android
	int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE];                // Local fixed size array
	FILE *f;

	// Open for binary reading
	f = fopen(fileName, "rb");

	if (f == NULL)
	{
		Log("LOAD OGG FILE FAIL");
		Log(fileName);
		return false;
	}
	// end if

	vorbis_info *pInfo;
	OggVorbis_File oggFile;

	// Try opening the given file
	if (ov_open(f, &oggFile, NULL, 0) != 0)
	{
		Log("NOT RIGHT OGG FILE");
		Log(fileName);
		return false;
	}
	// end if

	// Get some information about the OGG file
	pInfo = ov_info(&oggFile, -1);

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;
	// end if

	// The frequency of the sampling rate
	freq = pInfo->rate;

	// Keep reading until all is read
	do
	{
		// Read up to a buffer's worth of decoded sound data
#ifdef Win32
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
#endif
#ifdef Android
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, NULL);
#endif
#ifdef WindowPhone
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
#endif
		if (bytes < 0)
		{
			ov_clear(&oggFile);
			Log("Error encoding ogg file");
			Log(fileName);
			return false;
		}
		// end if

		// Append to end of buffer
		buffer.insert(buffer.end(), array, array + bytes);
	} while (bytes > 0);

	// Clean up!
	ov_clear(&oggFile);
#endif
	return true;
}
// end of LoadOGG

SoundManager::SoundManager()
{
	m_pDevice = alcOpenDevice(NULL);
	m_pContext = NULL;
	if (!m_pDevice)
	{
		Log("Error open AL Device!\n");
	}
	else
	{
		m_pContext = alcCreateContext(m_pDevice, 0);
		alcMakeContextCurrent(m_pContext);
		alGetError();
	}
	count=0;
	isSoundOn=true;
}

void SoundManager::releaseSource(SoundData sound)
{
	if (sound.buffer)
		alDeleteBuffers(1, &sound.buffer);
	if (sound.source)
		alDeleteSources(1, &sound.source);
	sound.buffer = sound.source = 0;
}

void SoundManager::DeleteSound(string soundname){
	if (_soundList.count(soundname) >= 1){
		SoundData* sound = _soundList[soundname];
		if (sound)
		releaseSource(*sound);
		_soundList.erase(soundname);
		Delete(sound);
	}
	
}
SoundManager::~SoundManager()
{
	map<string, SoundData*>::iterator iter = _soundList.begin();
	while (iter != _soundList.end())
	{
		if (iter->second)
		releaseSource(*iter->second);
		Delete(iter->second);
		iter++;
	}
	_soundList.clear();
}

SoundData* SoundManager::LoadWAV(char* sound_name)
{
	//
	ALuint buffer_id, source_id;
	ALsizei freq, size;
	ALenum format;
	unsigned char* data;
	SoundData* result;
	// check if loaded already
	string path =string(sound_name);
	// if not -> load
	if (_soundList.count(string(sound_name)) < 1)
	{
		// load wav file

		if (!loadWavFile((char*)path.c_str(), data, &size, &freq, &format))
			return 0;


		// buffer the sound
		//create our openAL buffer and check for success
		alGenBuffers(1, &buffer_id);
		alGenSources(1, &source_id);
		//check for success
		if (alGetError() != AL_NO_ERROR)
		{
			Log("Error generating OpenAL buffer.\n");
			return 0;
		}

		alBufferData(buffer_id, format, (void*)data,
			size, freq);
		delete[] data;
		// bind the source
		alSourcei(source_id, AL_BUFFER, buffer_id);
		result = new SoundData(buffer_id, source_id);
		_soundList[string(sound_name)] = result;
	}
	else
		result = _soundList[string(sound_name)];
	return result;
}

SoundData* SoundManager::LoadOGG(char* sound_name)
{
#if defined Win32 || defined Android
	ALuint buffer_id, source_id;
	ALsizei freq, size;
	ALenum format;
	SoundData* result;
	// check if loaded already
	string path = string(sound_name);
	// if not -> load
	if (_soundList.count(string(sound_name)) < 1)
	{
		// load wav file
		vector<char> buffer;
		if (!loadOGG((char*)path.c_str(), buffer, format, freq))
			return 0;


		// buffer the sound
		//create our openAL buffer and check for success
		alGenBuffers(1, &buffer_id);
		alGenSources(1, &source_id);
		//check for success
		if (alGetError() != AL_NO_ERROR)
		{
			Log("Error generating OpenAL buffer.\n");
			return 0;
		}

		alBufferData(buffer_id, format, &buffer[0], static_cast<ALsizei>(buffer.size()), freq);
		// bind the source
		alSourcei(source_id, AL_BUFFER, buffer_id);
		result = new SoundData(buffer_id, source_id);
		_soundList[string(sound_name)] = result;
	}
	else
		result = _soundList[string(sound_name)];
	return result;
#endif
#if defined WindowPhone
	return 0;
#endif
}

void SoundManager::PlayEffect(SoundData sound, bool loop)
{
	if (!isSoundOn)
		return;
	alSourcePlay(sound.source);
}

void SoundManager::PlayMusic(SoundData sound, bool loop)
{
	if (!isSoundOn)
		return;
	if (!isPlaying(sound))
		alSourcePlay(sound.source);
}

void SoundManager::SetPositionAttribute(SoundData sound, Vector3 sound_position, Vector3 sound_velocity,
	Vector3 listener_position, Vector3 listener_velocity,
	Vector3 listener_orientation, Vector3 listener_up_vector
	)
{
	ALuint source = sound.source;
	// Position of the source sound.
	ALfloat SourcePos[] = { sound_position.x, sound_position.y, sound_position.z };

	// Velocity of the source sound.
	ALfloat SourceVel[] = { sound_velocity.x, sound_velocity.y, sound_velocity.z };

	// Position of the listener.
	ALfloat ListenerPos[] = { listener_position.x, listener_position.y, listener_position.z };

	// Velocity of the listener.
	ALfloat ListenerVel[] = { listener_velocity.x, listener_velocity.y, listener_velocity.z };

	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	ALfloat ListenerOri[] = { listener_orientation.x, listener_orientation.y, listener_orientation.z,
		listener_up_vector.x, listener_up_vector.y, listener_up_vector.z };

	alListenerfv(AL_POSITION, ListenerPos);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	alSourcefv(source, AL_POSITION, SourcePos);
	alSourcefv(source, AL_VELOCITY, SourceVel);
}


void SoundManager::SetSoundAttribute(SoundData sound, ALfloat pitch, ALfloat gain)
{
	ALuint source = sound.source;
	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, gain);
}

void SoundManager::Pause(SoundData sound)
{
	if (isPlaying(sound))
		alSourcePause(sound.source);
}
void SoundManager::Stop(SoundData sound)
{
	if (isPlaying(sound))
		alSourceStop(sound.source);
}
bool SoundManager::isPlaying(SoundData sound)
{
	ALenum sound_state;
	alGetSourcei(sound.source, AL_SOURCE_STATE, &sound_state);
	return (sound_state == AL_PLAYING);
}

bool SoundManager::loadWavFile(char* filename, unsigned char* &data, ALsizei* size, ALsizei* frequency, ALenum* format)
{

	//Local Declarations
	FILE* soundFile = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	soundFile = fopen(filename, "rb");
	if (!soundFile)
	{
		Log("ERROR loading Sound!\n");
		return false;
	}

	// Read in the first chunk into the struct
	fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

	//check for RIFF and WAVE tag in memeory
	if ((riff_header.chunkID[0] != 'R' ||
		riff_header.chunkID[1] != 'I' ||
		riff_header.chunkID[2] != 'F' ||
		riff_header.chunkID[3] != 'F') ||
		(riff_header.format[0] != 'W' ||
		riff_header.format[1] != 'A' ||
		riff_header.format[2] != 'V' ||
		riff_header.format[3] != 'E'))
		printf("Invalid RIFF or WAVE Header");

	//Read in the 2nd chunk for the wave info
	fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
	//check for fmt tag in memory
	if (wave_format.subChunkID[0] != 'f' ||
		wave_format.subChunkID[1] != 'm' ||
		wave_format.subChunkID[2] != 't' ||
		wave_format.subChunkID[3] != ' ')
		printf("Invalid Wave Format");

	//check for extra parameters;
	if (wave_format.subChunkSize > 16)
		fseek(soundFile, sizeof(short), SEEK_CUR);

	//Read in the the last byte of data before the sound file
	fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
	//check for data tag in memory
	if (wave_data.subChunkID[0] != 'd' ||
		wave_data.subChunkID[1] != 'a' ||
		wave_data.subChunkID[2] != 't' ||
		wave_data.subChunkID[3] != 'a')
		throw ("Invalid data header");

	//Allocate memory for data
	data = new unsigned char[wave_data.subChunk2Size];

	// Read in the sound data into the soundData variable
	if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
		throw ("error loading WAVE data into struct!");

	//Now we set the variables that we passed in with the
	//data from the structs
	*size = wave_data.subChunk2Size;
	*frequency = wave_format.sampleRate;
	//The format is worked out by looking at the number of
	//channels and the bits per sample.
	if (wave_format.numChannels == 1) {
		if (wave_format.bitsPerSample == 8)
			*format = AL_FORMAT_MONO8;
		else if (wave_format.bitsPerSample == 16)
			*format = AL_FORMAT_MONO16;
	}
	else if (wave_format.numChannels == 2) {
		if (wave_format.bitsPerSample == 8)
			*format = AL_FORMAT_STEREO8;
		else if (wave_format.bitsPerSample == 16)
			*format = AL_FORMAT_STEREO16;
	}
	fclose(soundFile);
	return true;
}

int SoundManager::Load()
{
	return 0;
	//int err = -1;
	//Sound*temp = LoadWAV("..\\Sound\\test.wav");
	//if (!temp)
	//	return err;
	//listSound[count++] = temp;
}

void SoundManager::PlayEffect(string sound_name, bool loop)
{
	if (_soundList[sound_name])
	PlayEffect(*_soundList[sound_name], loop);
}

void SoundManager::PlayMusic(string sound_name, bool loop)
{
	if (_soundList[sound_name])
	PlayMusic(*_soundList[sound_name], loop);
}


void SoundManager::Pause(string sound_name)
{
	if (_soundList[sound_name])
	Pause(*_soundList[sound_name]);
}
void SoundManager::Stop(string sound_name)
{
	if (_soundList[sound_name])
	Stop(*_soundList[sound_name]);
}
bool SoundManager::isPlaying(string sound_name)
{
	if (_soundList[sound_name])
	return isPlaying(*_soundList[sound_name]);
}
 
void SoundManager::StopAll()
{
	map<string, SoundData*>::iterator iter = _soundList.begin();
	while (iter != _soundList.end())
	{
		if (iter->second)
		Stop(*iter->second);
		iter++;
	}
}


void SoundManager::SetPositionAttribute(string sound_name, Vector3 sound_position, Vector3 sound_velocity,
	Vector3 listener_position, Vector3 listener_velocity,
	Vector3 listener_orientation, Vector3 listener_up_vector)
{
	SoundData snd = *_soundList[sound_name];
	SetPositionAttribute(snd,  sound_position,  sound_velocity,
		 listener_position,  listener_velocity,
		 listener_orientation,  listener_up_vector);
}
void SoundManager::SetSoundAttribute(string sound_name, ALfloat pitch, ALfloat gain)
{
	SoundData snd = *_soundList[sound_name];
	SetSoundAttribute(snd, pitch, gain);
}


void SoundManager::TurnOnSound(float volume)
{
#if defined Win32 || defined Android
	map<string, SoundData*>::iterator iter = _soundList.begin();
	while (iter != _soundList.end())
	{
		float newVolume = volume;
		alSourcef((*iter->second).source, AL_GAIN, newVolume);
		iter++;
	}
	isSoundOn=true;
#endif
}

void SoundManager::TurnOffSound()
{
#if defined Win32 || defined Android
	map<string, SoundData*>::iterator iter = _soundList.begin();
	while (iter != _soundList.end())
	{
		float newVolume = 0.0f;
		alSourcef((*iter->second).source, AL_GAIN, newVolume);
		iter++;
	}
	isSoundOn=false;	
#endif
}

void SoundManager::SetVolumeForSound(string sound_name,float volume)
{
#if defined Win32 || defined Android
	SoundData snd = *_soundList[sound_name];
	if (snd.source>0)
		SetVolumeForSound(snd,volume);
#endif
}

void SoundManager::SetVolumeForSound(SoundData sound,float volume)
{
#if defined Win32 || defined Android
	float newVolume = volume;
	alSourcef(sound.source, AL_GAIN, newVolume);
#endif
}