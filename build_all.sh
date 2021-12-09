
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
  g++ -std=c++2a -O0 -g -pthread -c ./Sdk/EntryPoint/*.cpp
  g++ -std=c++2a -O0 -g -pthread -o ./.exe.debug/Sdk_EntryPoint ./*.o ./.lib.debug/Sdk.a
  rm ./*.o

  ## build Sdk entry point exe [release]
  g++ -std=c++2a -O3 -pthread -c ./Sdk/EntryPoint/*.cpp
  g++ -std=c++2a -O3 -pthread -o ./.exe.release/Sdk_EntryPoint ./*.o ./.lib.release/Sdk.a
  rm ./*.o

# Db_2021

  ## copy build setting for Db_2021
  cp -rf ./_BuildSettings_LinuxGcc/Db_2021.inl ./BuildSetting.inl

  ## build DbCore static lib [debug]
  g++ -std=c++2a -O0 -g -pthread -c ./Db_2021/DbCore/*/Implementation/*.cpp
  ar crv ./.lib.debug/DbCore.a ./*.o
  rm ./*.o

  ## build DbCore static lib [release]
  g++ -std=c++2a -O3 -pthread -c ./Db_2021/DbCore/*/Implementation/*.cpp
  ar crv ./.lib.release/DbCore.a ./*.o
  rm ./*.o

  ## build Db entry point exe [debug]
  g++ -std=c++2a -O0 -g -pthread -c ./Db_2021/EntryPoint/*.cpp
  g++ -std=c++2a -O0 -g -pthread -o ./.exe.debug/Db_2021_EntryPoint ./*.o ./.lib.debug/Sdk.a ./.lib.debug/DbCore.a
  rm ./*.o

  ## build Db entry point exe [release]
  g++ -std=c++2a -O3 -pthread -c ./Db_2021/EntryPoint/*.cpp
  g++ -std=c++2a -O3 -pthread -o ./.exe.release/Db_2021_EntryPoint ./*.o ./.lib.release/Sdk.a ./.lib.release/DbCore.a
  rm ./*.o

# Copy All Exe
  cp -rf ./.exe.release/* ./_Release/

