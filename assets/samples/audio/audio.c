#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "prog9.h"

int little_endian_2(FILE *fptr){

    unsigned char c1,c2;
    fscanf(fptr, "%c%c", &c1, &c2);		// same thing as little endian 4 but with 2 instead
    int value;
    value = c2;
    value <<= 8;
    value += c1;
    return value;
 
}

int little_endian_4(FILE *fptr){

	// little endian 4 create 4 unsigned char (bytes)
    unsigned char c1,c2,c3,c4;
    fscanf(fptr, "%c%c%c%c", &c1, &c2, &c3, &c4);	// copy the bytes into there
    int value;
    value = c4;		// the integer value is equal to the most significant bit
    value <<= 8;		// bit shift left by 8 bit so now c4 is the most significant bits
    value += c3;		// add in the new value in the most significant bit location
    value <<= 8;		// bit shift etc
    value += c2;		// continue to do that
    value <<= 8;
    value += c1;
    return value;

}

WAV *read_file(char *wavfile){

	FILE * filePTR;
	filePTR = fopen(wavfile, "r");	// opening the file
	int i;
	WAV* wav_ptr = (WAV *) malloc(sizeof(WAV));	// allocating the area


	// copy all the information from the file into wav_ptr
	fread(wav_ptr->RIFF, 4, 1, filePTR);
	wav_ptr->ChunkSize = little_endian_4(filePTR);
	fread(wav_ptr->WAVE, 4, 1, filePTR);
	fread(wav_ptr->fmt, 4, 1, filePTR);
	wav_ptr->Subchunk1Size = little_endian_4(filePTR);
	wav_ptr->AudioFormat = little_endian_2(filePTR);
	wav_ptr->NumChan = little_endian_2(filePTR);
	wav_ptr->SamplesPerSec = little_endian_4(filePTR);
	wav_ptr->bytesPerSec = little_endian_4(filePTR);
	wav_ptr->blockAlign = little_endian_2(filePTR);
	wav_ptr->bitsPerSample = little_endian_2(filePTR);
	wav_ptr->extra = (char *) malloc(wav_ptr->Subchunk1Size - 16 * sizeof(char));
	fread(wav_ptr->extra, wav_ptr->Subchunk1Size - 16, 1, filePTR);
	fread(wav_ptr->Subchunk2ID, 4, 1, filePTR);
	wav_ptr->Subchunk2Size = little_endian_4(filePTR);
	wav_ptr->data = (short int*) malloc((8*wav_ptr->Subchunk2Size)/(wav_ptr->bitsPerSample) * sizeof(short int));

	// copy all sound data into ram
	for (i = 0; i < (8*wav_ptr->Subchunk2Size)/(wav_ptr->bitsPerSample); i++) {
		wav_ptr->data[i] = little_endian_2(filePTR);
	}

	// print out all the information
	printf("RIFF: %c%c%c%c\n", wav_ptr->RIFF[0],wav_ptr->RIFF[1],wav_ptr->RIFF[2],wav_ptr->RIFF[3]);
	printf("ChunkSize: %d\n", wav_ptr->ChunkSize);
	printf("WAVE: %c%c%c%c\n", wav_ptr->WAVE[0],wav_ptr->WAVE[1],wav_ptr->WAVE[2],wav_ptr->WAVE[3]);
	printf("fmt: %c%c%c%c\n", wav_ptr->fmt[0],wav_ptr->fmt[1],wav_ptr->fmt[2],wav_ptr->fmt[3]);
	printf("Subchunk1Size: %d\n", wav_ptr->Subchunk1Size);
  	printf("AudioFormat: %d\n", wav_ptr->AudioFormat);
	printf("NumChan: %d\n", wav_ptr->NumChan);
	printf("SamplesPerSec: %d\n", wav_ptr->SamplesPerSec);
	printf("bytesPerSec: %d\n", wav_ptr->bytesPerSec);
	printf("blockAlign: %d\n", wav_ptr->blockAlign);
	printf("bitsPerSample: %d\n", wav_ptr->bitsPerSample);
	printf("Subchunk2ID: %c%c%c%c\n", wav_ptr->Subchunk2ID[0], wav_ptr->Subchunk2ID[1], wav_ptr->Subchunk2ID[2], wav_ptr->Subchunk2ID[3]);
	printf("Subchunk2Size: %d\n", wav_ptr->Subchunk2Size);

	// close file pointer
  	fclose(filePTR);

  return wav_ptr;

}

