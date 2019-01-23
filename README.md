# VSCamOpenCV
Apps using openCV camera libraries.

# Description
Defferent apps are called from "main.cpp" with its command line argument. 

**main.cpp**
```
int main(int argc, const char** argv)
{
	string cmd = argv[1];
...
```

## Description for each apps
### Points game
```
cmd == "ar_g01"
```
![ar_point_game](https://user-images.githubusercontent.com/27729058/51584604-74755200-1f19-11e9-81b9-1904992d9b5e.png)

It detects Aruco markers and shows points which it has. Points start from 0 and increase after markers hits to fruits. 
3 kinds of fruits appears at random. 
It disappears after a moment.
* Apples: 1 points
* Pears: 3 points
* Black apples: -10 points

It gets 3 points after it hits to a pear.

![img_game01_hit](https://user-images.githubusercontent.com/27729058/51585050-3842f100-1f1b-11e9-90fa-3cf0baa1a425.png)

### Ar marker creation
```
cmd == "ar_c"
```
It outputs 250 different Aruco markers.

### Face Detection
```
cmd == "fd"
```
Simple face detection. Only calls face-detection function from openCV libralies.

# Requirements
* opencv-3.4.3
* opencv-contrib-3.4.3

You needs to build opencv-contrib on your own to get dlls and libs as the library is only available in raw version. 
I followed this instruction to build it.

https://qiita.com/tomochiii/items/fa26404ebc5fcd4481b9
