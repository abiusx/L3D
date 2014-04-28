// Source file for the mesh file viewer



////////////////////////////////////////////////////////////
// INCLUDE FILES
////////////////////////////////////////////////////////////

#include "opengl_glut.h"
#include "R3/R3.h"
#include "R3Mesh.h"
#include <math.h>


////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
////////////////////////////////////////////////////////////

// Program arguments

static char *input_mesh_name = NULL;
static char *output_mesh_name = NULL;
static char *output_image_name = NULL;
static int print_verbose = 0;



// Display variables

static R3Mesh *mesh = NULL;
static R3Point camera_eye (0, 0, 4);
static R3Vector camera_towards(0, 0, 1);
static R3Vector camera_up(0, 1, 0);
static double camera_yfov = 0.75;
static R3MeshFace *pick_face = NULL;
static R3Point pick_position = R3zero_point;
static bool pick_active = false;
static int show_faces = 1;
static int show_edges = 0;
static int show_vertices = 0;
static int show_normals = 0;
static int show_curvatures = 0;
static int show_bbox = 0;
static int show_ids = 0;
static int show_pick = 1;
static int save_image = 0;
static int quit = 0;



// GLUT variables 

static int GLUTwindow = 0;
static int GLUTwindow_height = 800;
static int GLUTwindow_width = 800;
static int GLUTmouse[2] = { 0, 0 };
static int GLUTbutton[3] = { 0, 0, 0 };
static int GLUTmodifiers = 0;



// GLUT command list

enum {
  DISPLAY_FACE_TOGGLE_COMMAND,
  DISPLAY_EDGE_TOGGLE_COMMAND,
  DISPLAY_VERTEX_TOGGLE_COMMAND,
  DISPLAY_NORMAL_TOGGLE_COMMAND,
  DISPLAY_CURVATURE_TOGGLE_COMMAND,
  DISPLAY_BBOX_TOGGLE_COMMAND,
  TWIST_COMMAND,
  SAVE_IMAGE_COMMAND,
  SAVE_MESH_COMMAND,
  QUIT_COMMAND,
};



////////////////////////////////////////////////////////////
// GLUT USER INTERFACE FUNCTIONS
////////////////////////////////////////////////////////////

void GLUTDrawText(const R3Point& p, const char *s)
{
  // Draw text string s and position p
  glRasterPos3d(p[0], p[1], p[2]);
#ifndef __CYGWIN__
  while (*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *(s++));
#else
  while (*s) glutBitmapCharacter((void*)7, *(s++));
#endif
}
  



void GLUTSaveImage(const char *filename)
{ 
  // Create image
  R2Image image(GLUTwindow_width, GLUTwindow_height);

  // Read screen into buffer
  GLfloat *pixels = new GLfloat [ 3 * GLUTwindow_width * GLUTwindow_height ];
  glReadPixels(0, 0, GLUTwindow_width, GLUTwindow_height, GL_RGB, GL_FLOAT, pixels);

  // Load pixels from frame buffer
  GLfloat *pixelsp = pixels;
  for (int j = 0; j < GLUTwindow_height; j++) {
    for (int i = 0; i < GLUTwindow_width; i++) {
      double r = (double) *(pixelsp++);
      double g = (double) *(pixelsp++);
      double b = (double) *(pixelsp++);
      R2Pixel pixel(r, g, b, 1);
      image.SetPixel(i, j, pixel);
    }
  }

  // Write image to file
  image.Write(filename);

  // Delete buffer
  delete [] pixels;
}




void GLUTStop(void)
{
  // Save mesh
  if (output_mesh_name) mesh->Write(output_mesh_name);

  // Destroy window 
  glutDestroyWindow(GLUTwindow);

  // Delete mesh
  delete mesh;

  // Exit
  exit(0);
}



void GLUTResize(int w, int h)
{
  // Resize window
  glViewport(0, 0, w, h);

  // Remember window size 
  GLUTwindow_width = w;
  GLUTwindow_height = h;

  // Redraw
  glutPostRedisplay();
}



