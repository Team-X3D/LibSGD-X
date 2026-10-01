Images
======

Images are rectangular 2d textures that can be drawn to the 2D overlay with :any:`sgd_Draw2DImage`, or used to create :any:`Sprites` with :any:`sgd_CreateSprite`. Images can have multiple animation frames, although each frame must have the same dimensions.

To load a simple single frame image, use :any:`sgd_LoadImage`.

To load a multiframe image, use :any:`sgd_LoadArrayImage`. Image frames must be laid out in a grid of equisized cells, with an optional gap on the right/bottom edge of each cell. See the parameters of :any:`sgd_LoadArrayImage` for more information.


Types
-----

.. doxygengroup:: ImageTypes
    :content-only:

Functions
---------

.. doxygengroup:: Image
    :content-only:
