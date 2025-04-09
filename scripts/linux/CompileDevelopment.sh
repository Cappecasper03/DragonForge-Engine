cd ../..

xmake repo -u
xmake f -y -p "linux" -a "x86_64" -m "Development" -c
xmake -r

cd ./scripts/linux