void GLUTRedraw(void)
{
  // Set projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double mesh_radius = mesh->Radius(); 
  gluPerspective(180.0*camera_yfov/M_PI, (GLdouble) GLUTwindow_width /(GLdouble) GLUTwindow_height, 
    0.01 * mesh_radius, 100 * mesh_radius);

  // Set camera transformation
  R3Vector& t = camera_towards;
  R3Vector& u = camera_up;
  R3Vector r = camera_up % camera_towards;
  GLdouble camera_matrix[16] = { r[0], u[0], t[0], 0, r[1], u[1], t[1], 0, r[2], u[2], t[2], 0, 0, 0, 0, 1 };
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixd(camera_matrix);
  glTranslated(-camera_eye[0], -camera_eye[1], -camera_eye[2]);

  // Clear window 
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set lights
  static GLfloat light0_position[] = { 3.0, 4.0, 5.0, 0.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  static GLfloat light1_position[] = { -3.0, -2.0, -3.0, 0.0 };
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

  // Draw faces
  if (show_faces) {
    glEnable(GL_LIGHTING);
    static GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    static GLfloat specular[] = { 0.2, 0.2, 0.2, 1.0 };
    static GLfloat shininess[] = { 64 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse); 
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular); 
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess); 
    for (int i = 0; i < mesh->NFaces(); i++) {
      glBegin(GL_POLYGON);
      R3MeshFace *face = mesh->Face(i);
      const R3Vector& normal = face->plane.Normal();
      glNormal3d(normal[0], normal[1], normal[2]);
      for (unsigned int j = 0; j < face->vertices.size(); j++) {
        R3MeshVertex *vertex = face->vertices[j];
        const R3Point& p = vertex->position;
        glVertex3f(p[0], p[1], p[2]);
      }
      glEnd();
    }
  }

  // Draw edges
  if (show_edges) {
    glDisable(GL_LIGHTING);
    glColor3d(0.3, 0.3, 0.3);
    glLineWidth(3);
    for (int i = 0; i < mesh->NFaces(); i++) {
      glBegin(GL_LINE_LOOP);
      R3MeshFace *face = mesh->Face(i);
      for (unsigned int j = 0; j < face->vertices.size(); j++) {
        R3MeshVertex *vertex = face->vertices[j];
        const R3Point& p = vertex->position;
        glVertex3f(p[0], p[1], p[2]);
      }
      glEnd();
    }
  }

  // Draw vertices
  if (show_vertices) {
    glDisable(GL_LIGHTING);
    glColor3d(0, 0, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (int i = 0; i < mesh->NVertices(); i++) {
      R3MeshVertex *vertex = mesh->Vertex(i);
      const R3Point& p = vertex->position;
      glVertex3f(p[0], p[1], p[2]);
    }
    glEnd();
  }

  // Draw vertex IDs
  if (show_ids) {
    char buffer[256];
    glDisable(GL_LIGHTING);
    glColor3d(0, 0, 0);
    for (int i = 0; i < mesh->NVertices(); i++) {
      R3MeshVertex *vertex = mesh->Vertex(i);
      sprintf(buffer, "%d", vertex->id);
      GLUTDrawText(vertex->position, buffer);
    }
  }

  // Draw normals
  if (show_normals) {
    glDisable(GL_LIGHTING);
    glColor3d(0, 0, 0);
    glLineWidth(3);
    glBegin(GL_LINES);
    for (int i = 0; i < mesh->NVertices(); i++) {
      R3MeshVertex *vertex = mesh->Vertex(i);
      double length = vertex->AverageEdgeLength();
      const R3Point& p = vertex->position;
      R3Vector v = length * vertex->normal;
      glVertex3f(p[0], p[1], p[2]);
      glVertex3f(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
    }
    glEnd();
  }

  // Draw curvatures
  if (show_curvatures) {
    glDisable(GL_LIGHTING);
    glPointSize(10);
    glBegin(GL_POINTS);
    for (int i = 0; i < mesh->NVertices(); i++) {
      R3MeshVertex *vertex = mesh->Vertex(i);
      const R3Point& p = vertex->position;
      double curvature = vertex->curvature;
      double magnitude = curvature * mesh->Radius();
      if (curvature < 0) glColor3d(-magnitude, 0, 0);
      else glColor3d(0, 0, magnitude);
      glVertex3f(p[0], p[1], p[2]);
    }
    glEnd();
  }

  // Draw bounding box
  if (show_bbox) {
    const R3Box& bbox = mesh->bbox;
    glDisable(GL_LIGHTING);
    glColor3d(1, 0, 0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex3d(bbox[0][0], bbox[0][1], bbox[0][2]);
    glVertex3d(bbox[0][0], bbox[0][1], bbox[1][2]);
    glVertex3d(bbox[0][0], bbox[1][1], bbox[1][2]);
    glVertex3d(bbox[0][0], bbox[1][1], bbox[0][2]);
    glVertex3d(bbox[0][0], bbox[0][1], bbox[0][2]);
    glVertex3d(bbox[1][0], bbox[0][1], bbox[0][2]);
    glVertex3d(bbox[1][0], bbox[0][1], bbox[1][2]);
    glVertex3d(bbox[1][0], bbox[1][1], bbox[1][2]);
    glVertex3d(bbox[1][0], bbox[1][1], bbox[0][2]);
    glVertex3d(bbox[1][0], bbox[0][1], bbox[0][2]);
    glVertex3d(bbox[1][0], bbox[0][1], bbox[1][2]);
    glVertex3d(bbox[0][0], bbox[0][1], bbox[1][2]);
    glVertex3d(bbox[0][0], bbox[1][1], bbox[1][2]);
    glVertex3d(bbox[1][0], bbox[1][1], bbox[1][2]);
    glVertex3d(bbox[1][0], bbox[1][1], bbox[0][2]);
    glVertex3d(bbox[0][0], bbox[1][1], bbox[0][2]);
    glEnd();
  }

  // Draw pick position
  if (show_pick && pick_active) {
    // Draw pick position
    glEnable(GL_LIGHTING);
    static GLfloat diffuse[] = { 1, 0, 0, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse); 
    double radius = 0.01 * mesh->Radius();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(pick_position[0], pick_position[1], pick_position[2]);
    static GLUquadricObj *sphere = gluNewQuadric();
    gluQuadricNormals(sphere, (GLenum) GLU_SMOOTH);
    gluQuadricDrawStyle(sphere, (GLenum) GLU_FILL);
    gluSphere(sphere, radius, 8, 8);
    glPopMatrix();

    // Draw pick face
    if (pick_face) {
      glDisable(GL_LIGHTING);
      glColor3f(1, 1, 0);
      glEnable(GL_POLYGON_OFFSET_FILL);
      glPolygonOffset(-2, -2);
      glBegin(GL_POLYGON);
      for (unsigned int j = 0; j < pick_face->vertices.size(); j++) {
        R3MeshVertex *vertex = pick_face->vertices[j];
        const R3Point& p = vertex->position;
        glVertex3f(p[0], p[1], p[2]);
      }
      glEnd();
      glDisable(GL_POLYGON_OFFSET_FILL);
    }
  }

  // Write image
  if (save_image) {
    char image_name[256];
    static int image_number = 1;
    for (;;) {
      sprintf(image_name, "image%d.jpg", image_number++);
      FILE *fp = fopen(image_name, "r");
      if (!fp) break; 
      else fclose(fp);
    }
    GLUTSaveImage(image_name);
    printf("Saved %s\n", image_name);
    save_image = 0;
  }

  // Quit here so that can save image before exit
  if (quit) {
    if (output_image_name) GLUTSaveImage(output_image_name);
    if (output_mesh_name) mesh->Write(output_mesh_name);
    GLUTStop();
  }

  // Swap buffers 
  glutSwapBuffers();
}    



bool 
GLUTPick(int x, int y, R3Mesh *mesh, R3MeshFace **pick_face, R3Point *pick_position) 
{
  // Check position
  if ((x < 0) || (GLUTwindow_width <= x) || (y < 0) || (GLUTwindow_height <= y)) { 
    printf("Pick (%d,%d) outside viewport: (0,%d) (0,%d)\n", x, y, GLUTwindow_width, GLUTwindow_height); 
    return false;
  }

  // Allocate select buffer
  const int SELECT_BUFFER_SIZE = 1024;
  GLuint select_buffer[SELECT_BUFFER_SIZE];
  GLint select_buffer_hits;

  // Initialize select buffer
  glSelectBuffer(SELECT_BUFFER_SIZE, select_buffer);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);

  // Initialize view transformation
  GLint viewport[4];
  glViewport(0, 0, GLUTwindow_width, GLUTwindow_height);
  glGetIntegerv(GL_VIEWPORT, viewport);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix((GLdouble) x, (GLdouble) y, 1, 1, viewport);

  // Set projection transformation
  // NOTE: THIS MUST MATCH CODE IN GLUTRedraw
  double mesh_radius = mesh->Radius(); 
  gluPerspective(180.0*camera_yfov/M_PI, (GLdouble) GLUTwindow_width /(GLdouble) GLUTwindow_height, 
    0.01 * mesh_radius, 100 * mesh_radius);

  // Set camera transformation
  // NOTE: THIS MUST MATCH CODE IN GLUTRedraw
  R3Vector& t = camera_towards;
  R3Vector& u = camera_up;
  R3Vector r = camera_up % camera_towards;
  GLdouble camera_matrix[16] = { r[0], u[0], t[0], 0, r[1], u[1], t[1], 0, r[2], u[2], t[2], 0, 0, 0, 0, 1 };
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixd(camera_matrix);
  glTranslated(-camera_eye[0], -camera_eye[1], -camera_eye[2]);

  // Draw mesh with pick names into selection buffer
  glLoadName(0); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (int i = 0; i < mesh->NFaces(); i++) { 
    R3MeshFace *face = mesh->Face(i);
    glLoadName(i + 1); 
    glBegin(GL_POLYGON);
    for (unsigned int j = 0; j < face->vertices.size(); j++) {
      R3MeshVertex *vertex = face->vertices[j];
      const R3Point& p = vertex->position;
      glVertex3f(p[0], p[1], p[2]);
    }
    glEnd();
  }
  glFlush();
  select_buffer_hits = glRenderMode(GL_RENDER);

  // Process select buffer to find front-most hit
  int hit = 0;
  GLuint hit_z = 0xFFFFFFFF;
  GLuint *bufp = select_buffer;
  GLuint numnames, z1, z2;
  for (int i = 0; i < select_buffer_hits; i++) {
    numnames = *bufp++;
    z1 = *bufp++;
    z2 = *bufp++;
    while (numnames--) {
      if (z1 < hit_z) {
        hit = (int) *bufp;
        hit_z = z1/2 + z2/2;
      }
      bufp++;
    }
  }

  // Return closest face
  if ((hit > 0) && (hit <= mesh->NFaces())) {
    // Find face
    if (pick_face) {
      // Subtract one because added one in glLoadName
      *pick_face = mesh->Face(hit - 1);
    }

    // Find hit position
    if (pick_position) {
      GLdouble p[3];
      GLdouble modelview_matrix[16];
      GLdouble projection_matrix[16];
      GLint viewport[16];
      glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
      glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
      glGetIntegerv(GL_VIEWPORT, viewport);
      GLdouble z = (GLdouble) hit_z / (GLdouble) 0xFFFFFFFF;
      gluUnProject(x, y, z, modelview_matrix, projection_matrix, viewport, &(p[0]), &(p[1]), &(p[2]));
      pick_position->Reset(p[0], p[1], p[2]);
    }

    // Return hit
    return true;
  }
  else {
    // Return no hit
    return false;
  }
}



void GLUTMotion(int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;
  
  // Compute mouse movement
  int dx = x - GLUTmouse[0];
  int dy = y - GLUTmouse[1];
  
  // Process mouse motion event
  if ((dx != 0) || (dy != 0)) {
    R3Point mesh_center = mesh->Center();
    if ((GLUTbutton[0] && (GLUTmodifiers & GLUT_ACTIVE_SHIFT)) || GLUTbutton[1]) {
      // Scale world 
      double factor = (double) dx / (double) GLUTwindow_width;
      factor += (double) dy / (double) GLUTwindow_height;
      factor = exp(2.0 * factor);
      factor = (factor - 1.0) / factor;
      R3Vector translation = (mesh_center - camera_eye) * factor;
      camera_eye += translation;
      glutPostRedisplay();
    }
    else if (GLUTbutton[0] && (GLUTmodifiers & GLUT_ACTIVE_CTRL)) {
      // Translate world
      double length = R3Distance(mesh_center, camera_eye) * tan(camera_yfov);
      double vx = length * (double) dx / (double) GLUTwindow_width;
      double vy = length * (double) dy / (double) GLUTwindow_height;
      R3Vector camera_right = camera_up % camera_towards;
      R3Vector translation = -((camera_right * vx) + (camera_up * vy));
      camera_eye += translation;
      glutPostRedisplay();
    }
    else if (GLUTbutton[0]) {
      // Rotate world
      double vx = (double) dx / (double) GLUTwindow_width;
      double vy = (double) dy / (double) GLUTwindow_height;
      double theta = 4.0 * (fabs(vx) + fabs(vy));
      R3Vector camera_right = camera_up % camera_towards;
      R3Vector vector = (camera_right * vx) + (camera_up * vy);
      R3Vector rotation_axis = vector % camera_towards;
      rotation_axis.Normalize();
      camera_eye.Rotate(R3Line(mesh_center, rotation_axis), theta);
      camera_towards.Rotate(rotation_axis, theta);
      camera_up.Rotate(rotation_axis, theta);
      camera_right = camera_up % camera_towards;
      camera_up = camera_towards % camera_right;
      camera_towards.Normalize();
      camera_up.Normalize();
      glutPostRedisplay();
    }
  }

  // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;
}



void GLUTMouse(int button, int state, int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;
  
  // Process mouse button event
  if (state == GLUT_DOWN) {
    if (button == GLUT_LEFT_BUTTON) {
    }
    else if (button == GLUT_MIDDLE_BUTTON) {
    }
    else if (button == GLUT_RIGHT_BUTTON) {
    }
  }

  // Remember button state 
  int b = (button == GLUT_LEFT_BUTTON) ? 0 : ((button == GLUT_MIDDLE_BUTTON) ? 1 : 2);
  GLUTbutton[b] = (state == GLUT_DOWN) ? 1 : 0;

  // Remember modifiers 
  GLUTmodifiers = glutGetModifiers();

   // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  // Redraw
  glutPostRedisplay();
}



void GLUTSpecial(int key, int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;

  // Process keyboard button event 
  switch (key) {
  case GLUT_KEY_F1:
    save_image = 1;
    break;
  }

  // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  // Remember modifiers 
  GLUTmodifiers = glutGetModifiers();

  // Redraw
  glutPostRedisplay();
}



void GLUTKeyboard(unsigned char key, int x, int y)
{
  // Invert y coordinate
  y = GLUTwindow_height - y;

  // Process keyboard button event 
  switch (key) {
  case ' ':
    pick_active = GLUTPick(x, y, mesh, &pick_face, &pick_position);
    if (pick_active) 
      printf("Picked face %d with area %g at position (%g %g %g )\n", pick_face->id, 
        pick_face->Area(), pick_position[0], pick_position[1], pick_position[2]);  
    break;

  case 'B':
  case 'b':
    show_bbox = !show_bbox;
    break;

  case 'C':
  case 'c':
    show_curvatures = !show_curvatures;
    break;

  case 'E':
  case 'e':
    show_edges = !show_edges;
    break;

  case 'F':
  case 'f':
    show_faces = !show_faces;
    break;

  case 'I':
  case 'i':
    show_ids = !show_ids;
    break;

  case 'N':
  case 'n':
    show_normals = !show_normals;
    break;

  case 'P':
  case 'p':
    show_pick = !show_pick;
    break;

  case 'Q':
  case 'q':
    quit = 1;
    break;

  case 'V':
  case 'v':
    show_vertices = !show_vertices;
    break;

  case 27: // ESCAPE
    quit = 1;
    break;
  }

  // Remember mouse position 
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  // Remember modifiers 
  GLUTmodifiers = glutGetModifiers();

  // Redraw
  glutPostRedisplay();
}



void GLUTCommand(int cmd)
{
  // Execute command
  switch (cmd) {
  case DISPLAY_FACE_TOGGLE_COMMAND: show_faces = !show_faces; break;
  case DISPLAY_EDGE_TOGGLE_COMMAND: show_edges = !show_edges; break;
  case DISPLAY_VERTEX_TOGGLE_COMMAND: show_vertices = !show_vertices; break;
  case DISPLAY_NORMAL_TOGGLE_COMMAND: show_normals = !show_normals; break;
  case DISPLAY_CURVATURE_TOGGLE_COMMAND: show_curvatures = !show_curvatures; break;
  case DISPLAY_BBOX_TOGGLE_COMMAND: show_bbox = !show_bbox; break;
  case TWIST_COMMAND: mesh->Twist(0.5); break;
  case SAVE_IMAGE_COMMAND: if (output_image_name) GLUTSaveImage(output_image_name); break;
  case SAVE_MESH_COMMAND: if (output_mesh_name) mesh->Write(output_mesh_name); break;
  case QUIT_COMMAND: quit = 1; break;
  }

  // Mark window for redraw
  glutPostRedisplay();
}



void GLUTCreateMenu(void)
{
  // Display sub-menu
  int display_menu = glutCreateMenu(GLUTCommand);
  glutAddMenuEntry("Faces (F)", DISPLAY_FACE_TOGGLE_COMMAND);
  glutAddMenuEntry("Edges (E)", DISPLAY_EDGE_TOGGLE_COMMAND);
  glutAddMenuEntry("Vertices (V)", DISPLAY_VERTEX_TOGGLE_COMMAND);
  glutAddMenuEntry("Normals (N)", DISPLAY_NORMAL_TOGGLE_COMMAND);
  glutAddMenuEntry("Curvatures (C)", DISPLAY_CURVATURE_TOGGLE_COMMAND);
  glutAddMenuEntry("Bounding box (B)", DISPLAY_BBOX_TOGGLE_COMMAND);

  // Warp sub-menu
  int warp_menu = glutCreateMenu(GLUTCommand);
  glutAddMenuEntry("Twist", TWIST_COMMAND);

  // Save sub-menu
  int save_menu = glutCreateMenu(GLUTCommand);
  glutAddMenuEntry("Image", SAVE_IMAGE_COMMAND);
  glutAddMenuEntry("Mesh", SAVE_MESH_COMMAND);

  // Main menu
  glutCreateMenu(GLUTCommand);
  glutAddSubMenu("Display", display_menu);
  glutAddSubMenu("Warp", warp_menu);
  glutAddSubMenu("Save", save_menu);
  glutAddMenuEntry("Quit", QUIT_COMMAND);

  // Attach main menu to right mouse button
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}



void GLUTInit(int *argc, char **argv)
{
  // Open window 
  glutInit(argc, argv);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(GLUTwindow_width, GLUTwindow_height);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // | GLUT_STENCIL
  GLUTwindow = glutCreateWindow("OpenGL Viewer");

  // Initialize GLUT callback functions 
  glutReshapeFunc(GLUTResize);
  glutDisplayFunc(GLUTRedraw);
  glutKeyboardFunc(GLUTKeyboard);
  glutSpecialFunc(GLUTSpecial);
  glutMouseFunc(GLUTMouse);
  glutMotionFunc(GLUTMotion);
    
  // Initialize lights 
  static GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  static GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glEnable(GL_LIGHT0);
  static GLfloat light1_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);

  // Initialize graphics modes 
  glEnable(GL_DEPTH_TEST);

  // Create menus
  GLUTCreateMenu();
}



