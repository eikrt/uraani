#include "sound.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

void *playSound(void *filename) {
        const char *expr = "((loop true) (notes 60 61 62))"; 
        int index = 0;
        Node *root = parseSExpression(expr, &index);
        freeNodes(root);
        SDL_Init(SDL_INIT_AUDIO);
        SDL_AudioSpec wavSpec;
        Uint32 wavLength;
        Uint8 *wavBuffer;

        if (SDL_LoadWAV("sound/piano.wav", &wavSpec, &wavBuffer, &wavLength) ==
            NULL) {
                printf("Could not load audio file: %s\n", SDL_GetError());
                return;
        }

        SDL_AudioDeviceID deviceId =
            SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
        if (deviceId == 0) {
                printf("Could not open audio device: %s\n", SDL_GetError());
                SDL_FreeWAV(wavBuffer);
                return;
        }

        int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        if (success < 0) {
                printf("Could not queue audio: %s\n", SDL_GetError());
                SDL_CloseAudioDevice(deviceId);
                SDL_FreeWAV(wavBuffer);
                return;
        }

        SDL_PauseAudioDevice(deviceId, 0);

        SDL_Delay(wavLength / (wavSpec.freq / 1000));

        SDL_CloseAudioDevice(deviceId);
        SDL_FreeWAV(wavBuffer);
}
