SET mkgen_srcDir="../Engine"
SET mkgen_compiler="g++"
SET mkgen_flags="-std=c++11 -std=gnu++11 -lGLEW -lGLU -lGL -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lpugixml -lLinearMath -lBulletCollision -lBulletDynamics -lassimp -lQRCode -lpthread -lsioclient -lboost_system -lboost_random $(shell sdl2-config --cflags) -I/usr/include/SDL2 -I/usr/include/bullet"
SET mkgen_debugFlags="-g -Wall -pedantic"
SET mkgen_releaseFlags="-O2 -Wall -pedantic"
SET mkgen_outputPath="../bin/"
SET mkgen_outputFile="Engine"
SET mkgen_resourceFolderName="resources"
SET mkgen_resourceFolderPath="../Engine/"
SET mkgen_resourceFolderOutputPath="../bin/"

call python makeFileGenerator.py %mkgen_srcDir% %mkgen_compiler% %mkgen_flags% %mkgen_debugFlags% %mkgen_releaseFlags% %mkgen_outputPath% %mkgen_outputFile% %mkgen_resourceFolderName% %mkgen_resourceFolderPath% %mkgen_resourceFolderOutputPath%
