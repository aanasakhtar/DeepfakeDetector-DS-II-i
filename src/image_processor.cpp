#include "image_processor.hpp"

ImageProcessor::ImageProcessor()
{
    std::string faceCascadePath = cv::samples::findFile("haarcascades/haarcascade_frontalface_default.xml");
    if (!faceDetector.load(faceCascadePath))
    {
        std::cerr << "Error loading face cascade classifier" << std::endl;
    }
}

cv::Mat ImageProcessor::loadImage(const std::string &filepath)
{
    cv::Mat image = cv::imread(filepath, cv::IMREAD_COLOR);
    if (image.empty())
    {
        std::cerr << "Error: Could not load image: " << filepath << std::endl;
    }
    return image;
}

cv::Mat ImageProcessor::preprocess(const cv::Mat &image)
{
    if (image.empty()) return image;

    cv::Mat processed = image.clone();
    cv::GaussianBlur(processed, processed, cv::Size(3, 3), 0);

    processed = cropToFace(processed);
    if (processed.empty()) processed = image.clone();

    cv::resize(processed, processed, cv::Size(32, 32), 0, 0, cv::INTER_AREA);
    return processed;
}

cv::Mat ImageProcessor::preprocess(const cv::Mat &image, bool focusOnFace)
{
    if (image.empty()) return image;

    cv::Mat processed = image.clone();
    cv::GaussianBlur(processed, processed, cv::Size(3, 3), 0);

    if (focusOnFace)
    {
        processed = cropToFace(processed);
        if (processed.empty()) processed = image.clone();
    }

    cv::resize(processed, processed, cv::Size(32, 32), 0, 0, cv::INTER_AREA);
    return processed;
}

bool ImageProcessor::isValidImage(const cv::Mat &image) const
{
    return !image.empty();
}

bool ImageProcessor::meetsQualityRequirements(const cv::Mat &image, int minWidth, int minHeight) const
{
    return isValidImage(image) && image.cols >= minWidth && image.rows >= minHeight;
}

std::vector<cv::Rect> ImageProcessor::detectFaces(const cv::Mat &image)
{
    std::vector<cv::Rect> faces;
    cv::Mat gray;
    if (image.channels() > 1) cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    else gray = image.clone();
    faceDetector.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));
    return faces;
}


cv::Mat ImageProcessor::cropToFace(const cv::Mat &image)
{
    std::vector<cv::Rect> faces = detectFaces(image);
    if (faces.empty())
    {
        std::cout << "No faces detected, using full image" << std::endl;
        return image.clone();
    }

    auto largestFace = std::max_element(faces.begin(), faces.end(),
        [](const cv::Rect &a, const cv::Rect &b) { return a.area() < b.area(); });
    std::cout << "Largest face detected: x=" << largestFace->x << ", y=" << largestFace->y
              << ", width=" << largestFace->width << ", height=" << largestFace->height << std::endl;

    int margin = std::min(largestFace->width, largestFace->height) / 4;
    cv::Rect roi = *largestFace;
    roi.x = std::max(0, roi.x - margin);
    roi.y = std::max(0, roi.y - margin);
    roi.width = std::min(image.cols - roi.x, roi.width + 2 * margin);
    roi.height = std::min(image.rows - roi.y, roi.height + 2 * margin);

    return image(roi).clone();
}