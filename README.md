# Urho3D-plugin-for-OBS-Studio
Attempt to integrate Urho3D into OBS Studio. It buggy and published to get help.

To change Urho3D home directory, change parameter in file Urho3DModule.cpp to your own directory.
```
engineParameters_[EP_RESOURCE_PATHS] = "/home/scaled/projects/Urho3D/Urho3DModule/Data;/home/scaled/projects/Urho3D/Urho3DModule/CoreData";
```

# Installing

Firstly, you need to compile plugin. libobs-dev and Urho3D must be installed.
```
cd Urho3D-plugin-for-OBS-Studio
mkdir build
cd build
cmake ..
make
```

Secondly, copy or link `liburho3d-obs-plugin.so` into directory `~/.config/obs-studio/plugins/liburho3d-obs-plugin/bin/64bit/liburho3d-obs-plugin.so` . Create missing directories if needed.
```
mkdir -p ~/.config/obs-studio/plugins/liburho3d-obs-plugin/bin/64bit/
cp liburho3d-obs-plugin.so ~/.config/obs-studio/plugins/liburho3d-obs-plugin/bin/64bit/liburho3d-obs-plugin.so
```

Then you can run OBS Studio and add `Urho3DModule` video source.
