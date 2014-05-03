// Source file for mesh class



// Include files

#include "R3Mesh.h"
#include "lplus.h"

void R3Mesh::
Twist(double angle)
{
  // Twist mesh by an angle, or other simple mesh warping of your choice.
  // See Scale() for how to get the vertex positions, and see bbox for the bounding box.

  // FILL IN IMPLEMENTATION HERE

  // Update mesh data structures
  Update();
}
R3Shape R3Mesh::Leaf(const R3Vector direction)
{
  float z;
  z=direction.Dot(R3Vector(0,1,0))/4.0; //bend towards earth
  
  if (z==0) z=(rand()%20 -10 ) /100.0; //some random bend if non

  vector<R3MeshVertex *> face;
  face.push_back(CreateVertex(R3Point(0,.01,0)));
  face.push_back(CreateVertex(R3Point(.2,.1,0)));
  face.push_back(CreateVertex(R3Point(.25,.3,0)));
  face.push_back(CreateVertex(R3Point(.2,.6,z/2)));
  

  face.push_back(CreateVertex(R3Point(0,1-z,z)));
  face.push_back(CreateVertex(R3Point(-.2,.6,z/2)));
  face.push_back(CreateVertex(R3Point(-.25,.3,0)));
  face.push_back(CreateVertex(R3Point(-.2,.1,0)));
  CreateFace(face);
  return face;

}
R3Shape R3Mesh::Circle(float radius,int slices)
{
  vector<R3MeshVertex *> face_vertices;
  for(int i=0; i<slices; i++) 
  {
    R3MeshVertex*t;
    float theta = ((float)i)* (2.0*M_PI/slices);
    t=CreateVertex(R3Point(radius*cos(theta), 0, radius*sin(theta))); //vertices at edges of circle
    face_vertices.push_back(t);
  }
  CreateFace(face_vertices);
  return face_vertices;

}
R3Shape R3Mesh::Cylinder(float topBottomRatio,int slices)
{
  static bool cached=false;
  cached=false;
  static vector<R3Point> cache;
  int cacheIndex=0;

  float length=1,radius=1;
  float topRadius=topBottomRatio;
  R3Shape vertices;
  R3Shape bottom_circle;
  R3Shape top_circle;
  R3Shape side;
  for(int i=0; i<slices; i++) 
  {
    R3MeshVertex*t1,*t2;
    R3Point p;
    float theta = ((float)i)* (2.0*M_PI/slices);

    if (!cached) cache.push_back( p=R3Point(topRadius*cos(theta), length, topRadius*sin(theta)) );
    else p=cache[cacheIndex++];
    t1=CreateVertex(p) ; //vertices at edges of circle
    top_circle.push_back(t1);
    vertices.push_back(t1);

    if (!cached) cache.push_back( p=R3Point(radius*cos(theta), 0, radius*sin(theta)) );
    else p=cache[cacheIndex++];
    t2=CreateVertex(p); //vertices at edges of circle
    bottom_circle.push_back(t2);
    vertices.push_back(t2);
  }
  int size=vertices.size();
  for (int i=0;i<size;i+=2)
  {

    side.push_back(vertices[i]);
    side.push_back(vertices[i+1]);
    side.push_back(vertices[(i+2)%size]);
    CreateFace(side);
    side.clear();

    side.push_back(vertices[i+1]);
    side.push_back(vertices[(i+3)%size]);
    side.push_back(vertices[(i+2)%size]);
    CreateFace(side);
    side.clear();
  }
  CreateFace(top_circle);
  CreateFace(bottom_circle);
  if (!cached)
    cached=true;
  return vertices;
}
void R3Mesh::AddCoords()
{
  float width=.05;
  R3Shape shape;
  shape.push_back(CreateVertex(R3Point(3,0,0)));
  shape.push_back(CreateVertex(R3Point(0,width,0)));
  shape.push_back(CreateVertex(R3Point(0,-width,0)));
  CreateFace(shape);
  shape.clear();
  shape.push_back(CreateVertex(R3Point(0,2,0)));
  shape.push_back(CreateVertex(R3Point(-width,0,0)));
  shape.push_back(CreateVertex(R3Point(width,0,0)));
  CreateFace(shape);
  shape.clear();
  shape.push_back(CreateVertex(R3Point(0,0,1)));
  shape.push_back(CreateVertex(R3Point(0,-width,0)));
  shape.push_back(CreateVertex(R3Point(0,width,0)));
  CreateFace(shape);


}
void R3Mesh::
Tree(const char * descriptor_filename,const int iterations)
{
  /** turtle system test *
    TurtleSystem t(this);
  t.pitchDown(90);
  t.turnRight(90);
  t.move(10);
  printf("%f %f %f\n",t.position.X(),t.position.Y(),t.position.Z());
  printf("%f %f %f\n",t.direction.X(),t.direction.Y(),t.direction.Z());
  printf("%f %f %f\n",t.right.X(),t.right.Y(),t.right.Z());
  * end turtle system test **/

  // AddCoords();
  // R3Shape cylinder=Cylinder();
  // TranslateShape(cylinder,-50,0,0);
  // Update();
  // return;

  // Leaf(R3Vector(0,1,0));
  // Update();
  // return;

  LPlusSystem l(this);
  string lsystem=l.generateFromFile(descriptor_filename,iterations);
  l.draw(lsystem); 
  Update();

}
////////////////////////////////////////////////////////////
// MESH CONSTRUCTORS/DESTRUCTORS
////////////////////////////////////////////////////////////

