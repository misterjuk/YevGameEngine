#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_plot.h"
#include <numeric>

struct Transform
{
    float matrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 };
};

class GameObject3D
{
public:
    Transform transform;
    int ID;
};

class GameObject3DAlt
{
public:
    int ID;
    Transform transform;
};

std::vector<float> stepSizes;
std::vector<float> timingsGameObject3D;
std::vector<float> timingsGameObject3DAlt;
bool runBenchmark = false;
int numSamples = 10;  // Default number of samples

void RunBenchmarkGameObject3D()
{
    const int experiments = numSamples; // Use the number of samples defined by the user
    const int arr_length = 5000000;
    std::vector<GameObject3D> array3(arr_length); // Using vector

    stepSizes.clear();
    timingsGameObject3D.clear();

    std::cout << "GameObject3D Results:\n";

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        std::vector<long long> timingResults;

        for (int exp = 0; exp < experiments; ++exp)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < arr_length; i += stepsize)
            {
                array3[i].ID *= 2; // Operation on ID
            }

            auto end = std::chrono::high_resolution_clock::now();
            timingResults.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        std::sort(timingResults.begin(), timingResults.end());
        timingResults.erase(timingResults.begin());
        timingResults.pop_back();

        long long total_time = std::accumulate(timingResults.begin(), timingResults.end(), 0LL);
        float averageTime = static_cast<float>(total_time) / timingResults.size();
        stepSizes.push_back(static_cast<float>(stepsize));
        timingsGameObject3D.push_back(averageTime);

        // Print to console
        std::cout << stepsize << "\t" << averageTime << " microseconds\n";
    }
}

void RunBenchmarkGameObject3DAlt()
{
    const int experiments = numSamples;
    const int arr_length = 5000000;
    std::vector<GameObject3DAlt> array3(arr_length); // Using vector

    timingsGameObject3DAlt.clear();

    std::cout << "GameObject3DAlt Results:\n";

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        std::vector<long long> timingResults;

        for (int exp = 0; exp < experiments; ++exp)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < arr_length; i += stepsize)
            {
                array3[i].ID *= 2; // Operation on ID
            }

            auto end = std::chrono::high_resolution_clock::now();
            timingResults.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        std::sort(timingResults.begin(), timingResults.end());
        timingResults.erase(timingResults.begin());
        timingResults.pop_back();

        long long total_time = std::accumulate(timingResults.begin(), timingResults.end(), 0LL);
        float averageTime = static_cast<float>(total_time) / timingResults.size();
        timingsGameObject3DAlt.push_back(averageTime);

        // Print to console
        std::cout << stepsize << "\t" << averageTime << " microseconds\n";
    }
}

