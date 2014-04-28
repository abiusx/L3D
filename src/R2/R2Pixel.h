// Include file for the pixel class 
#ifndef R2_PIXEL_INCLUDED
#define R2_PIXEL_INCLUDED


// Class definition 

class R2Pixel {
 public:
  // Constructor functions
  R2Pixel(void);
  R2Pixel(const R2Pixel& pixel);
  R2Pixel(double r, double g, double b, double a);
  R2Pixel(const double rgba[4]);

  // Component access functions/operators
  double Red(void) const;
  double Green(void) const;
  double Blue(void) const;
  double Alpha(void) const;
  double operator[](int i) const;
  double& operator[](int i);
  double Component(int i) const;
  double *Components(void);

  // Property functions/operators
  double Luminance(void) const;
  bool IsBlack(void) const;
  bool IsWhite(void) const;
  bool operator==(const R2Pixel& pixel) const;
  bool operator!=(const R2Pixel& pixel) const;

  // Manipulation functions/operations
  void SetRed(double red);
  void SetGreen(double green);
  void SetBlue(double blue);
  void SetAlpha(double alpha);
  void Reset(double red, double green, double blue, double alpha);
  void Clamp(double maximum_value = 1.0);

  // Assignment operators
  R2Pixel& operator=(const R2Pixel& pixel);
  R2Pixel& operator+=(const R2Pixel& pixel);
  R2Pixel& operator-=(const R2Pixel& pixel);
  R2Pixel& operator*=(const R2Pixel& pixel);
  R2Pixel& operator*=(double scale);
  R2Pixel& operator/=(double scale);
 
  // Arithmetic operations
  friend R2Pixel operator+(const R2Pixel& pixel1, const R2Pixel& pixel2);
  friend R2Pixel operator-(const R2Pixel& pixel1, const R2Pixel& pixel2);
  friend R2Pixel operator*(const R2Pixel& pixel1, const R2Pixel& pixel2);
  friend R2Pixel operator*(const R2Pixel& pixel, double scale);
  friend R2Pixel operator*(double scale, const R2Pixel& pixel);
  friend R2Pixel operator/(const R2Pixel& pixel, double scale);

 private:
  double c[4];
};



// Public variables 

extern R2Pixel R2null_pixel;
extern R2Pixel R2black_pixel;
extern R2Pixel R2red_pixel;
extern R2Pixel R2green_pixel;
extern R2Pixel R2blue_pixel;
extern R2Pixel R2yellow_pixel;
extern R2Pixel R2cyan_pixel;
extern R2Pixel R2magenta_pixel;
extern R2Pixel R2white_pixel;



// Inline functions 

inline double R2Pixel::
Red(void) const
{
  // Return red component of pixel
  return(c[0]);
}



inline double R2Pixel::
Green(void) const
{
  // Return green component of pixel
  return(c[1]);
}



inline double R2Pixel::
Blue(void) const
{
  // Return blue component of pixel
  return(c[2]);
}



inline double R2Pixel::
Alpha(void) const
{
  // Return alpha component of pixel
  return(c[3]);
}



inline double *R2Pixel::
Components(void)
{
  // Return pixel array
  return c;
}



inline double R2Pixel::
Component(int i) const
{
  assert((i>=0)&&(i<=3));
  return(c[i]);
}



inline double R2Pixel::
operator[](int i) const
{
  return Component(i);
}



inline double& R2Pixel::
operator[] (int i) 
{
  assert((i>=0)&&(i<=3));
  return(c[i]);
}



inline bool R2Pixel::
IsBlack (void) const
{
  // Return whether color is black
  return ((c[0] == 0.0) && (c[1] == 0.0) && (c[2] == 0.0));
}



inline bool R2Pixel::
IsWhite (void) const
{
  // Return whether color is white
  return ((c[0] == 1.0) && (c[1] == 1.0) && (c[2] == 1.0));
}



inline double R2Pixel::
Luminance(void) const
{
  // Return luminance
  return 0.30 * c[0] + 0.59 * c[1] + 0.11 * c[2];
}



inline void R2Pixel::
SetRed(double red)
{
  // Set red component
  c[0] = red;
}



inline void R2Pixel::
SetGreen(double green)
{
  // Set green component
  c[1] = green;
}



inline void R2Pixel::
SetBlue(double blue)
{
  // Set blue component
  c[2] = blue;
}



inline void R2Pixel::
SetAlpha(double alpha)
{
  // Set alpha component
  c[3] = alpha;
}



inline void R2Pixel::
Reset (double red, double green, double blue, double alpha)
{
  // Set all components
  c[0] = red;
  c[1] = green;
  c[2] = blue;
  c[3] = alpha;
}



inline void R2Pixel::
Clamp(double maximum_value)
{
  // Set all components
  if (c[0] > maximum_value) c[0] = maximum_value;
  if (c[1] > maximum_value) c[1] = maximum_value;
  if (c[2] > maximum_value) c[2] = maximum_value;
  if (c[3] > maximum_value) c[3] = maximum_value;

  for( int i=0; i<4; i++ )
	  if ( c[i] < 0.0 )
		  c[i] = 0.0;
}