R3Mesh::
R3Mesh(void)
: bbox(R3null_box)
{
}



R3Mesh::
R3Mesh(const R3Mesh& mesh)
: bbox(R3null_box)
{
  // Create vertices
  for (int i = 0; i < mesh.NVertices(); i++) {
    R3MeshVertex *v = mesh.Vertex(i);
    CreateVertex(v->position, v->normal, v->texcoords);
  }

  // Create faces
  for (int i = 0; i < mesh.NFaces(); i++) {
    R3MeshFace *f = mesh.Face(i);
    vector<R3MeshVertex *> face_vertices;
    for (unsigned int j = 0; j < f->vertices.size(); j++) {
      R3MeshVertex *ov = f->vertices[j];
      R3MeshVertex *nv = Vertex(ov->id);
      face_vertices.push_back(nv);
    }
    CreateFace(face_vertices);
  }
}



R3Mesh::
~R3Mesh(void)
{
  // Delete faces
  for (int i = 0; i < NFaces(); i++) {
    R3MeshFace *f = Face(i);
    delete f;
  }

  // Delete vertices
  for (int i = 0; i < NVertices(); i++) {
    R3MeshVertex *v = Vertex(i);
    delete v;
  }
}



////////////////////////////////////////////////////////////
// MESH PROPERTY FUNCTIONS
////////////////////////////////////////////////////////////

R3Point R3Mesh::
Center(void) const
{
  // Return center of bounding box
  return bbox.Centroid();
}



double R3Mesh::
Radius(void) const
{
  // Return radius of bounding box
  return bbox.DiagonalRadius();
}



////////////////////////////////////////////////////////////
// MESH PROCESSING FUNCTIONS
////////////////////////////////////////////////////////////

void R3Mesh::TranslateShape(vector<R3MeshVertex *> shape,double dx,double dy,double dz)
{
  R3Vector translation(dx, dy, dz);

  // Update vertices
  for (unsigned int i = 0; i < shape.size(); i++) {
    R3MeshVertex *vertex = shape[i];
    vertex->position.Translate(translation);
  }

  // Update mesh data structures
}
void R3Mesh::
Translate(double dx, double dy, double dz)
{
  TranslateShape(vertices,dx,dy,dz);
  Update();
}




void R3Mesh::
ScaleShape(vector<R3MeshVertex *> shape,double sx, double sy, double sz)
{
  // Scale the mesh by increasing the distance 
  // from every vertex to the origin by a factor 
  // given for each dimension (sx, sy, sz)

  // This is implemented for you as an example 

  // Update vertices
  for (unsigned int i = 0; i < shape.size(); i++) {
    R3MeshVertex *vertex = shape[i];
    vertex->position[0] *= sx;
    vertex->position[1] *= sy;
    vertex->position[2] *= sz;
  }

  // Update mesh data structures
}
void R3Mesh::Scale(double sx,double sy,double sz)
{
  ScaleShape(vertices,sx,sy,sz);
  Update();
}

