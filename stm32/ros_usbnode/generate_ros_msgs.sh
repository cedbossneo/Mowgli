#!/opt/homebrew/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
# Clone the repository https://github.com/ClemensElflein/open_mower_ros in a temporary directory
OM_DIR=/tmp/open_mower_ros
DYN_DIR=/tmp/dynamic_reconfigure
#rm -Rf extra_packages/* $OM_DIR $DYN_DIR
git clone https://github.com/ClemensElflein/open_mower_ros $OM_DIR
git clone https://github.com/ros/dynamic_reconfigure $DYN_DIR
cd $OM_DIR && git submodule update --init --recursive
cd $SCRIPT_DIR
# Use msg-import to import the messages from the temporary directory
declare -a PACKAGES_NAME
declare -a PACKAGES_PATH
declare -A PACKAGES_DESC

PACKAGES_NAME[0]="xbot_msgs"
PACKAGES_PATH[0]="$OM_DIR/src/lib/xbot_msgs/msg"
PACKAGES_NAME[1]="mower_msgs"
PACKAGES_PATH[1]="$OM_DIR/src/mower_msgs/msg"

PACKAGES_NAME[2]="xbot_msgs"
PACKAGES_PATH[2]="$OM_DIR/src/lib/xbot_msgs/srv"
PACKAGES_NAME[3]="mower_msgs"
PACKAGES_PATH[3]="$OM_DIR/src/mower_msgs/srv"

PACKAGES_NAME[4]="dynamic_reconfigure"
PACKAGES_PATH[4]="$DYN_DIR/srv"
PACKAGES_NAME[5]="dynamic_reconfigure"
PACKAGES_PATH[5]="$DYN_DIR/msg"
for pkg in ${!PACKAGES_NAME[@]}; do
  pn=${PACKAGES_NAME[$pkg]}
  msgList=$(find ${PACKAGES_PATH[$pkg]} -name '*.msg')
  mkdir -p extra_packages/${pn}/{msg,srv}
  cat <<EOF > extra_packages/${pn}/package.xml
<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
<package format="3">
  <name>${pn}</name>
  <version>0.0.0</version>
  <description>${pn}</description>
  <maintainer email="root@todo.todo">root</maintainer>
  <license>TODO: License declaration</license>

  <buildtool_depend>ament_cmake</buildtool_depend>

  <build_depend>rosidl_default_generators</build_depend>
  <exec_depend>rosidl_default_runtime</exec_depend>
  <member_of_group>rosidl_interface_packages</member_of_group>

  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
EOF
  for file in $msgList; do
    echo "Importing message $file"
    filename=$(basename $file)
    cp $file extra_packages/${pn}/msg/$filename
    PACKAGES_DESC[$pn]="${PACKAGES_DESC[$pn]}\"msg/${filename}\"
"
  done
  srvList=$(find ${PACKAGES_PATH[$pkg]} -name '*.srv')
  for file in $srvList; do
    echo "Importing service $file"
    filename=$(basename $file)
    cp $file extra_packages/${pn}/srv/$filename
    PACKAGES_DESC[$pn]="${PACKAGES_DESC[$pn]}\"srv/${filename}\"
"
  done
    cat <<EOF > extra_packages/${pn}/CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(${pn})

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(\${PROJECT_NAME}
  ${PACKAGES_DESC[${pn}]}
 )

ament_package()
EOF
done
