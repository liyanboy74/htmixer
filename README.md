# HTMixer
HTMixer is a Static HTML page generator. this tool can combine different pages and add content to them. each content can be specific to one or more templates. this tool is written in C language and can be used for any file, but customized for Github Pages. 

[Read More](https://liyanboy74.github.io/htmixer/)

## Compile & Install
```
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
sudo make install
```

## Example Generate & Deploy
```
cd ../example
make
make deploy
```