void R3Mesh::
RotateShape(vector<R3MeshVertex *> shape,double angle, const R3Vector& axis)
{
  for (unsigned int i = 0; i < shape.size(); i++) {
    R3MeshVertex *vertex = shape[i];
    vertex->position.Rotate(axis, angle);
  }

  // Update mesh data structures

}

void R3Mesh::
RotateShape(vector<R3MeshVertex *> shape,double angle, const R3Line& axis)
{
  // Rotate the mesh counter-clockwise by an angle 
  // (in radians) around a line axis

  // This is implemented for you as an example 

  // Update vertices
  for (unsigned int i = 0; i < shape.size(); i++) {
    R3MeshVertex *vertex = shape[i];
    vertex->position.Rotate(axis, angle);
  }

  // Update mesh data structures
}
void R3Mesh::Rotate(double angle, const R3Line& axis)
{
  RotateShape(vertices,angle,axis);
  Update();
}


////////////////////////////////////////////////////////////
// MESH ELEMENT CREATION/DELETION FUNCTIONS
////////////////////////////////////////////////////////////

R3MeshVertex *R3Mesh::
CreateVertex(const R3Point& position, const R3Vector& normal, const R2Point& texcoords)
{
  // Create vertex
  R3MeshVertex *vertex = new R3MeshVertex(position, normal, texcoords);

  // Update bounding box
  bbox.Union(position);

  // Set vertex ID
  vertex->id = vertices.size();

  // Add to list
  vertices.push_back(vertex);

  // Return vertex
  return vertex;
}



R3MeshFace *R3Mesh::
CreateFace(const vector<R3MeshVertex *>& vertices)
{
  // Create face
  R3MeshFace *face = new R3MeshFace(vertices);

  // Set face  ID
  face->id = faces.size();

  // Add to list
  faces.push_back(face);

  // Return face
  return face;
}



void R3Mesh::
DeleteVertex(R3MeshVertex *vertex)
{
  // Remove vertex from list
  for (unsigned int i = 0; i < vertices.size(); i++) {
    if (vertices[i] == vertex) {
      vertices[i] = vertices.back();
      vertices[i]->id = i;
      vertices.pop_back();
      break;
    }
  }

  // Delete vertex
  delete vertex;
}



void R3Mesh::
DeleteFace(R3MeshFace *face)
{
  // Remove face from list
  for (unsigned int i = 0; i < faces.size(); i++) {
    if (faces[i] == face) {
      faces[i] = faces.back();
      faces[i]->id = i;
      faces.pop_back();
      break;
    }
  }

  // Delete face
  delete face;
}



////////////////////////////////////////////////////////////
// UPDATE FUNCTIONS
////////////////////////////////////////////////////////////

void R3Mesh::
Update(void)
{
  // Update everything
  UpdateBBox();
  UpdateFacePlanes();
  UpdateVertexNormals();
  UpdateVertexCurvatures();
}



void R3Mesh::
UpdateBBox(void)
{
  // Update bounding box
  bbox = R3null_box;
  for (unsigned int i = 0; i < vertices.size(); i++) {
    R3MeshVertex *vertex = vertices[i];
    bbox.Union(vertex->position);
  }
}



void R3Mesh::
UpdateVertexNormals(void)
{
  // Update normal for every vertex
  for (unsigned int i = 0; i < vertices.size(); i++) {
    vertices[i]->UpdateNormal();
  }
}




void R3Mesh::
UpdateVertexCurvatures(void)
{
  // Update curvature for every vertex
  for (unsigned int i = 0; i < vertices.size(); i++) {
    vertices[i]->UpdateCurvature();
  }
}




void R3Mesh::
UpdateFacePlanes(void)
{
  // Update plane for all faces
  for (unsigned int i = 0; i < faces.size(); i++) {
    faces[i]->UpdatePlane();
  }
}



////////////////////////////////////////////////////////////////////////
// I/O FUNCTIONS
////////////////////////////////////////////////////////////////////////

