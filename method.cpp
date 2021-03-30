#include <iostream>
#include <vector>
#include <ctime>


using namespace std;

const double idT = 0.5;
const int nCities = 6;
const int graph[nCities][nCities] = {
    0,  34, 24, 35,  1,  7,
    34,  0,  5, 19, 13,  2,
    24,  5,  0, 16, 87, 32,
    35, 19, 16,  0, 55, 18,
    1,  13, 87, 55,  0, 32,
    7,   2, 32, 18, 32,  0,    
};


void generatePrimaryArray(std::vector<int> &arr, size_t size);
std::vector<int> swapRandomCities(std::vector<int> &primaryArray);
int fitnessFunction(const std::vector<int> &arr);
void outputVector(const std::vector<int> &arr);
void lexicAlg(const std::vector<int> &primaryArr, std::vector<int> &outputArr);
void simulationAnnealing(const vector<int> &primaryArr, std::vector<int> &resultArr);


int main()
{
    srand(time(NULL));

    std::vector<int> primaryArr;
    generatePrimaryArray(primaryArr, nCities);

    std::vector<int> simAnnArr;
    simulationAnnealing(primaryArr, simAnnArr);
    std::cout << "Minimum dist simulating annealing: " << fitnessFunction(simAnnArr) << std::endl;
    outputVector(simAnnArr);

    std::cout << "Primary: " << fitnessFunction(primaryArr) << std::endl;
    outputVector(primaryArr);

    std::vector<int> lexicArr;
    lexicAlg(primaryArr, lexicArr);
    std::cout << "Lexic min: " << fitnessFunction(lexicArr) << std::endl;
    outputVector(lexicArr);
}


void generatePrimaryArray(std::vector<int>& arr, size_t size)
{
    for (int i = 0; i < size; ++i)
    {
        arr.push_back(i);
    }
}

std::vector<int> swapRandomCities(std::vector<int>& primaryArray)
{
    std::vector<int> arr = primaryArray;
    int a = rand() % arr.size();
    int b = 0;
    while (true)
    {
        b = rand() % arr.size();
        if (a != b)
        {
            break;
        }
    }
    std::swap(arr[a], arr[b]);
    return arr;
}

int fitnessFunction(const std::vector<int>& arr)
{
    int resultDist = 0;
    for (int i = 0; i < nCities; ++i)
    {
        int city1 = arr[i];
        int city2 = arr[0];
        if (i != arr.size() - 1)
        {
            city2 = arr[i + 1];
        }
        //std::cout << city1 << " --> " << city2 << " = " << graph[city1][city2] << std::endl;

        resultDist += graph[city1][city2];
    }
    return resultDist;
}

void outputVector(const std::vector<int>& arr)
{
    for (int i = 0; i < arr.size(); ++i)
    {
        std::cout << " " << arr[i];
    }
    std::cout << std::endl;
}

void lexicAlg(const std::vector<int>& primaryArr, std::vector<int>& outputArr)
{
    std::vector<int> tasks_min;
    std::vector<int> tasks_tmp;
    tasks_tmp = tasks_min = primaryArr;

    while (true)
    {
        // Find the largest i-number such that ai < ai + 1
        int i = static_cast<int>(tasks_tmp.size()) - 2;
        for (; i >= 0 && tasks_tmp[i + 1] < tasks_tmp[i]; i--);

        if (i == -1)
            break;

        // Find such a j-number for which aj > ai.
        std::int32_t j = static_cast<std::int32_t>(tasks_tmp.size()) - 1;
        for (; j >= 0 && tasks_tmp[j] < tasks_tmp[i]; j--);

        std::swap(tasks_tmp[i], tasks_tmp[j]);
        std::reverse(tasks_tmp.begin() + (i + 1), tasks_tmp.end());

        if (fitnessFunction(tasks_min) > fitnessFunction(tasks_tmp))
            tasks_min = tasks_tmp;
    }
    outputArr = tasks_min;
}

void simulationAnnealing(const vector<int>& primaryArr, std::vector<int>& resultArr)
{
    resultArr = primaryArr;
    auto currentArr = primaryArr;
    auto currentDist = fitnessFunction(currentArr);
    for (double modificatorTemperature = 100.0; modificatorTemperature > 0.1; modificatorTemperature *= idT)
    {
        std::vector<int> tmpArr = swapRandomCities(currentArr);
        //std::cout << "\nResult array:" << std::endl;
        int tmpDist = fitnessFunction(tmpArr);
        std::cout << "Result: " << tmpDist << std::endl;
        outputVector(tmpArr);

        if (currentDist < tmpDist)
        {
            double p = 100.0 * std::exp((tmpDist - currentDist) / modificatorTemperature);
            if (p > (rand() % 100))
            {
                currentDist = tmpDist;
                currentArr = tmpArr;
            }
        }
        else
        {
            currentDist = tmpDist;
            currentArr = tmpArr;
            if (fitnessFunction(resultArr) > fitnessFunction(currentArr))
            {
                resultArr = currentArr;
            }
        }
    }
}