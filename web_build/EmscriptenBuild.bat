em++ --bind -O3 -std=c++20 ..\game\src\main.cpp -s ASSERTIONS=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s ALLOW_MEMORY_GROWTH=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -o game.html --use-preload-plugins --preload-file ./res -DASSETS_PATH=\"./res/\"