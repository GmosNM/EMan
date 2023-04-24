# EMan

## Build
```console
$ cmake -B bld
$ cmake --build bld
```
if you use ninja 

```console
$ cmake -GNinja -B bld
$ cmake --build bld
```

## Using

All the packages i hard coded 
```console
$ EMan showPackages
```

Add a package to your project
```console
$ EMan add [Package Name]
```

for creating cmake file for all the packages you added
```console
$ EMan cmake
```
![image](https://user-images.githubusercontent.com/37900384/234088751-08afdeef-baab-4a31-a062-15bc6dc368ec.png)


for building cmake and moving the compile_commands.json to the current dir (cus i use nvim)
```console
$ EMan build
```

for updateing the cmake file and .eman file (it auto add the src folder files)
```console
$ EMan update
```
