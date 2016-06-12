Simple PBM/PGM/PPM image library for C/C++ programs. Use `make` to run test code with `gcc`. Check out the Makefile for more. For more information on the PPM image format, [check out the Wikipedia page](https://en.wikipedia.org/wiki/Netpbm_format).

The API is fairly simple, and consists of the following functions:

### easyppm_create
Takes width, height, and image type. Supported image types are `IMAGETYPE_PBM` (black and white), `IMAGETYPE_PGM` (greyscale), and `IMAGETYPE_PPM` (color).

    PPM ppm = easyppm_create(256, 256, IMAGETYPE_PPM);

### easyppm_clear
Fills the entire image with the specified color.

    easyppm_clear(&ppm, easyppm_rgb(255, 0, 0)); // Red

### easyppm_set
Sets pixel at (x,y) to specified color. Origin defaults to upper-left corner.

    easyppm_set(&ppm, 0, 0, easyppm_rgb(255, 0, 0));

### easyppm_get
Gets pixel at (x,y).

    color c = easyppm_get(&ppm, 0, 0);

### easyppm_rgb
Returns RGB representation of a color for use in PPM (color) images. Values range from 0 to 255. Attempting to set a pixel in a non-PPM (i.e. PBM/PGM) image using this function will result in a runtime error.

    color c = easyppm_rgb(255, 0, 0); // Red

### easyppm_grey
Returns the greyscale representation of a color for use in PGM (greyscale) images. Values range from 0 to 255. Attempting to set a pixel in a non-PGM (i.e. PBM/PPM) image will result in a runtime error.

    color c = easyppm_grey(128); // Grey

### easyppm_black_white
Returns the black-and-white representation of a color for use in PBM (black-and-white) images. Values include 0 (white) and 1 (black). Attempting to set a pixel in a non-PBM (i.e. PGM/PPM) image will result in a runtime error.

    color c = easyppm_black_white(0); // White

### easyppm_gamma_correct
Gamma-correct entire image by specified amount.

    easyppm_gamma_correct(&ppm, 2.2f);

### easyppm_invert_y
Invert image across y-axis. Note that this does NOT change the origin from the upper-left corner to the lower-left corner; subsequent calls to `easyppm_get()` and `easyppm_set()` will still be relative to the upper-left corner. However, this is useful as a final step before sending the image to a system with a different origin (e.g. OpenGL).

    easyppm_invert_y(&ppm);

### easyppm_read
Reads non-binary PBM/PGM/PPM images from disk and stores them in the given PPM struct. Because this currently requires max channel values of 255 and one pixel per line, it will likely not work correctly for images not created using `easyppm` (this is something I hope to address soon).

    easyppm_read(&ppm, "image.ppm");

### easyppm_write
Writes non-binary PBM/PGM/PPM images to disk from the given PPM struct.

    easyppm_write(&ppm, "image.ppm");

### easyppm_destroy
Frees all resources.

    easyppm_destroy(&ppm);
