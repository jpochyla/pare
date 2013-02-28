#include <stdlib.h>
#include <math.h>
#include "pare.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static float
random_float(float min, float max) {
  return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

void
age_particles(par_sys_t *sys, float dt) {
  for (int i = 0; i < sys->par_count; i++) {
    par_t *par = &sys->par[i];
    par->age  += dt;
    if (par->age > par->lft) {
      --i;
      --sys->par_count;
      *par = sys->par[sys->par_count];
    } else {
      par->vel += sys->env_acc * dt;
      par->vel += par->acc * dt;
      par->pos += par->vel * dt;
    }
  }
}

void
emit_particles(par_sys_t *sys, float n) {
  int np  = n + sys->emit_residue;
  for (int i = sys->par_count; i < sys->par_count + np; i++) {
    const float velmag = random_float(sys->min_emit_velmag, sys->max_emit_velmag);
    const float accmag = random_float(sys->min_emit_accmag, sys->max_emit_accmag);
    const float angle  = random_float(sys->min_emit_angle, sys->max_emit_angle);
    const float lft    = random_float(sys->min_emit_lft, sys->max_emit_lft);
    const vec_t dir    = {cosf(angle), sinf(angle)};
    par_t *par = &sys->par[i];
    par->pos   = sys->emit_pos;
    par->vel   = dir * velmag;
    par->acc   = dir * accmag;
    par->lft   = lft;
    par->age   = 0.0f;
  }
  sys->emit_residue = n - np;
  sys->par_count   += np;
}

void
update_particles(par_sys_t *sys, float dt) {
  age_particles(sys, dt);
  emit_particles(sys, dt * sys->emit_rate);
}
