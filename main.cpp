#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;



Mat removeLight(Mat img, Mat pattern, int method) {

  Mat aux;
  // if method is normalization
  if(method==1)
  {
    // Require change our image to 32 float for division
    Mat img32, pattern32;
    img.convertTo(img32, CV_32F);
    pattern.convertTo(pattern32, CV_32F);
    // Divide the image by the pattern
    aux= 1-(img32/pattern32);
    // Scale it to convert to 8bit format
    aux=aux*255;
    // Convert 8 bits format
    aux.convertTo(aux, CV_8U);
  }else{
    aux= pattern-img;
  }
  return aux;
}

Mat calculateLightPattern(Mat img) {
  Mat pattern;
  // Basic and effective way to calculate the light pattern from one image
  cv::blur(img, pattern, cv::Size(img.cols/3,img.cols/3));
  return pattern;
}

Mat threshold(Mat img_no_light, int method_light) {
  // Binarize image for segment
  Mat img_thr;
  if (method_light!=2) {
    threshold(img_no_light, img_thr, 30, 255, THRESH_BINARY);
  } else {
    threshold(img_no_light, img_thr, 140, 255, THRESH_BINARY_INV);
  }
  return img_thr;
}

Scalar randomColor( RNG& rng ) {
    int icolor = (unsigned) rng;
    return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}

void ConnectedComponents(Mat img) {
  // Use connected components to divide our possibles parts of images
  Mat labels;
  int num_objects = connectedComponents(img, labels);
  // Check the number of objects detected
  if(num_objects < 2 ){
    cout << "No objects detected" << endl;
    return;
  }else{
    cout << "Number of objects detected: " << num_objects - 1 << endl;
  }
  // Create output image coloring the objects
  Mat output= Mat::zeros(img.rows, img.cols, CV_8UC3);
  RNG rng( 0xFFFFFFFF );
  for(int i=1; i<num_objects; i++){
    Mat mask= labels==i;
    output.setTo(randomColor(rng), mask);
  }
  imshow("Result", output);
}

int main( int argc, const char** argv) {

  String img_file= "driver1.jpg";
  String light_pattern_file="driver1.jpg";

  // Load image to process
  Mat img = imread(img_file, CV_LOAD_IMAGE_COLOR);
  if(img.data==NULL){
    cout << "Error loading image "<< img_file << endl;
    return 0;
  }

  Mat img_noise;
  cv::medianBlur(img, img_noise, 3);

  Mat light_pattern;
  light_pattern = calculateLightPattern(img);
  Mat img_no_light;
  img_no_light = removeLight(img_noise, light_pattern, 1);


    cv::imshow("My Image", img_no_light);
    cv::waitKey(0);
    return 0;


}
