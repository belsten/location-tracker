// Alex Belsten
// belsta@rpi.edu
// github: belsten
// builds a map and consolidates data from google
// 12/18/2017

#include <cstdlib>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>
#include <stdlib.h>
#include <algorithm>
#include "json.hpp"
#include "image.h"

using nlohmann::json;



void parse_arguments(int argc, char* argv[], std::string& in, std::string out) {
  for (int i = 1; i < argc; i++) {
    if (argv[i] == std::string("-input_image")) {
      i++;
      assert (i < argc);
      in = argv[i];
    } else if (argv[i] == std::string("-output_image")) {
      i++;
      assert (i < argc);
      out = argv[i];
    }
  }
}

template <class T>
int longitude_to_x(float longitude, const Image<T>& a) {
   return (a.Width() * (float)((abs(longitude) - 65.0) / 60.0));
}


template <class T>
int latitude_to_y(float latitude, const Image<T>& a) {
  return (a.Height() - ((float)a.Height() * (float)((abs(latitude) - 25.0) / 25.0)));
}

int main(int argc, char* argv[]) {

  std::string input_image, output_image;
  input_image = std::string("danny.ppm");
  //parse_arguments(argc, argv, input_image, output_image);

  std::ifstream stream("Location History.json");
  json object_data;
  Image<Color> input, output;
  input.Load(input_image);

  input.InitializeOutputFile(output);
  stream >> object_data;
  int x_, y_;
  float longitude, latitude;
  for (int i = 0; i < object_data.at("locations").size(); i++) {
    std::string unix_t = object_data.at("locations")[i].at("timestampMs");
    longitude = (float)object_data.at("locations")[i].at("longitudeE7") / 10000000.0;
    latitude = (float)object_data.at("locations")[i].at("latitudeE7") / 10000000.0;
    x_ = longitude_to_x(longitude, input);
    y_ = latitude_to_y (latitude , input);
    assert(y_ < input.Height());
    assert(x_ < input.Width());
    //if (i % 100 == 0 ) std::cout<<x_<<" " <<longitude<<" "<<y_<<" " <<latitude<<std::endl;
    output.SetPixel(x_, y_, Color());
  }

  output.Save(("output_" + input_image));
}
