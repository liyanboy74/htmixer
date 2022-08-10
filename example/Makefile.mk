# Makefile Template for HTMixer
# by liyanboy74

htmixer		 =	"../../build/htmixer"
generate-dir =	out

default : generate
##################################### GENARATE ########################################
# Use 'htmixer' for mix 'Doc' and 'Var' files.
# for Example: htmixer OUTPUT_FILR -d DOC1 DOC2 -v VAR1 VAR2 VAR3
# The Var replace by same name in Doc file

# Start Generate Static Web Pages
generate :
	rm -rf $(generate-dir)
	mkdir $(generate-dir)

# Run Mixer
	$(htmixer) ./$(generate-dir)/index.html -d ./doc.html -v ./var.txt


##################################### DEPLOY ##########################################
# Deploy on Github Pages, branch [gh-pages]
# more on : https://pypi.org/project/ghp-import/
# Add CNAME by '-c SITENAME' command
deploy:
	ghp-import -p $(generate-dir)


#######################################################################################