#include "../helper.h"
#include "Sound.h"

AudioManager *aMan = 0;

int initAudio() {
	aMan = calloc(1, sizeof(AudioManager));
	aMan->sounds = makeList();
	aMan->volumes = calloc(1, sizeof(float));
	aMan->volumes[0] = 1;
	aMan->vGroups = 1;
	//aMan->stream = calloc(1, sizeof(PaStream));
	aMan->mix = makeList();

	PaStreamParameters outputParameters;
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		goto exit;
	}
	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

	if (outputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default output device.\n");
		goto exit;
	}

	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	double sampleRate = 44100.0;

	err = Pa_OpenStream(
			&aMan->stream,
			NULL, /* no input */
			&outputParameters,
			sampleRate,
			FPB,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			paLibsndfileCb,
			aMan);

	if(err != paNoError) {
		printf("open stream error  ");
		aMan->stream = 0;//do i need?
		goto exit;
	}

	err = Pa_StartStream(aMan->stream);
	if (err != paNoError) {
		goto exit;
	}
	const PaStreamInfo *info = Pa_GetStreamInfo(aMan->stream);
	aMan->sampleRate = info->sampleRate;
	aMan->bpm = 120.0;
	return err;

exit:
	if (err != paNoError) { 
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
	}
	if (aMan != 0) {
		freeAudioManager();
	}
	Pa_Terminate();

}

static int paLibsndfileCb(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData) {
	float *out = (float*)outputBuffer;
	memset(out, 0, framesPerBuffer * 2 * sizeof(float));
	AudioManager *a = userData;
	long long bufferStart = aMan->currentFrame;
	long long bufferEnd = bufferStart + framesPerBuffer;

	// check total songs for schedule
	/*
	linkedList *cur = a->sounds;
	while (cur) {
		Sound *s = cur->data;
		if (s && s->scheduled) {
			if (!s->active && bufferEnd >= s->nextTriggerFrame) {
				s->readFrames = 0;
				s->active = true;
				addToList(&aMan->mix, s);

				s->nextTriggerFrame += s->intervalFrames;
			}
		}
		cur = cur->next;
	}
	*/
	// mix of current songs
	linkedList *cur = a->mix;

	while (cur) {
		Sound *s = cur->data;
		if (!s) {
			cur = cur->next;
			continue;
		}

		if (s->scheduled && !s->active) {
			//if (bufferEnd >= s->nextTriggerFrame) {
			if (s->nextTriggerFrame >= bufferStart && s->nextTriggerFrame < bufferEnd) {
				s->readFrames = 0;
				s->active = true;
				s->bufferOffset = s->nextTriggerFrame - bufferStart;
				s->nextTriggerFrame += s->intervalFrames;
			}
		}


		// mixing
		if (s->active) {
			long remaining = s->totalFrames - s->readFrames;
			if (remaining <= 0) {
				s->readFrames = 0;
				if (!s->loop && !s->scheduled) {
					s->active = false;
					//cur->data = 0;
					cur = cur->next;
					continue;
				}
			}
			long framesToMix = remaining < framesPerBuffer ? remaining : framesPerBuffer;
			long sampleOffset = s->readFrames * 2;
			float volume = a->volumes[s->volume];

			long mixStart = 0;
			if (s->scheduled) {
				mixStart = s->bufferOffset;
				s->bufferOffset = 0;
			}

			for (long i = mixStart*2; i < framesToMix * 2; i++) {
				long buffIndex = sampleOffset + i - mixStart*2;
				if (buffIndex < s->totalFrames * 2) {
					out[i] += s->buff[buffIndex] * volume;
				}
			}
			s->readFrames += framesToMix;
		}
		cur = cur->next;
	}

	for (long i = 0; i < framesPerBuffer * 2; i++) {
		if (out[i] > 1.0f) {
			out[i] = 1.0f;
		} else if (out[i] < -1.0f) {
			out[i] = -1.0f;
		}
	}

	aMan->currentFrame += framesPerBuffer;

	return paContinue;
}

