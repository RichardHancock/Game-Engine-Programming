MKGEN_SRC_DIR="../Engine"
MKGEN_COMPILER="g++"
MKGEN_FLAGS="-std=c++11 -std=gnu++11 -framework OpenGL -L/usr/local/lib -L/usr/local/lib/pugixml-1.8  -lGLEW -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lpugixml -lLinearMath -lBulletCollision -lBulletDynamics -lassimp -lQRCode -lpthread -lsioclient -lboost_system -lboost_random \$(shell sdl2-config --cflags) -I/usr/local/include -I/usr/local/include/assimp -I/usr/local/include/pugixml-1.8 -I/usr/local/include/bullet"
MKGEN_DEBUG_FLAGS="-g -Wall -pedantic"
MKGEN_RELEASE_FLAGS="-O2 -Wall -pedantic"
MKGEN_OUTPUT_PATH="../bin/"
MKGEN_OUTPUT_FILE="Engine"
MKGEN_RESOURCE_FOLDER_NAME="resources"
MKGEN_RESOURCE_FOLDER_PATH="../Engine/"
MKGEN_RESOURCE_FOLDER_OUTPUT_PATH="/usr/local/share/RH/Engine/"
python makeFileGenerator.py "$MKGEN_SRC_DIR" "$MKGEN_COMPILER" "$MKGEN_FLAGS" "$MKGEN_DEBUG_FLAGS" "$MKGEN_RELEASE_FLAGS" "$MKGEN_OUTPUT_PATH" "$MKGEN_OUTPUT_FILE" "$MKGEN_RESOURCE_FOLDER_NAME" "$MKGEN_RESOURCE_FOLDER_PATH" "$MKGEN_RESOURCE_FOLDER_OUTPUT_PATH"
