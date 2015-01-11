#include "functions.h"

/*
 * getRadius - returns the radius based on the sigma value
 * INPUTS: sigma - sigma in the Guassian distribution
 * OUTPUTS: none
 * RETURN VALUE: radius - radius of the filter
 */
int getRadius(double sigma)
{
  return ceil(3 * sigma);
}

/*
 * calculateGausFilter - calculates the Gaussian filter
 * INTPUTS: gausFitler - pointer to the array for the gaussian filter
 *          sigma - the sigma in the Gaussian distribution
 * OUTPUTS: none
 * RETURN VALUE: none
 */
void calculateGausFilter(double *gausFilter, double sigma)
{
    double sum = 0.0;
    int radius = getRadius(sigma);
    double tempVal = 2 * M_PI*sigma*sigma;               //2(pi)(sigma)^2

    int row, col, idx = 0;
    for (row = -radius; row <= radius; row++)
        for (col= -radius; col<= radius; col++)
            {
            gausFilter[idx] = (exp(-(row*row + col*col) / (2*sigma*sigma)) / sqrt(tempVal));
            sum += gausFilter[idx++];
            }

    //normalize the coefficients

    int i;
    for (i = 0; i<idx; i++)
        gausFilter[i] /= sum;
  return;
}

/* convolveImage - performs a convolution between a filter and image
 * INPUTS: inRed - pointer to the input red channel
 *         inBlue - pointer to the input blue channel
 *         inGreen - pointer to the input green channel
 *         inAlpha - pointer to the input alpha channel
 *         filter - pointer to the convolution filter
 *         radius - radius of the convolution filter
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: outRed - pointer to the output red channel
 *          outBlue - pointer to the output blue channel
 *          outGreen - pointer to the output green channel
 *          outAlpha - pointer to the output alpha channel
 * RETURN VALUES: none
 */
void convolveImage(uint8_t *inRed,uint8_t *inBlue,uint8_t *inGreen,
                   uint8_t *inAlpha, uint8_t *outRed,uint8_t *outBlue,
                   uint8_t *outGreen,uint8_t *outAlpha,const double *filter,
                   int radius,int width,int height)
{

  int row, col, x, y;   // init the variables for the row iterator and column iterator
  int range = radius * 2 + 1;   // range of the filter

  if (radius < 1) {

    for (row = 0; row < height; row++) {
      for (col = 0; col < width; col++) {

        outRed[row * width + col] = inRed[row * width + col];
        outGreen[row * width + col] = inGreen[row * width + col];
        outBlue[row * width + col] = inBlue[row * width + col];
        outAlpha[row * width + col] = inAlpha[row * width + col];

      }
    }

    return;

  }

  double sum[4];
  int i;

  for (row = 0; row < height; row++) {
    for (col = 0; col < width; col++) {		// these 2 loops parse through the image pixel by pixel

      sum[0] = 0;
      sum[1] = 0;
      sum[2] = 0;
      sum[3] = 0;   // reset the sum values to 0 for the next out pixel;

      for (y = 0; y < range; y++) { // row iterator
        for (x = 0; x < range; x++) { // column iterator for the filters

          if ((row + y - radius) < 0 || (col + x - radius) < 0 || (row + y - radius) >= height || (col + x - radius) >= width) {	// bounds check
            for (i = 0; i < 4; i++) {
              sum[i] += 0;	// add 0 to the sum if its out of the bounds
            }
           continue; // then it will continue through the filter
          }

          sum[0] += inRed[(row + y - radius) * width + (col + x - radius)] * filter[y * range + x];		// add the value after the filter is applied
          sum[1] += inGreen[(row + y - radius) * width + (col + x - radius)] * filter[y * range + x];
          sum[2] += inBlue[(row + y - radius) * width + (col + x - radius)] * filter[y * range + x];
          sum[3] += inAlpha[(row + y - radius) * width + (col + x - radius)] * filter[y * range + x];

        }
      }

      for(i = 0; i< 4; i++) {
        sum[i] = sum[i] > 255 ? 255 : sum[i];	// clamp the values
        sum[i] = sum[i] < 0 ? 0 : sum[i];
      }

      outRed[row * width + col] = sum[0];	// save the new value to the output values
      outGreen[row * width + col] = sum[1];
      outBlue[row * width + col] = sum[2];
      outAlpha[row * width + col] = inAlpha[row * width + col];

    }
  }

  return;

}