int R3Mesh::
Read(const char *filename)
{
  // Parse input filename extension
  const char *extension;
  if (!(extension = strrchr(filename, '.'))) {
    printf("Filename %s has no extension (e.g., .ply)\n", filename);
    return 0;
  }

  // Read file of appropriate type
  int status = 0;
  if (!strncmp(extension, ".ray", 4)) 
    status = ReadRay(filename);
  else if (!strncmp(extension, ".off", 4)) 
    status = ReadOff(filename);
  else if (!strncmp(extension, ".jpg", 4)) 
    status = ReadImage(filename);
  else if (!strncmp(extension, ".jpeg", 4)) 
    status = ReadImage(filename);
  else if (!strncmp(extension, ".bmp", 4)) 
    status = ReadImage(filename);
  else if (!strncmp(extension, ".ppm", 4)) 
    status = ReadImage(filename);
  else {
    fprintf(stderr, "Unable to read file %s (unrecognized extension: %s)\n", filename, extension);
    return status;
  }

  // Update mesh data structures
  Update();

  // Return success
  return 1;
}



int R3Mesh::
Write(const char *filename)
{
  // Parse input filename extension
  const char *extension;
  if (!(extension = strrchr(filename, '.'))) {
    printf("Filename %s has no extension (e.g., .ply)", filename);
    return 0;
  }

  // Write file of appropriate type
  if (!strncmp(extension, ".ray", 4)) 
    return WriteRay(filename);
  else if (!strncmp(extension, ".off", 4)) 
    return WriteOff(filename);
  else {
    fprintf(stderr, "Unable to write file %s (unrecognized extension: %s)", filename, extension);
    return 0;
  }
}



////////////////////////////////////////////////////////////
// IMAGE FILE INPUT/OUTPUT
////////////////////////////////////////////////////////////

int R3Mesh::
ReadImage(const char *filename)
{
  // Create a mesh by reading an image file, 
  // constructing vertices at (x,y,luminance), 
  // and connecting adjacent pixels into faces. 
  // That is, the image is interpretted as a height field, 
  // where the luminance of each pixel provides its z-coordinate.

  // Read image
  R2Image *image = new R2Image();
  if (!image->Read(filename)) return 0;

  // Create vertices and store in arrays
  R3MeshVertex ***vertices = new R3MeshVertex **[image->Width() ];
  for (int i = 0; i < image->Width(); i++) {
    vertices[i] = new R3MeshVertex *[image->Height() ];
    for (int j = 0; j < image->Height(); j++) {
      double luminance = image->Pixel(i, j).Luminance();
      double z = luminance * image->Width();
      R3Point position((double) i, (double) j, z);
      R2Point texcoords((double) i, (double) j);
      vertices[i][j] = CreateVertex(position, R3zero_vector, texcoords);
    }
  }

  // Create faces
  vector<R3MeshVertex *> face_vertices;
  for (int i = 1; i < image->Width(); i++) {
    for (int j = 1; j < image->Height(); j++) {
      face_vertices.clear();
      face_vertices.push_back(vertices[i-1][j-1]);
      face_vertices.push_back(vertices[i][j-1]);
      face_vertices.push_back(vertices[i][j]);
      face_vertices.push_back(vertices[i-1][j]);
      CreateFace(face_vertices);
    }
  }

  // Delete vertex arrays
  for (int i = 0; i < image->Width(); i++) delete [] vertices[i];
    delete [] vertices;

  // Delete image
  delete image;

  // Return success
  return 1;
}



////////////////////////////////////////////////////////////
// OFF FILE INPUT/OUTPUT
////////////////////////////////////////////////////////////

