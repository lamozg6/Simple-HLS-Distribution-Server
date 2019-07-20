# Simple-HLS-Distribution-Server

HLS-streaming server using oat++ (AKA oatpp) Async API.

Server protected by Basic Auth standard and use cookies.

Live stream is tested with Safari-Browser and VLC-player.

## Overview

Server generates infinite .m3u8 playlist from the pre-generated 
```playlist_live.m3u8``` file in the ```video``` folder.  
File ```video/playlist_live.m3u8``` together with video chunks is generated using ```ffmpeg```.  

Server is built using oat++ Async-Api and has following endpoints:

- ```("GET", "/authenticate/{login}/{password}", Authenticate)``` - Authenticate page to get token for HTTP Live Streaming access
- ```("GET", "/record", Record)``` - Encode the HLS stream to MPG format and save it in ```/video/stream``` 
- ```("GET", "/frames", Frames)``` - Encode the frames from HLS stream to JPG and save it in ```/video/frames``` 
- ```("GET", "/*", Static)``` - Endpoint serving media files with range requests support

### Project layout

```

|- CMakeLists.txt                       // projects CMakeLists.txt
|- src/                                 // source folder
|- utility/install-oatpp-modules.sh     // utility script to install required oatpp-modules.
|- video/                               // media files and playlists here
     |- stream/                         // folder to save HLS encoded to MPG
     |- frames/                         // folder to save HLS frames encoded to JPG
     |- generate_pls.sh                 // example how to use ```ffmpeg``` to generate initial playlist and video chunks
     |- playlist_live.m3u8              // playlist used to generate infinite playlist for http-live-streaming

```
```
- src/
    |
    |- controller/              // Folder containing UserController where all endpoints are declared
    |- hls/                     // Playlist generator is here
    |- Utils.hpp                // Utils
    |- AppComponent.hpp         // Service config
    |- App.cpp                  // main() is here
    
```

---

### Build and Run

#### Using CMake

**Requires**

- `oatpp` module installed. You may run `utility/install-oatpp-modules.sh` 
script to install required oatpp modules.

```
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./hls-stream-exe             # - run application.
```

#### In Docker

```
$ docker build -t stream-hls .
$ docker run -p 8000:8000 -t stream-hls
```

## Notes

### Urls
- localhost:8000/authenticate/{login}/{password} - Authenticate to start streaming
- localhost:8000/frames - Save stream as frames
- localhost:8000/record - Save stream as video

### Files
- {repo}/video/playlist_live.m3u8 - playlist used for live HLS streaming
- {repo}/video/generate_pls.sh - example script used to generate sub-playlists and video fragmets using ffmpeg tool. #EXTINF sections of sub-playlists then have to be manualy moved to playlist_live.m3u8.

### Note
Authentication done using cookies, but VLC doesn't support it. For playing stream in VLC you need to comment out line ```//#define VLC``` in ```MediaController.hpp```, then authenticate by link localhost:8000/authenticate/admin/123 (i.e. using curl) and after that open network stream in VLC.
