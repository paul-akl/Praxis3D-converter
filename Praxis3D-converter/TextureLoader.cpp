
#include "TextureLoader.h"

DEFINE_ENUM(TextureExportFormatType, TEXTUREEXPORTFORMAT)

TextureLoader::TextureLoader()
{
	// Set the black texture
	m_blackTexture.setValues(100, 100, 4);
	m_blackTexture.m_pixelData = new unsigned char[m_blackTexture.m_pixelDataSize];
	for(decltype(m_blackTexture.m_size) i = 0; i < m_blackTexture.m_pixelDataSize; i++)
		m_blackTexture.m_pixelData[i] = 0;
}

TextureLoader::~TextureLoader()
{
	// Make sure to clean up all the textures (if they haven't been deleted already)
	for(decltype(m_textures.size()) size = m_textures.size(), i = 0; i < size; i++)
		if(m_textures[i] != nullptr)
			delete m_textures[i];
	// Lionginas Virbalas
}

bool TextureLoader::saveTexture(Texture *p_texture, std::string& p_filename, TextureExportFormatType p_format)
{
	// Check if texture pointer and filename are valid
	if(p_texture != nullptr && !p_filename.empty())
	{		
		// Convert texture to a bitmap
		p_texture->m_bitmap = FreeImage_ConvertFromRawBits(	p_texture->m_pixelData, 
															p_texture->m_textureWidth, 
															p_texture->m_textureHeight, 
															p_texture->m_pitch, 
															p_texture->m_bytesPerPixel * 8, 
															FI_RGBA_RED_MASK,
															FI_RGBA_GREEN_MASK,
															FI_RGBA_BLUE_MASK,
															FALSE);

		// Assign the appropriate Free Image texture format
		FREE_IMAGE_FORMAT imageFormat;
		switch(p_format)
		{
		case tiff:
			imageFormat = FREE_IMAGE_FORMAT::FIF_TIFF;
			break;
		case png:
			imageFormat = FREE_IMAGE_FORMAT::FIF_PNG;
			break;
		case bmp:
			imageFormat = FREE_IMAGE_FORMAT::FIF_BMP;
			break;
		case jpg:
			imageFormat = FREE_IMAGE_FORMAT::FIF_JPEG;
			break;
		}

		// Save the texture to file; add an appropriate file extension to the filename
		return (FreeImage_Save(imageFormat, p_texture->m_bitmap, (p_filename + '.' + GetString(p_format)).c_str()) != 0);
	}

	return false;
}

