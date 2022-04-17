
# Add Build Folder
  rm -rdf ./*.o ./.lib.debug ./.lib.release ./.exe.debug ./.exe.release
  mkdir ./.lib.debug ./.lib.release ./.exe.debug ./.exe.release

# Sdk

  ## copy build setting for Sdk
  cp -rf ./_BuildSettings_LinuxGcc/Sdk.inl ./BuildSetting.inl

  ## build Sdk static lib [debug]
  g++ -std=c++2a -O0 -g -pthread -c ./Sdk/Sdk/Environment/EnvironmentLinuxGcc/Implementation/*.cpp
  ar crv ./.lib.debug/Sdk.a ./*.o
  rm ./*.o

  ## build Sdk static lib [release]
  g++ -std=c++2a -O3 -pthread -c ./Sdk/Sdk/Environment/EnvironmentLinuxGcc/Implementation/*.cpp
  ar crv ./.lib.release/Sdk.a ./*.o
  rm ./*.o

  ## build Sdk entry point exe [debug]
  g++ -std=c++2a -O0 -g -pthread -c ./Sdk/UnitTest/*.cpp
  g++ -std=c++2a -O0 -g -pthread -o ./.exe.debug/Sdk_UnitTest ./*.o ./.lib.debug/Sdk.a
  rm ./*.o

  ## build Sdk entry point exe [release]
  g++ -std=c++2a -O3 -pthread -c ./Sdk/UnitTest/*.cpp
  g++ -std=c++2a -O3 -pthread -o ./.exe.release/Sdk_UnitTest ./*.o ./.lib.release/Sdk.a
  rm ./*.o

# Copy All Exe
  cp -rf ./.exe.release/* ./_Release/