/* convertToGray - convert the input image to grayscale
 * INPUTS: inRed - pointer to the input red channel
 *         inBlue - pointer to the input blue channel
 *         inGreen - pointer to the input green channel
 *         inAlpha - pointer to the input alpha channel
 *         gMonoMult - pointer to array with constants to be multipiled 
 *                     with RBG channels to convert image to grayscale
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: outRed - pointer to the output red channel
 *          outBlue - pointer to the output blue channel
 *          outGreen - pointer to the output green channel
 *          outAlpha - pointer to the output alpha channel
 * RETURN VALUES: none
 */
void convertToGray(uint8_t *inRed,uint8_t *inBlue,uint8_t *inGreen,
                   uint8_t *inAlpha,uint8_t *outRed,uint8_t *outBlue,
                   uint8_t *outGreen,uint8_t *outAlpha,
                   const double *gMonoMult,int height,int width)
{

  int row, col;

  for (row = 0; row < height; row++) {
    for (col = 0; col < width ; col++) {
      
      double convFactor = gMonoMult[0] * inRed[row*width + col] + gMonoMult[1] * inGreen[row*width + col] + gMonoMult[2] * inBlue[row*width + col];

      convFactor = convFactor > 255 ? 255 : convFactor;	// clamp the values
      convFactor = convFactor < 0 ? 0 : convFactor;
      // set all the values equal tot he conversion factor
      outRed[row*width + col] = convFactor;
      outGreen[row*width + col] = convFactor;
      outBlue[row*width + col] = convFactor;
      outAlpha[row*width + col] = inAlpha[row*width + col];

    }
  }

  return;
}

/* flipImage - flips the image in both the horizontal and vertical directions
 * INPUTS: inRed - pointer to the input red channel
 *         inBlue - pointer to the input blue channel
 *         inGreen - pointer to the input green channel
 *         inAlpha - pointer to the input alpha channel
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: outRed - pointer to the output red channel
 *          outBlue - pointer to the output blue channel
 *          outGreen - pointer to the output green channel
 *          outAlpha - pointer to the output alpha channel
 * RETURN VALUES: none
 */
void flipImage(uint8_t *inRed,uint8_t *inBlue,uint8_t *inGreen,
               uint8_t *inAlpha,uint8_t *outRed,uint8_t *outBlue,
               uint8_t *outGreen,uint8_t *outAlpha,int height,int width)
{


  int row, col;

  // loop to flip the image

  int temp = height;
  height = width;
  width = temp;   // swap cuz ece198kl is retarded

  for (row = 0; row < height; row++) {
    for (col = 0; col < width; col++) {

      outRed[(height - row - 1)*width + (width - col - 1)] = inRed[row * width + col];
      outGreen[(height - row - 1)*width + (width - col - 1)] = inGreen[row * width + col];
      outBlue[(height - row - 1)*width + (width - col - 1)] = inBlue[row * width + col];
      outAlpha[(height - row - 1)*width + (width - col - 1)] = inAlpha[row * width + col];
    }
  }

  return;
}

/* invertImage - inverts the colors of the image
 * INPUTS: inRed - pointer to the input red channel
 *         inBlue - pointer to the input blue channel
 *         inGreen - pointer to the input green channel
 *         inAlpha - pointer to the input alpha channel
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: outRed - pointer to the output red channel
 *          outBlue - pointer to the output blue channel
 *          outGreen - pointer to the output green channel
 *          outAlpha - pointer to the output alpha channel
 * RETURN VALUES: none
 */