int R3Mesh::
ReadOff(const char *filename)
{
  // Open file
  FILE *fp;
  if (!(fp = fopen(filename, "r"))) {
    fprintf(stderr, "Unable to open file %s\n", filename);
    return 0;
  }

  // Read file
  int nverts = 0;
  int nfaces = 0;
  int nedges = 0;
  int line_count = 0;
  int vertex_count = 0;
  int face_count = 0;
  char buffer[1024];
  char header[64];
  while (fgets(buffer, 1023, fp)) {
    // Increment line counter
    line_count++;

    // Skip white space
    char *bufferp = buffer;
    while (isspace(*bufferp)) bufferp++;

    // Skip blank lines and comments
    if (*bufferp == '#') continue;
    if (*bufferp == '\0') continue;

    // Check section
    if (nverts == 0) {
      // Read header keyword
      if (strstr(bufferp, "OFF")) {
        // Check if counts are on first line
        int tmp;
        if (sscanf(bufferp, "%s%d%d%d", header, &tmp, &nfaces, &nedges) == 4) {
          nverts = tmp;
        }
      }
      else {
        // Read counts from second line
        if ((sscanf(bufferp, "%d%d%d", &nverts, &nfaces, &nedges) != 3) || (nverts == 0)) {
          fprintf(stderr, "Syntax error reading header on line %d in file %s\n", line_count, filename);
          fclose(fp);
          return 0;
        }
      }
    }
    else if (vertex_count < nverts) {
      // Read vertex coordinates
      double x, y, z;
      if (sscanf(bufferp, "%lf%lf%lf", &x, &y, &z) != 3) {
        fprintf(stderr, "Syntax error with vertex coordinates on line %d in file %s\n", line_count, filename);
        fclose(fp);
        return 0;
      }

      // Create vertex
      CreateVertex(R3Point(x, y, z), R3zero_vector, R2zero_point);

      // Increment counter
      vertex_count++;
    }
    else if (face_count < nfaces) {
      // Read number of vertices in face 
      int face_nverts = 0;
      bufferp = strtok(bufferp, " \t");
      if (bufferp) face_nverts = atoi(bufferp);
      else {
        fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
        fclose(fp);
        return 0;
      }

      // Read vertex indices for face
      vector<R3MeshVertex *> face_vertices;
      for (int i = 0; i < face_nverts; i++) {
        R3MeshVertex *v = NULL;
        bufferp = strtok(NULL, " \t");
        if (bufferp) v = Vertex(atoi(bufferp));
        else {
          fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
          fclose(fp);
          return 0;
        }

        // Add vertex to vector
        face_vertices.push_back(v);
      }

      // Create face
      CreateFace(face_vertices);

      // Increment counter
      face_count++;
    }
    else {
      // Should never get here
      fprintf(stderr, "Found extra text starting at line %d in file %s\n", line_count, filename);
      break;
    }
  }

  // Check whether read all vertices
  if ((vertex_count != nverts) || (NVertices() < nverts)) {
    fprintf(stderr, "Expected %d vertices, but read %d vertex lines and created %d vertices in file %s\n", 
      nverts, vertex_count, NVertices(), filename);
  }

  // Check whether read all faces
  if ((face_count != nfaces) || (NFaces() < nfaces)) {
    fprintf(stderr, "Expected %d faces, but read %d face lines and created %d faces in file %s\n", 
      nfaces, face_count, NFaces(), filename);
  }

  // Close file
  fclose(fp);

  // Return number of faces read
  return NFaces();
}



int R3Mesh::
WriteOff(const char *filename)
{
  // Open file
  FILE *fp = fopen(filename, "w");
  if (!fp) {
    fprintf(stderr, "Unable to open file %s\n", filename);
    return 0;
  }

  // Write header
  fprintf(fp, "OFF\n");
  fprintf(fp, "%d %d %d\n", NVertices(), NFaces(), 0);

  // Write vertices
  for (int i = 0; i < NVertices(); i++) {
    R3MeshVertex *vertex = Vertex(i);
    const R3Point& p = vertex->position;
    fprintf(fp, "%g %g %g\n", p.X(), p.Y(), p.Z());
    vertex->id = i;
  }

  // Write Faces
  for (int i = 0; i < NFaces(); i++) {
    R3MeshFace *face = Face(i);
    fprintf(fp, "%d", (int) face->vertices.size());
    for (unsigned int j = 0; j < face->vertices.size(); j++) {
      fprintf(fp, " %d", face->vertices[j]->id);
    }
    fprintf(fp, "\n");
  }

  // Close file
  fclose(fp);

  // Return number of faces
  return NFaces();
}



////////////////////////////////////////////////////////////
// RAY FILE INPUT/OUTPUT
////////////////////////////////////////////////////////////

