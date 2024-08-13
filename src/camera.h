#ifndef CAMERA_H
#define CAMERA_H

#include <execution>
#include <ctime>
#include <ratio>
#include <chrono>

#include "rt_main.h"
#include "hittable.h"
#include "material.h"

class camera{
public:
	double aspect_ratio = 1.0;
	int image_width = 1000;
	int samples_per_pixel = 10;
	int max_depth = 10;

	void render_row(int j){
		for(int i=0;i<image_width;i++){
			color pixel_color(0,0,0);
			for(int sample=0;sample<samples_per_pixel;sample++){
				ray r = get_ray(i,j);
				pixel_color += ray_color(r,max_depth,m_World);
			}
			Image[i*image_height + j] = (pixel_samples_scale*pixel_color);
			//write_color(std::cout,pixel_samples_scale*pixel_color);
		}
	}

	void render(hittable_list& world){
		using namespace std::chrono;

		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		initialize();
		m_World = world;
		std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
		std::vector<int> ridx(image_height);
		std::iota(ridx.begin(),ridx.end(),0);
		std::for_each(std::execution::par_unseq,ridx.begin(),ridx.end(),[&](int j)
			{render_row(j);}
		);

		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

		std::clog << "Total time " << time_span.count() << " seconds.\n";
		std::clog << "\rDone.                 \n";
	}

	void to_file(){
		for(int j=0;j<image_height;j++){
			for(int i=0;i<image_width;i++){
				write_color(std::cout,Image[i*image_height + j]);
			}
		}
	}

private:
	int image_height;
	point3 center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;


	double pixel_samples_scale;

	hittable_list m_World;

	std::vector<color> Image;

	void initialize(){
		int img_h = int(image_width/aspect_ratio);
		image_height = (img_h < 1) ? 1 : img_h;
		Image.resize(image_width*image_height);
		//Camera
		center = point3(0,0,0);

		pixel_samples_scale = 1.0 / samples_per_pixel;

		auto focal_length = 1.0;
		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (double(image_width)/image_height);

		auto viewport_u = vec3(viewport_width,0,0);
		auto viewport_v = vec3(0,-viewport_height,0);

		pixel_delta_u = viewport_u/image_width;
		pixel_delta_v = viewport_v/image_height;

		auto viewport_upper_left = center- vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	vec3 sample_square() const{
		return vec3(random_double() - 0.5, random_double()- 0.5, 0);
	}

	ray get_ray(int i, int j) const {
		auto offset = sample_square();
		auto pixel_sample = pixel00_loc + ((i+offset.x())*pixel_delta_u) + ((j+offset.y()) * pixel_delta_v);
		auto ray_origin = center;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	color ray_color(const ray& r, int depth,const hittable& world) const{
		if(depth <= 0)
			return color(0,0,0);
		hit_record rec;
		if(world.hit(r,interval(0.001,infinity),rec)){
			ray scattered;
			color attenuation;
			if(rec.mat->scatter(r,rec,attenuation,scattered))
				return attenuation * ray_color(scattered,depth-1,world);
			return color(0,0,0);
		}
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5*(unit_direction.y()+1.0);
		return (1.0-a)*color(1.0,1.0,1.0) + a* color(0.5,0.7,1.0); //lerp
	}
};

#endif