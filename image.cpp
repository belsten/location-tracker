#include <cstring>
#include <cstdio>
#include "image.h"

// template <class T>
// void Image<T>::InitializeOutputFile(Image<T> &out) const {
//   out.Allocate(width, height);
//   int w = width;
//   int h = height;
//   for (int i = 0; i < w; i++) {
//     for (int j = 0; j < h; j++) {
//       out.SetPixel(i,j, this -> GetPixel(i,j));
//     }
//   }
// }

// ====================================================================
// EXPLICIT SPECIALIZATIONS for Color images (.ppm)
// ====================================================================
template <>
bool Image<Color>::Save(const std::string &filename) const {
  int len = filename.length();
  if (!(len > 4 && filename.substr(len-4) == std::string(".ppm"))) {
    std::cerr << "ERROR SAVING: This is not a PPM filename: " << filename << std::endl;
    return false;
  }
  FILE *file = fopen(filename.c_str(), "wb");
  if (file == NULL) {
    std::cerr << "Unable to open " << filename << " for writing\n";
    return false;
  }

  // misc header information
  fprintf (file, "P6\n");
  fprintf (file, "%d %d\n", width,height);
  fprintf (file, "255\n");

  // the data
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x=0; x<width; x++) {
      Color v = GetPixel(x,y);
      fputc ((unsigned char)(v.r),file);
      fputc ((unsigned char)(v.g),file);
      fputc ((unsigned char)(v.b),file);
    }
  }
  fclose(file);
  return true;
}

template <>
bool Image<Color>::Load(const std::string &filename) {
  int len = filename.length();
  if (!(len > 4 && filename.substr(len-4) == std::string(".ppm"))) {
    std::cerr << "ERROR LOADING: This is not a PPM filename: " << filename << std::endl;
    return false;
  }
  FILE *file = fopen(filename.c_str(),"rb");
  if (file == NULL) {
    std::cerr << "Unable to open " << filename << " for reading\n";
    return false;
  }

  // misc header information
  char tmp[100];
  fgets(tmp,100,file);
  //assert (strstr(tmp,"P6"));
  fgets(tmp,100,file);
  while (tmp[0] == '#') { fgets(tmp,100,file); }
  sscanf(tmp,"%d %d",&width,&height);
  fgets(tmp,100,file);
  //assert (strstr(tmp,"255"));

  // the data
  delete [] data;
  data = new Color[height*width];
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      Color c;
      c.r = fgetc(file);
      c.g = fgetc(file);
      c.b = fgetc(file);
      SetPixel(x,y,c);
    }
  }
  fclose(file);
  return true;
}



// ====================================================================
// EXPLICIT SPECIALIZATIONS for greyscale images (.pbm)
// ====================================================================
template <>
bool Image<int>::Save(const std::string &filename) const {
  int len = filename.length();
  if (!(len > 4 && filename.substr(len-4) == std::string(".pgm"))) {
    std::cerr << "ERROR SAVING: This is not a PPM filename: " << filename << std::endl;
    return false;
  }
  FILE *file = fopen(filename.c_str(), "wb");
  if (file == NULL) {
    std::cerr << "Unable to open " << filename << " for writing\n";
    return false;
  }

  // misc header information
  fprintf (file, "P5\n");
  fprintf (file, "%d %d\n", width,height);
  fprintf (file, "255\n");

  // the data
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x=0; x<width; x++) {
      int v = GetPixel(x,y);
      fputc ((unsigned char)(v),file);
    }
  }
  fclose(file);
  return true;
}

template <>
bool Image<int>::Load(const std::string &filename) {
  int len = filename.length();
  if (!(len > 4 &&
        (filename.substr(len-4) == std::string(".pgm") ||
         filename.substr(len-4) == std::string(".pbm")))) {
    std::cerr << "ERROR LOADING: This is not a PBM filename: " << filename << std::endl;
    return false;
  }
  FILE *file = fopen(filename.c_str(),"rb");
  if (file == NULL) {
    std::cerr << "Unable to open " << filename << " for reading\n";
    return false;
  }

  // misc header information
  char tmp[100];
  fgets(tmp,100,file);
  //assert (strstr(tmp,"P5"));
  fgets(tmp,100,file);
  while (tmp[0] == '#') { fgets(tmp,100,file); }
  int read = sscanf(tmp,"%d %d",&width,&height);
  if (read == 1) {
    fgets(tmp,100,file);
    while (tmp[0] == '#') { fgets(tmp,100,file); }
    sscanf(tmp,"%d",&height);
  }
  fgets(tmp,100,file);
  //assert (strstr(tmp,"255"));

  // the data
  delete [] data;
  data = new int[height*width];
  // flip y so that (0,0) is bottom left corner
  for (int y = height-1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      int v = fgetc(file);
      SetPixel(x,y,v);
    }
  }
  fclose(file);
  return true;
}

// ====================================================================
// ====================================================================
