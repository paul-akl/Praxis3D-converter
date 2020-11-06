#pragma once

#include <FreeImage.h>
#include <string>
#include <vector>

#include "EnumFactory.h"

// Texture file format
#define TEXTUREEXPORTFORMAT(Code) \
Code(tiff, = 0) \
Code(png,) \
Code(bmp,) \
Code(jpg,) \
Code(numOfTextureExportTypes, )
DECLARE_ENUM(TextureExportFormatType, TEXTUREEXPORTFORMAT)

// Layout of the texture color channels
enum class TextureFormat : unsigned int
{
	TextureFormat_Null = 0,
	TextureFormat_Red,
	TextureFormat_Green,
	TextureFormat_Blue,
	TextureFormat_Alpha,
	TextureFormat_RGB,
	TextureFormat_RGBA
};

// A single loaded texture
class Texture
{
	friend class TextureLoader;
public:
	~Texture()
	{

	}

private:
	// Make sure that the Texture can only be created by the TextureLoader
	Texture()
	{
		m_size = 0;
		m_pitch = 0;
		m_numChannels = 0;
		m_textureWidth = 0;
		m_pixelDataSize = 0;
		m_textureHeight = 0;
		m_bytesPerPixel = 0;
		m_samplesPerPixel = 0;

		m_greyscale = false;

		m_bitmap = nullptr;
		m_pixelData = nullptr;
		m_textureFormat = TextureFormat::TextureFormat_Null;
	}

	// Sets all internal values
	void setValues(unsigned int p_textureWidth, unsigned int p_textureHeight, unsigned int p_numberOfChannels)
	{
		m_size = p_textureWidth * p_textureHeight;
		m_pitch = p_textureWidth * p_numberOfChannels;
		m_numChannels = p_numberOfChannels;
		m_textureWidth = p_textureWidth;
		m_textureHeight = p_textureHeight;
		m_pixelDataSize = m_size * m_numChannels;
		m_bytesPerPixel = m_numChannels;
		m_samplesPerPixel = m_bytesPerPixel / sizeof(BYTE);
	}

	// Array index operator for pixelData. Wraps the index based on pixelData size
	const inline unsigned char &operator[](const unsigned int p_index) const { return m_pixelData[p_index % m_pixelDataSize]; }

	// Raw pixel data
	FIBITMAP* m_bitmap;
	unsigned char *m_pixelData;

	// Texture channel layout
	TextureFormat m_textureFormat;
	bool m_greyscale;

	unsigned int	m_size,					// Number of pixels
					m_pitch,				// Width of texture in bytes
					m_numChannels,			// Number of color channels per pixel
					m_textureWidth,			// Width of the texture
					m_textureHeight,		// Height of the texture
					m_pixelDataSize,		// Size of the pixelData array
					m_bytesPerPixel,		// Number of bytes per pixel
					m_samplesPerPixel;		// Number of samples per pixel
};

// Manages Textures; only the TextureLoader can instance Textures
// Maintains a list of all loaded textures and deletes them upon destruction
class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	// Saves the given texture to file; adds the file extension to the end of the filename based on the given texture format
	bool saveTexture(Texture *p_texture, std::string &p_filename, TextureExportFormatType p_format);

	// Loads a texture from the given filename
	// Returns the texture if successful; returns nullptr if unsuccessful
	Texture *loadTexture(std::string &p_filename, bool p_makeGreyscale = false); 

	// Loads the 4 given textures (by filename), saturates them (gets average of all channels), combines the 4 different channels from each given texture into a single texture
	// Replaces all invalid textures with black color; returns the texture if successful; returns nullptr if unsuccessful
	Texture *loadCombinedTexture(std::string& p_redChanel, std::string& p_greenChanel, std::string& p_blueChanel, std::string& p_alphaChanel);

private:
	// Saturates all channels of the texture
	void makeGreyscale(Texture& p_texture);

	// All successfully loaded textures
	std::vector<Texture*> m_textures;

	// Texture with black pixels, used instead of invalid given textures
	Texture m_blackTexture;
};

