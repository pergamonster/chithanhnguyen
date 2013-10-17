#################################################
#                                               #
#   C++ PROGRAMMING                             #
#   MASTER LAURE ELIE 2013-2014                 #
#   Student Chi Thanh NGUYEN                    # 
#                                               #
#   chithanhnguyen.math@gmail.com               #
#################################################

#---------------------------#
# DO NOT MODIFY THIS FILE   #
#---------------------------# 

# Configurations for cmake build, do not modify this file
# If you want to change the build options, copy this file to a file named exactly "user-config.cmake"
# DO NOT add user-config.cmake to svn repository
# Modify options in the user-config.cmake and TYPE the command cmake for generating the project (see README.txt)


### Choose your build type 
###
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

### Uncomment this line if you want to build tests , only IF YOU HAVE BOOTS
###
set(M2MO_BUILD_TEST TRUE)

### If you have to build cpp project with a special compiler, IDE, 
### Search the document for it about your cmake_generator, 
### Uncomment these two lines and modify "your_special_cmake_generator" 
###
#set(M2MO_USER_DEFINE_GENERATOR)
#set(CMAKE_GENERATOR "your_special_cmake_generator")
