# HTMixer
HTMixer is a Static HTML page generator. This tool can combine different pages and add content to them.
Content can be specific to one or more templates.
This tool is written in C language and can be used for any text file, but customized for Github Pages. 

[Read More](https://liyanboy74.github.io/htmixer/)

### Compile & Install
```
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
sudo make install
```

### Commands
```
$> htmixer -?

HTMixer is a Static HTML page generator. This tool can combine different pages and add content to them.
Content can be specific to one or more templates. This tool is written in C language and can be used for any file, but customized for Github Pages.
More info at https://github.com/liyanboy74/htmixer .

COMMANDS:
[FILE]          Output          Output file name
                                Ex: htmixer ./blog/index.html

-v [FILE]       Var             Name of file contains variable
                                Ex: htmixer -v home-text.txt blog-text.txt

-d [FILE]       Doc             Name of file contains html,style or body
                                Ex: htmixer -d home.html shop.html

--version       Version         Print version info
-l[0-3]         Set log level   Print more info while generating
                                Ex: htmixer -l1
                                Default set to 1

-?,-h,--help    Help            Print this list
```

### Sitemap
Use [SMG](https://github.com/liyanboy74/smg) for generate Sitemap.

### Trusted By
- [IOElectro](https://github.com/ioelectro/ioelectro.github.io) 
