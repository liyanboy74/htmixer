# Makefile Template for HTMixer
# by liyanboy74

# Generate Dir
generate-dir 		= gh-pages

default : compile

# Compice C prog using GCC
compile :
	rm -rf build
	mkdir build
	gcc htmixer.c -o ./build/htmixer

# Remove Compiled app and Generated files
clean :
	rm -rf $(generate-dir)
	rm -rf build

# Deploy on Github Pages, branch [gh-pages]
# more on : https://pypi.org/project/ghp-import/
# Add CNAME by '-c SITENAME' command
deploy:
	ghp-import -p $(generate-dir)

##################################### GENARATE ########################################
# Use 'build/htmixer' for mix 'Doc' and 'Var' files.
# for Example: htmixer OUTPUT_FILR -d DOC1 DOC2 -v VAR1 VAR2 VAR3
# The Var replace by same name in Doc file

# Start Generate Static Web Pages
generate :
	rm -rf $(generate-dir)
	mkdir $(generate-dir)

# Copy Doc
	cp -r ./doc/htmixer.png ./$(generate-dir)/htmixer.png

# Run Mixer
	./build/htmixer ./$(generate-dir)/index.html -d ./doc/home.html -v ./var/home.txt