Texture *TextureLoader::loadTexture(std::string& p_filename, bool p_makeGreyscale)
{
	Texture* returnTexture = nullptr;

	// Check if the filename is valid
	if(!p_filename.empty())
	{
		returnTexture = new Texture();

		// Read the format of the texture
		FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType((p_filename).c_str(), 0);
		
		// Read the actual texture
		returnTexture->m_bitmap = FreeImage_Load(imageFormat, (p_filename).c_str());

		// Check if the texture was read form file successfully 
		if(returnTexture->m_bitmap)
		{
			// Save the created texture in a vector
			m_textures.push_back(returnTexture);

			returnTexture->m_pitch = FreeImage_GetPitch(returnTexture->m_bitmap);

			// Calculate the number of bytes per pixel
			returnTexture->m_bytesPerPixel = FreeImage_GetLine(returnTexture->m_bitmap) / FreeImage_GetWidth(returnTexture->m_bitmap);
			// Calculate the number of samples per pixel
			returnTexture->m_samplesPerPixel = returnTexture->m_bytesPerPixel / sizeof(BYTE);

			// Only supporting 24bits or 32bits per pixel
			if(returnTexture->m_samplesPerPixel == 3)
			{
				returnTexture->m_textureFormat = TextureFormat::TextureFormat_RGB;
				returnTexture->m_bitmap = FreeImage_ConvertTo24Bits(returnTexture->m_bitmap);
			}
			else
			{
				returnTexture->m_textureFormat = TextureFormat::TextureFormat_RGBA;
				returnTexture->m_bitmap = FreeImage_ConvertTo32Bits(returnTexture->m_bitmap);
			}

			// Get texture width, height and size
			returnTexture->m_textureWidth = FreeImage_GetWidth(returnTexture->m_bitmap);
			returnTexture->m_textureHeight = FreeImage_GetHeight(returnTexture->m_bitmap);
			returnTexture->m_size = returnTexture->m_textureWidth * returnTexture->m_textureHeight;
			returnTexture->m_pixelDataSize = returnTexture->m_size * returnTexture->m_samplesPerPixel;

			// Texture data passed to the GPU must be in an unsigned char array format
			returnTexture->m_pixelData = (unsigned char*)FreeImage_GetBits(returnTexture->m_bitmap);

			// Temp variable for swapping color channels
			unsigned char blue = 0;

			// Define number of channels per pixel
			returnTexture->m_numChannels = returnTexture->m_textureFormat == TextureFormat::TextureFormat_RGB ? 3 : 4;

			if(p_makeGreyscale)
			{
				makeGreyscale(*returnTexture);
			}
			else
			{
				// FreeImage loads in BGR format, therefore swap of bytes is needed (Or usage of GL_BGR)
				/*for(unsigned int i = 0; i < returnTexture->m_size; i++)
				{
					blue = returnTexture->m_pixelData[i * returnTexture->m_numChannels + 0];																// Store blue
					returnTexture->m_pixelData[i * returnTexture->m_numChannels + 0] = returnTexture->m_pixelData[i * returnTexture->m_numChannels + 2];	// Set red
					returnTexture->m_pixelData[i * returnTexture->m_numChannels + 2] = blue;																// Set blue
				}*/
			}
		}
		else
		{
			// If the texture loading failed, delete the newly created texture and return a null pointer
			delete returnTexture;
			returnTexture = nullptr;
		}

	}

	return returnTexture;
}

Texture *TextureLoader::loadCombinedTexture(std::string &p_redChanel, std::string &p_greenChanel, std::string &p_blueChanel, std::string &p_alphaChanel)
{
	Texture *returnTexture = nullptr;

	// Load the textures for each color channel
	Texture *redChannelTexture	 = loadTexture(p_redChanel),
			*greenChannelTexture = loadTexture(p_greenChanel),
			*blueChannelTexture  = loadTexture(p_blueChanel),
			*alphaChannelTexture = loadTexture(p_alphaChanel);

	// Check if the textures were loaded; if they are, saturate the colors; if they are not, set them to black
	if(redChannelTexture == nullptr)
		redChannelTexture = &m_blackTexture;
	else
		makeGreyscale(*redChannelTexture);

	if(greenChannelTexture == nullptr)
		greenChannelTexture = &m_blackTexture;
	else
		makeGreyscale(*greenChannelTexture);

	if(blueChannelTexture == nullptr)
		blueChannelTexture = &m_blackTexture;
	else
		makeGreyscale(*blueChannelTexture);

	if(alphaChannelTexture == nullptr)
		alphaChannelTexture = &m_blackTexture;
	else
		makeGreyscale(*alphaChannelTexture);

	// Get the number of pixels from the texture with the most pixels
	unsigned int pixelSize = redChannelTexture->m_size;
	Texture *biggestTexture = redChannelTexture;
	if(greenChannelTexture->m_size > pixelSize)
	{
		pixelSize = greenChannelTexture->m_size;
		biggestTexture = greenChannelTexture;
	}
	if(blueChannelTexture->m_size > pixelSize)
	{
		pixelSize = blueChannelTexture->m_size;
		biggestTexture = blueChannelTexture;
	}
	if(alphaChannelTexture->m_size > pixelSize)
	{
		pixelSize = alphaChannelTexture->m_size;
		biggestTexture = alphaChannelTexture;
	}

	// Define samples per pixel for each channel texture
	const unsigned int	redSamplesPerPixel = redChannelTexture->m_samplesPerPixel,
						greenSamplesPerPixel = greenChannelTexture->m_samplesPerPixel,
						blueSamplesPerPixel = blueChannelTexture->m_samplesPerPixel,
						alphaSamplesPerPixel = alphaChannelTexture->m_samplesPerPixel;

	// Define the pixel data of the combined texture
	unsigned char *pixelData = new unsigned char[static_cast<std::size_t>(pixelSize) * 4];

	// Go over each pixel and assign them to the combined texture
	for(unsigned int i = 0; i < pixelSize; i++)
	{
		pixelData[i * 4 + 0] = (*redChannelTexture)[i * redSamplesPerPixel];
		pixelData[i * 4 + 1] = (*greenChannelTexture)[i * greenSamplesPerPixel];
		pixelData[i * 4 + 2] = (*blueChannelTexture)[i * blueSamplesPerPixel];
		pixelData[i * 4 + 3] = (*alphaChannelTexture)[i * alphaSamplesPerPixel];

		//pixelData[i * 4 + 0] = (*redChannelTexture)[i * redSamplesPerPixel];
		//pixelData[i * 4 + 1] = greenChannelTexture->m_pixelData[i * greenSamplesPerPixel];
		//pixelData[i * 4 + 2] = (*blueChannelTexture)[i * blueSamplesPerPixel];
		//pixelData[i * 4 + 3] = (*alphaChannelTexture)[i * alphaSamplesPerPixel];
	}

	// Construct the new texture
	returnTexture = new Texture();
	returnTexture->setValues(biggestTexture->m_textureWidth, biggestTexture->m_textureHeight, 4);
	returnTexture->m_pixelData = pixelData;


	//returnTexture->m_textureHeight = biggestTexture->m_textureHeight;
	//returnTexture->m_textureWidth = biggestTexture->m_textureWidth;
	//returnTexture->m_pitch = returnTexture->m_textureWidth * 4;
	//returnTexture->m_pixelData = pixelData;
	//returnTexture->m_samplesPerPixel =  4;
	//returnTexture->m_bytesPerPixel = returnTexture->m_samplesPerPixel * sizeof(unsigned char);
	//biggestTexture->m_pixelData = pixelData;

	// Save the new texture in the texture array
	m_textures.push_back(returnTexture);

	return returnTexture;
}

