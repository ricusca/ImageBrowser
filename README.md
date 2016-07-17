ImageBrowser 
-----------------

URL:  https://github.com/ricusca/ImageBrowser

Usage
------------------

ImageBrowser http_path_to_json_config [logLevel]

	* logLevel can be: logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4

	* an url to a default config is used if http_path_to_json_config is not provided

Technologies:
------------------
- developed in C++ using Visual Studio 2015
- additional libraries: 
	* SDL2 - for rendering
	* SDL2_Image - for loading PNGs
	* SDL2_ttf - for text rendering
	* CMake - for managing the build process
	* jsoncpp - for parsing the JSON files
	* libCurl - for downloading files via HTTP

What it does:
-----------------

- Download PNGs from web and show them using SDL
- Navigate through images:
	* clicking on the left area of the first image of a page will load previous page
	* clicking on the right area of the last image of a page will load next page
	* hovering with the mouse over a picture will higlight it (zoom it)
- caches a number of pages in memory in order to optimize use of memory
	* i.e. using default configuration with a page size of 4 images and 4 cached pages the memory will always contain a maximum of 16 images in memory


Technical details
-----------------

Classes

*Timer 
-used to compute number of ticks passed in order to obtain the framerate 

*Browser 
- the controller of the app. It is responsible to initialize all the subsystems(rendere, httclient etc.) , get and pass data between them
- handles the input and know which data to keep in memory or not
- defines how many pages to cache via Browser::PAGES_TO_CACHE
- defines how big is page (how mnay images to display) through Browser::PAGES_TO_CACHE
- reads the configuration data received via http

*DownloadFile
- holds data for a file that needs to be/is downloaded
- because mData is raw pointer copy constructor and assignment operator have been marked as deleted(but the implementation is still there). This was done in order to avoid memory issue(use of unecesary memory) when deep copy takes place or other potential bugs in case of shallow copy
- has the possiblity to retry the download in case it fails for some reasons. 

*HttpClient
- a wrapper via libCurl to download data
- uses a producer consumer algorithm : 1 consumer(requests) to many producers(downloads)
- the number of threads for the producer is set via static variable HttpClient::NUM_THREADS and is by default equal to 4
- max file size is defined via a static variable and is set to HttpClient::MAX_IMG_SIZE = 150000. There will be allocated NUM_THREADS*MAX_IMG_SIZE

*Request
- simple class that represents a http request

*Renderer
- wrapper via SDL in order to render data on screen
- it is instantiated via a factory method
- in theory it could be replace with any other custom render that implements(/has a wrapper) IRenderer interface

*ObjRenderable and TextRenderable
- renderable objects containing data needed by rendered to know to identify textures and render them
- image size is hardoced in ObjRenderable to 171 pixels width and 211 height

TO DOs
------
- add easing animations when page are changed
- remove static variables and add them in (a/the) configuration file
- add new class for for responsible for images animation, pages and grid
- compile it under unix