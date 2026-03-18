#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "graphics.h"

#define FRAME_COUNT 120
#define IMG_WIDTH 800
#define IMG_HEIGHT 600
#define TOTAL_PARTICLES 15000
#define DELTA_TIME 0.0166f

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    Image *src = image_create(IMG_HEIGHT, IMG_WIDTH);
    if (!src) return 1;

    ParticleSystem ps;
    Point emitter = { IMG_WIDTH / 2.0f, IMG_HEIGHT * 0.75f, 0.0f };
    
    init_particle_system(&ps, TOTAL_PARTICLES, 2.5f, emitter);
    init_fire(&ps, emitter);

    char filename[64];

    for (int frame = 0; frame < FRAME_COUNT; frame++) {
        update_particles(&ps, DELTA_TIME);
        render_particles(&ps, src, IMG_WIDTH, IMG_HEIGHT);

        snprintf(filename, sizeof(filename), "fire_frame_%03d.ppm", frame);
        image_write(src, filename);
        
        image_reset(src);
    }

    image_free(src);
    free(ps.particles);

    return 0;
}