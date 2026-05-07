//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <algorithm>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        if (!image_data.empty())
        {
            // --- old version (kept for reference) ---
            // cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);

            // OpenCV loads images as BGR by default. Convert to RGB so shaders treat
            // sampled colors as (R,G,B) consistently.
            cv::cvtColor(image_data, image_data, cv::COLOR_BGR2RGB);
            width = image_data.cols;
            height = image_data.rows;
        }
        else
        {
            width = 0;
            height = 0;
        }
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        if (image_data.empty() || width <= 0 || height <= 0)
        {
            return Eigen::Vector3f(0, 0, 0);
        }

        // --- old version (kept for reference; can go out-of-bounds) ---
        // auto u_img = u * width;
        // auto v_img = (1 - v) * height;
        // auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        // return Eigen::Vector3f(color[0], color[1], color[2]);

        // Clamp UVs to [0,1] to avoid out-of-bounds access from interpolation.
        u = std::clamp(u, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);

        // Map to pixel coordinates. Use width-1/height-1 so u=1,v=0 stay in range.
        const int x = std::clamp(static_cast<int>(u * (width - 1)), 0, width - 1);
        const int y = std::clamp(static_cast<int>((1.0f - v) * (height - 1)), 0, height - 1);

        const auto color = image_data.at<cv::Vec3b>(y, x);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

};
#endif //RASTERIZER_TEXTURE_H
