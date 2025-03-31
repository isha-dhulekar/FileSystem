#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <map>

namespace fs = std::filesystem;

// Function to copy a file from source to destination
void copyFile(const fs::path& source, const fs::path& destination)
{
    try {
        // Create destination directory if it doesn't exist
        fs::create_directories(destination.parent_path());

        // Copy the file
        fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
        std::cout << "Copied: " << source << " -> " << destination << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error copying file: " << e.what() << std::endl;
    }
}

// Function to find all f1 folders and copy their image files
void findAndCopyF1Files(const fs::path& rootDir, const fs::path& destDir)
{
    std::vector<fs::path> imageFiles;

    // Find all image files in f1 folders
    for (const auto& entry : fs::recursive_directory_iterator(rootDir)) {
        if (entry.is_directory() && entry.path().filename() == "RUV113") {

            for (const auto& subentry : fs::recursive_directory_iterator(entry.path())) {
                if (subentry.is_directory() && subentry.path().filename() == "MARK") {
                    // For each f1 folder, find image files
                    for (const auto& file : fs::directory_iterator(entry.path())) {
                        if (file.is_regular_file()) {
                            std::string ext = file.path().extension().string();
                            // Common image extensions
                            if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" ||
                                ext == ".gif" || ext == ".bmp" || ext == ".tiff") {
                                imageFiles.push_back(file.path());
                            }
                        }
                    }
                }
            }


        }
    }
            

    std::cout << "Found " << imageFiles.size() << " image files in 'MARK' folders.\n";

    // Copy files with ascending numeric names
    int fileCounter = 1;
    for (const auto& imgFile : imageFiles) {
        // Get file extension
        std::string extension = imgFile.extension().string();

        // Create new filename with ascending number
        std::string newFileName = "image_" + std::to_string(fileCounter) + extension;
        fs::path destPath = destDir / newFileName;

        copyFile(imgFile, destPath);
        fileCounter++;
    }

    std::cout << "Copied " << (fileCounter - 1) << " files with ascending numeric names." << std::endl;
}

int main()
{
    // Set your root directory and destination directory
    std::string rootDirectory = R"(D:\Result\2025\3\7)";  // Change this to your main folder path
    std::string destinationDirectory = R"(F:\PCデータ\出口_PC\業務\THK(三重)\クロスローラーリング\THK三重　クロスローラ（客先から送付された画像データー）\20250310\RUV168)";  // Change to your destination folder

    try {
        // Ensure the destination directory exists
        fs::create_directories(destinationDirectory);

        // Find and copy files
        findAndCopyF1Files(rootDirectory, destinationDirectory);

        std::cout << "Copy operation completed successfully.\n";
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