void TextureLoader::makeGreyscale(Texture& p_texture)
{
	// Check if the texture is 24bits or 32bits per pixel (RGB or RGBA)
	switch(p_texture.m_textureFormat)
	{
	case TextureFormat::TextureFormat_RGB :
	{
		// Go over each pixel
		for(unsigned int i = 0; i < p_texture.m_size; i++)
		{
			// Average the color between 3 colors (RGB)
			const unsigned char averageColor = (p_texture.m_pixelData[i * p_texture.m_numChannels + 0] / 3.0) +
				(p_texture.m_pixelData[i * p_texture.m_numChannels + 1] / 3.0) +
				(p_texture.m_pixelData[i * p_texture.m_numChannels + 2] / 3.0);

			// Set each color to the average
			p_texture.m_pixelData[i * p_texture.m_numChannels + 0] = averageColor;
			p_texture.m_pixelData[i * p_texture.m_numChannels + 1] = averageColor;
			p_texture.m_pixelData[i * p_texture.m_numChannels + 2] = averageColor;
		}

		// Set greyscale flag
		p_texture.m_greyscale = true;
	}
		break;

	case TextureFormat::TextureFormat_RGBA :
	{
		// Go over each pixel
		for(unsigned int i = 0; i < p_texture.m_size; i++)
		{
			// Average the color between 4 colors (RGBA)
			const unsigned char averageColor = (p_texture.m_pixelData[i * p_texture.m_numChannels + 0] / 4.0) +
				(p_texture.m_pixelData[i * p_texture.m_numChannels + 1] / 4.0) +
				(p_texture.m_pixelData[i * p_texture.m_numChannels + 2] / 4.0) +
				(p_texture.m_pixelData[i * p_texture.m_numChannels + 3] / 4.0);
			
			// Set each color to the average
			p_texture.m_pixelData[i * p_texture.m_numChannels + 0] = averageColor;
			p_texture.m_pixelData[i * p_texture.m_numChannels + 1] = averageColor;
			p_texture.m_pixelData[i * p_texture.m_numChannels + 2] = averageColor;
			p_texture.m_pixelData[i * p_texture.m_numChannels + 3] = averageColor;
		}
		
		// Set greyscale flag
		p_texture.m_greyscale = true;
	}
		break;
	}
}
