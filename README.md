# HTMixer
Static HTML Page Generator.

## Introductions
HTMixer is a Static HTML page generator. This tool can combine different pages and add content to them. Each content can be specific to one or more templates. This tool is written in c language and can be used for any file, but customized for Github Pages. 

![htmixer](https://user-images.githubusercontent.com/64005694/144050591-0a888a34-46b5-452c-aace-df584f2ee9ba.png)

## Compile
This tool is written in `C` language and uses `GCC` and `Make` for compilation.
```
make
```
After compile source, executable app will be generated in `build` folder, do not remove that!
## Generate
Generator use `Make` for running commands in the `Makefile`. 
```
make generate
```
In default the generated file stored in `gh-pages` folder, in head of `Makefile` you can changes the output folder.
## Deploy
Deployment use `ghp-import` for `Github Pages`.
```
make deploy
```
You can add `CNAME` by editing `deploy` command in `Makefile`.

## Variable & Loop.
Variable defined in var folder,but Loops defined in doc folder close to HTML files.
### Variable:
Variables that are set in the var folder are replaced in the doc folder.

Example set variable `home.txt` : 
```
{{PAGE-TITLE   HTMixer}} 
```
Example use variable `home.html` : 
```
{{PAGE-TITLE}} 
```
### Loops:

Loops set in doc To repeat its internal code.
If the internal code use Variable, the Variable rename by loop counter.

For example [`VAR`->`VAR-0`,`VAR-1`,`VAR-2`,`...`,`VAR-N`].

Example `home.html` : 
 ```
{{FOR(0,5)
 //5 Times from 0 to 4
}}

{{FOR(5,6)
 //1 Times 5
}}

{{FOR(1,3)
 //2 Times from 1 to 2
}}
 ```
## Capability
### Variable:
This tool supports interdependent variables up to `five` steps.
Example:
```
{{VAR1 You?}}
{{VAR2 Are {{VAR1}}}}
{{VAR3 How {{VAR2}}}}
```
`VAR3` = `How Are You?`

### Loops:
It is also allowed two Nested loop.

Example:
```
{{FOR(0,2)
  {{FOR(0,2)
    {{VAR}}
  }}
}}
```
Result:
```
{{VAR-0-0}}
{{VAR-0-1}}
{{VAR-1-0}}
{{VAR-1-1}}
```
