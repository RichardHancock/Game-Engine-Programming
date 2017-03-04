MKGEN_SRC_DIR="../Engine"
MKGEN_COMPILER="g++"
MKGEN_FLAGS="-std=c++11 -std=gnu++11 -lGLEW -lGLU -lGL -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lpugixml -lLinearMath -lBulletCollision -lBulletDynamics -lassimp -lboost_system -lboost_random -lsioclient \$(shell sdl2-config --cflags) -I/usr/include/SDL2 -I/usr/include/bullet"
MKGEN_DEBUG_FLAGS="-g -Wall -pedantic"
MKGEN_RELEASE_FLAGS="-O2 -Wall -pedantic"
MKGEN_OUTPUT_PATH="../bin/"
MKGEN_OUTPUT_FILE="Engine"
MKGEN_RESOURCE_FOLDER_NAME="resources"
MKGEN_RESOURCE_FOLDER_PATH="../Engine/"
python makeFileGenerator.py "$MKGEN_SRC_DIR" "$MKGEN_COMPILER" "$MKGEN_FLAGS" "$MKGEN_DEBUG_FLAGS" "$MKGEN_RELEASE_FLAGS" "$MKGEN_OUTPUT_PATH" "$MKGEN_OUTPUT_FILE" "$MKGEN_RESOURCE_FOLDER_NAME" "$MKGEN_RESOURCE_FOLDER_PATH"
