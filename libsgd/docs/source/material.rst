Materials
=========

A material is a set of properties that define how a 3D object looks when it’s rendered. You can think of it as the "skin" of an object. Materials may be used when creating meshes, planes and terrains.

The are several types of material available, including simple "emissive" materials that represent surfaces that emit their own light regardless of scene lighting, and more sophisticated "pbr" materials for more realistic 'physically based rendering' surfaces that interact with lights in the scene.

All material types have the following properties in common:

* Blend mode

A material's blend mode controls how the material is combined with existing pixels, see :any:`SGD_BlendMode`.

* Depth function

A material's depth function controls how the material is depth sorted against existing geometry, see :any:`SGD_DepthFunc`.

* Cull mode

A material's cull mode controls whether back facing, front facing or bothback and front facing trianles are rendered, see :any:`SGD_CullMode`.


Custom materials
----------------

Each material type may also have a number of custom properties which may be set using one of :any:`sgd_SetMaterialTexture`, :any:`sgd_SetMaterialColor` and :any:`sgd_SetMaterialFloat`.

You can also specify the properties of a material using a special JSON based file format. The file should be named 'material.sgd' and placed in the directory that also contains the texture images used by the material.

The file should consist of a single JSON object that contains the following keys:

.. list-table:: SGD Material JSON format
   :widths: 25 25 50
   :header-rows: 1

   * - Key
     - Value
     - Description
   * - "class"
     - String equal to "Material"
     - Specifies this is an SGD material object.
   * - "type"
     - String equal to "emissive" or "pbr"
     - Specifies the material type. Must be present.
   * - "blendMode"
     - Number equal to a valid :any:`SGD_BlendMode`
     - Specifies material blend mode, defaults to SGD_BLEND_MODE_OPAQUE.
   * - "depthFunc"
     - Number equal to a valid :any:`SGD_DepthFunc`
     - Specifies material depth function, defaults to SGD_DEPTH_FUNC_LESS_EQUAL.
   * - "cullMode"
     - Number equal to a valid :any:`SGD_CullMode`
     - Specifies material cull mode, defaults to SGD_CULL_MODE_BACK.
   * - "textures"
     - Object
     - Specifies the textures use by the material.
   * - "colors"
     - Object
     - Specifies the colors used by the material.
   * - "floats"
     - Object
     - Specifies the floats used by the material.

Each key/value pair in the "textures", "colors" and "floats" objects are used to initialize custom material properties. The values of the "textures" object must be strings that contain the relative path of the textures to load. The values of the "colors" object must be 4 element arrays of numbers and the values of the "floats" object must be simple number values.

For example , here is the JSON material.sgd for a PBR brick material:

.. code-block:: JSON

    {
        "class": "Material",
        "type": "pbr",
        "textures": {
            "albedo": "Bricks076C_1K-JPG_Color.jpg",
            "roughness": "Bricks076C_1K-JPG_Roughness.jpg",
            "occlusion": "Bricks076C_1K-JPG_AmbientOcclusion.jpg",
            "normal": "Bricks076C_1K-JPG_NormalGL.jpg"
        }
    }

The material only specifies textures, so colors will be left to their default values for the given material type ("pbr" in this case).

The directory structure for this file and it's texture should be like this:

.. code-block:: BASH

    Bricks076C_1K-JPG/
        material.sgd
        Bricks076C_1K-JPG_Color.jpg
        Bricks076C_1K-JPG_Roughness.jpg
        Bricks076C_1K-JPG_AmbientOcclusion.jpg
        Bricks076C_1K-JPG_NormalGL.jpg

To load this material, you just need to pass the path to the top level directorty to sgd_LoadMaterial().


Emissive Materials
------------------

Emissive materials are the simplest type of material you can use. Emissive materials support a custom "emissive" texture
property and a custom "emissive" color property which are multiplied together to produce a final surface color. Emissive materials are unaffected by scene lighting. Both emissive texture and color default to white.

To create an 'empty' emissive material use :any:`sgd_CreateEmissiveMaterial`. Once created, it is up to you to assign a texture and/or color using using :any:`sgd_SetMaterialTexture` and :any:`sgd_SetMaterialColor` with the "emissive" property name.

To load an emissive material from an image file file use :any:`sgd_LoadEmissiveMaterial`. This is a simple helper function that loads a 2d texture using :any:`sgd_Load2DTexture` and assigns it to the "emissive" texture property of a new emissive material. If you require more control than this, eg: you want to specify texture format or texture flags, you will have to create a material as above and assign textures manually.


PBR Materials
-------------

PBR (physically based rendering) materials are the most physically accurate type of material available, but also the slowest to render.

To create an 'empty' PBR material use :any:`sgd_CreatePBRMaterial`. Once created, it is up to you to assign textures, colors and floats to the material using :any:`sgd_SetMaterialTexture`, :any:`sgd_SetMaterialColor` and :any:`sgd_SetMaterialFloat`.

To load a PBR material from an image file file use :any:`sgd_LoadPBRMaterial`. This is a simple helper function that loads a 2d texture using :any:`sgd_Load2DTexture` and assigns it to the "albedo" texture property of a new PBR material. If you require more control than this, eg: you want to specify texture format or texture flags, you will have to create a material as above and assign textures manually.

The custom material properties for PBR materials are:

.. list-table:: PBR Material Parameters
   :widths: 15 20 40 25
   :header-rows: 1

   * - Parameter
     - Types(s)
     - Description
     - Default value
   * - "albedo"
     - 2d texture, color
     - Base color used for PBR lighting.
     - White texture/white color.
   * - "emissive"
     - 2d texture, color
     - Emissive color added after PBR lighting.
     - White texture/black color.
   * - "metalness"
     - 2d texture, float
     - Metalness value used for lighting. The final metalness value is read from the texture blue component and scaled by the metalness float value.
     - White texture/0 scale.
   * - "roughness"
     - 2d texture, float.
     - Roughness value used for lighting. The final roughness value is read from the texture green component and scaled by the roughness float value.
     - White texture/1 scale.
   * - "occlusion"
     - 2d texture
     - Ambient occlusion value, read from the texture red component.
     - White texture.
   * - "normal"
     - 2d texture
     - Surface normal map.
     - Flat normal texture.


Types
-----

.. doxygengroup:: MaterialTypes
    :content-only:


Functions
---------

.. doxygengroup:: Material
    :content-only:
