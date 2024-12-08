#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

int main() 
{
    try 
    {
        std::wstring directoryPath = L"C:/Users/roma1/source/repos/lab3oop/lab3oop2/мамка";
        if (!std::filesystem::exists(directoryPath)) {
            std::cerr << "Error: Directory does not exist!" << std::endl;
            return -1;
        }
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) 
        {
            if (entry.is_regular_file()) 
            {
                std::string fileName = entry.path().string();
                std::string extension = entry.path().extension().string();
                if (extension == ".jpg" || extension == ".jpeg" || extension == ".png" || extension == ".bmp" || extension == ".tiff") 
                {
                    cv::Mat img = cv::imread(fileName);
                    if (img.empty()) 
                    {
                        std::cerr << "Error: Could not load image " << fileName << std::endl;
                        continue;
                    }
                    cv::Mat mirroredImg;
                    cv::flip(img, mirroredImg, 1);
                    std::string newFileName = entry.path().stem().string() + "-mirrored.png";
                    std::string savePath = (entry.path().parent_path() / newFileName).string();
                    if (cv::imwrite(savePath, mirroredImg)) 
                    {
                        std::cout << "Saved mirrored image as " << savePath << std::endl;
                    }
                    else 
                    {
                        std::cerr << "Error: Could not save image " << savePath << std::endl;
                    }
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) 
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) 
    {
        std::cerr << "Unknown error occurred!" << std::endl;
    }

    return 0;
}