Sound *processAudioFile(char *file) {
	linkedList *cur = aMan->sounds;
	int fileLen = strlen(file);
	while (cur) {
		Sound *s = cur->data;
		if (s) {
			if (memcmp(s->file, file, fileLen) == 0) {
				printf("already have the sound\n");
				return s;
			}
		}
		cur = cur->next;
	}
	SNDFILE *infile = 0;
	SF_INFO sfinfo ;
	if (!(infile = sf_open(file, SFM_READ, &sfinfo))) {
		printf ("Not able to open input file %s.\n", file) ;
		sf_perror (NULL) ;
		return  0 ;
	}
	long read = 0;
	long size = sf_seek(infile, 0, SEEK_END);
	sf_seek(infile, 0, SEEK_SET);
	Sound *s = calloc(1, sizeof(Sound));
	s->file = calloc(fileLen + 1, sizeof(char));
	memcpy(s->file, file, fileLen);
	s->file[fileLen] = '\0';
	s->readFrames = 0;
	s->buff = calloc(size, sizeof(float));
	s->volume = 0;//aMan->volumes;
	sf_read_float(infile, s->buff, size);
	//s->len = size / (FPB * 2);
	s->totalFrames = size / 2;
	sf_close(infile);
	addToList(&aMan->sounds, s);
	return s;
}

void cleanUpPlayedAudio() {
	if (!aMan) {
		return;
	}
	linkedList *cur = aMan->mix;
	linkedList *pre = 0;
	while (cur) {
		Sound *s = cur->data;
		if (s && !s->active && !s->scheduled) {
			linkedList *tmp = cur;
			if (!pre) {
				aMan->mix = cur->next;
			} else {
				pre->next = cur->next;
			}
			cur = cur->next;
			free(tmp);
			aMan->mixCount--;
			continue;
		}
		pre = cur;
		cur = cur->next;
	}
}

void changeVolume(int group, float vol) {
	if (group < aMan->vGroups) {
		aMan->volumes[group] = vol;
	}
}

int addVolGroup() {
	if (aMan) {
		int cur = aMan->vGroups;
		aMan->vGroups++;
		float *tmp = calloc(aMan->vGroups, sizeof(float));
		for (int i = 0; i < aMan->vGroups - 1; i++) {
			tmp[i] = aMan->volumes[i];
		}
		tmp[aMan->vGroups-1] = 1;
		free(aMan->volumes);
		aMan->volumes = tmp;
		return cur;
	} else {
		return -1;
	}
}
void changeVolGroup(Sound *s, int group) {
	s->volume = group;
}

void playAudio(Sound *s) {
	s->active = true;
	addToList(&aMan->mix, s);
	if (aMan->mixCount == 0) {
		Pa_StartStream(aMan->stream);
	}
	aMan->mixCount++;
	/*
		 aMan->m->sounds[aMan->m->num] = s;
		 aMan->m->num++;
	 */
}

void stopAudio(Sound *s) {
	//printf("stopping audio %s\n", s->file);
	if (removeFromList(&aMan->mix, s)) {
		//printf("stop successful\n");
		aMan->mixCount--;
		if (aMan->mixCount == 0) {
			Pa_StopStream(aMan->stream);
		}
	}
}

void scheduleAudio(Sound *s, double frequency) {
	s->scheduled = true;
	s->intervalFrames = (long long)(aMan->sampleRate * frequency / aMan->bpm);
	s->nextTriggerFrame = 0;
	s->active = false;
	s->bufferOffset = 0;
	playAudio(s);
}


void freeSound(void *snd) {
	Sound *s = snd;
	free(s->file);
	free(s->buff);
	free(s);
}

void endAudio() {
	freeAudioManager();
	Pa_Terminate();
}

void freeAudioManager() {
	if (aMan) {
		if (aMan->stream) {
			Pa_StopStream(aMan->stream);
			Pa_CloseStream(aMan->stream);
		}
		free(aMan->volumes);
		freeListSaveObj(&aMan->mix);
		deleteList(&aMan->sounds, freeSound);
		//free(aMan->stream);
		free(aMan);
	}
}