void invertImage(uint8_t *inRed,uint8_t *inBlue,uint8_t *inGreen,
                 uint8_t *inAlpha,uint8_t *outRed,uint8_t *outBlue,
                 uint8_t *outGreen,uint8_t *outAlpha,int height,int width)
{

  int row, col;

  // loop to invert the image

  for (row = 0; row < height; row++) {
    for (col = 0; col < width ; col++) {

      outRed[row*width + col] = 255-inRed[row*width + col];
      outGreen[row*width + col] = 255-inGreen[row*width + col];
      outBlue[row*width + col] = 255-inBlue[row*width + col];
      outAlpha[row*width + col] = inAlpha[row*width + col];

    }
  }
  return;
}

/* pixelate - pixelates the image
 * INPUTS: inRed - pointer to the input red channel
 *         inBlue - pointer to the input blue channel
 *         inGreen - pointer to the input green channel
 *         inAlpha - pointer to the input alpha channel
 *         pixelateY - height of the block
 *         pixelateX - width of the block
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: outRed - pointer to the output red channel
 *          outBlue - pointer to the output blue channel
 *          outGreen - pointer to the output green channelfilter
 *          outAlpha - pointer to the output alpha channel
 * RETURN VALUES: none
 */
void pixelate(uint8_t *inRed,uint8_t *inBlue,uint8_t *inGreen,
              uint8_t *inAlpha,uint8_t *outRed,uint8_t *outBlue,
              uint8_t *outGreen,uint8_t *outAlpha,int pixelY,int pixelX,
              int width,int height)
{

  int row, col, x, y, i;
  double total[4];
  int avgCount;

  for (row = 0; row < height + pixelY; row += pixelY) {
    for (col = 0; col < width + pixelX; col += pixelX) {		// This will go through the image and increment by a pixelX / pixelY

      for (i = 0; i < 4; i++)
        total[i] = 0;		// add 0 to the total

      avgCount = 0;

      for (y = 0; y < pixelY; y++) {	// parse through the pixelX x pixelY box to add all the values
        for (x = 0; x < pixelX; x++) {

          if ((row + y) >= height || (col + x) >= width)		// bounds checker
            continue;

          total[0] += inRed[(row + y) * width + (col + x)];		// add up to the total
          total[1] += inGreen[(row + y) * width + (col + x)];
          total[2] += inBlue[(row + y) * width + (col + x)];
          total[3] += inAlpha[(row + y) * width + (col + x)];
          avgCount++;

        }
      }


      for (i = 0; i < 4; i++) 
        total[i] /= avgCount;		// average it out

      for (y = 0; y < pixelY; y++) {
        for (x = 0; x < pixelX; x++) {		// parse through the box again to set the output values to the new averaged value

          if ((row + y) >= height || (col + x) >= width)		// bounds checker
            continue;

          outRed[(row + y) * width + (col + x)] = total[0];
          outGreen[(row + y) * width + (col + x)] = total[1];		// set the output to the averaged total of the input value in a pixelX x pixelY box
          outBlue[(row + y) * width + (col + x)] = total[2];
          outAlpha[(row + y) * width + (col + x)] = total[3];

        }
      }

    }
  }

  return;

}

/* colorDodge - blends the bottom layer with the top layer
 * INPUTS: botRed - pointer to the bottom red channel
 *         botBlue - pointer to the bottom blue channel
 *         botGreen - pointer to the bottom green channel
 *         botAlpha - pointer to the bottom alpha channel
 *         topRed - pointer to the top red channel
 *         topBlue - pointer to the top blue channel
 *         topGreen - pointer to the top green channel
 *         topAlpha - pointer to the top alpha channel
 *         width - width of the input image
 *         height - height of the input image
 * OUTPUTS: outRed - pointer to the output red channel
 *          outBlue - pointer to the output blue channel
 *          outGreen - pointer to the output green channel
 *          outAlpha - pointer to the output alpha channel
 * RETURN VALUES: none
 */
