#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include "VideoDetector.h"

class FaceListener : public affdex::FaceListener {
  public:
    void onFaceFound(float timestamp, affdex::FaceId faceId);
    void onFaceLost(float timestamp, affdex::FaceId faceId);
    FaceListener(affdex::VideoDetector detector) {
      this->detector = detector;
      detector.setFaceListener(this);
    }
  private:
    affdex::Detector detector;
};

void FaceListener::onFaceFound(float timestamp, affdex::FaceId faceId) {
  //std::cout << "Found: " << timestamp << std::endl;
}

void FaceListener::onFaceLost(float timestamp, affdex::FaceId faceId) {
  //std::cout << "Lost: " << timestamp << std::endl;
}


class ImageListener : public affdex::ImageListener {
  public:
    void onImageResults(std::map<affdex::FaceId, affdex::Face> faces, affdex::Frame image);
    void onImageCapture(affdex::Frame image);
    void setVideo(int videoId);
    void reset();
    std::ofstream outputFile;
    ImageListener(std::string tag, std::string filename, affdex::VideoDetector detector) {
      this->videoId = -1;
      this->tag = tag;
      this->outputFile.open(filename);
      this->frame = 0;
      this->detector = detector;
      detector.setImageListener(this);
    }

  private:
    affdex::Detector detector;
    std::string tag;
    int videoId;
    long unsigned int frame;
};

void ImageListener::onImageResults(
    std::map<affdex::FaceId, affdex::Face> faces,
    affdex::Frame image) {
  std::map<affdex::FaceId, affdex::Face>::iterator it;
  float emotions[9] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  for (it = faces.begin(); it != faces.end(); it++) {
    affdex::Emotions e = it->second.emotions;
    emotions[0] = e.joy;
    emotions[1] = e.fear;
    emotions[2] = e.disgust;
    emotions[3] = e.sadness;
    emotions[4] = e.anger;
    emotions[5] = e.surprise;
    emotions[6] = e.contempt;
    emotions[7] = e.valence;
    emotions[8] = e.engagement;

    std::ofstream& ostream = this->outputFile;

    ostream << this->tag << ", " << this->videoId << ", " << this->frame << ", " << image.getTimestamp() << ", " << it->first;
    for (int i = 0; i < 9; i++) {
      ostream << ", " << emotions[i];
    }
    ostream << std::endl;
  }
}

void ImageListener::onImageCapture (affdex::Frame image) {
  this->frame += 1;
}

void ImageListener::reset() {
  this->frame = 0;
}

void ImageListener::setVideo(int videoId) {
  this->videoId = videoId;
}


class ProcessStatusListener : public affdex::ProcessStatusListener {
  public:
    void onProcessingException(affdex::AffdexException ex);
    void onProcessingFinished();
    bool* ptrDoneProcessing;
    ProcessStatusListener(bool* ptrDoneProcessing, affdex::VideoDetector detector) {
      this->ptrDoneProcessing = ptrDoneProcessing;
      this->detector = detector;
      detector.setProcessStatusListener(this);
    }

  private:
    affdex::Detector detector;
};

void ProcessStatusListener::onProcessingException(affdex::AffdexException ex) {
  std::cout << "::Exception::" << std::endl;
}

void ProcessStatusListener::onProcessingFinished() {
  *(this->ptrDoneProcessing) = true;
}


int main(int argc, const char *argv[])
{
  affdex::VideoDetector detector = affdex::VideoDetector(15);
  std::string classifierPath="/home/cnezin/affdex-sdk/data/";
  detector.setClassifierPath(classifierPath);

  bool doneProcessing = false;
  bool* ptrDoneProcessing = &doneProcessing;
  // Parse inputs
  std::istringstream ss(argv[1]);
  int numVideos;
  ss >> numVideos;
  std::string tag = argv[2];

  std::cout << tag << std::endl;
  std::cout << numVideos << std::endl;
  std::string inputFilepath = tag + "-videos/";
  std::string outputFilepath = tag + ".csv";

  FaceListener faceListener = FaceListener(detector);
  ImageListener imageListener = ImageListener(tag, outputFilepath, detector);
  ProcessStatusListener processStatusListener(ptrDoneProcessing, detector);

  detector.setDetectAllEmotions(true);
 
  for (int i = 0; i < numVideos; i++) { 
    detector.start();
    std::cout << "VideoId: " << i << std::endl;
    imageListener.setVideo(i);
    detector.process(inputFilepath + std::to_string(i) + ".mp4");
    while (true) {
      if (doneProcessing) {
        *ptrDoneProcessing = false;
        break;
      }
      usleep(1e6);
    }
    detector.reset();
    imageListener.reset();
  }
  detector.stop();
  return 0;
}