void GLUTMainLoop(void)
{
  // Setup initial camera parameters
  double mesh_radius = mesh->Radius();
  R3Point mesh_center = mesh->Center();
  camera_eye = mesh_center + 2.5 * mesh_radius * camera_towards;

  // Run main loop -- never returns 
  glutMainLoop();
}


////////////////////////////////////////////////////////////
// PROGRAM ARGUMENT PARSING
////////////////////////////////////////////////////////////

int 
ParseArgs(int argc, char **argv)
{
  // Innocent until proven guilty
  int print_usage = 0;

  // Parse arguments
  argc--; argv++;
  while (argc > 0) {
    if ((*argv)[0] == '-') {
      if (!strcmp(*argv, "-help")) { print_usage = 1; }
      else if (!strcmp(*argv, "-v")) { print_verbose = 1; }
      else if (!strcmp(*argv, "-exit_immediately")) { quit = 1; }
      else if (!strcmp(*argv, "-output_image")) { argc--; argv++; output_image_name = *argv; }
      else if (!strcmp(*argv, "-output_mesh")) { argc--; argv++; output_mesh_name = *argv; }
      else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
      argv++; argc--;
    }
    else {
      if (!input_mesh_name) input_mesh_name = *argv;
      else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
      argv++; argc--;
    }
  }

  // Check input_mesh_name
  if (!input_mesh_name || print_usage) {
    printf("Usage: meshview <input.off> [-output_image <output.jpg>] [-output_mesh <output.off>] [-exit_immediately] [-v]\n");
    return 0;
  }

  // Return OK status 
  return 1;
}



////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////

int 
main(int argc, char **argv)
{
  // Initialize GLUT
  GLUTInit(&argc, argv);

  // Parse program arguments
  if (!ParseArgs(argc, argv)) exit(1);

  // Allocate mesh
  mesh = new R3Mesh();
  if (!mesh) {
    fprintf(stderr, "Unable to allocate mesh\n");
    exit(-1);
  }

  // Read mesh
  if (!mesh->Read(input_mesh_name)) {
    fprintf(stderr, "Unable to read mesh from %s\n", input_mesh_name);
    exit(-1);
  }

  // Run GLUT interface
  GLUTMainLoop();

  // Return success 
  return 0;
}









