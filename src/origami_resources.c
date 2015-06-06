#include <pebble.h>
#include "origami_resources.h"

GDrawCommandImage *origami_zoo_get_animal(Origami animal)
{
	switch (animal) {
		case SNAIL:
			return gdraw_command_image_create_with_resource(RESOURCE_ID_ORIGAMI_SNAIL);
		default:
		case BUTTERFLY:
			return gdraw_command_image_create_with_resource(RESOURCE_ID_ORIGAMI_BUTTERFLY);
  }
}

GDrawCommandImage *origami_zoo_random_animal()
{
	srand (time(NULL));
	int num = rand()%2;

	if (num == 1)
	{
		return origami_zoo_get_animal(SNAIL);
	}
	else
	{
		return origami_zoo_get_animal(BUTTERFLY);
	}
}
