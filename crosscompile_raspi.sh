SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

echo "Building and installing Boost for aarch64"

mkdir $SCRIPT_DIR/boost_arch64_install
cd $SCRIPT_DIR/ThirdParty/boost-1.86.0
./bootstrap.sh

#set aarch64 toolset
sed -i 's|using gcc ;|using gcc : aarch64 : aarch64-linux-gnu-g++ ;|g' project-config.jam

./b2 toolset=gcc target-os=linux architecture=arm address-model=64 --prefix=$SCRIPT_DIR/boost_arch64/installation install