void RenderExercise2UI()
{
    ImGui::Begin("Exercise 2");

    if (ImGui::Button("Run Benchmark")) {
        runBenchmark = true;
    }

    // Input box for number of samples
    ImGui::InputInt("Number of Samples", &numSamples, 1, 10, ImGuiInputTextFlags_CharsDecimal);
    numSamples = std::clamp(numSamples, 5, 100);

    if (runBenchmark) {
        RunBenchmarkGameObject3D();
        RunBenchmarkGameObject3DAlt();
        runBenchmark = false;
    }

    if (!timingsGameObject3D.empty() && !timingsGameObject3DAlt.empty()) {
        // **Find the max Y value across both**
        float maxTime = std::max(*std::max_element(timingsGameObject3D.begin(), timingsGameObject3D.end()),
            *std::max_element(timingsGameObject3DAlt.begin(), timingsGameObject3DAlt.end()));

        // **Plot for GameObject3D**
        ImGui::PlotConfig conf;
        conf.values.xs = stepSizes.data();
        conf.values.ys = timingsGameObject3D.data();
        conf.values.count = static_cast<int>(stepSizes.size());
        conf.scale.min = 0;
        conf.scale.max = maxTime; // **Set max scale**
        conf.tooltip.show = true;
        conf.tooltip.format = "Step Size=%.2f, Time=%.2f us";
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(250, 140);
        conf.line_thickness = 2.f;
        ImGui::Plot("GameObject3D Step Size vs Time", conf);

        // **Plot for GameObject3DAlt**
        conf.values.ys = timingsGameObject3DAlt.data();
        ImGui::Plot("GameObject3DAlt Step Size vs Time", conf);

        // Define an array of pointers to float arrays
        const float* ys_list[] = { timingsGameObject3D.data(), timingsGameObject3DAlt.data() };

        ImGui::PlotConfig confCombined;
        confCombined.values.xs = stepSizes.data();
        confCombined.values.ys_list = ys_list;  // Fix: use the array of pointers
        confCombined.values.ys_count = 2;  // Two lines
        confCombined.values.count = static_cast<int>(stepSizes.size());
        confCombined.scale.min = 0;
        confCombined.scale.max = maxTime; // Set the max scale for both graphs
        confCombined.tooltip.show = true;
        confCombined.tooltip.format = "Step Size=%.2f, Time=%.2f us";
        confCombined.grid_x.show = true;
        confCombined.grid_y.show = true;
        confCombined.frame_size = ImVec2(250, 140);
        confCombined.line_thickness = 2.f;

        ImGui::Plot("Combined Step Size vs Time", confCombined);
    }

    ImGui::End();
}


// Buffer size: 2^26 integers (huge buffer)
const int BUFFER_SIZE = 1 << 26;

std::vector<float> stepSizes1;
std::vector<float> timings1;
bool runBenchmark1 = false;
int numSamples1 = 10;  // Default number of samples

// Function to perform benchmarking (runs only when triggered)
void RunBenchmarkExercise1()
{
    std::vector<int> buffer(BUFFER_SIZE, 1); // Allocate buffer & initialize to 1
    stepSizes1.clear();
    timings1.clear();

    std::cout << "Exercise 1 Results:\n";

    for (int step = 1; step <= 1024; step *= 2)
    {
        std::vector<long long> timingResults;

        for (int exp = 0; exp < numSamples1; ++exp)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < BUFFER_SIZE; i += step)
            {
                buffer[i] *= 2; // Simple operation
            }

            auto end = std::chrono::high_resolution_clock::now();
            timingResults.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        std::sort(timingResults.begin(), timingResults.end());
        timingResults.erase(timingResults.begin());
        timingResults.pop_back();

        long long total_time = std::accumulate(timingResults.begin(), timingResults.end(), 0LL);
        float averageTime = static_cast<float>(total_time) / timingResults.size();
        stepSizes1.push_back(static_cast<float>(step));
        timings1.push_back(averageTime);

        std::cout << "Step size: " << step << " - Time: " << averageTime << " microseconds\n";
    }
}

// ImGui window for Exercise 1
void RenderExercise1UI()
{
    ImGui::Begin("Exercise 1");

    if (ImGui::Button("Run Benchmark"))
    {
        runBenchmark1 = true;
    }

    // Input box for number of samples
    ImGui::InputInt("Number of Samples", &numSamples1, 1, 10, ImGuiInputTextFlags_CharsDecimal);
    numSamples1 = std::clamp(numSamples1, 5, 100);

    if (runBenchmark1)
    {
        RunBenchmarkExercise1();
        runBenchmark1 = false;
    }

    if (!timings1.empty())
    {
        float maxTime = *std::max_element(timings1.begin(), timings1.end());

        ImGui::PlotConfig conf;
        conf.values.xs = stepSizes1.data();
        conf.values.ys = timings1.data();
        conf.values.count = static_cast<int>(stepSizes1.size());
        conf.scale.min = 0;
        conf.scale.max = maxTime;
        conf.tooltip.show = true;
        conf.tooltip.format = "Step Size=%.2f, Time=%.2f us";
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(250, 140);
        conf.line_thickness = 2.f;

        ImGui::Plot("Step Size vs Time (Exercise 1)", conf);
    }

    ImGui::End();
}
