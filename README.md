Image filter
============
Image filter is something that you can see often in the image processing software, for example PhotoShop, it can enhance contrast, brightness for you images. This project is going to implement some basic image filters using **Qt**.

### Filter List
- Point Operations
  * [x] Threshold
  * [x] Contrast
  * [x] Quantization(Dithering isn't good)
  * [x] Histogram Stretching
  * [x] Histogram Matching

- Neighborhood Operations
  * [x] Blur
  * [x] Sharpen
  * [x] Median Filter
  * [x] Convolution

**Note:This is a Qt project, in order to run this you have to have Qt installed!**

**Qt:** Qt is a cross-platform application framework that is widely used for developing application software that can be run on various software and hardware platforms with little or no change in the underlying codebase, while still being a native application with the capabilities and speed thereof

Getting started
===============

Step 1. Install [Qt](https://www.qt.io/download/)

Step 2. Navigate to one of the folders (that's `my_opengl_version`, `qip_1st_semester`, and `qip_2nd_semester`. Each one is different project)

Step 3. Running following commands one by one(assume we're in `qip_2nd_semester`).


```bash
qmake qip.pro
```
	
This command will generate a `Makefile`, note that in folder 'my_opengl_version', we use hw.pro instead.

Next

```bash
make
```
	
This command will complie the program and produce excutable.
	
Lastly

```bash
./qip
```
	
to run your excutable.





ScreenShots
===========

### Input:
![alt text](https://github.com/dongliang3571/Senior_design_csc599866/blob/master/screenshots/input.png?raw=true "Threshold input")

### Output:
- Threshold
![alt text](https://github.com/dongliang3571/Senior_design_csc599866/blob/master/screenshots/threshold.png?raw=true "Logo Title Text 1")

- Contrast
![alt text](https://github.com/dongliang3571/Senior_design_csc599866/blob/master/screenshots/contrast.png?raw=true "Logo Title Text 1")

