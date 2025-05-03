#include "image_processor.hpp"

ImageProcessor::ImageProcessor()
{
    // Load the face cascade classifier
    std::string faceCascadePath = cv::samples::findFile("haarcascades/haarcascade_frontalface_default.xml");
    if (!faceDetector.load(faceCascadePath))
    {
        std::cerr << "Error loading face cascade classifier" << std::endl;
        // Continue without face detection if it fails
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
    if (image.empty())
    {
        return image;
    }

    cv::Mat processed = image.clone();

    // Apply basic preprocessing for better hash generation
    // 1. Remove noise with a slight blur
    cv::GaussianBlur(processed, processed, cv::Size(3, 3), 0);

    return processed;
}

cv::Mat ImageProcessor::preprocess(const cv::Mat &image, bool focusOnFace)
{
    if (image.empty())
    {
        return image;
    }

    cv::Mat processed = image.clone();

    // Apply basic preprocessing for better hash generation
    // 1. Remove noise with a slight blur
    cv::GaussianBlur(processed, processed, cv::Size(3, 3), 0);

    if (focusOnFace)
    {
        cropToFace(processed);
    }
    return processed;
}

bool ImageProcessor::isValidImage(const cv::Mat &image) const
{
    return !image.empty();
}

bool ImageProcessor::meetsQualityRequirements(const cv::Mat &image, int minWidth, int minHeight) const
{
    if (!isValidImage(image))
    {
        return false;
    }

    // Check minimum dimensions
    return (image.cols >= minWidth && image.rows >= minHeight);
}

std::vector<cv::Rect> ImageProcessor::detectFaces(const cv::Mat &image)
{
    std::vector<cv::Rect> faces;

    // Convert to grayscale for face detection
    cv::Mat gray;
    if (image.channels() > 1)
    {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }
    else
    {
        gray = image.clone();
    }

    // Detect faces
    faceDetector.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

    return faces;
}

cv::Mat ImageProcessor::cropToFace(const cv::Mat &image)
{
    std::vector<cv::Rect> faces = detectFaces(image);

    if (faces.empty())
    {
        // No faces detected, return original
        return image.clone();
    }

    // Find the largest face (assuming it's the main one)
    auto largestFace = std::max_element(faces.begin(), faces.end(),
                                        [](const cv::Rect &a, const cv::Rect &b)
                                        {
                                            return a.area() < b.area();
                                        });

    // Add some margin around the face
    int margin = std::min(largestFace->width, largestFace->height) / 4;

    cv::Rect roi = *largestFace;
    roi.x = std::max(0, roi.x - margin);
    roi.y = std::max(0, roi.y - margin);
    roi.width = std::min(image.cols - roi.x, roi.width + 2 * margin);
    roi.height = std::min(image.rows - roi.y, roi.height + 2 * margin);

    // Return the cropped face region
    return image(roi).clone();
}