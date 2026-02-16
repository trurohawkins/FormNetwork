SoundBank *sounds = 0;

int processAudioFile(char *file, bool loop) {
	//linkedList *cur = aMan->sounds;
	int fileLen = strlen(file);
	for (int i = 0; i < sounds->soundNum; i++) {
		Sound s = sounds->bank[i];
		if (memcmp(s.file, file, fileLen) == 0) {
			printf("already have the sound\n");
			return i;
		}
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
	Sound *s = &sounds->bank[sounds->soundNum];//calloc(1, sizeof(Sound));
	s->file = calloc(fileLen + 1, sizeof(char));
	memcpy(s->file, file, fileLen);
	s->file[fileLen] = '\0';
	s->readFrames = 0;
	s->buff = calloc(size, sizeof(float));
	s->volume = 0;//aMan->volumes;
	s->loop = loop;
	sf_read_float(infile, s->buff, size);
	//s->len = size / (FPB * 2);
	s->totalFrames = size / 2;
	sf_close(infile);
	//addToList(&aMan->sounds, s);
	sounds->soundNum++;
	return sounds->soundNum-1;
}

void playAudio(int sound) {
	if (sound >= 0 && sound < sounds->soundNum) {
		AudioCommand ac;// = calloc(1, sizeof(AudioCommand));
		ac.cmd = 0;
		ac.data = sound;
		aqPush(&audioQueue, &ac, sizeof(AudioCommand));
		if (!streaming) {
			Pa_StartStream(aMan->stream);
			streaming = true;
		}
	}
	//addToList(&aMan->mix, s);
	//aMan->mixCount++;
	/*
		 aMan->m->sounds[aMan->m->num] = s;
		 aMan->m->num++;
	 */
}

void stopAudio(int sound) {
	if (sound >= 0 && sound < sounds->soundNum) {
		AudioCommand ac;
		ac.cmd = 1;
		ac.data = sound;
		aqPush(&audioQueue, &ac, sizeof(AudioCommand));
		//printf("stopping audio %s\n", s->file);
		/*
		if (removeFromList(&aMan->mix, s)) {
			//printf("stop successful\n");
			aMan->mixCount--;
			if (aMan->mixCount == 0) {
				Pa_StopStream(aMan->stream);
			}
		}
		*/
	}
}

void scheduleAudio(int sound, double frequency) {
	if (sound >= 0 && sound < sounds->soundNum) {
		AudioCommand ac;
		ac.cmd = 0;
		ac.sound = sound;
		ac.data = frequency;
		aqPush(&audioQueue, &ac, sizeof(AudioCommand));
	}
	/*
	Sound s = sounds->bank[sound];
	s.scheduled = true;
	s.intervalFrames = (long long)(aMan->sampleRate * frequency / aMan->bpm);
	s.nextTriggerFrame = 0;
	s.active = false;
	s.bufferOffset = 0;
	//playAudio(s);
	*/
}

void freeSound(void *snd) {
	Sound *s = snd;
	free(s->file);
	free(s->buff);
	free(s);
}

void freeSoundBank() {
	for (int i = 0; i < sounds->soundNum; i++) {
		Sound s = sounds->bank[i];
		free(s.file);
		free(s.buff);
	}
	free(sounds);
}

