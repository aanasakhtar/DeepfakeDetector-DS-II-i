#include "hash_generator.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>

HashGenerator::HashGenerator(int featureSize) : featureSize(featureSize) {}

cv::Mat HashGenerator::preprocessForHash(const cv::Mat &image, int size)
{
    cv::Mat grayscale;
    if (image.channels() == 3 || image.channels() == 4)
    {
        cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
    }
    else
    {
        grayscale = image.clone();
    }
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
    clahe->apply(grayscale, grayscale);
    return grayscale;
}

std::vector<float> HashGenerator::generateDCTFeatures(const cv::Mat &image)
{
    cv::Mat preprocessed = preprocessForHash(image, 32);

    cv::Mat floatImg;
    preprocessed.convertTo(floatImg, CV_32F);
    cv::Mat dct;
    cv::dct(floatImg, dct);

    cv::Mat dctFull = dct(cv::Rect(0, 0, 8, 8));
    std::vector<float> features(featureSize, 0.0);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0;
            for (int di = 0; di < 2; di++)
                for (int dj = 0; dj < 2; dj++)
                    sum += dctFull.at<float>(2*i + di, 2*j + dj);
            features[i * 4 + j] = sum / 4.0;
        }
    }

    // Normalize to unit length
    float norm = 0.0;
    for (float f : features) norm += f * f;
    norm = std::sqrt(norm);
    if (norm > 0)
    {
        for (float &f : features) f /= norm;
    }

    std::cout << "Generated DCT features (size=" << features.size() << "): ";
    for (float f : features) std::cout << f << " ";
    std::cout << std::endl;

    return features;
}

std::string HashGenerator::hashToString(const std::vector<float> &features)
{
    std::stringstream ss;
    for (float f : features) ss << f << ",";
    std::string result = ss.str();
    if (!result.empty()) result.pop_back(); // Remove trailing comma
    return result;
}

std::vector<float> HashGenerator::stringToHash(const std::string &hashStr)
{
    std::vector<float> features;
    std::stringstream ss(hashStr);
    std::string token;
    while (std::getline(ss, token, ','))
    {
        if (token.empty())
        {
            std::cerr << "Warning: Empty token in hash string: " << hashStr << std::endl;
            continue;
        }
        try
        {
            size_t pos;
            float value = std::stof(token, &pos);
            if (pos != token.length())
            {
                std::cerr << "Warning: Invalid float format in token: " << token << std::endl;
                continue;
            }
            features.push_back(value);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing token '" << token << "': " << e.what() << std::endl;
            continue;
        }
    }
    if (features.empty())
    {
        std::cerr << "Warning: No valid features parsed from hash string: " << hashStr << std::endl;
    }
    return features;
}