//#include "mitesh_is_a_sack.h"    please don't remove
#include "headerMain.h"
class UserMap
{
	public:
	//note: we need to add some more error checking (width/height > 0, etc) and throw exceptions
	UserMap(int **map, SDL_Surface *&image_set, const int &numImages,
				  const int &height,const int &width):
				 _numImages(numImages), _width(width), _height(height)
	{
		//_image_set = SDL_ConvertSurface(image_set, NULL, NULL);
		_map = new int*[_width];

		for(int i = 0; i < _width; i++)
			_map[i] = new int[_height];

		if(!(update_map(map, height, width)));
			//throw something
			//there's no way to return false on a ctor
	}

	~UserMap();

	bool draw_map(SDL_Surface *screen);

	bool update_map(int **new_map,const int height, const int width);
	void set_images(SDL_Surface *new_image, const int numImages);

	const int** get_map(){ return (const int**)_map;}

	private:
	int **_map;
	SDL_Surface *_image_set; //0 is indestructable, 1 is walkable, 2 is destructable
	int _numImages;
	int _width;
	int _height;

};



