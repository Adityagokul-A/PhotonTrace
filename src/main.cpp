#include "rt_main.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
//Refer ray-tracing in a weekend	

int main(){
	hittable_list world;

	
	//Scene 1
	auto mat_ground = make_shared<lambertian>(color(0.8,0.8,0.8));
	auto mat_right = make_shared<metal>(color(0.9,0.9,0.168),1.0);
	auto mat_left = make_shared<metal>(color(0.9,0.2,0.168),1.0);
	auto mat_up = make_shared<metal>(color(0.1,0.2,0.9),1.0);
	auto mat_diel = make_shared<dielectric>(1.50);
	auto mat_diel2 = make_shared<dielectric>(1.00/1.50);

	world.add(make_shared<sphere>(point3(0,-100.5,-1),100,mat_ground));
	world.add(make_shared<sphere>(point3(0.5,0,-1),0.25,mat_right));
	world.add(make_shared<sphere>(point3(0.0,0,-1),0.25,mat_up));
	world.add(make_shared<sphere>(point3(-0.5,0,-1),0.25,mat_left));
	

	//Scene 2
	/*auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }*/


	camera cam;
	cam.image_width = 1000;
	cam.aspect_ratio = 16.0/9.0;
	cam.samples_per_pixel = 500;
	cam.max_depth = 20;

	cam.render(world);
	cam.to_file();

	return 0;
}