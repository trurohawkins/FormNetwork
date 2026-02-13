#include <portaudio.h>
#include <sndfile.h>

//Frames PerBuffer
#define FPB 4096

typedef struct Sound {
	char *file;
	float *buff;
	long totalFrames;

	long readFrames;
	bool loop;
	int volume;
	bool active;

	//timing scheduling
	long long nextTriggerFrame;
	long long intervalFrames;
	long long bufferOffset;
	bool scheduled;
} Sound;

typedef struct AudioManager {
	linkedList *mix;
	int mixCount;
	float *volumes;
	int vGroups;
	linkedList *sounds;
	PaStream *stream;

	long long currentFrame;
	double sampleRate;
	double bpm;
} AudioManager;

extern AudioManager *aMan;

int initAudio();
void endAudio();
static int paLibsndfileCb(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData);
void cleanUpPlayedAudio();
void changeVolume(int group, float vol);
Sound *processAudioFile(char *file);
void changeVolGroup(Sound *s, int group);
void playAudio(Sound *s);
void stopAudio(Sound *s);
void scheduleAudio(Sound *s, double frequency);
int addVolGroup();
void freeSound(void *snd);
void freeAudioManager();

