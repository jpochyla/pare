typedef float  vec_t __attribute__((ext_vector_type(2)));
typedef struct par_t par_t;
typedef struct par_sys_t par_sys_t;

struct par_t {
  vec_t pos;
  vec_t vel;
  vec_t acc;
  float age;
  float lft;
};

struct par_sys_t {
  par_t *par;
  int    par_count;
  int    par_size;

  vec_t  env_acc;
  vec_t  emit_pos;
  float  emit_rate;
  float  emit_residue;

  float  min_emit_lft,    max_emit_lft;
  float  min_emit_angle,  max_emit_angle;
  float  min_emit_accmag, max_emit_accmag;
  float  min_emit_velmag, max_emit_velmag;
};

void age_particles(par_sys_t *, float);
void emit_particles(par_sys_t *, float);
void update_particles(par_sys_t *, float);
