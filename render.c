#include <math.h>
#include <GL/glew.h>
#if __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
#include "pare.h"

#define PAR_SIZE 64 * 1000

static par_t     par_buf[PAR_SIZE];
static par_sys_t par_sys = {
  .par             = par_buf,
  .par_count       = 0,
  .par_size        = PAR_SIZE,
  .emit_rate       = 50000.f,
  .emit_residue    = 0.f,
  .min_emit_lft    = 0.3f,
  .max_emit_lft    = 1.0f,
  .min_emit_angle  = 0.0f * M_PI,
  .max_emit_angle  = 2.0f * M_PI,
  .min_emit_accmag = 10.f,
  .max_emit_accmag = 30.f,
  .min_emit_velmag = 100.f,
  .max_emit_velmag = 200.f,
  .emit_pos        = {0.f, 0.f},
  .env_acc         = {0.f, 0.f}
};

typedef float color_t __attribute__((ext_vector_type(4)));

static float
lerpf(float a, float b, float c) {
  return a + (b - a) * c;
}

static color_t
lerpc(color_t a, color_t b, float c) {
  return (color_t){
    lerpf(a[0], b[0], c),
    lerpf(a[1], b[1], c),
    lerpf(a[2], b[2], c),
    lerpf(a[3], b[3], c),
  };
}

static void
render_particles(par_sys_t *sys) {
  glBegin(GL_LINES);
  for (int i = 0; i < sys->par_count; i++) {
    const par_t *par  = &sys->par[i];
    const vec_t head  = par->pos;
    const vec_t tail  = par->pos - par->vel * par->age * 0.1f;
    const color_t col = lerpc((color_t){1.0f, 0.1f, 1.1f, 1.0f},
                              (color_t){0.1f, 0.1f, 1.0f, 0.1f}, par->age / par->lft);
    glColor4f(col[0], col[1], col[2], col[3]);
    glVertex2f(tail.x, tail.y);
    glVertex2f(head.x, head.y);
  }
  glEnd();
}

static void
reshape(int w, int h) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glViewport(0.0f, 0.0f, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, w, 0.0f, h, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void
display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  render_particles(&par_sys);
  glutSwapBuffers();
}

static void
update(int dt) {
  const float et = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
  const vec_t op = par_sys.emit_pos;
  par_sys.emit_pos = (vec_t){
    op.x + (200 * cosf(et)),
    op.y + (150 * sinf(et * 2.f))
  };
  update_particles(&par_sys, dt * 0.001f);
  par_sys.emit_pos = op;
  glutTimerFunc(dt, update, dt);
  glutPostRedisplay();
}

static void
move(int x, int y) {
  const int ww = glutGet(GLUT_WINDOW_HEIGHT);
  par_sys.emit_pos.x = x;
  par_sys.emit_pos.y = ww - y;
}

static void
click(int button, int state, int x, int y) {
  move(x, y);
}

int
main(int argc, char **argv) {
  const int ww = 800;
  const int wh = 600;
  const int dt = 10;

  glutInit(&argc, argv);
  glutInitWindowSize(ww, wh);
  glutInitDisplayMode(GLUT_RGB);
  glutCreateWindow("Particles");

  move(ww / 2, wh / 2);

  glutMouseFunc(click);
  glutMotionFunc(move);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutTimerFunc(dt, update, dt);

  glutMainLoop();
  return 0;
}