inline R2Pixel 
operator+(const R2Pixel& pixel1, const R2Pixel& pixel2)
{
  // Add rgb components of two pixels
  double r = pixel1.c[0] + pixel2.c[0];
  double g = pixel1.c[1] + pixel2.c[1];
  double b = pixel1.c[2] + pixel2.c[2];
  double a = pixel1.c[3];
  return R2Pixel(r, g, b, a);
}



inline R2Pixel 
operator-(const R2Pixel& pixel1, const R2Pixel& pixel2)
{
  // Subtract rgb components of two pixels
  double r = pixel1.c[0] - pixel2.c[0];
  double g = pixel1.c[1] - pixel2.c[1];
  double b = pixel1.c[2] - pixel2.c[2];
  double a = pixel1.c[3];
  return R2Pixel(r, g, b, a);
}



inline R2Pixel 
operator*(const R2Pixel& pixel1, const R2Pixel& pixel2)
{
  // Multiply rgb components of two pixels
  double r = pixel1.c[0] * pixel2.c[0];
  double g = pixel1.c[1] * pixel2.c[1];
  double b = pixel1.c[2] * pixel2.c[2];
  double a = pixel1.c[3];
  return R2Pixel(r, g, b, a);
}



inline R2Pixel 
operator*(const R2Pixel& pixel, double scale)
{
  // Scale rgb components by scalar
  double r = pixel.c[0] * scale;
  double g = pixel.c[1] * scale;
  double b = pixel.c[2] * scale;
  double a = pixel.c[3];
  return R2Pixel(r, g, b, a);
}



inline R2Pixel 
operator*(double scale, const R2Pixel& pixel)
{
  return pixel * scale;
}



inline R2Pixel 
operator/(const R2Pixel& pixel, double scale)
{
  // Divide rgb components by scalar
  assert(scale != 0);
  double r = pixel.c[0] / scale;
  double g = pixel.c[1] / scale;
  double b = pixel.c[2] / scale;
  double a = pixel.c[3];
  return R2Pixel(r, g, b, a);
}



// Public functions 

inline R2Pixel::
R2Pixel(void)
{
  // Initialize components to zero
  c[0] = 0; 
  c[1] = 0; 
  c[2] = 0;
  c[3] = 0;
}



inline R2Pixel::
R2Pixel(const R2Pixel& pixel)
{
  // Copy components 
  c[0] = pixel.c[0];
  c[1] = pixel.c[1];
  c[2] = pixel.c[2];
  c[3] = pixel.c[3];
}



inline R2Pixel::
R2Pixel(double red, double green, double blue, double alpha)
{
  // Set components 
  c[0] = red; 
  c[1] = green; 
  c[2] = blue;
  c[3] = alpha;
}



inline R2Pixel::
R2Pixel(const double rgba[4])
{
  // Set components 
  c[0] = rgba[0]; 
  c[1] = rgba[1]; 
  c[2] = rgba[2];
  c[3] = rgba[3];
}



inline bool R2Pixel::
operator==(const R2Pixel& pixel) const
{
  // Return whether pixel is equal
  return ((c[0] == pixel.c[0]) && (c[1] == pixel.c[1]) && (c[2] == pixel.c[2]) && (c[3] == pixel.c[3]));
}



inline bool R2Pixel::
operator!=(const R2Pixel& pixel) const
{
  // Return whether pixel is not equal
  return ((c[0] != pixel.c[0]) || (c[1] != pixel.c[1]) || (c[2] != pixel.c[2]) || (c[3] != pixel.c[3]));
}



inline R2Pixel& R2Pixel::
operator=(const R2Pixel& pixel)
{
  // Copy components
  c[0] = pixel.c[0];
  c[1] = pixel.c[1];
  c[2] = pixel.c[2];
  c[3] = pixel.c[3];
  return *this;
}



inline R2Pixel& R2Pixel::
operator+=(const R2Pixel& pixel)
{
  c[0] += pixel.c[0];
  c[1] += pixel.c[1];
  c[2] += pixel.c[2];
  return *this;
}



inline R2Pixel& R2Pixel::
operator-=(const R2Pixel& pixel)
{
  c[0] -= pixel.c[0];
  c[1] -= pixel.c[1];
  c[2] -= pixel.c[2];
  return *this;
}



inline R2Pixel& R2Pixel::
operator*=(const R2Pixel& pixel)
{
  c[0] *= pixel.c[0];
  c[1] *= pixel.c[1];
  c[2] *= pixel.c[2];
  return *this;
}



inline R2Pixel& R2Pixel::
operator*=(double a)
{
  c[0] *= a;
  c[1] *= a;
  c[2] *= a;
  return *this;
}



inline R2Pixel& R2Pixel::
operator/=(double a)
{
  //  assert(!zero(a)); 
  c[0] /= a;
  c[1] /= a;
  c[2] /= a;
  return *this;
}







#endif
