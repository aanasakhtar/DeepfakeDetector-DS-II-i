#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class ImageProcessor
{
public:
    ImageProcessor();

    // Load image from file
    cv::Mat loadImage(const std::string &filepath);

    // Basic preprocessing for better hash generation
    cv::Mat preprocess(const cv::Mat &image);
    cv::Mat preprocess(const cv::Mat &image, bool focusOnFace);

    // Check if image is valid
    bool isValidImage(const cv::Mat &image) const;

    // Check if image meets minimum quality requirements
    bool meetsQualityRequirements(const cv::Mat &image, int minWidth = 100, int minHeight = 100) const;

    // Get face regions (useful for profile pictures)
    std::vector<cv::Rect> detectFaces(const cv::Mat &image);

    // Crop to main face region (useful for profile pictures)
    cv::Mat cropToFace(const cv::Mat &image);

private:
    // Face detector
    cv::CascadeClassifier faceDetector;
};