void colorDodge(uint8_t *botRed,uint8_t *botBlue,uint8_t *botGreen,
                uint8_t *botAlpha,uint8_t *topRed,uint8_t *topBlue,
                uint8_t *topGreen,uint8_t *topAlpha,uint8_t *outRed,
                uint8_t *outBlue,uint8_t *outGreen,uint8_t *outAlpha,
                int width,int height)
{

  int row, col;

  for (row = 0; row < height; row++) {
    for (col = 0; col < width; col++) {

      outRed[row*width + col] = ((topRed[row*width + col]==255)?topRed[row*width + col]:min(((botRed[row*width + col]<<8)/(255-topRed[row*width + col])),255));
      outGreen[row*width + col] = ((topGreen[row*width + col]==255)?topGreen[row*width + col]:min(((botGreen[row*width + col]<<8)/(255-topGreen[row*width + col])),255));
      outBlue[row*width + col] = ((topBlue[row*width + col]==255)?topBlue[row*width + col]:min(((botBlue[row*width + col]<<8)/(255-topBlue[row*width + col])),255));
      outAlpha[row * width + col] = botAlpha[row * width + col];

    }
  }

  return;
  
}

/* pencilSketch - creates a pencil sketch of the input image
 * INPUTS: inRed - pointer to the input red channel
 *         inBlue - pointer to the input blue channel
 *         inGreen - pointer to the input green channel
 *         inAlpha - pointer to the input alpha channel
 *         invRed - pointer to temporary red channel for inversion
 *         invBlue - pointer to temporary blue channel for inversion
 *         invGreen - pointer to temporary green channel for inversion
 *         invAlpha - pointer to temporary alpha channel for inversion
 *         blurRed - pointer to temporary red channel for blurring
 *         blurBlue - pointer to temporary blue channel for blurring
 *         blurGreen - pointer to temporary green channel for blurring
 *         blurAlpha - pointer to temporary alpha channel for blurring
 *         filter - pointer to the gaussian filter to blur the image
 *         radius - radius of the gaussian filter
 *         width - width of the input image
 *         height - height of the input image
 *         gMonoMult - pointer to array with constants to be multipiled with 
 *                     RBG channels to convert image to grayscale
 * OUTPUTS: outRed - pointer to the output red channel
 *          outBlue - pointer to the output blue channel
 *          outGreen - pointer to the output green channel
 *          outAlpha - pointer to the output alpha channel
 * RETURN VALUES: none
 */
void pencilSketch(uint8_t *inRed,uint8_t *inBlue,uint8_t *inGreen,
                  uint8_t *inAlpha,uint8_t *invRed,uint8_t *invBlue,
                  uint8_t *invGreen,uint8_t *invAlpha,uint8_t *blurRed,
                  uint8_t *blurBlue,uint8_t *blurGreen,uint8_t *blurAlpha,
                  uint8_t *outRed,uint8_t *outBlue,uint8_t *outGreen,
                  uint8_t *outAlpha,const double *filter,int radius,
                  int width,int height,const double *gMonoMult)
{

  convertToGray(inRed, inBlue, inGreen, inAlpha, invRed, invBlue, invGreen, invAlpha, gMonoMult, width, height);
  invertImage(invRed, invBlue, invGreen, invAlpha, invRed, invBlue, invGreen, invAlpha, width, height);
  convolveImage(inRed, inBlue, inGreen, inAlpha, blurRed, blurBlue, blurGreen, blurAlpha, filter, radius, width, height);
  colorDodge(blurRed, blurBlue, blurGreen, blurAlpha, invRed, invBlue, invGreen, invAlpha, outRed, outBlue, outGreen, outAlpha, width, height);

  return;
}
