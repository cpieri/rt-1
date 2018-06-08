#include "rt.h"
#include <math.h>

static double	get_intensity(t_intersect inter, t_light light, t_data data)
{
	t_vector3d	lightdir;
	double		intensity;

	lightdir = vector_op(light.origin, inter.point, '-');
	intensity = dotprod(normalize(lightdir), normalize(inter.normal));
	intensity = intensity <= 0 ? data.ambiantlight :
		intensity * light.intensity + data.ambiantlight;
	intensity = intensity >= 1 ? 1 : intensity;
	return (intensity);
}

static int		fuse(int start, int finish, int tresh)
{
	t_color	a;
	t_color	b;
	t_color	threshold;

	a = int_to_color(start);
	b = int_to_color(finish);
	threshold = int_to_color(tresh);
	a.r = a.r + (b.r > threshold.r ? threshold.r : b.r);
	a.g = a.g + (b.g > threshold.g ? threshold.g : b.g);
	a.b = a.b + (b.b > threshold.b ? threshold.r : b.b);
	return (color_to_int(a));
}

static int		brillance(int start, t_intersect inter, t_light light)
{
	t_color		a;
	t_color		b;
	t_vector3d	lightdir;
	double		intensity;
	t_vector3d	reflect;

	lightdir = vector_op(light.origin, inter.point, '-');
	intensity = 2 * dotprod(inter.normal, lightdir);
	reflect = vector_op(new_vector_3d_unicoord(intensity), inter.normal, '*');
	reflect = vector_op(reflect, lightdir, '-');
	intensity = dotprod(reflect,
			vector_op(inter.dir_to_cam, new_vector_3d_unicoord(-1), '*')) - 0.5;
	intensity = intensity < 0 ? 0 : intensity;
	intensity = pow(intensity, 14 / (get_length(lightdir) + 0.1));
	intensity *= inter.shape_copy.brillance * light.intensity;
	intensity = ftb_clamp(intensity, 0, 1);
	a = int_to_color(start);
	b = int_to_color(light.color);
	a.r += b.r * intensity;
	a.g += b.g * intensity;
	a.b += b.b * intensity;
	return (color_to_int(a));
}

static int			shadows(t_scene scene, t_intersect *inter, t_light light)
{
	t_ray		ray;
	t_vector3d	dir;
	t_vector3d	rayorigin;
	double		dist;

	rayorigin = vector_op(inter->normal, new_vector_3d_unicoord(0.00001), '*');
	rayorigin = vector_op(inter->point, rayorigin, '+');
	dir = vector_op(light.origin, rayorigin, '-');
	ray = new_ray(rayorigin, normalize(dir));
	dist = get_length(dir);
	return (get_nearest_intersection(&ray, scene, inter, dist));
}

int				set_color(t_scene scene,
		t_intersect intersection, t_data data)
{
	int			ret;
	int			tmp;
	double		intensity;
	t_intersect	shadowinter;
	t_light		light;

	ret = 0;
	while (scene.light_lst != NULL)
	{
		shadowinter = intersection;
		light = *(t_light*)scene.light_lst->content;
		if (light.color == -1)
			break ;
		intensity = get_intensity(intersection, light, data);
		tmp = interpolate(0, intersection.shape_copy.color, intensity);
		if (!shadows(scene, &shadowinter, light))
			tmp = brillance(tmp, intersection, light);
		else
			tmp = interpolate(0, tmp,
	ftb_clamp(data.ambiantlight + 1 - shadowinter.shape_copy.opacity, 0, 1));
		ret = fuse(ret, tmp, light.color);
		scene.light_lst = scene.light_lst->next;
	}
	return (ret);
}
