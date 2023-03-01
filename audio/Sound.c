#include "Sound.h"

AudioManager *aMan;

int initAudio() {
	PaStreamParameters outputParameters;
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		goto exit;
	}
	aMan = calloc(1, sizeof(AudioManager));
	aMan->sounds = makeList();
	aMan->volumes = calloc(1, sizeof(float));
	aMan->volumes[0] = 1;
	aMan->vGroups = 1;
	//aMan->stream = calloc(1, sizeof(PaStream));
	aMan->mix = makeList();
	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

	if (outputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default output device.\n");
		goto exit;
	}

	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
			&aMan->stream,
			NULL, /* no input */
			&outputParameters,
			44100,
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
	long readcount = 0;
	memset(out, 0x00, framesPerBuffer);
	int f = framesPerBuffer * 2;
	int first = 0;
	AudioManager *a = userData;
	linkedList *cur = a->mix;
	linkedList *pre = 0;
	while (cur) {
		Sound *s = cur->data;
		if (s) {
			float volume = a->volumes[s->volume];
			if (s->reads >= s->len) {
				if (s->loop) {
					s->reads = 0;
				} else {
					cur->data = 0;
					continue;
				}
			}
			if (first == 0) {
				memcpy(out, s->buff + f * s->reads, f * sizeof(float));
				for (int i = 0; i < f; i++) {
					out[i] *= volume;
				}
				first = 1;
			} else {
				for (int i = 0; i < f; i++) {
					out[i] += (s->buff[s->reads * f + i] * volume);
				}
			}
			s->reads++;
		}
		pre = cur;
		cur = cur->next;
	}
	/*	
			for (int i = 0; i < m->max; i++) {
			Sound *s = m->sounds[i];
			if (s) {
			if (s->reads >= s->len) {
			printf("song %i complete\n", i);
			if (s->loop) {
			s->reads = 0;
			} else {
			m->sounds[i] = 0;
			m->num--;
			continue;
			}
			}
			if (first == 0) {
			memcpy(out, s->buff + f * s->reads, f * sizeof(float));
			first = 1;
			} else {
			for (int j = 0; j < f; j++) {
			out[j] += s->buff[s->reads * f + j];
			}
			}
			s->reads++;
			}
			}
	 */
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
	s->reads = 0;
	s->buff = calloc(size, sizeof(float));
	s->volume = 0;//aMan->volumes;
	sf_read_float(infile, s->buff, size);
	s->len = size / (FPB * 2);
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
		if (!cur->data) {
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
