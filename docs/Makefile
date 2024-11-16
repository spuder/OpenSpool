# Run bundle exec jekyll serve

# defualt to serve
all: clean serve

.PHONY: clean
clean: 
	bundle exec jekyll clean

.PHONY: serve
serve:
	bundle exec jekyll serve --incremental --livereload --host 0.0.0.0 --port 4000

.PHONY: build
build:
	bundle exec jekyll build