void sloop(WAV *inwav, char *outfile, double a, double b, int n){

	int i = 0, j = 0, k = 0;
	FILE *outPTR;		// initialize the file pointer
	outPTR = fopen(outfile, "w");	// open the file

	double sizeDif = n * (b-a) * inwav->bytesPerSec;	// additional amount of samples
	double newChunk = inwav->ChunkSize - inwav->Subchunk2Size + sizeDif;	// find the new chunk size


	// fwrite(inwav->RIFF, 4, 1, outPTR);
	// fwrite(&newChunk, 1, 4, outPTR);
	// fwrite(inwav->WAVE, 4, 1, outPTR);
	// fwrite(inwav->fmt, 4, 1, outPTR);
	// fwrite(&inwav->Subchunk1Size, 1, 20, outPTR);
	// fwrite(inwav->extra, 1, inwav->Subchunk1Size - 16, outPTR);
	// fwrite(inwav->Subchunk2ID, 4, 1, outPTR);
	// fwrite(&sizeDif, 1, 4, outPTR);
	// for (j = 0; j < n; j++)
	// 	for (i = a * inwav->SamplesPerSec * inwav->NumChan; i < b * inwav->SamplesPerSec * inwav->NumChan; i++)
	// 		fwrite(&inwav->data[i], 1, sizeof(short int), outPTR);

	WAV *out_wav = (WAV *) malloc(sizeof(WAV));	// allocating the space for the new data


	//This chunk copies over all the header information from inwav to outwav so the outwav is pretty much a copy of the inwav so far

	for (i = 0; i < 4; i++) {		// copying all of the new data into the new wav
		out_wav->RIFF[i] = inwav->RIFF[i];
		out_wav->WAVE[i] = inwav->WAVE[i];
		out_wav->fmt[i] = inwav->fmt[i];
		out_wav->Subchunk2ID[i] = inwav->Subchunk2ID[i];
	}
	out_wav->ChunkSize = newChunk;
	out_wav->Subchunk1Size = inwav->Subchunk1Size;
	out_wav->AudioFormat = inwav->AudioFormat;
	out_wav->NumChan = inwav->NumChan;
	out_wav->SamplesPerSec = inwav->SamplesPerSec;
	out_wav->bytesPerSec = inwav->bytesPerSec;
	out_wav->blockAlign = inwav->blockAlign;
	out_wav->bitsPerSample = inwav->bitsPerSample;
	out_wav->extra = (char *) malloc( sizeof(char) * out_wav->Subchunk1Size);
	out_wav->extra = inwav->extra;
	out_wav->Subchunk2Size = sizeDif;
	out_wav->data = (short int*) malloc( sizeof(short int) * (out_wav->Subchunk2Size*8/out_wav->bitsPerSample));

	for (i = 0, k = 0; i < n; i++)
		for (j = out_wav->NumChan * a * inwav->SamplesPerSec; j < out_wav->NumChan * b * inwav->SamplesPerSec; j++, k++)
			out_wav->data[k] = inwav->data[j];	// loop the stuff i want to grab to the out wav

	fwrite(out_wav, 1, 36, outPTR);	// print out the stuff
	fwrite(out_wav->extra, 1, (inwav->Subchunk1Size - 16), outPTR);	// print out the extra params
	fwrite(out_wav->Subchunk2ID, 1, 8, outPTR);			// writing all the data
	for (i = 0; i < out_wav->Subchunk2Size*8/out_wav->bitsPerSample; i++) {	// writing the actual sound data
		fwrite(&out_wav->data[i], 1, sizeof(short int), outPTR);
	}

	free(inwav->data);	// free all the memory
	free(inwav);	// free the memory
	free(out_wav->data);
	free(out_wav);

    fclose(outPTR);
    return;

}

//Extra credit
void lo_pass(WAV *inwav, char *outfile, int freq){
	double r = 0.5;
	int i;

	FILE * filePTR;
	filePTR = fopen(outfile, "w");		// file pointer 
	short int *out_data = (short int *) malloc( sizeof(short int) * inwav->Subchunk2Size * 8/inwav->bitsPerSample);

	double a1, a2, a3, b1, b2, c;

	c = 1.0/(tan (M_PI * freq/inwav->SamplesPerSec));		// doing
	a1 = 1.0 / (1.0 + r * c + c * c);		// the necessary
	a2 = 2.0 * a1;	// calculations
	a3 = a1;	// for the low
	b1 = 2.0 * (1.0 - c * c) * a1;	// pass
	b2 = (1.0 - r * c + c * c) * a1;	// filter

	for (i = 0; i < (inwav->Subchunk2Size * 8/inwav->bitsPerSample); i+=inwav->NumChan) {	// copy the left channel
		if (i - inwav->NumChan < 0 && i - 2 * inwav->NumChan < 0) out_data[i] = (a1 * inwav->data[i]);
		else if (i - 2 * inwav->NumChan < 0) out_data[i] = (a1 * inwav->data[i]) + (a2 * inwav->data[i - inwav->NumChan]) - (b1 * out_data[i - inwav->NumChan]);
		else out_data[i] = (a1 * inwav->data[i]) + (a2 * inwav->data[i - inwav->NumChan]) + (a3 * inwav->data[i - (2 * inwav->NumChan)]) - (b1 * out_data[i - inwav->NumChan]) - (b2 * out_data[i - (inwav->NumChan * 2)]);
	}

	if (inwav->NumChan > 1)		// copy the right channel
		for (i = 1; i < (inwav->Subchunk2Size * 8/inwav->bitsPerSample); i+=inwav->NumChan) {
			if (i - inwav->NumChan < 0 && i - 2 * inwav->NumChan < 0) out_data[i] = (a1 * inwav->data[i]);
			else if (i - 2 * inwav->NumChan < 0) out_data[i] = (a1 * inwav->data[i]) + (a2 * inwav->data[i - inwav->NumChan]) - (b1 * out_data[i - inwav->NumChan]);
			else out_data[i] = (a1 * inwav->data[i]) + (a2 * inwav->data[i - inwav->NumChan]) + (a3 * inwav->data[i - (2 * inwav->NumChan)]) - (b1 * out_data[i - inwav->NumChan]) - (b2 * out_data[i - (inwav->NumChan * 2)]);
		}

	fwrite(inwav, 1, 36, filePTR);		// write the inwave file
	fwrite(inwav->extra, 1, (inwav->Subchunk1Size - 16), filePTR);
	fwrite(inwav->Subchunk2ID, 1, 8, filePTR);		// write the id adn stuff
	fwrite(out_data, 1, inwav->Subchunk2Size, filePTR);		// write the file data

	free (inwav->data);		// free memory
	free (out_data);
	free (inwav);

	fclose(filePTR);
	return;
}