int R3Mesh::
ReadRay(const char *filename)
{
  // Open file
  FILE *fp;
  if (!(fp = fopen(filename, "r"))) {
    fprintf(stderr, "Unable to open file %s", filename);
    return 0;
  }

  // Read body
  char cmd[128];
  int polygon_count = 0;
  int command_number = 1;
  while (fscanf(fp, "%s", cmd) == 1) {
    if (!strcmp(cmd, "#vertex")) {
      // Read data
      double px, py, pz;
      double nx, ny, nz;
      double ts, tt;
      if (fscanf(fp, "%lf%lf%lf%lf%lf%lf%lf%lf", &px, &py, &pz, &nx, &ny, &nz, &ts, &tt) != 8) {
        fprintf(stderr, "Unable to read vertex at command %d in file %s", command_number, filename);
        return 0;
      }

      // Create vertex
      R3Point point(px, py, pz);
      R3Vector normal(nx, ny, nz);
      R2Point texcoords(ts, tt);
      CreateVertex(point, normal, texcoords);
    }
    else if (!strcmp(cmd, "#shape_polygon")) {
      // Read data
      int m, nverts;
      if (fscanf(fp, "%d%d", &m, &nverts) != 2) {
        fprintf(stderr, "Unable to read polygon at command %d in file %s", command_number, filename);
        return 0;
      }

      // Get vertices
      vector<R3MeshVertex *> face_vertices;
      for (int i = 0; i < nverts; i++) {
        // Read vertex id
        int vertex_id;
        if (fscanf(fp, "%d", &vertex_id) != 1) {
          fprintf(stderr, "Unable to read polygon at command %d in file %s", command_number, filename);
          return 0;
        }

        // Get vertex
        R3MeshVertex *v = Vertex(vertex_id);
        face_vertices.push_back(v);
      }

      // Create face
      CreateFace(face_vertices);

      // Increment polygon counter
      polygon_count++;
    }

    // Increment command number
    command_number++;
  }

  // Close file
  fclose(fp);

  // Return number of faces created
  return polygon_count;
}



int R3Mesh::
WriteRay(const char *filename)
{
  // Open file
  FILE *fp;
  if (!(fp = fopen(filename, "w"))) {
    fprintf(stderr, "Unable to open file %s", filename);
    return 0;
  }

  // Write vertices
  for (int i = 0; i < NVertices(); i++) {
    R3MeshVertex *vertex = Vertex(i);
    const R3Point& p = vertex->position;
    const R3Vector& n = vertex->normal;
    const R2Point& t = vertex->texcoords;
    fprintf(fp, "#vertex %g %g %g  %g %g %g  %g %g\n", p.X(), p.Y(), p.Z(), 
      n.X(), n.Y(), n.Z(), t.X(), t.Y());
    vertex->id = i;
  }

  // Write faces
  for (int i = 0; i < NFaces(); i++) {
    R3MeshFace *face = Face(i);
    int nvertices = face->vertices.size();
    fprintf(fp, "#shape_polygon 0 %d ", nvertices);
    for (int j = 0; j < nvertices; j++) {
      R3MeshVertex *v = face->vertices[j];
      fprintf(fp, "%d ", v->id);
    }
    fprintf(fp, "\n");
  }

  // Close file
  fclose(fp);

  // Return number of faces written
  return NFaces();
}



////////////////////////////////////////////////////////////
// MESH VERTEX MEMBER FUNCTIONS
////////////////////////////////////////////////////////////

R3MeshVertex::
R3MeshVertex(void)
: position(0, 0, 0),
normal(0, 0, 0),
texcoords(0, 0),
curvature(0),
id(0)
{
}



R3MeshVertex::
R3MeshVertex(const R3MeshVertex& vertex)
: position(vertex.position),
normal(vertex.normal),
texcoords(vertex.texcoords),
curvature(vertex.curvature),
id(0)
{
}




R3MeshVertex::
R3MeshVertex(const R3Point& position, const R3Vector& normal, const R2Point& texcoords)
: position(position),                    
normal(normal),
texcoords(texcoords),
curvature(0),
id(0)
{
}




double R3MeshVertex::
AverageEdgeLength(void) const
{
  // Return the average length of edges attached to this vertex
  // This feature should be implemented first.  To do it, you must
  // design a data structure that allows O(K) access to edges attached
  // to each vertex, where K is the number of edges attached to the vertex.

  // FILL IN IMPLEMENTATION HERE  (THIS IS REQUIRED)
  // BY REPLACING THIS ARBITRARY RETURN VALUE
  fprintf(stderr, "Average vertex edge length not implemented\n");
  return 0.12345;
}




