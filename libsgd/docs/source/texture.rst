Textures
========

LibSGD supports 2d textures, cube textures and array textures.

All textures have a :any:`SGD_TextureFormat` property that describes the format of the indivdual 'texels' in the texture, and a :any:`SGD_TextureFlags` property that describes how the texture is filtered and addressed when used for rendering.


2D Textures
-----------

A 2D texture is a simple rectangular texture.

2D textures are usually used with materials to alter the appearance of meshes, planes and terrains.

You can load a 2D texture from an image file using :any:`sgd_Load2DTexture`. LibSGD supports the following image file formats:

.. code-block::

    JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
    PNG 1/2/4/8/16-bit-per-channel
    TGA (not sure what subset, if a subset)
    BMP non-1bpp, non-RLE
    PSD (composited view only, no extra channels, 8/16 bit-per-channel)
    GIF (*comp always reports as 4-channel)
    HDR (radiance rgbE format)
    PIC (Softimage PIC)
    PNM (PPM and PGM binary only)
    EXR (OpenEXR high dynamic range format)


Cube Textures
-------------

A cube texture, or cubemap, is a special type of texture used to create skyboxes and environment maps. Instead of being a single flat image, a cube texture consists of six square images, each representing one side of a cube (left, right, top, bottom, front, and back). Together, these six images form a 360-degree view, with the viewer located at the center.

The faces of a cube texture must always be square.

You can load a cube texture from an image file using :any:`sgd_LoadCubeTexture`. Cube texture images must be laid out in a specific way:

.. image:: cubemap_layout.png


Array Textures
--------------

An array texture allows you to store multiple 2D textures in a single texture object. Instead of having separate texture objects for each image, an array texture organizes several textures into a single array, which can be accessed in shaders.

Array textures are used to create images, which can be used to create sprites or drawn to 2D overlays.

You can load an array Texture from an image file using the :any:`sgd_LoadArrayTexture`. Array texture image 'frames' must be laid out in a grid in equisized cells, with an optional gap on the right/bottom edge of each cell.


Types
-----

.. doxygengroup:: TextureTypes
    :content-only:


Functions
---------

.. doxygengroup:: Texture
    :content-only:
