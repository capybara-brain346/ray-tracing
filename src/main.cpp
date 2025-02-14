#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <iostream>

using namespace std;

color ray_color(const ray &r)
{
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main()
{
    auto aspect_ratio = 16.0 / 9.0;
    int img_width = 400;
    int img_height = int(img_width / aspect_ratio);
    img_height = (img_height < 1) ? 1 : img_height;

    auto focal_lenght = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(img_width) / img_height);
    auto camera_center = point3(0, 0, 0);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(-1, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / img_width;
    auto pixel_delta_v = viewport_v / img_height;

    auto viewport_upper_left = camera_center - vec3(0, 0, focal_lenght) - viewport_u / 2 - viewport_v / 2;

    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    cout
        << "P3\n"
        << img_width << " " << img_height << "\n255\n";

    for (int j = 0; j < img_height; j++)
    {
        clog << "\rScanlines remaining: " << (img_height - j) << " " << flush;
        for (int i = 0; i < img_width; i++) // Fixed condition here
        {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(cout, pixel_color);
        }
    }
    clog << "\r Done.";
}