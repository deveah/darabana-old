/*
 *	darabana
 *	minimal drum synth
 *
 *	Author: Vlad Dumitru
 *	As long as you retain this notice, you are free to do whatever you want
 *	with this file. If we meet some day, and you shall consider me worthy,
 *	I will gladly accept donations in beer.
*/

/*	some presets:
	2 0.9995 35 0.001 5 0.99 0.93 (bd)
	2 0.9995 70 0.002 1 0.9998 0.5 (sd/clap)
	0 0 0 0 1 0.995 0 (closed hat)
	0 0 0 0 1 0.9999 0 (crash)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sndfile.h>

#define SAMPLE_RATE 44100
#define OUTPUT_FILE "output.wav"

SNDFILE *out;

int main( int argc, char** argv )
{
	int i;
	float amp, decay, freq, freqdecay;
	float noise, noisedecay, noisefilter;
	float buf[SAMPLE_RATE];
	float f = 0;

	if( argc < 8 )
		return 0;
	
	SF_INFO sfinfo;
	sfinfo.samplerate = SAMPLE_RATE;
	sfinfo.channels = 1;
	sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
	out = sf_open( OUTPUT_FILE, SFM_WRITE, &sfinfo );

	amp = atof( argv[1] );
	decay = atof( argv[2] );
	freq = atof( argv[3] );
	freqdecay = atof( argv[4] );
	noise = atof( argv[5] );
	noisedecay = atof( argv[6] );
	noisefilter = atof( argv[7] );

	for( i = 0; i < SAMPLE_RATE; i++ )
	{
		buf[i] = amp * sin( 2.0 * M_PI * freq * (double)i / (double)SAMPLE_RATE );
		f = (noisefilter) * f + (1.0f-noisefilter) * noise * ( (float)( rand()%100 ) / 100.0f - 0.5f );
		buf[i] += f;	

		noise *= noisedecay;
		amp *= decay;
		freq -= freqdecay;

		if( noise < .0 ) noise = .0;
		if( amp < .0 ) amp = .0;
		if( freq < .0 ) freq = .0;
		if( buf[i] < -1.0 ) buf[i] = -1.0;
		if( buf[i] >  1.0 ) buf[i] =  1.0;
	}

	sf_write_float( out, buf, SAMPLE_RATE );

	sf_close( out );
	return 0;
}
