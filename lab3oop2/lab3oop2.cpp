#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <stdexcept>

int main()
{
    try
    {
        std::wstring directoryPath = L"C:/Users/roma1/source/repos/lab3oop/lab3oop2/мамка";
        if (!std::filesystem::exists(directoryPath))
        {
            throw std::invalid_argument("The specified directory does not exist.");
        }
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
        {
            if (!entry.is_regular_file())
            {
                throw std::runtime_error("Not a regular file: " + entry.path().string());
            }
            std::string fileName = entry.path().string();
            std::string extension = entry.path().extension().string();
            if (extension != ".jpg" && extension != ".jpeg" && extension != ".png" && extension != ".bmp" && extension != ".tiff")
            {
                throw std::invalid_argument("Unsupported file type: " + extension);
            }
            cv::Mat img = cv::imread(fileName);
            if (img.empty())
            {
                throw std::runtime_error("Could not load image: " + fileName);
            }
            cv::Mat mirroredImg;
            cv::flip(img, mirroredImg, 1);
            std::string newFileName = entry.path().stem().string() + "-mirrored.png";
            std::string savePath = (entry.path().parent_path() / newFileName).string();
            if (!cv::imwrite(savePath, mirroredImg))
            {
                throw std::runtime_error("Could not save image: " + savePath);
            }
            std::cout << "Saved mirrored image as " << savePath << std::endl;
        }
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Invalid Argument Error: " << e.what() << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Filesystem Error: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred!" << std::endl;
    }
    return 0;
}
