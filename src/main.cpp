#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "detector.hpp"

namespace fs = std::filesystem;

void printUsage(const char *programName)
{
    std::cout << "Usage: " << programName << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --init <folder>     Initialize database with images from folder" << std::endl;
    std::cout << "  --check <image>     Check if an image is a duplicate/fake" << std::endl;
    std::cout << "  --add <image>       Add a single image to the database" << std::endl;
    std::cout << "  --database <file>   Specify database file (default: phash_db.dat)" << std::endl;
    std::cout << "  --threshold <num>   Set Hamming distance threshold (default: 5)" << std::endl;
    std::cout << "  --save              Save database after operations" << std::endl;
    std::cout << "  --help              Show this help message" << std::endl;
}

void initializeDatabase(Detector &detector, const std::string &folderPath)
{
    std::cout << "Initializing database with images from: " << folderPath << std::endl;

    int count = 0;
    for (const auto &entry : fs::recursive_directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            std::string extension = entry.path().extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

            if (extension == ".jpg" || extension == ".jpeg" || extension == ".png" || extension == ".bmp")
            {
                if (detector.addOriginalImage(entry.path().string()))
                {
                    count++;
                    std::cout << "Added: " << entry.path().filename() << std::endl;
                }
            }
        }
    }

    std::cout << "Added " << count << " images to the database" << std::endl;
}

void checkImage(Detector &detector, const std::string &imagePath)
{
    std::cout << "Checking image: " << imagePath << std::endl;

    DetectionDetails details = detector.checkImage(imagePath);

    switch (details.result)
    {
    case DetectionResult::ORIGINAL:
        std::cout << "Result: ORIGINAL - No matches found in database" << std::endl;
        break;

    case DetectionResult::EXACT_DUPLICATE:
        std::cout << "Result: EXACT DUPLICATE DETECTED!" << std::endl;
        break;

    case DetectionResult::SIMILAR:
    {
        std::cout << "Result: SIMILAR IMAGE DETECTED!" << std::endl;
        std::cout << "Number of similar matches: " << details.matches.size() << std::endl;
        std::string bestMatchPath = "Unknown";
        for (const auto &match : details.matches)
        {
            if (match.imageId == details.bestMatchId)
            {
                bestMatchPath = match.sourcePath;
                break;
            }
        }
        std::cout << "Best match (ID: " << details.bestMatchId
                  << ", Distance: " << details.bestMatchDistance
                  << ", Path: " << bestMatchPath << ")" << std::endl;

        std::cout << "Similar images:" << std::endl;
        int count = 0;
        for (const auto &match : details.matches)
        {
            std::cout << "  - " << match.imageId << " (" << match.sourcePath << ")" << std::endl;
            count++;
            if (count >= 5)
            {
                std::cout << "  ... and " << (details.matches.size() - 5) << " more" << std::endl;
                break;
            }
        }
        break;
    }
    case DetectionResult::INVALID_IMAGE:
        std::cout << "Result: INVALID IMAGE - Could not process the image" << std::endl;
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printUsage(argv[0]);
        return 1;
    }

    std::string databaseFile = "phash_db.dat";
    int threshold = 3;
    bool saveDatabase = false;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "--help")
        {
            printUsage(argv[0]);
            return 0;
        }
        else if (arg == "--database" && i + 1 < argc)
        {
            databaseFile = argv[++i];
        }
        else if (arg == "--threshold" && i + 1 < argc)
        {
            threshold = std::stoi(argv[++i]);
        }
        else if (arg == "--save")
        {
            saveDatabase = true;
        }
    }

    Detector detector(16, threshold); // Changed from 64 to match HashGenerator
    bool initialized = detector.initialize(databaseFile);

    if (!initialized)
    {
        std::cout << "Created new database. Will be saved to: " << databaseFile << std::endl;
    }
    else
    {
        std::cout << "Loaded database with " << detector.getDatabaseSize() << " images" << std::endl;
    }

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "--init" && i + 1 < argc)
        {
            initializeDatabase(detector, argv[++i]);
            saveDatabase = true;
        }
        else if (arg == "--check" && i + 1 < argc)
        {
            checkImage(detector, argv[++i]);
        }
        else if (arg == "--add" && i + 1 < argc)
        {
            std::string imagePath = argv[++i];
            if (detector.addOriginalImage(imagePath))
            {
                std::cout << "Added image to database: " << imagePath << std::endl;
                saveDatabase = true;
            }
            else
            {
                std::cout << "Failed to add image: " << imagePath << std::endl;
            }
        }
        else if (arg == "--database" || arg == "--threshold" || arg == "--save")
        {
            i++;
        }
    }

    if (saveDatabase && !detector.saveDatabase(databaseFile))
    {
        std::cout << "Error: Failed to save database" << std::endl;
        return 1;
    }
    else if (saveDatabase)
    {
        std::cout << "Database saved to: " << databaseFile << std::endl;
    }

    return 0;
}