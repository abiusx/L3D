// Modified mesh processing starter code, originally by Adam Finkelstein.
// CS 6501 -- 2D/3D Shape Manipulation, 3D Printing



// Include files
#ifdef _WIN32
#include <windows.h>
#endif

#include "R2/R2.h"
#include "R3/R3.h"
#include "R3Mesh.h"



// Program arguments

static void 
ShowUsage(void)
{
  // Print usage message and exit
  fprintf(stderr, "Usage: meshpro treedescription.l output_mesh\n");
  exit(EXIT_FAILURE);
}



static void 
CheckOption(char *option, int argc, int minargc)
{
  // Check if there are enough remaining arguments for option
  if (argc < minargc)  {
    fprintf(stderr, "Too few arguments for %s\n", option);
    ShowUsage();
    exit(-1);
  }
}



int 
main(int argc, char **argv)
{
  // Look for help
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-help")) {
      ShowUsage();
    }
  }

  // Read input and output mesh filenames
  if (argc < 3)  ShowUsage();
  argv++, argc--; // First argument is program name
  char *tree_file_name=*argv;argv++;argc--;
  int iterations=0;
  if (argc>1)
    iterations=atoi(*argv++),argc--;
  char *output_mesh_name = *argv; argv++, argc--; 

  // Allocate mesh
  R3Mesh *mesh = new R3Mesh();
  if (!mesh) {
    fprintf(stderr, "Unable to allocate mesh\n");
    exit(-1);
  }

  // Read input mesh
  // if (!mesh->Read(input_mesh_name)) {
    // fprintf(stderr, "Unable to read mesh from %s\n", input_mesh_name);
    // exit(-1);
  // }

  mesh->Tree(tree_file_name,iterations);
  // Write output mesh
  if (!mesh->Write(output_mesh_name)) {
    fprintf(stderr, "Unable to write mesh to %s\n", output_mesh_name);
    exit(-1);
  }

  // Delete mesh
  delete mesh;
  printf("All done.\n");
  // Return success
  return EXIT_SUCCESS;
}



