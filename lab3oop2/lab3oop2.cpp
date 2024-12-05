#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <iterator>

class FileProcessor 
{
private:
    std::vector<std::string> fileNames;
    std::vector<std::string> noFile;
    std::vector<std::string> badData;
    std::vector<std::string> overflowFiles;
    std::vector<int> products;

    void writeToFile(const std::string& fileName, const std::vector<std::string>& data)
    {
        std::ofstream outputFile(fileName);
        if (!outputFile)
        {
            std::cerr << "Error: Could not create file " << fileName << std::endl;
            throw std::runtime_error("File creation failed");
        }
        std::copy(data.begin(), data.end(), std::ostream_iterator<std::string>(outputFile, "\n"));
    }
public:
    FileProcessor(const std::vector<std::string>& files) : fileNames(files) {}

    int tryParseInt(const std::string& str) 
    {
        std::size_t pos;
        int result = std::stoi(str, &pos);
        if (pos != str.length()) 
        {
            throw std::invalid_argument("Invalid integer format");
        }
        return result;
    }
    void processFiles() 
    {
        std::for_each(fileNames.begin(), fileNames.end(), [this](const std::string& fileName) 
            {
            try 
            {
                processFile(fileName);
            }
            catch (const std::ios_base::failure&) 
            {
                noFile.push_back(fileName);
            }
            catch (const std::invalid_argument&) 
            {
                badData.push_back(fileName);
            }
            catch (const std::overflow_error&) 
            {
                overflowFiles.push_back(fileName);
            }
            });
    }
    void processFile(const std::string& fileName)
    {
        std::ifstream inputFile(fileName);
        if (!inputFile) 
        {
            throw std::ios_base::failure("File not found");
        }
        std::string line1, line2;
        if (!std::getline(inputFile, line1)||!std::getline(inputFile, line2)) 
        {
            throw std::ios_base::failure("Bad data");
        }
        int num1 = tryParseInt(line1);
        int num2 = tryParseInt(line2);
        if (isOverflow(num1, num2)) 
        {
            throw std::overflow_error("Overflow detected");
        }
        int product = num1 * num2;
        products.push_back(product);
    }
    bool isOverflow(int num1, int num2) 
    {
        if (num1 > 0 && num2 > 0 && num1 > std::numeric_limits<int>::max() / num2) 
        {
            throw std::overflow_error("Overflow detected");   
        }
        if (num1 < 0 && num2 < 0 && num1 < std::numeric_limits<int>::min() / num2) 
        {
            throw std::overflow_error("Overflow detected");
        }
        if ((num1 > 0 && num2 < 0 && num2 < std::numeric_limits<int>::min() / num1) || (num1 < 0 && num2 > 0 && num1 < std::numeric_limits<int>::min() / num2)) 
        {
            throw std::overflow_error("Overflow detected");
        }
        return false;
    }
    void writeResults() 
    {
        writeToFile("no_file.txt", noFile);
        writeToFile("bad_data.txt", badData);
        writeToFile("overflow.txt", overflowFiles);
        if (!products.empty()) 
        {
            double average = std::accumulate(products.begin(), products.end(), 0.0) / products.size();
            std::cout << "Average of products: " << average << std::endl;
        }
        else 
        {
            std::cerr << "No valid products to calculate the average." << std::endl;
        }
    }
};
int main() 
{
    std::vector<std::string> fileNames = 
    {
        "10.txt", "11.txt", "12.txt", "13.txt", "14.txt",
        "15.txt", "16.txt", "17.txt", "18.txt", "19.txt",
        "20.txt", "21.txt", "22.txt", "23.txt", "24.txt",
        "25.txt", "26.txt", "27.txt", "28.txt", "29.txt"
    };
    try 
    {
        FileProcessor processor(fileNames);
        processor.processFiles();
        processor.writeResults();
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