void R3MeshVertex::
UpdateNormal(void)
{
  // Compute the surface normal at a vertex.  This feature should be implemented
  // second.  To do it, you must design a data structure that allows O(K)
  // access to faces attached to each vertex, where K is the number of faces attached
  // to the vertex.  Then, to compute the normal for a vertex,
  // you should take a weighted average of the normals for the attached faces, 
  // where the weights are determined by the areas of the faces.
  // Store the resulting normal in the "normal"  variable associated with the vertex. 
  // You can display the computed normals by hitting the 'N' key in meshview.

  // FILL IN IMPLEMENTATION HERE (THIS IS REQUIRED)
  // fprintf(stderr, "Update vertex normal not implemented\n");
}




void R3MeshVertex::
UpdateCurvature(void)
{
  // Compute an estimate of the Gauss curvature of the surface 
  // using a method based on the Gauss Bonet Theorem, which is described in 
  // [Akleman, 2006]. Store the result in the "curvature"  variable. 

  // FILL IN IMPLEMENTATION HERE
  // fprintf(stderr, "Update vertex curvature not implemented\n");
}





////////////////////////////////////////////////////////////
// MESH FACE MEMBER FUNCTIONS
////////////////////////////////////////////////////////////

R3MeshFace::
R3MeshFace(void)
: vertices(),
plane(0, 0, 0, 0),
id(0)
{
}



R3MeshFace::
R3MeshFace(const R3MeshFace& face)
: vertices(face.vertices),
plane(face.plane),
id(0)
{
}



R3MeshFace::
R3MeshFace(const vector<R3MeshVertex *>& vertices)
: vertices(vertices),
plane(0, 0, 0, 0),
id(0)
{
  UpdatePlane();
}



double R3MeshFace::
AverageEdgeLength(void) const
{
  // Check number of vertices
  if (vertices.size() < 2) return 0;

  // Compute average edge length
  double sum = 0;
  R3Point *p1 = &(vertices.back()->position);
  for (unsigned int i = 0; i < vertices.size(); i++) {
    R3Point *p2 = &(vertices[i]->position);
    double edge_length = R3Distance(*p1, *p2);
    sum += edge_length;
    p1 = p2;
  }

  // Return the average length of edges attached to this face
  return sum / vertices.size();
}



double R3MeshFace::
Area(void) const
{
  // Check number of vertices
  if (vertices.size() < 3) return 0;

  // Compute area using Newell's method (assumes convex polygon)
  R3Vector sum = R3null_vector;
  const R3Point *p1 = &(vertices.back()->position);
  for (unsigned int i = 0; i < vertices.size(); i++) {
    const R3Point *p2 = &(vertices[i]->position);
    sum += p2->Vector() % p1->Vector();
    p1 = p2;
  }

  // Return area
  return 0.5 * sum.Length();
}



void R3MeshFace::
UpdatePlane(void)
{
  // Check number of vertices
  int nvertices = vertices.size();
  if (nvertices < 3) { 
    plane = R3null_plane; 
    return; 
  }

  // Compute centroid
  R3Point centroid = R3zero_point;
  for (int i = 0; i < nvertices; i++) 
    centroid += vertices[i]->position;
  centroid /= nvertices;
  
  // Compute best normal for counter-clockwise array of vertices using newell's method
  R3Vector normal = R3zero_vector;
  const R3Point *p1 = &(vertices[nvertices-1]->position);
  for (int i = 0; i < nvertices; i++) {
    const R3Point *p2 = &(vertices[i]->position);
    normal[0] += (p1->Y() - p2->Y()) * (p1->Z() + p2->Z());
    normal[1] += (p1->Z() - p2->Z()) * (p1->X() + p2->X());
    normal[2] += (p1->X() - p2->X()) * (p1->Y() + p2->Y());
    p1 = p2;
  }
  
  // Normalize normal vector
  normal.Normalize();
  
  // Update face plane
  plane.Reset(centroid, normal);
}



