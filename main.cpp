#include "Core.h"
#include "BVH.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>

Color RayColor(const Ray& r, const Hittable& world, int Depth) 
{
    if (Depth <= 0)
    {
        return Color(0.0, 0.0, 0.0);
    }

    HitRecord rec;
    if (world.Hit(r, EPSILON, Inf, rec))
    {
        Ray Scattered;
        Color Attenuation;
        if (rec.MatPtr->Scatter(r, rec, Attenuation, Scattered))
        {
            return Attenuation * RayColor(Scattered, world, Depth - 1);
        }
        return Color(0.0, 0.0, 0.0);
    }

    // Render Background
    //return Color(0.0, 0.0, 0.0);

    Vec3 unit_direction = UnitVec(r.Direction());
    auto t = 0.5 * (unit_direction.Y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

void SpecificObject(HittableList& World)
{
    // Material

    auto MaterialGround = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto MaterialCenter = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto MaterialLeft = make_shared<Dielectric>(1.5);
    auto MaterialRight = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.2);

    // World
    
    World.Add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, MaterialCenter));
    World.Add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, MaterialGround));
    World.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, MaterialLeft));
    World.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.4, MaterialLeft));
    World.Add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, MaterialRight));
}

void RandomScene(HittableList& World)
{
    auto GroundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    World.Add(make_shared<Sphere>(Point3(0.0, -1000, 0.0), 1000.0, GroundMaterial));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto ChooseMat = RandomFloat();
            Point3 Center(a + 0.9 * RandomFloat(), 0.2, b + 0.9 * RandomFloat());

            if ((Center - Point3(4, 0.2, 0)).Length() > 0.9)
            {
                shared_ptr<Material> SphereMaterial;

                if (ChooseMat < 0.8)
                {
                    // Diffuse
                    auto Albedo = Color::Random() * Color::Random();
                    SphereMaterial = make_shared<Lambertian>(Albedo);
                    World.Add(make_shared<Sphere>(Center, 0.2, SphereMaterial));
                }
                else if (ChooseMat < 0.95)
                {
                    // Metal
                    auto Albedo = Color::Random(0.5, 1);
                    auto Fuzz = RandomFloat(0, 0.5);
                    SphereMaterial = make_shared<Metal>(Albedo, Fuzz);
                    World.Add(make_shared<Sphere>(Center, 0.2, SphereMaterial));
                }
                else
                {
                    // Glass
                    SphereMaterial = make_shared<Dielectric>(1.5);
                    World.Add(make_shared<Sphere>(Center, 0.2, SphereMaterial));
                }
            }
        }
    }

    auto Mat1 = make_shared<Dielectric>(1.5);
    World.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, Mat1));

    auto Mat2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    World.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, Mat2));

    auto Mat3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    World.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, Mat3));
}

int main() {

    // Timer

    using namespace std::chrono;
    FLOAT_TYPE delta_time;
    steady_clock::time_point TimeStart;
    steady_clock::time_point TimeEnd;
    duration<float> TimeSpan;

    // Bind cout to file

    std::ofstream file("Image.ppm");
    std::streambuf* x = std::cout.rdbuf(file.rdbuf());

    // World

    HittableList World;
    //SpecificObject(World);
    RandomScene(World);

    TimeStart = steady_clock::now();

    BvhNode BvhWorld(World);

    TimeEnd = steady_clock::now();
    TimeSpan = duration_cast<duration<float>>(TimeEnd - TimeStart);
    delta_time = TimeSpan.count();

    FLOAT_TYPE BvhBuildTime = delta_time;
      
    // Camera

    Point3 LookFrom(13, 2, 3);
    Point3 LookAt(0, 0, 0);
    Vec3 Vup(0, 1, 0);
    //FLOAT_TYPE DistToFocus = (LookFrom - LookAt).Length();
    FLOAT_TYPE DistToFocus = 10;
    FLOAT_TYPE Aperture = 0.1;

    //CameraFix Cam(90, 16.0 / 9.0);
    Camera Cam(LookFrom, LookAt, Vup, 20.f, 16.0 / 9.0, Aperture, DistToFocus);

    // Image

    const auto AspectRatio = Cam.GetAspectRatio();
    const int ImageWidth = 1280;
    const int ImageHeight = static_cast<int>(ImageWidth / AspectRatio);
    const int Spp = 32;
    const int MaxDepth = 16;
   
    // Render

    TimeStart = steady_clock::now();

    std::cout << "P3\n" << ImageWidth << " " << ImageHeight << "\n255\n";

    for (int j = ImageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < ImageWidth; ++i) {
            Color PixelColor(0.0, 0.0, 0.0);

            for (int s = 0; s < Spp; ++s)
            {
                auto u = (double(i) + RandomFloat()) / (ImageWidth - 1);
                auto v = (double(j) + RandomFloat()) / (ImageHeight - 1);
                Ray r = Cam.GetRay(u, v);
                PixelColor += RayColor(r, World, MaxDepth);
            }
            WriteColor(std::cout, PixelColor, Spp);

        }
    }

    TimeEnd = steady_clock::now();
    TimeSpan = duration_cast<duration<float>>(TimeEnd - TimeStart);
    delta_time = TimeSpan.count();

    FLOAT_TYPE RenderTime = delta_time;

    std::cerr << "\nDone.\n";

    std::cerr << "BVH Build Time:" << BvhBuildTime << "s, " << "Render Time : " << RenderTime << "s" << std::endl;
}