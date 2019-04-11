#ifdef SPRITE
#define SPRITE

#include "Includes.h"

class Sprite {
public:
	unsigned int textureId;
	float offsetX; 
	float offsetY;
	float z;


Sprite::Sprite(const char* filename, float paramOffsetX, float paramOffsetY, float paramZ)
{
	textureId = create_textures(filename);
	offsetX = paramOffsetX;
	offsetY = paramOffsetY;
	z = paramZ;
}

Sprite::~Sprite()
{
}

Sprite::Sprite() {
}

unsigned int create_textures(const char* filename) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//ou GL_REPEAT GL_CLAMP_TO_BORDER
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//ou GL_LINEAR GL_NEAREST
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int tex_width, tex_height;
	unsigned char *data = SOIL_load_image(filename, &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("%s", "Failed to load texture");
	}
	SOIL_free_image_data(data);
	return texture;
}


};

#endif // SPRITE